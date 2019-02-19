#include "blowfish.h"


#define BF_PTR2

#if defined(BF_PTR2)

#define BF_ENC(LL,R,KEY,Pi)(\
	LL ^= KEY[Pi],\
	t = KEY[BF_ROUNDS+2	+	0	+((R>>24)&0xFF)],\
	t += KEY[BF_ROUNDS+2	+	256	+((R>>16)&0xFF)],\
	t ^= KEY[BF_ROUNDS+2	+	512	+((R>>8)&0xFF)],\
	t += KEY[BF_ROUNDS+2	+	768	+((R)&0xFF)],\
	LL ^= t\
)
#elif defined(BF_PTR)


#ifndef BF_LONG_LOG2
#define BF_LONG_LOG2 2
#endif

#define BF_M    (0xFF<<BF_LONG_LOG2)
#define BF_0    (24-BF_LONG_LOG2)
#define BF_1    (16-BF_LONG_LOG2)
#define BF_2     (8-BF_LONG_LOG2)
#define BF_3     BF_LONG_LOG2 /* left shift */

#define BF_ENC(LL,R,S,P) \
	LL^=P; \
	LL^= (((*(BF_LONG *)((unsigned char *)&(S[  0])+((R>>BF_0)&BF_M))+ \
	*(BF_LONG *)((unsigned char *)&(S[256])+((R>>BF_1)&BF_M)))^ \
	*(BF_LONG *)((unsigned char *)&(S[512])+((R>>BF_2)&BF_M)))+ \
	*(BF_LONG *)((unsigned char *)&(S[768])+((R<<BF_3)&BF_M)));
#else
#define BF_ENC(LL,R,S,P) \
	LL^=P; \
	LL^=((( S[        ((int)(R>>24L)&0xff) ] + \
	S[0x0100+((int)(R>>16L)&0xff)])^ \
	S[0x0200+((int)(R>> 8L)&0xff)])+ \
	S[0x0300+((int)(R     )&0xff)])&0xffffffffL;
#endif


void BF_encrypt(BF_LONG *data,BF_KEY *key)
{
#ifndef BF_PTR2
	register BF_LONG l,r;
	const register BF_LONG *p,*s;

	p=key->P;
	s= &(key->S[0]);
	l=data[0];
	r=data[1];

	l^=p[0];
	BF_ENC(r,l,s,p[ 1]);
	BF_ENC(l,r,s,p[ 2]);
	BF_ENC(r,l,s,p[ 3]);
	BF_ENC(l,r,s,p[ 4]);
	BF_ENC(r,l,s,p[ 5]);
	BF_ENC(l,r,s,p[ 6]);
	BF_ENC(r,l,s,p[ 7]);
	BF_ENC(l,r,s,p[ 8]);
	BF_ENC(r,l,s,p[ 9]);
	BF_ENC(l,r,s,p[10]);
	BF_ENC(r,l,s,p[11]);
	BF_ENC(l,r,s,p[12]);
	BF_ENC(r,l,s,p[13]);
	BF_ENC(l,r,s,p[14]);
	BF_ENC(r,l,s,p[15]);
	BF_ENC(l,r,s,p[16]);
#if BF_ROUNDS == 20
	BF_ENC(r,l,s,p[17]);
	BF_ENC(l,r,s,p[18]);
	BF_ENC(r,l,s,p[19]);
	BF_ENC(l,r,s,p[20]);
#endif
	r^=p[BF_ROUNDS+1];

	data[1]=l&0xffffffffL;
	data[0]=r&0xffffffffL;
#else
	register BF_LONG l,r,t,*k;

	l=data[0];
	r=data[1];

	k =(BF_LONG *)key;

	l^=k[0];
	BF_ENC(r,l,k, 1);
	BF_ENC(l,r,k, 2);
	BF_ENC(r,l,k, 3);
	BF_ENC(l,r,k, 4);
	BF_ENC(r,l,k, 5);
	BF_ENC(l,r,k, 6);
	BF_ENC(r,l,k, 7);
	BF_ENC(l,r,k, 8);
	BF_ENC(r,l,k, 9);
	BF_ENC(l,r,k,10);
	BF_ENC(r,l,k,11);
	BF_ENC(l,r,k,12);
	BF_ENC(r,l,k,13);
	BF_ENC(l,r,k,14);
	BF_ENC(r,l,k,15);
	BF_ENC(l,r,k,16);
#if BF_ROUNDS == 20
	BF_ENC(r,l,k,17);
	BF_ENC(l,r,k,18);
	BF_ENC(r,l,k,19);
	BF_ENC(l,r,k,20);
#endif
	r^=k[BF_ROUNDS+1];

	data[1]=l&0xffffffffL;
	data[0]=r&0xffffffffL;
#endif

}

#ifndef BF_DEFAULT_OPTIONS

void BF_decrypt(BF_LONG *data,BF_KEY *key)
{
#ifndef BF_PTR2
	register BF_LONG l,r;
	const register *p,*s;

	p=key->P;
	s= &(key->S[0]);
	l=data[0];
	r=data[1];

	l^=p[BF_ROUNDS+1];
#if BF_ROUNDS == 20
	BF_ENC(r,l,s,p[20]);
	BF_ENC(l,r,s,p[19]);
	BF_ENC(r,l,s,p[18]);
	BF_ENC(l,r,s,p[17]);
#endif
	BF_ENC(r,l,s,p[16]);
	BF_ENC(l,r,s,p[15]);
	BF_ENC(r,l,s,p[14]);
	BF_ENC(l,r,s,p[13]);
	BF_ENC(r,l,s,p[12]);
	BF_ENC(l,r,s,p[11]);
	BF_ENC(r,l,s,p[10]);
	BF_ENC(l,r,s,p[ 9]);
	BF_ENC(r,l,s,p[ 8]);
	BF_ENC(l,r,s,p[ 7]);
	BF_ENC(r,l,s,p[ 6]);
	BF_ENC(l,r,s,p[ 5]);
	BF_ENC(r,l,s,p[ 4]);
	BF_ENC(l,r,s,p[ 3]);
	BF_ENC(r,l,s,p[ 2]);
	BF_ENC(l,r,s,p[ 1]);
	r^=p[0];

	data[1]=l&0xffffffffL;
	data[0]=r&0xffffffffL;
#else

	register BF_LONG l,r, t, *k;


	l=data[0];
	r=data[1];
	
	k=(BF_LONG *)key;

	l^=k[BF_ROUNDS+1];
#if BF_ROUNDS == 20
	BF_ENC(r,l,k,20);
	BF_ENC(l,r,k,19);
	BF_ENC(r,l,k,18);
	BF_ENC(l,r,k,17);
#endif
	BF_ENC(r,l,k,16);
	BF_ENC(l,r,k,15);
	BF_ENC(r,l,k,14);
	BF_ENC(l,r,k,13);
	BF_ENC(r,l,k,12);
	BF_ENC(l,r,k,11);
	BF_ENC(r,l,k,10);
	BF_ENC(l,r,k, 9);
	BF_ENC(r,l,k, 8);
	BF_ENC(l,r,k, 7);
	BF_ENC(r,l,k, 6);
	BF_ENC(l,r,k, 5);
	BF_ENC(r,l,k, 4);
	BF_ENC(l,r,k, 3);
	BF_ENC(r,l,k, 2);
	BF_ENC(l,r,k, 1);
	r^=k[0];

	data[1]=l&0xffffffffL;
	data[0]=r&0xffffffffL;
#endif

}

#endif



