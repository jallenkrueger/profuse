
#include <memory>

#include <Pascal/File.h>

#include <ProFUSE/auto.h>
#include <ProFUSE/Exception.h>

#include <Endian/Endian.h>
#include <Endian/IOBuffer.h>

#include <Device/BlockDevice.h>
#include <Cache/BlockCache.h>

#pragma mark -
#pragma mark VolumeEntry

using namespace LittleEndian;
using namespace Pascal;

using namespace Device;

unsigned VolumeEntry::ValidName(const char *cp)
{
    // 7 chars max.  Legal values: ascii, printable, 
    // no space/tab,
    // no $=?,:
    
    return Entry::ValidName(cp, 7);
}

VolumeEntry::VolumeEntry()
{
    _fileNameLength = 0;
    std::memset(_fileName, 0, 8);
    _lastVolumeBlock = 0;
    _fileCount = 0;
    _accessTime = 0;
    
    setInode(1);
    
    _inodeGenerator = 1; 
    _cache = NULL;
    _device = NULL;
}

VolumeEntry::VolumeEntry(const char *name, Device::BlockDevice *device)
{
#undef __METHOD__
#define __METHOD__ "VolumeEntry::VolumeEntry"

    unsigned length;
    length = ValidName(name);
    
    if (!length)
        throw ProFUSE::Exception(__METHOD__ ": Invalid volume name.");
    
    _firstBlock = 0;
    _lastBlock = 6;
    _fileKind = kUntypedFile;
    _inode = 1;
    _inodeGenerator = 1;
    
    _fileNameLength = length;
    
    std::memset(_fileName, 0, sizeof(_fileName));
    for (unsigned i = 0; i < _fileNameLength; ++i)
    {
        _fileName[i] = std::toupper(name[i]);
    }
    
    _lastVolumeBlock = device->blocks();
    _fileCount = 0;
    _accessTime = 0;
    _lastBoot = Date::Today(); 
    
    _cache = BlockCache::Create(device);
    _device = device;
    

    for (unsigned i = 2; i < 6; ++i)
    {
        _cache->zeroBlock(i);
    }
    
    void *vp = _cache->acquire(2);
    IOBuffer b(vp, 0x1a);
    
    writeDirectoryEntry(&b);
        
    _cache->release(2, true);
    
    _cache->sync();
}


VolumeEntry::VolumeEntry(Device::BlockDevice *device)
{
    unsigned blockCount;
    ProFUSE::auto_array<uint8_t> buffer(new uint8_t[512]);
    
    
    // read the header block, then load up all the header 
    // blocks.
    
    _device = device;
    _cache = BlockCache::Create(device);
    
    _cache->read(2, buffer.get());

    init(buffer.get());

    // todo -- verify reasonable values.
    
    //printf("%u %u\n", blocks(), _lastBlock - _firstBlock);
    
    blockCount = blocks();
    
    if (blockCount > 1)
    {
        buffer.reset(readDirectoryHeader());
    }
    
    // now load up all the children.
    // if this throws, memory could be lost...

    
    try
    {    
        
        for (unsigned i = 1; i <= _fileCount; ++i)
        {
            std::auto_ptr<FileEntry> child;
            
            //
            child.reset(new FileEntry(buffer.get() + i * 0x1a));
            
            child->setInode(++_inodeGenerator);
            child->_parent = this;
            _files.push_back(child.release());
        } 
    } 
    catch (...)
    {
        std::vector<FileEntry *>::iterator iter;
        for(iter = _files.begin(); iter != _files.end(); ++iter)
        {
            if (*iter) delete *iter;
        }
       
        throw;
    }


}

VolumeEntry::~VolumeEntry()
{

    std::vector<FileEntry *>::iterator iter;
    for(iter = _files.begin(); iter != _files.end(); ++iter)
    {
        if (*iter) delete *iter;
    }
    
    delete _cache;
    // _device is deleted by _cache.
    //delete _device;
}


void VolumeEntry::init(void *vp)
{
    Entry::init(vp);
    _fileNameLength = Read8(vp, 6);
    
    // verify filenamelength <= 7
    // verify fileKind == 0
    
    std::memcpy(_fileName, 7 + (uint8_t *)vp, _fileNameLength);
    
    _lastVolumeBlock = Read16(vp, 0x0e);
    _fileCount = Read16(vp, 0x10);
    _accessTime = Read16(vp, 0x12);
    _lastBoot = Date(Read16(vp, 0x14));
    
    setInode(1);
    _inodeGenerator = 1; 
}


FileEntry *VolumeEntry::fileAtIndex(unsigned i) const
{
    return i < _files.size() ? _files[i] : NULL;
}

FileEntry *VolumeEntry::fileByName(const char *name) const
{
    std::vector<FileEntry *>::const_iterator iter;
    for(iter = _files.begin(); iter != _files.end(); ++iter)
    {
        FileEntry *e = *iter;
        if (::strcasecmp(name, e->name()) == 0) return e;
    }
    return NULL;
}

