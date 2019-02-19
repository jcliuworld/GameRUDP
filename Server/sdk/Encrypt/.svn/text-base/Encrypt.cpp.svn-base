#include "Encrypt.h"
#include <string.h>

CEnctypt::CEnctypt()
{
	bzero(&key_des,sizeof(key_des));
	bzero(&key_rc5,sizeof(key_rc5));

	haveKey_des = false;
	haveKey_rc5 = false;

	method = ENCODE_NONE;
	enc_mask = dec_mask = 0;
}

void CEnctypt::DES_set_key(const_DES_cblock * key,DES_key_schedule *schedule)
{
	::DES_set_key(key,schedule);
}

void CEnctypt::DES_random_key(DES_cblock * ret)
{
	::DES_random_key(ret);
}

void CEnctypt::DES_encrypt1(DES_LONG * data,DES_key_schedule * ks, int enc)
{
	::DES_encrypt1(data,ks,&DES_SPtrans,enc);
}

void CEnctypt::DES_encrypt3(DES_LONG * data,DES_key_schedule * ks1,DES_key_schedule * ks2,DES_key_schedule * ks3)
{
	::DES_encrypt3(data,ks1,ks2,ks3,&DES_SPtrans);
}
void CEnctypt::DES_decrypt3(DES_LONG * data,DES_key_schedule * ks1,DES_key_schedule * ks2,DES_key_schedule * ks3)
{
	::DES_decrypt3(data,ks1,ks2,ks3,&DES_SPtrans);
}
void CEnctypt::RC5_32_set_key(RC5_32_KEY *key, int len, const unsigned char *data, int rounds)
{
	::RC5_32_set_key(key,len,data,rounds);
}
void CEnctypt::RC5_32_encrypt(RC5_32_INT *d,RC5_32_KEY *key)
{
	::RC5_32_encrypt(d,key);
}
void CEnctypt::RC5_32_decrypt(RC5_32_INT *d,RC5_32_KEY *key)
{
	::RC5_32_decrypt(d,key);
}
void CEnctypt::idea_set_encrypt_key(const unsigned char *key, IDEA_KEY_SCHEDULE *ks)
{
	::idea_set_encrypt_key(key,ks);
}
void CEnctypt::idea_set_decrypt_key(IDEA_KEY_SCHEDULE *ek, IDEA_KEY_SCHEDULE *dk)
{
	::idea_set_decrypt_key(ek,dk);
}
void CEnctypt::idea_encrypt(unsigned long *d, IDEA_KEY_SCHEDULE *ks)
{
	::idea_encrypt(d,ks);
}
void CEnctypt::CAST_set_key(CAST_KEY *key, int len, unsigned char *data)
{
	::CAST_set_key(key,len,data);
}
void CEnctypt::CAST_encrypt(CAST_LONG *data,CAST_KEY *key)
{
	::CAST_encrypt(data,key);
}
void CEnctypt::CAST_decrypt(CAST_LONG *data,CAST_KEY *key)
{
	::CAST_decrypt(data,key);
}

#define  ROTATE_LEFT(x,n)(((x)<<(n))|((x)>>(32-(n))))

int CEnctypt::encdec_des(unsigned char * data,unsigned int nLen,bool enc)
{
	if(( 0 == data)||(haveKey_des)) return -1;

	unsigned int offset = 0;
	while(offset<=nLen - 8)
	{
		if(0x80000000&(enc?enc_mask:dec_mask))
			DES_encrypt1((DES_LONG*)(data+offset),&key_des,enc);
		offset += 8;
		if(enc)
			enc_mask = ROTATE_LEFT(enc_mask,1);
		else
			dec_mask = ROTATE_LEFT(dec_mask,1);

	}
	return nLen - offset;
}

int CEnctypt::encdec_rc5(unsigned char * data,unsigned int nLen,bool enc)
{
	if(( 0 == data)||(haveKey_rc5)) return -1;

	unsigned int offset = 0;
	while(offset<=nLen - 8)
	{
		RC5_32_INT d[2];
		if(0x80000000&(enc?enc_mask:dec_mask))
		{
			memcpy(d,data+offset,sizeof(d));
			if(enc)
				RC5_32_encrypt(d,&key_rc5);
			else
				RC5_32_decrypt(d,&key_rc5);
			memcpy(data+offset,d,sizeof(d));
		}

		offset += sizeof(d);

		if(enc)
			enc_mask = ROTATE_LEFT(enc_mask,1);
		else
			dec_mask = ROTATE_LEFT(dec_mask,1);

	}
	return nLen - offset;
}

void CEnctypt::random_key_des(DES_cblock * ret)
{
	::DES_random_key(ret);
}
void CEnctypt::set_key_des(const_DES_cblock *key)
{
	::DES_set_key(key,&key_des);
	haveKey_des = true;
}
void CEnctypt::set_key_rc5(const unsigned char * data, int nLen,int rouunds = RC5_8_ROUNDS)
{
	::RC5_32_set_key(&key_rc5,nLen,data,rouunds);
	haveKey_rc5 = true;
}

int CEnctypt::encdec(void * data,unsigned int nLen,bool enc)
{
	if(ENCODE_NONE == method) return -1;

	if (ENCODE_DES == method)return encdec_des((unsigned char *)data,nLen,enc);

	if (ENCODE_RC5 == method)return encdec_rc5((unsigned char *)data,nLen,enc);
	
	return -2;
}

void CEnctypt::setEncMethod(encMethod method)
{
	this->method = method;
	enc_mask = dec_mask = 0xffffffff;
}
CEnctypt::encMethod CEnctypt::getEncMethod()const
{
	return method;
}
void CEnctypt::setEncMask(unsigned int m)
{
	enc_mask = m;
}
void CEnctypt::setDecMask(unsigned int m)
{
	dec_mask = m;
}
