
#ifndef HBM_H
#define HBM_H

#ifdef __cplusplus
extern "C" {
#endif

unsigned char * hbm_patternmatch(const unsigned char *text,int n,
                                            const unsigned char *pat,int m,
                                            int no_case,int codetype);

#ifdef __cplusplus
}
#endif

#endif
