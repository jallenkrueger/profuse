#include "DiskCopy42Image.h"
#include "MappedFile.h"
#include "Buffer.h"
#include "Endian.h"

#include <cstdio>

using namespace ProFUSE;
using namespace BigEndian;

DiskCopy42Image::DiskCopy42Image(MappedFile *f) :
    DiskImage(f),
    _changed(false)
{
}

DiskCopy42Image::DiskCopy42Image(const char *name, bool readOnly) :
    DiskImage(name, readOnly),
    _changed(false)
{
    Validate(file());
}

DiskCopy42Image::~DiskCopy42Image()
{
    if (_changed)
    {
        MappedFile *f = file();
        void *data = f->fileData();
        
        if (f)
        {
            uint32_t cs = Checksum(f->offset() + (uint8_t *)data, 
                f->blocks() * 512);
                
            Write32(data, 72, cs);
            f->sync();
        }
        // TODO -- checksum
    }
}

uint32_t DiskCopy42Image::Checksum(void *data, size_t size)
{
    uint32_t rv = 0;
    uint8_t *dp = (uint8_t *)data;   
 
    if (size & 0x01) return rv;
    
    for (size_t i = 0; i < size; i += 2)
    {
        rv += dp[i] << 8;
        rv += dp[i+1];
        
        rv = (rv >> 1) + (rv << 31);
    }
    
    return rv;
}

DiskCopy42Image *DiskCopy42Image::Open(MappedFile *f)
{
    Validate(f);
    return new DiskCopy42Image(f);
}

static uint8_t DiskFormat(size_t blocks)
{
    switch (blocks)
    {
    case 800: return 0;
    case 1600: return 1;
    case 1440: return 2;
    case 2880: return 3;
    default: return 0xff;
    }
}

static uint8_t FormatByte(size_t blocks)
{
    switch(blocks)
    {
    case 800: return 0x12;
    default: return 0x22;
    }
}
DiskCopy42Image *DiskCopy42Image::Create(const char *name, size_t blocks)
{
    MappedFile *file = new MappedFile(name, blocks * 512 + 84);
    file->setOffset(84);
    file->setBlocks(blocks);
    
    Buffer header(84);
    
    // name -- 64byte pstring. 
    header.pushBytes("\x08Untitled", 9);
    header.resize(64);
    
    // data size -- number of bytes
    header.push32be(blocks * 512);
    
    // tag size
    header.push32be(0);
    
    // data checksum
    // if data is 0, will be 0.
    //header.push32be(Checksum(file->fileData(), blocks * 512));
    header.push32be(0);
    
    // tag checksum
    header.push32be(0);
    
    // disk format.
    /*
     * 0 = 400k
     * 1 = 800k
     * 2 = 720k
     * 3 = 1440k
     * 0xff = other
     */
    header.push8(DiskFormat(blocks));
    
    // formatbyte
    /*
     * 0x12 = 400k
     * 0x22 = >400k mac
     * 0x24 = 800k appleII
     */
    header.push8(FormatByte(blocks));
    
    // private
    header.push16be(0x100);
    
    std::memcpy(file->fileData(), header.buffer(), 84);
    file->sync();
    
    return new DiskCopy42Image(file);
}

void DiskCopy42Image::Validate(MappedFile *file)
{
    size_t bytes = 0;
    size_t size = file->fileSize();
    const void *data = file->fileData();
    bool ok = false;
    uint32_t checksum;
    
    do {
        if (size < 84) break;
        
        // name must be < 64
        if (Read8(data, 0) > 63) break;
        
        if (Read32(data, 82) != 0x100)
            break;
        
        // bytes, not blocks.
        bytes = Read32(data, 64);
        
        if (bytes % 512) break;
        
        if (size < 84 + bytes) break;
        
        // todo -- checksum.
        checksum = Read32(data, 72);
        
        ok = true;
    } while (false);
    
    
    uint32_t cs = Checksum(64 + (uint8_t *)data, bytes);
    
    if (cs != checksum)
    {
        fprintf(stderr, "Warning: checksum invalid.\n");
    }
    file->reset();
    file->setOffset(64);
    file->setBlocks(bytes / 512);
}

void DiskCopy42Image::write(unsigned block, const void *bp)
{
    DiskImage::write(block, bp);
    _changed = true;
}