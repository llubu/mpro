/***
 * This implements the aes256cbc from *OpenSSL*
 * crypto library to encrypt/decrypt a file
 * @author dZONE
 * DATE 04-29-2012 v1.0
 ***/

# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <string.h>
# include <assert.h>
# include <error.h>

# include <openssl/evp.h>
# include <openssl/aes.h>
# include <sys/types.h>
# include <sys/stat.h>

# include "debug.h"

# define SIZE    (1024u)

int aes_init(unsigned char* pwd, unsigned int pwd_len, unsigned char* salt,
	EVP_CIPHER_CTX* e_ctx, EVP_CIPHER_CTX* d_ctx)
{
	int i;
	int rounds = 5;
	unsigned char key[32];
	unsigned char iv[32];

	i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(),
		salt, pwd, pwd_len, rounds, key, iv);

	if (i != 32)
	{
		printf("\n Error,Incorrect key size generated:%d:\n",i);
		return -1;
	}

	EVP_CIPHER_CTX_init(e_ctx);
	EVP_EncryptInit_ex(e_ctx, EVP_aes_256_cbc(), NULL, key, iv);
	EVP_CIPHER_CTX_init(d_ctx);
	EVP_DecryptInit_ex(d_ctx, EVP_aes_256_cbc(), NULL, key, iv);
	return 0;
}

int aes_encrypt(EVP_CIPHER_CTX* e, int in, int out )
{
	char inbuf[SIZE];
	char outbuf[SIZE + AES_BLOCK_SIZE];
	int inlen = 0;
	int flen = 0;
	int outlen = 0;

	// allows reusing of e for multiple cipher cycles
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
		if (write(out, outbuf, outlen) != outlen)
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
	char inbuf[SIZE + AES_BLOCK_SIZE];
	char outbuf[SIZE + AES_BLOCK_SIZE];

	if (!EVP_DecryptInit_ex(d, NULL, NULL, NULL, NULL))
	{
		perror("\n Eror in DECinit:");
		return 1;
	}

	while ((inlen = read(in,inbuf,SIZE)) > 0)
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

	if(!EVP_DecryptFinal_ex(d, outbuf, &flen))
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

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		perror("\n Error:\nCorrect Usage: Enter Password to be used");
		exit(-1);
	}

	EVP_CIPHER_CTX en,de;					/* The EVP structure which keeps track of all crypt operations see evp.h for details */
	int in, out, fd, dec;					/* fd for input and output files and random dev*/
	unsigned int pwd_len = strlen((const char *)argv[1]);			/* Length of the pwd supplied by the user */
	unsigned char *pwd =(unsigned char*) argv[1];			/* Pointer to the pwd supplied by the user */
	unsigned char salt[8];

	if((in = open("plain.txt",O_RDONLY)) == -1) 		 /* Opening a plain text file for encryption */
	{
		perror("\n Error,Opening file for reading::");
		exit(-1);
	}

	if((fd = open("/dev/random", O_RDONLY)) == -1)
	{
		perror("\n Error,Opening /dev/random::");
		exit(-1);
	}
	else{
		if(read(fd,salt,8) == -1)
		{
			perror("\n Error,reading from /dev/random::");
			exit(-1);
		}
	}

	if(aes_init(pwd,pwd_len,(unsigned char*) salt,&en,&de))		/* Generating Key and IV and initializing the EVP struct */
	{
		perror("\n Error, Cant initialize key and IV:");
		return -1;
	}

	if((out = open("encrypt.txt",O_RDWR|O_CREAT,0400|0200)) == -1)
	{
		perror("\n Error,Opening the file to be written::");
		exit(-1);
	}	
	if((dec = open("dec22.txt",O_RDWR|O_CREAT,0400|0200)) == -1)
	{
		perror("\n ERROR,Opening the file to write decrypted bytes::");
		exit(-1);
	}
	if(aes_encrypt(&en,in,out))
	{
		perror("\n ERROR,ENCRYPTING:");
		exit(-1);
	}
	else
	{
		if((lseek(out,0,SEEK_SET)) != 0)
		{
			perror("\n ERROR,lseek:");
			exit(-1);
		}
		if(aes_decrypt(&de,out,dec))
		{
			perror("\n ERROR,DECRYPTING DATA:");
			exit(-1);
		}
	}

	close(in);
	close(out);
	close(fd);
	close(dec); 
	EVP_CIPHER_CTX_cleanup(&en);
	EVP_CIPHER_CTX_cleanup(&de);
	return 0;
}
