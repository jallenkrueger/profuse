
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include <cerrno>


#ifdef __APPLE__
#include <sys/disk.h>
#endif

#ifdef __linux__
#include <sys/mount.h> 
#endif

#ifdef __SUN__
#include <sys/dkio.h>
#endif


#ifdef __FREEBSD__
#include <sys/disk.h>
#endif

#include <Device/RawDevice.h>

#include <ProFUSE/Exception.h>

using namespace Device;

using ProFUSE::Exception;
using ProFUSE::POSIXException;

#ifdef __SUN__
void RawDevice::devSize(int fd)
{
#undef __METHOD__
#define __METHOD__ "RawDevice::devSize"

    struct dk_minfo minfo;

    if (::ioctl(fd, DKIOCGMEDIAINFO, &minfo) < 0)
        throw POSIXException(__METHOD__ ": Unable to determine device size.", errno);
    
    _size = minfo.dki_lbsize * minfo.dki_capacity;
    _blockSize = 512; // not really, but whatever.
    _blocks = _size / 512;
    
}
#endif

#ifdef __APPLE__
void RawDevice::devSize(int fd)
{
#undef __METHOD__
#define __METHOD__ "RawDevice::devSize"

    uint32_t blockSize; // 32 bit
    uint64_t blockCount; // 64 bit
    
    if (::ioctl(fd, DKIOCGETBLOCKSIZE, &blockSize) < 0)
        throw POSIXException(__METHOD__ ": Unable to determine block size.", errno);
    
    
    if (::ioctl(fd, DKIOCGETBLOCKCOUNT, &blockCount) < 0)
        throw POSIXException(__METHOD__ ": Unable to determine block count.", errno);
        
    _blockSize = blockSize;
    _size = _blockSize * blockCount;
    _blocks = _size / 512;
}

#endif


#ifdef __linux__

void RawDevice::devSize(int fd)
{
#undef __METHOD__
#define __METHOD__ "RawDevice::devSize"

    int blocks;

    if (::ioctl(fd, BLKGETSIZE, &blocks) < 0)
        throw POSIXException(__METHOD__ ": Unable to determine device size.", errno);
    
    _size = 512 * blocks;
    _blockSize = 512; // 
    _blocks = blocks;
    
}

#endif

// TODO -- FreeBSD/NetBSD/OpenBSD

#ifdef __FREEBSD__

void RawDevice::devSize(int fd)
{
#undef __METHOD__
#define __METHOD__ "RawDevice::devSize"

    unisgned blockSize;
    off_t mediaSize;

    if (::ioctl(fd, DIOCGSECTORSIZE, &blockSize)
        throw POSIXException(__METHOD__ ": Unable to determine block size.", errno);

    if (::ioctl(fd, DIOCGMEDIASIZE, &mediaSize)
        throw POSIXException(__METHOD__ ": Unable to determine media size.", errno);

    _blockSize = blockSize;
    _size = mediaSize;
    _blocks = mediaSize / blockSize;

}

#endif

RawDevice::RawDevice(const char *name, bool readOnly) :
    _file(name, readOnly)
{
#undef __METHOD__
#define __METHOD__ "RawDevice::RawDevice"


    if (!_file.isValid())
    {
        throw new Exception(__METHOD__ ": Invalid file handle.");
    }
    
    _readOnly = readOnly;
    _size = 0;
    _blocks = 0;
    _blockSize = 0;
    
    
    devSize(_file.fd());
}

RawDevice::RawDevice(File& file, bool readOnly) :
    _file(file)
{
#undef __METHOD__
#define __METHOD__ "RawDevice::RawDevice"
    
    
    if (!_file.isValid())
    {
        throw new Exception(__METHOD__ ": Invalid file handle.");
    }
    
    _readOnly = readOnly;
    _size = 0;
    _blocks = 0;
    _blockSize = 0;
    
    
    devSize(_file.fd());    
}


RawDevice::~RawDevice()
{
}


RawDevice *RawDevice::Open(const char *name, bool readOnly)
{
    return new RawDevice(name, readOnly);
}


void RawDevice::read(unsigned block, void *bp)
{
#undef __METHOD__
#define __METHOD__ "RawDevice::read"

    if (block >= _blocks) throw Exception(__METHOD__ ": Invalid block number.");
    if (bp == 0) throw Exception(__METHOD__ ": Invalid address."); 

    // sun -- use pread
    // apple - read full native block(s) ?

    off_t offset = block * 512;    
    size_t ok = ::pread(_file.fd(), bp, 512, offset);
    
    // TODO -- EINTR?
    if (ok != 512)
        throw ok < 0
            ? POSIXException(__METHOD__ ": Error reading block.", errno)
            : Exception(__METHOD__ ": Error reading block.");
}


void RawDevice::read(TrackSector ts, void *bp)
{
#undef __METHOD__
#define __METHOD__ "RawDevice::read"

    unsigned block = ts.track * 8 + ts.sector / 2;
    if (block >= _blocks) throw Exception(__METHOD__ ": Invalid block number.");
    if (bp == 0) throw Exception(__METHOD__ ": Invalid address."); 

    // sun -- use pread
    // apple - read full native block(s) ?

    off_t offset = (ts.track * 16 + ts.sector) * 256;    
    size_t ok = ::pread(_file.fd(), bp, 256, offset);
    
    // TODO -- EINTR?
    if (ok != 256)
        throw ok < 0
            ? POSIXException(__METHOD__ ": Error reading block.", errno)
            : Exception(__METHOD__ ": Error reading block.");
}


void RawDevice::write(unsigned block, const void *bp)
{
#undef __METHOD__
#define __METHOD__ "RawDevice::write"

    if (block > _blocks) throw Exception(__METHOD__ ": Invalid block number.");

    if (_readOnly)
        throw Exception(__METHOD__ ": File is readonly.");


    off_t offset = block * 512;    
    size_t ok = ::pwrite(_file.fd(), bp, 512, offset);
    
    if (ok != 512)
        throw ok < 0 
            ? POSIXException(__METHOD__ ": Error writing block.", errno)
            : Exception(__METHOD__ ": Error writing block.");
}


void RawDevice::write(TrackSector ts, const void *bp)
{
#undef __METHOD__
#define __METHOD__ "RawDevice::write"

    unsigned block = ts.track * 8 + ts.sector / 2;
    if (block > _blocks) throw Exception(__METHOD__ ": Invalid block number.");

    if (_readOnly)
        throw Exception(__METHOD__ ": File is readonly.");


    off_t offset = (ts.track * 16 + ts.sector) * 256;    
    size_t ok = ::pwrite(_file.fd(), bp, 256, offset);
    
    if (ok != 256)
        throw ok < 0 
            ? POSIXException(__METHOD__ ": Error writing block.", errno)
            : Exception(__METHOD__ ": Error writing block.");
}


bool RawDevice::readOnly()
{
    return _readOnly;
}

bool RawDevice::mapped()
{
    return false;
}


unsigned RawDevice::blocks()
{
    return _blocks;
}


void RawDevice::sync()
{
#undef __METHOD__
#define __METHOD__ "RawDevice::sync"

    if (_readOnly) return;
    
    if (::fsync(_file.fd()) < 0)
        throw POSIXException(__METHOD__ ": fsync error.", errno);
}
