#include <algorithm>
#include <cerrno>

#include <sys/stat.h>

#include <File/MappedFile.h>
#include <ProFUSE/Exception.h>


using ProFUSE::POSIXException;

MappedFile::MappedFile()
{
    _length = -1;
    _address = MAP_FAILED;
    _readOnly = true;
}

MappedFile::MappedFile(MappedFile &mf)
{
    _address = mf._address;
    _length = mf._length;
    _readOnly = mf._readOnly;
    
    mf._address = MAP_FAILED;
    mf._length = -1;
    mf._readOnly = true;
}

MappedFile::MappedFile(const File &f, bool readOnly, size_t size)
{
    _length = -1;
    _address = MAP_FAILED;
    _readOnly = readOnly;
    
    init(f, readOnly, size);
}


MappedFile::MappedFile(const char *name, bool readOnly)
{
    File f(name, readOnly);
    
    _length = -1;
    _address = MAP_FAILED;
    _readOnly = readOnly;
    
    init(f, readOnly, -1);
    
}



MappedFile::~MappedFile()
{
    close();
}



void MappedFile::init(const File &f, bool readOnly, size_t size)
{
#undef __METHOD__
#define __METHOD__ "MappedFile::init"
    
    struct stat st;
    
    // close enough
    if (f.fd() < 0)
        throw POSIXException( __METHOD__, EBADF);
    
    
    if (size <= 0)
    {
        if (::fstat(f.fd(), &st) != 0)
            throw POSIXException(__METHOD__ ": fstat", errno);
        
        if (!S_ISREG(st.st_mode))
            throw POSIXException(__METHOD__, ENODEV);
        
        size = st.st_size;
    }
    
    _length = size;
    _address = ::mmap(0, _length, 
                      readOnly ? PROT_READ : PROT_READ | PROT_WRITE, 
                      MAP_FILE | MAP_SHARED, f.fd(), 0); 
    
    if (_address == MAP_FAILED)
        throw POSIXException(__METHOD__ ": mmap", errno);
    
    _readOnly = readOnly;
}



void MappedFile::close()
{
    #undef __METHOD__
    #define __METHOD__ "MappedFile::close"
    
    if (_address != MAP_FAILED)
    {
        void *address = _address;
        size_t length = _length;
        
        _address = MAP_FAILED;
        _length = -1;
        _readOnly = true;
        
        if (::munmap(address, length) != 0)
            throw POSIXException(__METHOD__ ": munmap", errno);
    }
}

void MappedFile::sync()
{
    #undef __METHOD__
    #define __METHOD__ "MappedFile::sync"
    
    if (_address != MAP_FAILED)
    {
        if (::msync(_address, _length, MS_SYNC) != 0)
            throw POSIXException(__METHOD__ ": msync", errno);
    }
}

void MappedFile::adopt(MappedFile &mf)
{
    close();
    _address = mf._address;
    _length = mf._length;
    _readOnly = mf._readOnly;
    
    mf._address = MAP_FAILED;
    mf._length = -1;
    mf._readOnly = true;
}

void MappedFile::swap(MappedFile &mf)
{
    std::swap(_address, mf._address);
    std::swap(_length, mf._length);
    std::swap(_readOnly, mf._readOnly);
}


static MappedFile *Create(const char *name, size_t size)
{
#undef __METHOD__
#define __METHOD__ "MappedFile::Create"
    
    File fd(::open(name, O_CREAT | O_TRUNC | O_RDWR, 0644));

    if (!fd.isValid())
    {
        throw POSIXException(__METHOD__ ": Unable to create file.", errno);        
    }

    // TODO -- is ftruncate portable?
    if (::ftruncate(fd.fd(), size) < 0)
    {
        // TODO -- unlink?
        throw POSIXException(__METHOD__ ": Unable to truncate file.", errno);    
    }
    
    return new MappedFile(fd, false, size);
}