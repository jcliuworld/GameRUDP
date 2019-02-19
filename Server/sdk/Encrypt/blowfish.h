#ifndef _BLOWFISH_H__
#define _BLOWFISH_H__

#define BF_ENCRYPT      1
#define BF_DECRYPT      0

#define BF_LONG unsigned long
#define BF_LONG_LOG2 3

#define BF_ROUNDS       16
#define BF_BLOCK        8

typedef struct bf_key_st
{
	BF_LONG P[BF_ROUNDS+2];
	BF_LONG S[4*256];
} BF_KEY;


void  BF_set_key(BF_KEY *key, int len, unsigned char *data);
void  BF_encrypt(BF_LONG *data,BF_KEY *key);
void  BF_decrypt(BF_LONG *data,BF_KEY *key);


typedef void(* f_BF_set_key)(BF_KEY *key, int len, unsigned char *data);
typedef void(* f_BF_encrypt)(BF_LONG *data,BF_KEY *key);
typedef void(* f_BF_decrypt)(BF_LONG *data,BF_KEY *key);


#endif
