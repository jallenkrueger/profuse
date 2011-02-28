#include <Device/UniversalDiskImage.h>

#include <Endian/Endian.h>
#include <Endian/IOBuffer.h>

#include <ProFUSE/Exception.h>

#include <Cache/MappedBlockCache.h>
#include <Cache/ConcreteBlockCache.h>

using namespace Device;
using namespace LittleEndian;

using ProFUSE::Exception;
using ProFUSE::POSIXException;




UniversalDiskImage::UniversalDiskImage(MappedFile *file) :
    DiskImage(file)
{
    
    // at this point, file is no longer valid.

    uint8_t * data = (uint8_t *)address();
    

    

    _format = Read32(data, 0x0c);
    _flags = Read32(data, 0x10);
    _blocks = Read32(data, 0x14);

    _dataOffset = Read32(data, 0x18);
    _dataLength = Read32(data, 0x1c);
    
    
    setBlocks(_blocks);
    // TODO -- DO, Nibble support.
    setAdaptor(new POAdaptor(_dataOffset + data));
}

UniversalDiskImage *UniversalDiskImage::Create(const char *name, size_t blocks)
{
    // 64-byte header.
    MappedFile *file = MappedFile::Create(name, blocks * 512 + 64);

    uint8_t tmp[64];
    
    IOBuffer header(tmp, 64);
    

    // magic + creator
    header.writeBytes("2IMGPRFS", 8); 
    
    // header size.
    header.write16(64);
    
    // version
    header.write16(1);
    
    //image format -- ProDOS order
    header.write32(1); 
    
    // flags
    header.write32(0);
    
    // # blocks. s/b 0 unless prodos-order
    header.write32(blocks);
    
    // offset to disk data
    header.write32(64);
    
    // data length
    header.write32(512 * blocks);
    
    // comment offset, creator, reserved -- 0.
    header.setOffset(64, true);
    
    std::memcpy(file->address(), header.buffer(), 64);
    

    return new UniversalDiskImage(file);
}

UniversalDiskImage *UniversalDiskImage::Open(MappedFile *file)
{
    Validate(file);
    return new UniversalDiskImage(file);
}


/*
 * TODO -- support dos-order & nibblized
 * TODO -- honor read-only flag.
 *
 */
void UniversalDiskImage::Validate(MappedFile *file)
{
#undef __METHOD__
#define __METHOD__ "UniversalDiskImage::Validate"

    const void *data = file->address();
    size_t size = file->length();
    bool ok = false;
    unsigned blocks = 0;
    unsigned offset = 0;
    
    do {
        
        if (size < 64) break;
        
        if (std::memcmp(data, "2IMG", 4)) break;
        
        // only prodos supported, for now...
        // TODO -- Dos Order, Nibble support.
        if (Read32(data, 0x0c) != 1) break;
        
        blocks = Read32(data, 0x14);
        offset = Read32(data, 0x18);
        
        // file size == blocks * 512
        if (Read32(data, 0x1c) != blocks * 512) break;
        
        if (offset + blocks * 512 > size) break;
        
        ok = true;
    } while (false);
    
    if (!ok)
        throw Exception(__METHOD__ ": Invalid file format.");
    

}


bool UniversalDiskImage::readOnly()
{
    return (_flags & 0x8000000) || DiskImage::readOnly();
}


BlockCachePointer UniversalDiskImage::createBlockCache()
{
    if (_format == 1)
    {
        return MappedBlockCache::Create(shared_from_this(), _dataOffset + (uint8_t *)address());
    }
    
    return DiskImage::createBlockCache();
}

