# Introduction #

ProFUSE has been tested with FreeBSD version 7.1, [fuse4bsd](http://fuse4bsd.creo.hu/), 0.3.9, and FUSE 2.7.3

# Compiling #

The fusefs-kmod and fusefs-libs packages must be installed.

```
 make -f makefile.freebsd
```

# Usage #

```

profuse [options] disk-image mount-folder

```

# Limitations #

See the [fuse4bsd](http://fuse4bsd.creo.hu/) site for more details on the FreeBSD FUSE module.

Fuse For FreeBSD does not currently support extended attributes, so they will not be accessible.