/*** This file contains function aes_encrypt() : aes_decrypt() : aes_init() 
*@author dZONE
*ver 1.0 DATE 05-02-2012  05-08-2012 ***/

# include <stdio.h>
# include <stdlib.h>
# include <openssl/evp.h>
# include <openssl/aes.h>
# include <openssl/rand.h>
# include <sys/types.h>
# include <unistd.h>
# include <fcntl.h>
# include <ctype.h>
# include <sys/stat.h>
# include <string.h>
# include <assert.h>
# include <error.h>
# include "debug.h"
# include "aes_cipher.h"
# include "keystore.h"

# define SIZE 1024

/*********************************** CIPHER INITIALIZATION FUNCTION *****************************************************/

int aes_init(unsigned char* pwd, unsigned int pwd_len,EVP_CIPHER_CTX *e_ctx, EVP_CIPHER_CTX *d_ctx,int flag)	/* return 0:SUCCESS 1: ERROR */
{
        int i, rounds =5;                                       /* rounds */
        unsigned char key[32], iv[32], salt[8];

	switch(flag)
	{    	
		case 1:		/* ENCRYPTION INIT */
			if(!(RAND_bytes(salt,8))) 	/* Writes cryptographically secure random bytes in salt[] ????check for no of bytes used */
			{
				perror("\n ERROR,SALT::");
				return 1;
			}

        		i = EVP_BytesToKey(EVP_aes_256_cbc(),EVP_sha1(),salt,pwd,pwd_len,rounds,key,iv);
		        if(i != 32) 									/* Key len should be 256bits*/
        		{	   
                		dbug_p("ERROR,Incorrect key size generated:%d:\n",i);
	                	return 1; 
        		}   
    
        		EVP_CIPHER_CTX_init(e_ctx);
//			dbug_p("\n KEY::%s::IV::%s::SALT::%s::\n",key,iv,salt);   
  	    	        if(!(EVP_EncryptInit_ex(e_ctx, EVP_aes_256_cbc(), NULL, key, iv)))
			{
				perror("\n ERROR,ENCRYPT_INIT::");
				return 1;
			}

			if((creat_keystore((unsigned char*) NULL,key,iv)))                              /* Creating keystore */
	                {   
        	                dbug_p("ERROR,cant creat KeyStore\n");
                	        return 1;
	                }   
		break;			/* Case 1 ENDS */
	

		case 2:			/* DECRYPTION INIT */
			if((read_keystore(key,iv)))
			{
				dbug_p("ERROR,in reading KeyStore\n");
				return 1;
			}
        
			EVP_CIPHER_CTX_init(d_ctx);
	        	if(!(EVP_DecryptInit_ex(d_ctx, EVP_aes_256_cbc(), NULL, key, iv)))
			{
				perror("\n ERROR,DECRYPT_INIT::");
				return 1;
			}
			dbug_p("KEY::%s:: \n",key);
		break;			/* Case 2 ENDS */

		default:
			dbug_p("INCORRECT FLAG IN DEC_INIT\n");
			return 1;	     	 				
		break;
	}
	return 0;						/* SUCCESS */
}

/************************************ ENCRYPTION FUNCTION ****************************************************************/

int aes_encrypt(EVP_CIPHER_CTX *e,int in,int out)						/* Returns 0: SUCCESS 1: ERROR */
{
        char inbuf [SIZE];
        char outbuf[SIZE+AES_BLOCK_SIZE];
        int inlen = 0,flen=0,outlen =0;						
	
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
	
        return 0;			/* SUCCESS */
}

/************************************* DECRYPTION FUNCTION ***********************************************************/

int aes_decrypt(EVP_CIPHER_CTX *d,int in,int out)				/* returns 0: SUCCESS 1: ERROR */
{
        int inlen=0,flen=0,outlen=0;
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
        return 0;				/* SUCCESS */
}
	
