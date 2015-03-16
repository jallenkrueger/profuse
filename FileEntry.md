This is the file entry, as originally defined in ProDOS/SOS.

| **Name** | **Offset** | **Size** |
|:---------|:-----------|:---------|
| storage\_type | $00 | 4 bits (high) |
| name\_length | $00 | 4 bits (low) |
| file\_name | $01 | 15 bytes |
| file\_type | $10 | 1 byte |
| key\_pointer | $11 | 2 bytes |
| blocks\_used | $13 | 2 bytes |
| EOF | $15 | 3 bytes |
| creation | $18 | 4 bytes |
| version | $1c | 1 byte |
| min\_version | $1d | 1 byte |
| access | $1e | 1 byte |
| aux\_type | $1f | 2 bytes |
| last\_mod | $21 | 4 bytes |
| header\_pointer | $25 | 2 bytes |

# Updates #

  * The `version`/`min_version` fields were deprecated and replaced with filename case information as of IIgs System 5.  Refer to [GS/OS Technote #8](http://www.umich.edu/~archive/apple2/technotes/tn/gsos/TN.GSOS.008)

| **Name** | **Offset** | **Size** |
|:---------|:-----------|:---------|
| storage\_type | $00 | 4 bits (high) |
| name\_length | $00 | 4 bits (low) |
| file\_name | $01 | 15 bytes |
| file\_type | $10 | 1 byte |
| key\_pointer | $11 | 2 bytes |
| blocks\_used | $13 | 2 bytes |
| EOF | $15 | 3 bytes |
| creation | $18 | 4 bytes |
| file\_name\_case\_bits | $1c | 2 bytes |
| access | $1e | 1 byte |
| aux\_type | $1f | 2 bytes |
| last\_mod | $21 | 4 bytes |
| header\_pointer | $25 | 2 bytes |

# Fields #

## storage\_type ##

The storage type is stored in the high 4 bits.  Valid values are:

| $01 | [Seedling File](SeedlingFile.md) |
|:----|:---------------------------------|
| $02 | [Sapling File](SaplingFile.md) |
| $03 | [Tree File](TreeFile.md) |
| $04 | [Apple II Pascal Area](PascalArea.md) |
| $05 | [Extended File](ExtendedFile.md) |
| $0D | [Directory](DirectoryFile.md) |
| $0E | Subdirectory Key Block |
| $0F | Volume Key Block |

Type 0x04 and 0x05 were not a part of the original ProDOS specification and are not supported under ProDOS 8.  Refer to [ProDOS Technote #25](http://www.umich.edu/~archive/apple2/technotes/tn/pdos/TN.PDOS.025)

Types 0x0E and 0x0F are not used in the File Entry.