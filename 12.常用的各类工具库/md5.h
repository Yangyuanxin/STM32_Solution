#ifndef MD5_H
#define MD5_H

/* MD5 context. */
typedef struct
{
  unsigned long int state[4];   /* state (ABCD) */
  unsigned long int count[2];   /* number of bits, modulo 2^64 (lsb first) */
  unsigned char buffer[64];     /* input buffer */
}MD5_CTX;

#ifdef  __cplusplus
extern "C"
{
#endif

  void MD5Init(MD5_CTX *context);
  void MD5Update(MD5_CTX *context, unsigned char *input,unsigned int inputLen);
  void MD5Final(unsigned char digest[16], MD5_CTX *context);


  void md5(unsigned char * src, int src_len, unsigned char * dst);
  void md5s(unsigned char * src, int src_len, char * dst);

#ifdef  __cplusplus
}
#endif

#endif

