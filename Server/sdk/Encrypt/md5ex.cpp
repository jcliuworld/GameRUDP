#include "md5c.h"
#include "md5ex.h"
#include <string.h>
#include <stdio.h>

bool MD5Data(const void *pData,int size,unsigned char * pMD5 )
{
	MD5_CTX context;
	MD5Init(&context);
	MD5Update(&context,(unsigned char *)pData,size);
	MD5Final(pMD5,&context);
	return true;
}

bool MD5File(const char * pszFile, unsigned char * pMD5 )
{
	FILE * fp = fopen(pszFile,"rb");

	if (!fp) return false;

	unsigned char buffer[8192];

	size_t len;

	fseek(fp,0,SEEK_END);
	len = ftell(fp);
	fseek(fp,0,SEEK_SET);
	
	MD5_CTX context;
	MD5Init(&context);

	while(len)
	{
		size_t readLen = (len < 8192 ? len : 8192);
		if (1 != fread(buffer,readLen,1,fp))
		{
			MD5Final(pMD5,&context);
		}
		len -= readLen;
		
		MD5Update(&context,buffer,readLen);
	}

	MD5Final(pMD5,&context);
	fclose(fp);
	return true;
}

bool MD5String(const char * string, unsigned char * pMD5 )
{
	size_t size = strlen(string);
	return MD5Data(string,size,pMD5);
}

