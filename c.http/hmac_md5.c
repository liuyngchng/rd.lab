/**
 * https://www.rfc-editor.org/rfc/rfc2104.txt
 */
#include <stdio.h>
#include <strings.h>
#include "md5.h"
#include "utl.h"

void
hmac_md5(text, text_len, key, key_len, digest)
unsigned char*  text;                /* pointer to data stream */
int    			text_len;            /* length of data stream */
unsigned char*  key;                 /* pointer to authentication key */
int    			key_len;             /* length of authentication key */
unsigned char*  digest;              /* caller digest to be filled in */

{
	MD5_CTX context;
	unsigned char k_ipad[65];    	/* inner padding - key XORd with ipad */
	unsigned char k_opad[65];    	/* outer padding - key XORd with opad */
	unsigned char tk[16];
	int i;
	/* if key is longer than 64 bytes reset it to key=MD5(key) */
	if (key_len > 64) {
		MD5_CTX      tctx;
		MD5Init(&tctx);
		MD5Update(&tctx, key, key_len);
		MD5Final(&tctx, tk);
		key = tk;
		key_len = 16;
	}

	/*
	 * the HMAC_MD5 transform looks like:
	 *
	 * MD5(K XOR opad, MD5(K XOR ipad, text))
	 *
	 * where K is an n byte key
	 * ipad is the byte 0x36 repeated 64 times
	 * opad is the byte 0x5c repeated 64 times
	 * and text is the data being protected
	 */

	/* start out by storing key in pads */
	bzero( k_ipad, sizeof k_ipad);
	bzero( k_opad, sizeof k_opad);
	bcopy( key, k_ipad, key_len);
	bcopy( key, k_opad, key_len);

	/* XOR key with ipad and opad values */
	for (i=0; i<64; i++) {
		k_ipad[i] ^= 0x36;
		k_opad[i] ^= 0x5c;
	}
	/*
	 * perform inner MD5
	 */
	MD5Init(&context);                   	/* init context for 1st pass */
	MD5Update(&context, k_ipad, 64);      	/* start with inner pad */
	MD5Update(&context, text, text_len); 	/* then text of datagram */
	MD5Final(&context, digest);          	/* finish up 1st pass */
	/*
	 * perform outer MD5
	 */
	MD5Init(&context);                   	/* init context for 2nd pass */
	MD5Update(&context, k_opad, 64);     	/* start with outer pad */
	MD5Update(&context, digest, 16);     	/* then results of 1st hash */
	MD5Final(&context, digest);          	/* finish up 2nd pass */
}


