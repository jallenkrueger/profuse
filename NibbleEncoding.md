(Taken from Beneath Apple ProDOS)

# Address Fields #

An examination of the contents of the two types of fields is on order. The Address Field contains the address or identifying information about the Data Field which follows it.  The volume, track, and sector number of any given sector can be thought of as its ``address,'' much like a country, city, and street number might identify a house.  As shown previously in Figure 3.6, there are a number of components which make up the Address Field.  A more detailed illustration is given in Figure 3.12.


| Prologue | Volume | Track | Sector | Checksum | Epilogue |
|:---------|:-------|:------|:-------|:---------|:---------|
| `D5 AA 96` | `XX YY` | `XX YY` | `XX YY` | `XX YY` | `DE AA EB` |

Figure 3.12 Address Field

Each byte of the Address Field is encoded into two bytes when written to the disk.  APPENDIX C describes the ``4 and 4'' method used for Address Field encoding.


The prologue consists of three bytes which form a unique sequence, found in no other component of the track.  This fact enables ProDOS to locate and Address Field with almost no possibility of error.  The three bytes are $D5, $AA, and $96.  The $D5 and $AA are reserved (never written as data), this insuring the uniqueness of the prologue.  The $96, following this unique string, indicates that the data following constitutes an Address Field (as opposed to a Data Field).  The address information follows next, consisting of the volume`*`, track, and sector number and a checksum.  This information is absolutely essential for ProDOS to know where it is positioned on a particular diskette.  The checksum is computed by exclusive-ORing the first three pieces of information, and is used to verify it's integrity.  Lastly follows the epilogue, which contains the three bytes $DE, $AA, and $EB.  The $EB is only partly written during initialization, and is therefore never verified when an Address Field is read.  The epilogue bytes are sometimes referred to as bit-slip marks, which provide added assurance that the drive is still in sync with the bytes on the disk.  These bytes are probably unnecessary, but do provide a means of double checking.

`*` Volume number is a leftover from earlier operating systems and is not used by ProDOS.

# Data Fields #

The other field type is the Data Field.  Much like the Address Field, it consists of a prologue, data, checksum, and an epilogue (refer to Figure 3.13).  The prologue differs only in the third byte.  The bytes are $D5, $AA, and $AD, which again form a unique sequence, enabling ProDOS to locate the beginning of the sector data.  The data consists of 342 bytes of encoded data. (The encoding scheme used is quite complex and is documented in detail in APPENDIX C.) The data is followed by a checksum byte, used to verify the integrity of the data just read.  The epilogue portion of the Data Field is absolutely identical to the epilogue in the Address Field and serves the same function.

| Prologue | User Data | Dhecksum | Epilogue |
|:---------|:----------|:---------|:---------|
| `D5 AA AD` | 342 bytes data (six and two encoded) | `XX` |  `DE AA EB`|

Figure 3.13 Data Field


