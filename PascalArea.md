# Introduction #

An Apple II Pascal Area is a collection of psuedo-volumes (disk images) for use with the Pascal Profile Manager.

| storage\_type | $04 |
|:--------------|:----|
| file\_name | PASCAL.AREA |
| file\_type | $EF |


`key_pointer` is first block of the file.  The file extends to the end of the disk as a contiguous file.  Therefore, only one Pascal Area may exist per ProDOS volume.

References

  * [ProDOS Technote #25](http://www.umich.edu/~archive/apple2/technotes/tn/pdos/TN.PDOS.025)