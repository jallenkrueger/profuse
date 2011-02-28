#ifndef __DAVEXDISKIMAGE_H__
#define __DAVEXDISKIMAGE_H__

#include <string>

#include <Device/BlockDevice.h>
#include <Device/DiskImage.h>


namespace Device {

// only supports 1 file; may be split over multiple files.

class DavexDiskImage : public DiskImage {
public:
    
    virtual ~DavexDiskImage();
    
    static BlockDevicePointer Create(const char *name, size_t blocks);
    static BlockDevicePointer Create(const char *name, size_t blocks, const char *vname);
    static BlockDevicePointer Open(MappedFile *);

    virtual BlockCachePointer createBlockCache();

private:

    DavexDiskImage();
    
    DavexDiskImage(MappedFile *);
    static void Validate(MappedFile *);

    bool _changed;
    std::string _volumeName;    
};


}

#endif
