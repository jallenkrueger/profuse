# NAME #

apfm -- Apple Pascal File Manager

# SYNOPSIS #

`apfm [-h] [-f format] disk_image action [action args]`

| -h | Show Usage Information. |
|:---|:------------------------|
| -f | Specify image format. |

# ACTIONS #

`ls [-l]`

List files.  The `-l` flag will perform an extended listing.

```
$ apfm UCSD_Pascal_1.2_3.DSK ls

APPLE3:
SYSTEM.APPLE      32 25-Dec-83
FORMATTER.CODE     5 25-Dec-83
FORMATTER.DATA     6 25-Dec-83
BINDER.CODE        5 25-Dec-83
LINEFEED.CODE      2 25-Dec-83
LINEFEED.TEXT      4 17-May-82
II40.MISCINFO      1 25-Dec-83
II80.MISCINFO      1 25-Dec-83
IIE40.MISCINFO     1 25-Dec-83
BALANCED.TEXT     12 22-Jun-79
CROSSREF.TEXT      8 17-May-82
DISKIO.TEXT       22 14-Aug-80
GRAFCHARS.TEXT     6 22-Jun-79
GRAFDEMO.TEXT     28  4-May-79
HAZEL.MISCINFO     1 19-Mar-79
HAZELGOTO.TEXT     4 29-Mar-79
HILBERT.TEXT       6  4-May-79
SETUP.CODE        33  7-Feb-79
SPIRODEMO.TEXT     6  4-May-79
TREE.TEXT          8 22-Jun-79
128K.APPLE        32 25-Dec-83
128K.PASCAL       45 25-Dec-83
22/22 files<listed/in-dir>, 274 blocks used, 6 unused, 6 in largest
```

```
$ apfm UCSD_Pascal_1.2_3.DSK ls -l

APPLE3:
SYSTEM.APPLE      32 25-Dec-83     6   512  Datafile
FORMATTER.CODE     5 25-Dec-83    38   512  Codefile
FORMATTER.DATA     6 25-Dec-83    43   512  Datafile
BINDER.CODE        5 25-Dec-83    49   512  Codefile
LINEFEED.CODE      2 25-Dec-83    54   512  Codefile
LINEFEED.TEXT      4 17-May-82    56   512  Textfile
II40.MISCINFO      1 25-Dec-83    60   192  Datafile
II80.MISCINFO      1 25-Dec-83    61   192  Datafile
IIE40.MISCINFO     1 25-Dec-83    62   192  Datafile
BALANCED.TEXT     12 22-Jun-79    63   512  Textfile
CROSSREF.TEXT      8 17-May-82    75   512  Textfile
DISKIO.TEXT       22 14-Aug-80    83   512  Textfile
GRAFCHARS.TEXT     6 22-Jun-79   105   512  Textfile
GRAFDEMO.TEXT     28  4-May-79   111   512  Textfile
HAZEL.MISCINFO     1 19-Mar-79   139   192  Datafile
HAZELGOTO.TEXT     4 29-Mar-79   140   512  Textfile
HILBERT.TEXT       6  4-May-79   144   512  Textfile
SETUP.CODE        33  7-Feb-79   150   512  Codefile
SPIRODEMO.TEXT     6  4-May-79   183   512  Textfile
TREE.TEXT          8 22-Jun-79   189   512  Textfile
128K.APPLE        32 25-Dec-83   197   512  Datafile
128K.PASCAL       45 25-Dec-83   229   512  Codefile
< UNUSED >         6             274
22/22 files<listed/in-dir>, 274 blocks used, 6 unused, 6 in largest

```


`cat file [...]`

print one or more files to the screen.

```
$ apfm UCSD_Pascal_1.2_3.DSK cat LINEFEED.TEXT
{$SETC APPLE := 2}
{$S-}

{ The Apple Pascal system has a linefeed flag at location BF0F hex.  If
  this flag is set to 255 (FF hex), line feeds will be suppressed.  If
  it is set to 0 (default), line feeds will be passed. }
  
PROGRAM LINEFEED;

  TYPE PA=PACKED ARRAY[0..1] OF 0..255;
       TWOFACE=RECORD CASE BOOLEAN OF
                 TRUE:(INT:INTEGER);
                 FALSE:(PTR:^PA);
               END;
               
  VAR CHEAT:TWOFACE;
  
BEGIN
  CHEAT.INT:=-16625; {BF0F hex}
  CHEAT.PTR^[0]:=255 {Set flag}
END.

```