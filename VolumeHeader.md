This is the volume directory, as originally defined in ProDOS/SOS.

| **Name** | **Offset** | **Size** |
|:---------|:-----------|:---------|
| storage\_type | $00 | 4 bits (high) |
| name\_length | $00 | 4 bits (low) |
| file\_name | $01 | 15 bytes |
| reserved | $10 | 8 bytes |
| creation | $18 | 4 bytes |
| version | $1c | 1 byte |
| min\_version | $1d | 1 byte |
| access | $1e | 1 byte |
| entry\_length | $1f | 1 byte |
| entries\_per\_block | $20 | 1 byte |
| file\_count | $21 | 2 byte |
| bit\_map\_pointer | $23 | 2 bytes |
| total\_blocks | $25 | 2 bytes |

# Updates #

  * Part of the reserved data is used to store the modification date.  This is not documented but is reportedly specific to the GS/OS ProDOS FST.
  * Part of the reserved data is used to store `file_name` case information as of IIgs System 5.  Refer to [GS/OS Technote #8](http://www.umich.edu/~archive/apple2/technotes/tn/gsos/TN.GSOS.008).


| **Name** | **Offset** | **Size** |
|:---------|:-----------|:---------|
| storage\_type | $00 | 4 bits (high) |
| name\_length | $00 | 4 bits (low) |
| file\_name | $01 | 15 bytes |
| reserved | $10 | 2 bytes |
| last\_mod | $12 | 4 bytes |
| file\_name\_case\_bits | $16 | 2 bytes |
| creation | $18 | 4 bytes |
| version | $1c | 1 byte |
| min\_version | $1d | 1 byte |
| access | $1e | 1 byte |
| entry\_length | $1f | 1 byte |
| entries\_per\_block | $20 | 1 byte |
| file\_count | $21 | 2 byte |
| bit\_map\_pointer | $23 | 2 bytes |
| total\_blocks | $25 | 2 bytes |