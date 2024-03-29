#ifndef HMAC_MD5_H
#define HMAC_MD5_H

/**
 * text, 		pointer to data stream
 * text_len, 	length of data stream
 * key,			pointer to authentication key
 * key_len, 	length of authentication key
 * digest, 		caller digest to be filled in
 */
void hmac_md5(unsigned char* text, int text_len, unsigned char* key,
	int key_len, unsigned char* digest);

#endif
