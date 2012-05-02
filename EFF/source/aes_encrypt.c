/*** This function encryptes a file *fd* passed to it using AES256CBC
*@author dZONE
*ver 1.0 DATE 05-02-2012 ***/

# include <stdio.h>
# include <stdlib.h>
# include <openssl/evp.h>
# include <openssl/aes.h>
# include <sys/types.h>
# include <unistd.h>
# include <fcntl.h>
# include <ctype.h>
# include <sys/stat.h>
# include <string.h>
# include <assert.h>
# include <error.h>
# include "debug.h"

int aes_encrypt(EVP_CIPHER_CTX *e,int in)
{
        char inbuf [SIZE];
        char outbuf[SIZE+AES_BLOCK_SIZE];
        int inlen = 0,flen=0,outlen =0,out;						/* out is the fd of the temp file to be written with encrypted data*/
	
	if((out = open("/tmp/do_enc.txt",O_RDWR|O_CREAT,0400|0200)) <0)
	{
		perror("\n Error,Opening temp file::");
		return 1;
	}

        if(!EVP_EncryptInit_ex(e, NULL, NULL, NULL, NULL))                              /* allows reusing of e for multiple cipher cycles CHECK ????*/
        {
                perror("\n Error,ENCRYPR_INIT:");
                return 1;
        }
        while((inlen = read(in,inbuf,SIZE)) > 0)
        {
                if(!EVP_EncryptUpdate(e,(unsigned char*) outbuf, &outlen,(unsigned char*) inbuf,inlen))                 /* Update cipher text */
                {
                        perror("\n ERROR,ENCRYPR_UPDATE:");
                        return 1;
                }
                if(write(out,outbuf,outlen) != outlen)
                {
                        perror("\n ERROR,Cant write encrypted bytes to outfile:");
                        return 1;
                }
        }
        if(!EVP_EncryptFinal_ex(e, (unsigned char*) outbuf, &flen))                     /* updates the remaining bytes */
        {
                perror("\n ERROR,ENCRYPT_FINAL:");
                return 1;
        }
        if(write(out,outbuf,flen) != flen)
        {
                perror("\n ERROR,Wriring final bytes of data:");
                return 1;
        }
	
        return 0;
}
	
