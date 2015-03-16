# Introduction #

Extended attributes (xattr), while (sort of) supported under linux, are somewhat of a second class citizen.  ProFUSE makes use of them to reveal custom ProDOS information (such as resource forks, file types, and creator types).  For debugging and verification, we present the `xattr` utility

# Compiling #

Note:  OS X has an `xattr` utility (with a different syntax).  You can, of course, compile and use this one as well.

```
g++ xattr.cpp -o xattr
```

# Usage #


`xattr list filename [`_`attribute names`_` ...]`

`xattr dump filename [`_`attribute names`_` ...]`

`xattr read filename attribute`

`xattr write filename attribute`


If no attribute names are specified, all attributes will be listed or dumped, accordingly. `read` and `write` read or write data from `stdin` or `stdout`, respectively.

# Examples #
```

qnap:/usr/local/src/profuse# ./xattr list /mnt/Text/Dynobricks 
com.apple.FinderInfo           32
com.apple.TextEncoding         11
prodos.AuxType                  2
prodos.FileType                 1
prodos.ResourceFork           844
user.charset                    9
user.mime_type                 10

```


```

qnap:/usr/local/src/profuse# ./xattr dump /mnt/Text/Dynobricks 
com.apple.FinderInfo: 32
000000:	70 50 54 45 70 64 6f 73  00 00 00 00 00 00 00 00 	pPTEpdos........
000010:	00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00 	................

com.apple.TextEncoding: 11
000000:	4d 41 43 49 4e 54 4f 53  48 3b 30                	MACINTOSH;0     

prodos.AuxType: 2
000000:	45 54                                            	ET              

prodos.FileType: 1
000000:	50                                               	P               

prodos.ResourceFork: 844
000000:	00 00 00 00 8c 00 00 00  3e 01 00 00 00 00 00 00 	........>.......
000010:	00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00 	................
000020:	00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00 	................
...
000340:	00 00 00 00 5c 00 00 00  0c 00 00 00             	....\.......    

user.charset: 9
000000:	6d 61 63 69 6e 74 6f 73  68                      	macintosh       

user.mime_type: 10
000000:	74 65 78 74 2f 70 6c 61  69 6e                   	text/plain      


```

`xattr` is not ProDOS/ProFUSE specific, so you can use it on other file systems. Assuming you can find other files with extended attributes...