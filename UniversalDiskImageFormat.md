Please refer to the [documentation](http://web.archive.org/web/20060308055210/http://www.a2central.com/programming/filetypes/ftne00130.html) for any ambiguities.


Universal Disk Images (.2mg or .2img) consist of a 64-byte header followed by disk image data.  An optional comment chunk and creator-specific chunk (in that order) may be present after the image data.

All values are stored in little endian format.

| **Name** | **Offset** | **Size** |
|:---------|:-----------|:---------|
|  magic\_code | $00 | 4 bytes |
| creator\_code | $04  | 4 bytes |
| header\_length | $08 | 2 bytes |
| version | $0a | 2 bytes |
| format | $0c | 4 bytes |
| flags | $10 | 4 bytes |
| blocks | $14 | 4 bytes |
| data\_offset | $18 | 4 bytes |
| data\_length | $1c | 4 bytes |
| comment\_offset | $20 | 4 bytes |
| comment\_length | $24 | 4 bytes |
| creator\_offset | $28 | 4 bytes |
| creator\_length | $2c | 4 bytes |
| reserved | $30 | 16 bytes |


## magic\_code ##

This is the string '2IMG'.

## creator\_code ##
This is a four-character tag which identifies the file creator.  Currently know values are:

| **Application** | **Code** |
|:----------------|:---------|
| Bernie ][ the Rescue | B2TR |
| Catakig | CTKG |
| Sheppy's ImageMaker | ShIm |
| Sweet 16 | WOOF |
| ProFUSE | PRFS |

## header\_length ##
This is the length of the header.  Although the documentation specifies 52 (which excludes reserved bytes), some files are known to use 64.

## version ##
The version number of the image file.  This should be 1.

## image\_format ##
The image data format.  Valid values are:

| **Format** | **Value** |
|:-----------|:----------|
| DOS 3.3 Order | 0 |
| ProDOS Order | 1 |
| Nibblized data | 2 |

## flags ##
Bit flags pertaining to the disk image.  Undefined bits should be set to 0.

| **Bit** | **Description** |
|:--------|:----------------|
| 31 | If this bit is set, the disk is locked. |
| 8 | If this bit is set, bits 0--7 are valid.  If this bit is not set, a <br> volume number of 254 should be assumed  <br>
<tr><td> 0--7 </td><td> The DOS 3.3 volume number (0--254).  Only valid if bit 8 is set </td></tr></tbody></table>

<h2>blocks ##
The number of 512-byte blocks in the disk image.  For Non-ProDOS Order disk images, this field should be 0.

## data\_offset ##
Offset to the first byte of the image data.  The image data must come before the comment data and the creator data (if present).

## data\_length ##
Length of the image data in bytes.  For ProDOS Order disk image, this should be blocks `*` 512.

## comment\_offset ##
Offset to the first byte of the comment data.  May be 0 if there is no comment.

## comment\_length ##
Length of the comment data.  May be 0 if there is no comment.

## creator\_offset ##
Offset to the first byte of the creator-specific data.  May be 0 if there is no creator-specific data.

## creator\_length ##
Length of the creator-specific data.  May be 0 if there is no creator-specific data.