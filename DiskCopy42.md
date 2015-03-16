http://www.nulib.com/library/FTN.e00005.htm

I recently encountered a 32-meg image with the following values:

| **offset** | **description** | **value** |
|:-----------|:----------------|:----------|
| +80 | disk format | $ff |
| +81 | format byte | $22 |

## checksum algorithm (c) ##

```

/*
 * length must be even.
 */
uint32_t checksum(uint8_t *data, size_t length)
{
  size_t i;
  uint32_t rv = 0;

  for (i = 0; i < length; i += 2)
  {
    rv += data[i] << 8;
    rv += data[i+1];
    
    rv = (rv >> 1) + (rv << 31);
  }

  return rv;
}

```