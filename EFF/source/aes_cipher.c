# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <ctype.h>
# include <string.h>
# include <assert.h>
# include <error.h>

# include <openssl/evp.h>
# include <openssl/aes.h>
# include <openssl/rand.h>
# include <sys/types.h>
# include <sys/stat.h>

# include "debug.h"
# include "aes_cipher.h"
# include "keystore.h"

# define SIZE    (1024u)
#define DO_ENCRYPTION    (1u)
#define DO_DECRYPTION    (2u)

int aes_init(unsigned char* pwd, unsigned int pwd_len, EVP_CIPHER_CTX* e_ctx,
	EVP_CIPHER_CTX *d_ctx,int flag)
{
	int i;
	int rounds = 5;
	unsigned char key[32];
	unsigned char iv[32];
	unsigned char salt[8];
	unsigned char data[32];
	int data_len = 32;

	switch(flag)
	{
	case DO_ENCRYPTION:
		if (key_flag == 1)
		{
			if (!(RAND_bytes(salt, 8)))
			{
				perror("\n ERROR,SALT::");
				return 1;
			}
			if (!(RAND_bytes(data, 32)))
			{
				perror("\n ERROR,DATA::");
				return 1;
			}

			i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(),
				salt, data, data_len, rounds, key, iv);
			if (i != 32)
			{
				dbug_p("ERROR,Incorrect key size "
					"generated:%d:\n",i);
				return 1;
			}

			EVP_CIPHER_CTX_init(e_ctx);
			if (!(EVP_EncryptInit_ex(e_ctx, EVP_aes_256_cbc(),
				NULL, key, iv)))
			{
				perror("\n ERROR,ENCRYPT_INIT::");
				return 1;
			}

			if ((creat_keystore(pwd, pwd_len, key, iv)))
			{
				dbug_p("ERROR,cant creat KeyStore\n");
				return 1;
			}
			}
			else
			{
				if (key_flag == 2)
				{
					if ((read_keystore(pwd,
						pwd_len, key, iv)))
					{
						dbug_p("ERROR,in reading "
							"REUSE_KS_AES_INIT\n");
						return 1;
					}

					EVP_CIPHER_CTX_init(e_ctx);
					if (!(EVP_EncryptInit_ex(e_ctx,
						EVP_aes_256_cbc(), NULL, key,
						iv)))
					{
						perror("\n ERROR,ENCRYPT_INIT_"
							"RESUSE_KS::");
						return 1;
					}
				}
			}
			break;
	case DO_DECRYPTION:
		if ((read_keystore(pwd, pwd_len, key, iv)))
		{
			dbug_p("ERROR,in reading KeyStore\n");
			return 1;
		}

		EVP_CIPHER_CTX_init(d_ctx);
		if (!(EVP_DecryptInit_ex(d_ctx, EVP_aes_256_cbc(),
			NULL, key, iv)))
		{
			perror("\n ERROR,DECRYPT_INIT::");
			return 1;
		}
		dbug_p("KEY::%s:: \n",key);
		break;

	default:
		dbug_p("INCORRECT FLAG IN AES_INIT\n");
		return 1;
	}
	return 0;
}

int aes_encrypt(EVP_CIPHER_CTX* e, int in, int out)
{
	char inbuf [SIZE] = {0};
	char outbuf[SIZE + AES_BLOCK_SIZE] = {0};
	int inlen = 0;
	int flen = 0;
	int outlen = 0;

	if (!EVP_EncryptInit_ex(e, NULL, NULL, NULL, NULL))
	{
		perror("\n Error,ENCRYPR_INIT:");
		return 1;
	}

	while ((inlen = read(in, inbuf, SIZE)) > 0)
	{
		if (!EVP_EncryptUpdate(e, outbuf, &outlen, inbuf,inlen))
		{
			perror("\n ERROR,ENCRYPR_UPDATE:");
			return 1;
		}
		if (write(out,outbuf,outlen) != outlen)
		{
			perror("\n ERROR,Cant write encrypted "
				"bytes to outfile:");
			return 1;
		}
	}

	if (!EVP_EncryptFinal_ex(e, outbuf, &flen))
	{
		perror("\n ERROR,ENCRYPT_FINAL:");
		return 1;
	}

	if (write(out, outbuf, flen) != flen)
	{
		perror("\n ERROR,Wriring final bytes of data:");
		return 1;
	}

	return 0;
}

int aes_decrypt(EVP_CIPHER_CTX* d, int in, int out)
{
	int inlen = 0;
	int flen = 0;
	int outlen = 0;
	char inbuf[SIZE + AES_BLOCK_SIZE] = {0};
	char outbuf[SIZE+AES_BLOCK_SIZE] = {0};

	if (!EVP_DecryptInit_ex(d, NULL, NULL, NULL, NULL))
	{
		perror("\n Eror in DECinit:");
		return 1;
	}

	while ((inlen = read(in, inbuf, SIZE)) > 0)
	{
		if (!EVP_DecryptUpdate(d, outbuf, &outlen, inbuf, inlen))
		{
			perror("\n Error,DECRYPT_UPDATE:");
			return 1;
		}
		if ((write(out, outbuf, outlen)) != outlen)
		{
			perror("\n ERROR,Writing dec bytes:");
			return 1;
		}
	}

	if (!EVP_DecryptFinal_ex(d, outbuf, &flen))
	{
		perror("\n Error,DECRYPT_FINAL:");
		return 1;
	}

	if ((write(out, outbuf, flen)) != flen)
	{
		perror("\n ERROR,Writng FINAL dec bytes:");
		return 1;
	}
	return 0;
}