unsigned VolumeEntry::unlink(const char *name)
{
    unsigned index;

    
    if (_device->readOnly()) return ProFUSE::drvrWrtProt; // WRITE-PROTECTED DISK
    
    for(index = 0; index < _fileCount; ++index)
    {
        FileEntry *e = _files[index];
        if (::strcasecmp(name, e->name()) == 0)
        {
            delete e;
            _files[index] = NULL;
            break;
        }
    }
    if (index == _fileCount) return ProFUSE::fileNotFound; // FILE NOT FOUND

    _files.erase(_files.begin() + index);
    _fileCount--;

    // need to update the header blocks.
    ProFUSE::auto_array<uint8_t> buffer(readDirectoryHeader());


    // update the filecount.
    IOBuffer b(buffer.get(), 512 * blocks());
    writeDirectoryEntry(&b);
 
    // move up all the entries.
    uint8_t *address = buffer.get() + 0x1a + 0x1a * index;
    std::memmove(address, address + 0x1a, 0x1a * (_fileCount - index));
    // zero out the memory on the previous entry.
    std::memset(buffer.get() + 0x1a + _fileCount * 0x1a, 0, 0x1a);

    // now write to disk.
    writeDirectoryHeader(buffer.get());
    
    _cache->sync();
    return 0;
}



unsigned VolumeEntry::krunch()
{
    unsigned prevBlock;
    
    std::vector<FileEntry *>::const_iterator iter;
    
    bool gap = false;
    
    // sanity check to make sure no weird overlap issues.

    prevBlock = lastBlock();
    
    for (iter = _files.begin(); iter != _files.end(); ++iter)
    {
        FileEntry *e = *iter;
        
        unsigned first = e->firstBlock();
        unsigned last = e->lastBlock();
        
        if (first != prevBlock) gap = true;
        
        if (first < prevBlock) 
            return ProFUSE::damagedBitMap;
        
        if (last < first) 
            return ProFUSE::damagedBitMap;
        
        if (first < volumeBlocks()) 
            return ProFUSE::damagedBitMap;
        
        
        prevBlock = last;
        
    }

    
    if (!gap) return 0;
    
    
    // need to update the header blocks.
    ProFUSE::auto_array<uint8_t> buffer(readDirectoryHeader());
    IOBuffer b(buffer.get(), 512 * blocks());
    

    prevBlock = lastBlock();
    
    unsigned offset = 0;
    for (iter = _files.begin(); iter != _files.end(); ++iter, ++offset)
    {
        FileEntry *e = *iter;
        
        b.setOffset(0x1a + 0x1a * offset);
        
        unsigned first = e->firstBlock();
        unsigned last = e->lastBlock();
        
        unsigned blocks = last - first;
        unsigned offset = first - prevBlock;
        
        if (offset == 0)
        {
            prevBlock = last;
            continue;
        }
        
        e->_firstBlock = first - offset;
        e->_lastBlock = last - offset;
        
        e->writeDirectoryEntry(&b);
        
        for (unsigned i = 0; i < blocks; ++i)
        {
            uint8_t buffer[512];
            
            _cache->read(first + i, buffer);
            _cache->write(prevBlock +i, buffer);
            _cache->zeroBlock(first + i);
        }
    }
    
    // now save the directory entries.

    // load up the directory blocks.
    writeDirectoryHeader(buffer.get());
    
    
    _cache->sync();
    
    return 0;
}














void *VolumeEntry::loadBlock(unsigned block)
{
    return _cache->acquire(block);
}
void VolumeEntry::unloadBlock(unsigned block, bool dirty)
{
    return _cache->release(block, dirty);
}

void VolumeEntry::readBlock(unsigned block, void *buffer)
{
    _cache->read(block, buffer);
}
void VolumeEntry::writeBlock(unsigned block, void *buffer)
{
    _cache->write(block, buffer);
}


void VolumeEntry::sync()
{
    _cache->sync();
}

void VolumeEntry::writeDirectoryEntry(IOBuffer *b)
{
    Entry::writeDirectoryEntry(b);
    
    b->write8(0); // reserved
    b->write8(_fileNameLength);
    b->writeBytes(_fileName, 7);
    b->write16(_lastVolumeBlock);
    b->write16(_fileCount);
    b->write16(_accessTime);
    b->write16(_lastBoot);
    
    // rest is reserved.
    b->writeZero(4);
}



uint8_t *VolumeEntry::readDirectoryHeader()
{
    ProFUSE::auto_array<uint8_t> buffer(new uint8_t[512 * blocks()]);
 
    for (unsigned i = 0; i < blocks(); ++i)
    {
        _cache->read(2 + i, buffer.get() + i * 512);
    }
                                        
    return buffer.release();
}

void VolumeEntry::writeDirectoryHeader(uint8_t *buffer)
{
    for (unsigned i = 0; i < blocks(); ++i)
    {
        _cache->write(2 + i, buffer + 512 * i);
    }
}
