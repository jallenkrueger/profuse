# Introduction #

ProFUSE maps certain ProDOS metadata to extended attributes, which applications can access.

# Details #

| Attribute | Size |Type of file | Description |
|:----------|:-----|:------------|:------------|
| prodos.FileType |1 byte | All files (excluding directories) | The  ProDOS file type byte |
| prodos.AuxType | 2 bytes | All files (excluding directories) | The ProDOS aux type word, in little endian format |
| prodos.ResourceFork | variable | Extended files | The raw resource fork data |
| com.apple.FinderInfo | 32 bytes | Extended files | Macintosh Finder info |
| com.apple.TextEncoding | variable | Teach Text files | Indicates MacRoman encoding |