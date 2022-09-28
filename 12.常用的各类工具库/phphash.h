#ifndef PHPHASH_H
#define PHPHASH_H

#define HASH_OK 0
#define HASH_ERROR -1

#define HASH_FOUND 0
#define HASH_NO_FOUND -1

typedef struct tagHASHBUCKET HASHBUCKET, *LPHASHBUCKET;
typedef struct tagHASHTABLE HASHTABLE, *LPHASHTABLE;

struct tagHASHBUCKET
{
  unsigned long h;         /* key name's hash value */
  unsigned int key_length; /* key name length */
  void *data;              /* key value */
  LPHASHBUCKET next;
  LPHASHBUCKET previous;
  LPHASHBUCKET conflict_next;
  LPHASHBUCKET conflict_previous;
  char key[1]; /* key name的长度是不定的，根据实际情况来为key name分配存储空间 */
};

typedef unsigned long(*hash_func_t)(char *, unsigned int); /* hash 函数 */

/* 用来描述hash表的数据结构 */
/* hash表采用地址和数据分离的方法，地址采用顺序存储，数据采用链接存储*/
struct tagHASHTABLE
{
  unsigned int table_size; /* hash表大小*/
  unsigned int size_index; /* hash表大小在size_table中的取值的下标 */
  unsigned int elements;   /* hash表实际存储元素的个数 */
  hash_func_t hash;        /* 使用的hash函数 */
  LPHASHBUCKET p;          /* 未使用 */
  LPHASHBUCKET head;       /* hash表的表头指针 */
  LPHASHBUCKET tail;       /* hash表的表尾指针 */
  LPHASHBUCKET *buckets;   /* 指向bucket指针数组的指针 */
};

#ifdef __cplusplus
extern "C"
{
#endif
  extern int hash_create(LPHASHTABLE, unsigned int, hash_func_t);
  extern int hash_entry(LPHASHTABLE, char *, unsigned int, void *);
  extern int hash_find(LPHASHTABLE, char *, unsigned int, void **);
  extern int hash_update(LPHASHTABLE, char *, unsigned int, void *);
  extern int hash_remove(LPHASHTABLE, char *, unsigned int);
  extern int hash_destroy(LPHASHTABLE);
#ifdef __cplusplus
}
#endif
#endif

