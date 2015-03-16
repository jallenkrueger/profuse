# Introduction #

Sapling files have a 1-block index.  There is no resource fork

| storage\_type | $02 |
|:--------------|:----|
| blocks\_used | 1--257 |
| eof | 513--131072 |

The `key_pointer` field is a pointer of 256 blocks, each of which points to 512 bytes of data.

# Sparse Files #

If any block in the index is 0, that block should be considered zero-filled.

# References #