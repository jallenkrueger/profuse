# Introduction #

ProFUSE has been tested with Linux 2.6.x/ARM with gcc 4.x and fuse 2.7.4.  For lenny, that means `apt-get install libfuse2 libfuse-dev`.  Etch uses fuse 2.5.3, which is not supported.  You will need to compile and install your own libfuse.


# Compiling #

FUSE 2.6+ must be installed.

```
 make -f makefile.linux
```

# Usage #

```
profuse [options] disk-image mount-folder
```

# Limitations #

The `profuse` command must be run by root (or be setguid root).  By default, the mount dir can only be accessed by the user that mounted it.  If it wasn't you, it will show up as:

```
?--------- ? ?      ?           ?            ? mnt
```

and will be inaccessible.   The `-oallow_other` flag will give everyone access the mount dir.