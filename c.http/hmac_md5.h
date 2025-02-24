/**
 * https://www.rfc-editor.org/rfc/rfc2104.txt
 * HMAC: Hash-based Message Authentication Code
 * HMAC是一种利用密码学中的散列函数来进行消息认证的机制，可提供的消息认证包括一下两方面：
 *   (1) 消息完整性认证：能够证明消息内容在传送过程没有被修改;
 *   (2) 信源身份认证：因通信双方共享了认证的密钥，接收方能够认证发送该数据的信源是否与所宣称的一致。
 */
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
