# Introduction #

Seedling files have a data fork that can fit within 1 block (the `key_pointer`) and no resource fork.

| storage\_type | $01 |
|:--------------|:----|
| blocks\_used | 1 |
| eof | 0-512|



# Sparse Files #

Sparse seedling files may have an `eof` that exceed the 512 byte/1 block limit.  Bytes 513--`eof` are considered zero-filled.

# References #