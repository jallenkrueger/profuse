This is the directory header, as originally defined in ProDOS/SOS.

| **Name** | **Offset** | **Size** |
|:---------|:-----------|:---------|
| storage\_type | $00 | 4 bits (high) |
| name\_length | $00 | 4 bits (low) |
| file\_name | $01 | 15 bytes |
| reserved | $10 | 8 bytes |
| creation | $18 | 4 bytes |
| version | $1c | 1 byte |
| min\_version | $1d | 1 byte |
| access | $1e | 1 byte |
| entry\_length | $1f | 1 byte |
| entries\_per\_block | $20 | 1 byte |
| file\_count | $21 | 2 byte |
| parent\_pointer  | $23 | 2 bytes |
| parent\_entry\_number  | $25 | 1 byte |
| parent\_entry\_length  | $26 | 1 byte |


# Updates #

  * SOS 1.3 and ProDOS 1.7 source code refer to the reserved data as the password enabled byte/password.  Neither actually implemented password support, though.
  * SOS 1.3 stores $76 in the password\_enabled byte.
  * ProDOS 1.7  stores $75 in the password\_enabled byte.  When reading a directory, the password\_enabled byte MUST have 5 bits set (eg $75 or $76).  Otherwise, error $4a is returned.


| **Name** | **Offset** | **Size** |
|:---------|:-----------|:---------|
| storage\_type | $00 | 4 bits (high) |
| name\_length | $00 | 4 bits (low) |
| file\_name | $01 | 15 bytes |
| password\_enabled | $10 | 1 byte |
| encoded\_password | $11 | 7 bytes |
| creation | $18 | 4 bytes |
| version | $1c | 1 byte |
| min\_version | $1d | 1 byte |
| access | $1e | 1 byte |
| entry\_length | $1f | 1 byte |
| entries\_per\_block | $20 | 1 byte |
| file\_count | $21 | 2 byte |
| parent\_pointer  | $23 | 2 bytes |
| parent\_entry\_number  | $25 | 1 byte |
| parent\_entry\_length  | $26 | 1 byte |