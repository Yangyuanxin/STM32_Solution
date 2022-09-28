#ifndef SHA1_H
#define SHA1_H

typedef struct
{
  unsigned long state[5];
  unsigned long count[2];
  unsigned char buffer[64];
}SHA1_CTX;


#ifdef __cplusplus
extern "C"
{
#endif

  void SHA1Init(SHA1_CTX* context);
  void SHA1Update(SHA1_CTX* context, unsigned char* data, unsigned int len);
  void SHA1Final(unsigned char digest[20], SHA1_CTX* context);
  
  void sha1(unsigned char *src, int srclen, unsigned char *des);
  void sha1s(unsigned char *src, int srclen, char *des);

#ifdef __cplusplus
}
#endif

#endif

