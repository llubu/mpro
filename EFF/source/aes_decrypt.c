/*** This program decryptes a file based on the *fd* passed to it
*@author dZONE
*version 1.0 DATE 05-02-2012 ***/

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <openssl/evp.h>
# include <openssl/aes.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <string.h>
# include <error.h>
# include <assert.h>
# include "debug.h"
# include "aes_encrypt.h"


int aes_decrypt(EVP_CIPHER_CTX *e,int in,int out)
{
	inlen=0,flen=0,outlen=0;
        char inbuf[SIZE+AES_BLOCK_SIZE];                                /****** CHECK ???? ****/
        char outbuf[SIZE+AES_BLOCK_SIZE];

        if(!EVP_DecryptInit_ex(d, NULL, NULL, NULL, NULL))
        {
                perror("\n Eror in DECinit:");
                return 1;
        }

        while((inlen = read(in,inbuf,SIZE)) >0)
        {
		if(!EVP_DecryptUpdate(d,(unsigned char*)outbuf, &outlen,(unsigned char*)inbuf,inlen))
                {
                        perror("\n Error,DECRYPT_UPDATE:");
                        return 1;
                }
                if((write(out,outbuf,outlen)) != outlen)
                {
                       perror("\n ERROR,Writing dec bytes:");
                       return 1;
                }
        }

        if(!EVP_DecryptFinal_ex(d,(unsigned char*)outbuf,&flen))
        {
                perror("\n Error,DECRYPT_FINAL:");
                return 1;
        }
        if((write(out,outbuf,flen)) != flen)
        {
                perror("\n ERROR,Writng FINAL dec bytes:");
                return 1;
        }
	return 0;
}

