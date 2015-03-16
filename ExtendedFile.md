# Introduction #

Extended files are used for storing files which contain a resource fork.  They are not supported by ProDOS 8.


| storage\_type | $05 |
|:--------------|:----|

The `key_pointer` is a block containing 2 mini entries; one for the data fork (at offset $0), one for the resource fork (at offset $100).


## Mini Entry ##

| **Name** | **Offset** | **Size** |
|:---------|:-----------|:---------|
| storage\_type | $00 | 1 byte |
| key\_block | $01 | 2 bytes |
| blocks\_used | $03 | 2 bytes |
| eof | $05 | 2 bytes |

Unlike the [File Entry](FileEntry.md) `storage_type` field, the high nibble is 0.  The only valid values are $01, $02, or $03.

As of System 6.0, Macintosh finder information can be stored after the data fork mini entry (starting offset of $08).

| **Name** | **Offset** | **Size** | **Value** |
|:---------|:-----------|:---------|:----------|
| size\_1 | $00 | 1 byte | 18 |
| type\_1 | $01 | 1 byte | 1 |
| FInfo | $02 | 16 bytes | ... |
| size\_2 | $12 | 1 byte | 18 |
| type\_2 | $13 | 1 byte | 2 |
| xFInfo | $14 | 16 bytes | ... |
| end\_mark | $24 | 1 byte | 0 |

One or both of FInfo and xFInfo may be missing, in which case they should be considered 0-filled.

# References #

  * [ProDOS Technote #25](http://www.umich.edu/~archive/apple2/technotes/tn/pdos/TN.PDOS.025)
  * Programmer's Reference for 6.0 (pages 324, 325)