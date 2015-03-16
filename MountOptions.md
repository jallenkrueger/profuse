This is a list of common (and useful) mounting options for ProFUSE.  Some are ProFUSE specific, some are handled by FUSE or MacFUSE (see their documentation for a full list).

Example:

`profuse -odebug disk.raw /mnt/`



# ProFUSE specific #

`--dos-order`

While most ProDOS disk images are in ProDOS order, some people occasionally create them in dos 3.3 order (usually 5.25" disk images with a `.do` or `.dsk` extension).  the `--dos-order` flag will compensate for their screwed up interleaving.


# FUSE/MacFUSE specific #


`-odebug`

Print debugging information for every FUSE call made.

`-oallow_others`

Allow anyone to access the mounted disk image.  Normally, only the user who mounted it has access.

# MacFUSE specifix #

`-olocal`

Mount the disk image as a local disk, which will show up on the desktop (and may have other implications)