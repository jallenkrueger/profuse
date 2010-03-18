#ifndef __DAVEXDISKIMAGE_H__
#define __DAVEXDISKIMAGE_H__

#include <string>

#include <Device/BlockDevice.h>

namespace Device {

// only supports 1 file; may be split over multiple files.

class DavexDiskImage : public DiskImage {
public:
    
    DavexDiskImage(const char *, bool readOnly);
    virtual ~DavexDiskImage();
    
    static DavexDiskImage *Create(const char *name, size_t blocks);
    static DavexDiskImage *Create(const char *name, size_t blocks, const char *vname);
    static DavexDiskImage *Open(MappedFile *);


private:

    DavexDiskImage(MappedFile *);
    static void Validate(MappedFile *);

    bool _changed;
    std::string _volumeName;    
};


}

#endif
