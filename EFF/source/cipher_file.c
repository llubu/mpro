/* This do_cipher() function takes in input file argument and then prompts for pwd to encrypt a file
* @author dZONE
* DATE 05-04-2012 05-08-2012 v1.0 
* Currently only AES256CBC and .txt file are supported */

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <openssl/evp.h>
# include <openssl/aes.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <error.h>
# include <string.h>
# include "debug.h"
# include <errno.h>
# include "aes_cipher.h"
# include "cipher_helper.h"

struct stat a;					/* structure to copy the file permission of the original file */

int main()					/* Takes in input the file to be encrypted */
{
	char pwd[16];
	unsigned int pwd_len=0;
	int in,out,i=0,ncaps=0,ndig=0,ch=0;
	char ppath[256];
	char enc_path[] ="/tmp/do_encr";
	char dec_path[] ="/tmp/do_dec";

	EVP_CIPHER_CTX en,de;
	
	printf("\n ENTER THE FILE(PATH) TO BE ENCRYPTED/DECRYPTED (e.g /home/me/Desktop/a.txt) \n");
	fgets(ppath,256,stdin);									/* Input file path */

	while(ppath[i] != '\n') ++i;
	ppath[i] ='\0';	
	dbug_p("PPATH:%s:\n",ppath);

	if((in = open(ppath,O_RDONLY)) <0)			/* Opening the input file  */
	{
		perror("\n ERROR,OPEN IN ::");
		return 1;	
	}

	printf("\n ENTER 1 for ENCRYPTION & 2 for DECRYPTION \n");		/* Choice for E or D */
	scanf("%d ",&ch);
	
	printf("\n ENTER THE PASSWORD FOR THE FILE ENCRYPTION \n");
	fgets(pwd,16,stdin);
	dbug_p("PWD:%s: \n",pwd);

	pwd_len = strlen((const char*) pwd)-1;
	pwd[pwd_len] = '\0';

	i =0;
	while(pwd[i] != '\0' &&  pwd[i] != '\n') 			/* Password Complexity Rules */
	{
		if(pwd[i] >=48 && pwd[i] <=57) ++ndig;
		if(pwd[i] >=65 && pwd[i] <=90) ++ncaps;
		++i;
	}	
	if((ndig <2) || (!ncaps) || (pwd_len <=6) || (pwd_len >30))
	{
		printf("\n Password Complexity not satisfied\n Min Pwd length 7: Max Len 31 \n Atleast 1 Caps character : Atleast 2 digits \n");
		return 1;
	}
	dbug_p("PWDLEN:%d:\n",pwd_len);
	
	switch (ch)
	{
	case 1:
		if((out = open(enc_path,O_CREAT|O_RDWR,0400|0200)) <0)		/* Creating tmp file for writing */
		{
			perror("\n ERROR,CANT create temp file::");
			return 1;
		}

		if(fstat(in,&a) == -1)						/* Fetching file permission for file opened with fd *in* */
		{
			perror("\n ERROR,CANT COPY FILE PERMISSION::");
			return 1;
		}

		if(aes_init((unsigned char*)pwd,pwd_len,&en,(EVP_CIPHER_CTX *)NULL,ch))		/* Initializing only the encryption context */
		{
			dbug_p("ERROR_CIPHER INIT\n");
			return 1;
		}
	
		if(aes_encrypt(&en,in,out))							/* ENCRYPTING */
		{
			dbug_p("ERROR,ENCRYPTING\n");
			return 1;
		}
		dbug_p("ENCRYPTED \n");
												/* Replacing original file */
		if(replace(enc_path,ppath))
		{
			dbug_p("ERROR,REPLACING FILE \n");
			return 1;
		}
		if(cleanup(enc_path))								/* Cleaning all tmp files created */
		{
			dbug_p("ERROR,CANT CLEAN \n");
			return 1;
		}	
		if(file_perres(ppath,&a))							/* Restoring File permission */
		{
			dbug_p("ERROR,in restoring file per EE\n");
			return 1;
		}
		EVP_CIPHER_CTX_cleanup(&en);
		close(in);
		close(out);
	break;							/* Case 1 ENDS */	
	

	case 2:							/* DECRYPTION CASE */
		if(aes_init((unsigned char*)pwd,pwd_len,(EVP_CIPHER_CTX *)NULL,&de,ch))			/* Initializing Decrypt context only */
                {
                       	dbug_p("ERROR_CIPHER INIT\n");
	                       return 1;
        	}
		if((in =open(ppath,O_RDONLY))<0)							/* Opening encrypted file for decryption */
		{
			perror("\n ERROR,opening encrypted file::");
			return 1;
		}
		if((out =open(dec_path,O_CREAT|O_RDWR,0400|0200)) <0)					/* Opening temp file */
		{
			perror("\n ERROR,craeting dec file::");
			return 1;
		}
		if(aes_decrypt(&de,in,out))								/* Decrypting */
		{
			dbug_p("ERROR,decrypting file \n");
			return 1;
		}
		dbug_p("DECRYPTED \n");

		if(replace(dec_path,ppath))								/* Replacing files */
		{
			dbug_p("ERROR,REPALING DEC FILE \n");
			return 1;
		}
		if(cleanup(dec_path))									/* Deleting all tmp files created */
		{
			dbug_p("ERROR,CANT CLEAN \n");
			return 1;
		}
		EVP_CIPHER_CTX_cleanup(&de);							
		close(in);
		close(out);
	break;				/* CASE 2 ENDS */

	default:			/* Default case for the incorrect choice entered */	
		printf("\n INCORRECT CHOICE ENTERED \n");	
		return 1;	
	break;
	}
	return 0;		/* SUCCESS */
}
