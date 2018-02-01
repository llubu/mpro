#ifndef __AES_CIPHER_H__
#define __AES_CIPHER_H__

extern int key_flag;

int aes_init(unsigned char* pwd, unsigned int pwd_len,
	EVP_CIPHER_CTX* en, EVP_CIPHER_CTX* de, int flag);
int aes_encrypt(EVP_CIPHER_CTX* e, int in, int out);
int aes_decrypt(EVP_CIPHER_CTX* d, int in, int out);

#endif

