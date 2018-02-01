/* This do_cipher() function takes in input file argument and then prompts for pwd to encrypt a file
* @author dZONE
* DATE 05-04-2012 v1.0 
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

int main()					/* Takes in input the file to be encrypted */
{
	char pwd[16];
	unsigned int pwd_len=0;
	int in,out,i=0,ncaps=0,ndig=0;
	char ppath[256];
	char tmp_path[] ="/tmp/do_encr.txt";
	char dec_path[] ="/tmp/do_dec.txt";
	EVP_CIPHER_CTX en,de;
	
	printf("\n ENTER THE FILE(PATH) TO BE ENCRYPTED(e.g /home/me/Desktop/a.txt) \n");
	fgets(ppath,256,stdin);									/* Input file path */
	while(ppath[i] != '\n') ++i;
	ppath[i] ='\0';	
	dbug_p("PPATH:%s:\n",ppath);

	if((in = open(ppath,O_RDONLY)) <0)			/* Opening the input file  */
	{
		perror("\n ERROR,OPEN IN::");
		exit (1);
	}
	printf("\n ENTER THE PASSWORD FOR THE FILE ENCRYPTION \n");
	fgets(pwd,16,stdin);
	dbug_p("\n PWD:%s: \n",pwd);
	pwd_len = strlen((const char*) pwd)-1;
	i =0;
	while(pwd[i] != '\0' &&  pwd[i] != '\n') 			/* Password Complexity Rules */
	{
		if(pwd[i] >=48 && pwd[i] <=57) ++ndig;
		if(pwd[i] >=65 && pwd[i] <=90) ++ncaps;
		++i;
	}	
	if((ndig <2) || (!ncaps) || (pwd_len <=6))
	{
		printf("\n Password Complexity not satisfied\n Min Pwd length 7: Atleast 1 Caps character : Atleast 2 digits \n");
		return 1;
	}
				
	dbug_p("\n PWDLEN:%d:\n",pwd_len);

	if((out = open(tmp_path,O_CREAT|O_RDWR,0400|0200)) <0)
	{
		perror("\n ERROR,CANT create temp file::");
		return 1;
	}

/***************************start encrypting in file **************************************************************************************/
	if(aes_init((unsigned char*)pwd,pwd_len,&en,&de))
	{
		dbug_p("ERROR_CIPHER INIT\n");
		return 1;
	}
	else
	if(aes_encrypt(&en,in,out))
	{
		dbug_p("ERROR,ENCRYPTING\n");
		return 1;
	}

	dbug_p("ENCRYPTED \n");
/*********************** Replacing original file ***********************************************************/
//	close(in);
//	close(out);

	if(replace(tmp_path,ppath))
	{
		dbug_p("ERROR,REPLACING FILE \n");
		return 1;
	}
	if(cleanup(tmp_path))
	{
		dbug_p("ERROR,CANT CLEAN \n");
		return 1;
	}	
/************************** Decrypting *******************************************************************/
	getchar();
	if((in =open(ppath,O_RDONLY))<0)
	{
		perror("\n ERROR,opening encrypted file::");
		return 1;
	}
	if((out =open(dec_path,O_CREAT|O_RDWR,0400|0200)) <0)
	{
		perror("\n ERROR,craeting dec file::");
		return 1;
	}
	if(aes_decrypt(&de,in,out))
	{
		dbug_p("ERROR,decrypting file \n");
		return 1;
	}
	dbug_p("DECRYPTED \n");
	if(replace(dec_path,ppath))
	{
		dbug_p("ERROR,REPALING DEC FILE \n");
		return 1;
	}
		
	EVP_CIPHER_CTX_cleanup(&en);
	EVP_CIPHER_CTX_cleanup(&de);
	if(cleanup(dec_path))
	{
		dbug_p("ERROR,CANT CLEAN \n");
		return 1;
	}
	close(in);
	close(out);
	return 0;
}
