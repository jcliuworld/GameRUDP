/**
*
*
* DES 算法
*
*/

#ifndef HEADER_DES_H
#define HEADER_DES_H


#include <stdio.h>

#define DES_ENCRYPT	1
#define DES_DECRYPT	0

#ifndef DES_LONG
#define DES_LONG unsigned int
#endif

typedef unsigned char DES_cblock[8];
typedef unsigned char const_DES_cblock[8];
typedef DES_LONG t_DES_SPtrans[8][64];

extern  t_DES_SPtrans DES_SPtrans;

typedef struct DES_ks
{
	union
	{
		DES_cblock cblock;
		/* make sure things are correct size on machines with
		* 8 byte longs */
		DES_LONG deslong[2];
	} ks[16];
} DES_key_schedule;

extern const DES_LONG (*sp)[8][64];

#define ROTATE(a,n)  (((a)>>(int)(n)) + ((a)<<(32 - (int)(n))))
#define DES_KEY_SZ 	(sizeof(DES_cblock))

#define LOAD_DATA(R,S,u,t,E0,E1,tmp) \
	u = R^s[S    ]; \
	t = R^s[S + 1]

#define LOAD_DATA_tmp(a,b,c,d,e,f)  LOAD_DATA(a, b, c, d, e, f, g)

#define D_ENCRYPT(LL,R,S)                 \
{                                         \
	LOAD_DATA_tmp(R, S, u, t, E0, E1);    \
	t = ROTATE(t, 4);                     \
	LL ^= \
	(*sp)[0][(u>> 2L) & 0x3F]^ \
	(*sp)[2][(u>>10L) & 0x3F]^ \
	(*sp)[4][(u>>18L) & 0x3F]^ \
	(*sp)[6][(u>>26L) & 0x3F]^ \
	(*sp)[1][(t>> 2L) & 0x3F]^ \
	(*sp)[3][(t>>10L) & 0x3F]^ \
	(*sp)[5][(t>>18L) & 0x3F]^ \
	(*sp)[7][(t>>26L) & 0x3F]; \
}


#define PERM_OP(a,b,t,n,m)  ((t) = ((((a)>>(n)) ^ (b)) & (m)), \
	(b) ^= (t),                       \
	(a) ^= ((t)<<(n)))

#define IP(l,r)                        \
{                                      \
	register DES_LONG tt;              \
	\
	PERM_OP(r, l, tt,  4, 0x0F0F0F0F); \
	PERM_OP(l, r, tt, 16, 0x0000FFFF); \
	PERM_OP(r, l, tt,  2, 0x33333333); \
	PERM_OP(l, r, tt,  8, 0x00FF00FF); \
	PERM_OP(r, l, tt,  1, 0x55555555); \
}

#define FP(l,r)                        \
{                                      \
	register DES_LONG tt;              \
	\
	PERM_OP(l, r, tt,  1, 0x55555555); \
	PERM_OP(r, l, tt,  8, 0x00FF00FF); \
	PERM_OP(l, r, tt,  2, 0x33333333); \
	PERM_OP(r, l, tt, 16, 0x0000FFFF); \
	PERM_OP(l, r, tt,  4, 0x0F0F0F0F); \
}

#define HPERM_OP(a,t,n,m) ((t)=((((a)<<(16-(n)))^(a))&(m)),\
	(a)=(a)^(t)^(t>>(16-(n))))

#define ITERATIONS 16
#define HALF_INERATIONS 8

#define MAXWRITE	(1024*16)
#define BSIZE		(MAXWRITE+4)

#define c2l(c,l)        (l =((DES_LONG)(*((c)++)))    , \
	l|=((DES_LONG)(*((c)++)))<< 8L, \
	l|=((DES_LONG)(*((c)++)))<<16L, \
	l|=((DES_LONG)(*((c)++)))<<24L)

extern  DES_LONG des_skb[8][64];

typedef void (* f_DES_random_key)(DES_cblock * ret);
typedef void (* f_DES_set_key)(const_DES_cblock * key,DES_key_schedule *schedule);
typedef void (* f_DES_encrypt1)(DES_LONG * data,DES_key_schedule * ks,t_DES_SPtrans * sp, int enc);
typedef void (* f_DES_encrypt3)(DES_LONG * data,DES_key_schedule * ks1,DES_key_schedule * ks2,DES_key_schedule * ks3,t_DES_SPtrans * sp);
typedef void (* f_DES_decrypt3)(DES_LONG * data,DES_key_schedule * ks1,DES_key_schedule * ks2,DES_key_schedule * ks3,t_DES_SPtrans * sp);



#endif


