#ifndef CRC32_H
#define CRC32_H

#ifdef __cplusplus
extern "C"
{
#endif
  
  unsigned int CRC32 ( const unsigned char * pString );
  unsigned int CRC32n ( const unsigned char * pString, int iLen );
  unsigned int CRC32np ( const unsigned char * pString, int iLen, unsigned int uPrevCRC );
  unsigned long long FNV64 ( const unsigned char * s );
  unsigned long long FNV64n ( const unsigned char * s, int iLen );

#ifdef __cplusplus
}
#endif


#endif

