ProFUSE -- a FUSE filesystem for ProDOS disk images. Currently read only.

## Macintosh ##

Macintosh users will need to install [MacFUSE](http://code.google.com/p/macfuse/).  An installer disk image (10.4+ x86 and ppc) is available to the right (or a zip file with the binary, if that's your preferences)

### OS X Lion ###

ProFUSE does not work with OS X 10.7 (aka Lion). To be more accurate, MacFUSE is a 32-bit kernel extension.  As of 10.7, the OS X Kernel is 64-bit, MacFUSE is not supported, and therefore ProFUSE does not work.  (Certain computers, particularly recent xserves and Mac Pros used a 64-bit kernel with Snow Leopard, so they would experience the same problem).

It is unclear at this point whether MacFUSE will ever be updated to work with a 64-bit kernel.  I have had luck using [osxfuse](http://osxfuse.github.com/releases/OSXFUSE-2.3.dmg) -- the lion image opener app (1.0.1) is built against osxfuse.

## Linux ##

Linux users should follow the directions in the LinuxSupport page.

## FreeBSD ##

FreeBSD users should follow the directions in the [FreeBSDSupport](http://code.google.com/p/profuse/wiki/FreeBSDSupport) page.