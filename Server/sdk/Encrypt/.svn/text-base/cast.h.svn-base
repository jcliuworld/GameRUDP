#ifndef _CAST_H__
#define _CAST_H__

#define CAST_ENCRYPT    1
#define CAST_DECRYPT    0

#define CAST_LONG unsigned long

#define CAST_BLOCK      8
#define CAST_KEY_LENGTH 16

typedef struct cast_key_st
{
	CAST_LONG data[32];
	int short_key;
} CAST_KEY;

#if defined(OPENSSL_SYS_WIN32)&& defined(_MSC_VER)

#define ROTL(a,n)     (_lrotl(a,n))
#else
#define ROTL(a,n)     ((((a)<<(n))&0xffffffffL)|((a)>>(32-(n))))
#endif


extern CAST_LONG CAST_S_table0[256];
extern CAST_LONG CAST_S_table1[256];
extern CAST_LONG CAST_S_table2[256];
extern CAST_LONG CAST_S_table3[256];
extern CAST_LONG CAST_S_table4[256];
extern CAST_LONG CAST_S_table5[256];
extern CAST_LONG CAST_S_table6[256];
extern CAST_LONG CAST_S_table7[256];



#define E_CAST(n,key,L,R,OP1,OP2,OP3) \
{ \
	CAST_LONG a,b,c,d; \
	t=key[n*2] OP1 R; \
	t=ROTL(t,(key[n*2+1])); \
	a=CAST_S_table0[(t>> 8)&0xff]; \
	b=CAST_S_table1[(t    )&0xff]; \
	c=CAST_S_table2[(t>>24)&0xff]; \
	d=CAST_S_table3[(t>>16)&0xff]; \
	L^=(((a OP2 b) OP3 c) OP1 d); \
}

#define CAST_exp(l,A,a,n) \
	A[n/4]=l; \
	a[n+3]=(l    )&0xff; \
	a[n+2]=(l>> 8)&0xff; \
	a[n+1]=(l>>16)&0xff; \
	a[n+0]=(l>>24);

#define S4 CAST_S_table4
#define S5 CAST_S_table5
#define S6 CAST_S_table6
#define S7 CAST_S_table7

typedef void(* f_CAST_set_key)(CAST_KEY *key, int len, unsigned char *data);
typedef void(* f_CAST_encrypt)(CAST_LONG *data,CAST_KEY *key);
typedef void(* f_CAST_decrypt)(CAST_LONG *data,CAST_KEY *key);

#endif
