# Introduction #

The OS X version of ProFUSE (and MacFUSE) has a few enhancements and pitfalls.

# Details #

The profuse command needs to be run from the commandline (aka Terminal).

```
profuse name_of_disk_image
```


  * you can drag a file onto the terminal window if you don't want to type out the full pathname.
  * Unlike the linux/unix version, a mountpoint is not necessary (one will be created in the /Volumes/ folder for you).
  * In finder, choose Go->Computer (Shift-Command-C) to view mounted disks.  Your ProDOS disk image should appear there.
  * By default, only local disks appear on the Finder desktop.  To make ProDOS disk images show up on the Finder desktop, check "Connected servers" in the Finder preferences panel.