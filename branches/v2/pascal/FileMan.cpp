/*
 * FileMan utilities.
 *
 * L - list dir
 * E - 
 */


#include <cstdio>
#include <algorithm>
#include <memory>

#include <unistd.h>

#include "File.h"
#include "DateRec.h"
#include "../BlockDevice.h"


const char *MonthName(unsigned m)
{
    static const char *months[] = {
        "",
        "Jan",
        "Feb",
        "Mar",
        "Apr",
        "May",
        "Jun",
        "Jul",
        "Aug",
        "Sep",
        "Oct",
        "Nov",
        "Dec"
    };

    if (m > 12) return "";
    return months[m];
}

const char *FileType(unsigned ft)
{
    static const char *types[] = {
        "Unknown",
        "Badblocks",
        "Codefile",
        "Textfile",
        "Infofile",
        "Datafile",
        "Graffile",
        "Fotofile",
        "SecureDir"
    
    };
    
    if (ft < 8) return types[ft];
    
    return "";
}

void printUnusedRecord(unsigned block, unsigned size)
{
    std::printf("< UNUSED >      %4u            %4u\n", size, block);
}

void list(Pascal::VolumeEntry *volume, bool extended)
{
    unsigned fileCount = volume->fileCount();
    unsigned used = volume->blocks();
    unsigned max = 0;
    unsigned volumeSize = volume->volumeBlocks();
    unsigned lastBlock = volume->lastBlock();
    
    std::fprintf(stdout, "%s:\n", volume->name()); 
    
    for (unsigned i = 0; i < fileCount; ++i)
    {
        Pascal::FileEntry *e = volume->fileAtIndex(i);
        if (!e) continue;
        
        Pascal::DateRec dt = e->modification();
        
        //TODO -- include gaps.
        
        if (lastBlock != e->firstBlock())
        {
            unsigned size = e->firstBlock() - lastBlock;
            max = std::max(max, size);
        
            if (extended)
            {
                printUnusedRecord(lastBlock, size);
            }
        }
        lastBlock = e->lastBlock();
        
        if (extended)
        {
        
             std::printf("%-15s %4u %2u-%s-%2u %5u %5u  %s\n",
                e->name(), 
                e->blocks(),
                dt.day(),
                MonthName(dt.month()),
                dt.year() % 100,
                e->firstBlock(),
                e->lastByte(),
                FileType(e->fileKind())
            ); 
       
        }
        else
        {
            std::printf("%-15s %4u %2u-%s-%2u\n",
                e->name(), 
                e->blocks(),
                dt.day(),
                MonthName(dt.month()),
                dt.year() % 100
            ); 
        }
        used += e->blocks();
    }

    if (extended && (lastBlock != volumeSize))
    {
        printUnusedRecord(lastBlock, volumeSize - lastBlock);
    }    
    
    
    std::fprintf(stdout, 
        "%u/%u files<listed/in-dir>, "
        "%u blocks used, "
        "%u unused, "
        "%u in largest\n",
        fileCount, fileCount,
        used,
        volumeSize- used,
        max
    );
 

}


void usage()
{
    std::printf(
        "Pascal File Manager v 0.0\n\n"
        "Usage: fileman [-h] [-f format] action diskimage\n"
        "Options:\n"
        "  -h            Show usage information.\n"
        "  -f format     Specify disk format.  Valid values are:\n"
        "                 po: ProDOS order disk image\n"
        "                 do: DOS Order disk image\n"
        "\n"
        "Actions:\n"
        "  L            List files\n"
        "  E            List files (extended)\n"  
    );

}

int main(int argc, char **argv)
{
    std::auto_ptr<Pascal::VolumeEntry> volume;
    std::auto_ptr<ProFUSE::BlockDevice> device;
    
    std::string format;
    
    while ((int c = ::getopt(argc, argv, "f:h")) != -1)
    {
        switch(c)
        {
        case 'f':
            format = optarg;
            break;
            
        case 'h':
        case '?':
            usage();
            exit();
        }
    }
    
    
    const char *file = argv[1];
    
    try {
    
    device.reset( new ProFUSE::DOSOrderDiskImage(file, true));
    
    volume.reset( new Pascal::VolumeEntry(device.get()));
    
    device.release();

    list(volume.get(), true);
    
    }
    catch (ProFUSE::Exception& e)
    {
        std::fprintf(stderr, "%s\n", e.what());
        std::fprintf(stderr, "%s\n", strerror(e.error()));
    }
    

}