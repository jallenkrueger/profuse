/*
 *  Disk.h
 *  ProFUSE
 *
 *  Created by Kelvin Sherlock on 12/18/08.
 *
 */
#ifndef __DISK_H__
#define __DISK_H__

#include <fcntl.h>
#include <stdint.h>

#include <vector>

#include "File.h"


enum {
    P8_OK = 0,
    P8_INTERNAL_ERROR,
    P8_INVALID_FORK,
    P8_INVALID_BLOCK,
    P8_INVALID_STORAGE_TYPE,
    P8_CYCLICAL_BLOCK
    
};

enum {
    P8_DATA_FORK = 0,
    P8_RESOURCE_FORK = 1
};


/* flags */
enum {
    P8_DOS_ORDER = 1,
    P8_2MG = 2,
    P8_DC42 = 4
    
};


class Disk {

public:
    ~Disk();
    
    //static Disk *Open2MG(const char *file);
    static Disk *OpenFile(const char *file, unsigned flags);
    

    int Normalize(FileEntry &f, unsigned fork, ExtendedEntry *ee = NULL);
    
    int Read(unsigned block, void *buffer);
    int ReadIndex(unsigned block, void *buffer, unsigned level, off_t offset, unsigned blocks);

    int ReadFile(const FileEntry &f, void *buffer);
    
    void *ReadFile(const FileEntry &f, unsigned fork, uint32_t *size, int * error);
    

    int ReadVolume(VolumeEntry *volume, std::vector<FileEntry> *files);
    int ReadDirectory(unsigned block, SubdirEntry *dir, std::vector<FileEntry> *files);
    
private:
    Disk();
    uint8_t *_data;
    unsigned _offset;
    unsigned _blocks;
    size_t _size;
    
    unsigned _flags;
};

#endif

