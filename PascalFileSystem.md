# Introduction #

UCSD Pascal was a language/virtual machine/OS/File System developed by the University of California, San Diego.  Apple sold an Apple II implementation/port.  IIgs System 6 included a read-only Apple Pascal FST.

The Pascal File system used 512k blocks (like ProDOS) and 2-byte block pointers (which implies a maximum disk size of 32 Meg, like ProDOS).  Files don't store their file size per se, but rather the start block, end block, and size of last block.  This implies a maximum file size of 32 Meg, minus the overhead of boot and volume blocks.


Block 0 and 1 contain boot code.  Blocks 2--5 contain the volume header and up to 77 file entries.  When a file is deleted, all subsequent records are moved up.

# Volume Header #

All values are little-endian unless specified otherwise.  All blocks are 512 bytes in length.

| **Name** | **Offset** | **Size** |
|:---------|:-----------|:---------|
| first\_block | $00 | 2 bytes |
| last\_block | $02 | 2 bytes |
| file\_kind | $04 | 1 byte |
| reserved | $05 | 1 byte |
| volume\_name | $06 | 8 bytes |
| volume\_last\_block | $0e | 2 bytes |
| file\_count | $10 | 2 byte |
| load\_time | $12 | 2 bytes |
| boot\_date | $14 | 2 bytes |
| reserved | $16 | 4 bytes |

## fields ##
### first\_block ###

The first block of the file system header (boot block and directory blocks) This should always be 0.

### last\_block ###

The block _after_ the last block of the directory structure.  (i.e., the next available block). This should always be 6.

### file\_kind ###

This should always be 0.

### volume\_name ###

The volume name, stored with a leading length byte (i.e., a pstring).  May be 1--7 characters in length.


### volume\_last\_block ###
The number of blocks on the volume.

### file count ###

The number of files.  May be 0--77.

### load\_time ###

_????_

### boot\_date ###

The date last booted.


# File Entry #

| **Name** | **Offset** | **Size** |
|:---------|:-----------|:---------|
| first\_block | $00 | 2 bytes |
| last\_block | $02 | 2 bytes |
| file\_kind | $04 | 1 byte |
| status | $05 | 1 byte |
| file\_name | $06 | 16 bytes |
| last\_byte | $16 | 2 bytes |
| modification\_date | $18 | 2 bytes |


## fields ##

### first\_block ###

The first block of the file

### last\_block ###


The block _after_ the last file block  (i.e., the next available block).

### file\_kind ###

| **Name** | **Value** | **Use** |
|:---------|:----------|:--------|
| untypedfile | $00 | Volume Header |
| xdskfile | $01 | Bad blocks |
| codefile | $02 | Machine executable code |
| textfile | $03 | Text |
| infofile | $04 | Debugger Information |
| datafile | $05 | Data |
| graffile  | $06 | Compressed graphic image `*` |
| fotofile | $07 | Screen image |
| securedir | $08 | Subdirectory [???] `*` |

`*` Not implemented.

### status ###

Used by wildcard file listings [???]

N.B. -- bit 0 is the status bit, all other bits are reserved.

### file\_name ###

The file name, stored with a leading length byte (i.e., a pstring).  May be 1--15 characters in length.


### last\_byte ###

The number of bytes used in the last block.  Should be 1--512.

### modification\_date ###

The date the file was created or last modified.

# Dates #

Dates are stored in a 2 bytes structure:

| **Field** | **bits** | **range** |
|:----------|:---------|:----------|
| year | 15--9 | 0-100 |
| day |8--4 | 0--31 |
| month | 0--3 | 0 -- 12 |


`yyyy yyyd dddd mmmm`

Year 100 is used to indicate a temporary file.


# File Name Restrictions #

There seems to be a lot of bad information about file name restrictions. [Wikipedia](http://en.wikipedia.org/wiki/Apple_Pascal) states:

_Apple Pascal dropped the length to fifteen characters and allowed only letters, numbers and periods to be used._

The [UCSD Pascal II manual](http://www.bitsavers.org/pdf/univOfCalSanDiego/UCSD_PASCAL_II.0_Manual_Mar79.pdf) states:

_A volume name be be 7 or less characters long and may not contain '=', '$', '?' or ','._

_A legal filename can consist of up to 15 characters.  ... Lower-case letters are translated to upper-case, and blanks and non-printing characters are removed from the filename.  Legal characters for filenames are the alphanumerics and the special characters '-', '/', '\', '_', and '.'.

Research (with the file manager) has shown that the only illegal characters are:

  * $
  * =
  * ?
  * ,
  * whitespace (stripped out)

Additionally, the file manager cannot create (or transfer to) files with a '['.  I did not try creating a '[' file programmatically, so its status is ambiguous.

For volume names, ':' and subsequent characters are stripped.  The illegal filename characters are allowed but confuse the file manager.  You can still refer to the disk by volume number though.


# References #

  * http://groups.google.com/group/comp.sys.apple2.programmer/browse_thread/thread/85b993a1c8a0c504/5faf503ef4be5756?hl=en&ie=UTF-8
  * [Pascal Technote #16](http://www.umich.edu/~archive/apple2/technotes/tn/pasc/TN.PASC.016)