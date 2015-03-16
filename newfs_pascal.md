# NAME #

newfs\_pascal -- Create a new pascal file system

# SYNOPSIS #

`newfs_pascal [-v volume_name] [-s size] [-f format] file`

# DESCRIPTION #

`newfs_pascal` creates a new disk image and format it with an empty apple pascal file system.  `newfs_pascal` may also be used with an actual device (eg, `/dev/disk0s1`), assuming you have appropriate permissions.

# OPTIONS #

| `-v volume_name` | Specify the volume name. Default is based on the file name. |
|:-----------------|:------------------------------------------------------------|
| `-s size` | Specify the file size, in blocks.  May use K or M qualifier.  Default is 1600 blocks (800k) |
| `-f format` | Specify the file format.  Default is based on file name. |