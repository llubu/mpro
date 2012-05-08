/** This file contains function to creat and maintain the keystore respective to each logged in user
**  @author dZONE
**  DATE 05-06-2012 v1.0 **/

# include <stdio.h>
# include <stdlib.h>
# include <openssl/evp.h>
# include <openssl/rand.h>
# include <openssl/aes.h>
# include <openssl/sha.h>
# include <unistd.h>
# include <error.h>
# include <sys/types.h>
# include <fcntl.h>
# include <sys/stat.h>
# include "debug.h"
# include "keystore.h"

# define itr 1000
# define SIZE 32

int creat_keystore(unsigned char *pwd,unsigned int pwd_len,unsigned char *k,unsigned char *v)
{
	int ks,i,j,wr,outlen=0;
	unsigned char uname[256],salt[32],pkey[32],outbuf[SIZE+AES_BLOCK_SIZE],md[SHA256_DIGEST_LENGTH];
	unsigned char psalt[64],salt_phash[32];			/* password combined with salt */
	unsigned char path[256] = "/home/" ;
	EVP_CIPHER_CTX p;
	SHA256_CTX sh;				/* For password hash */

	if((getlogin_r((char *) uname,256)))
	{
		perror("\n ERROR,getlogin_r::");
		return 1;
	}

	/* Creates the ks file at /home/user/ */

	i=0;
	while(path[i] != '\0') { ++i;}
	
	j=0;
	while(uname[j] != '\0' && i <256 && j<256)
	{
		path[i] = uname[j];
		++i; ++j;
	}

	if(i <240)					/* Make the file name random */
	{	path[i] = '/';
		path[i+1] = '.';
		path[i+2] = 'k';
		path[i+3] = 's';
		path[i+4] = '.';
		path[i+5] = 't';
		path[i+6] = '\0';
	}
	dbug_p("FINAL PATH ::%s:: \n",path);
	
	if((ks=open((const char *)path,O_CREAT|O_RDWR,S_IREAD|S_IWRITE))== -1)
	{
		perror("\n ERROR,OPEN()::");
		return 1;
	}
	dbug_p("PWD IN GENKS::%s::\n",pwd);

	if(!(RAND_bytes(salt,32)))
	{
		perror("\n ERROR,RAND genKS::");
		return 1;
	}

	if(!(PKCS5_PBKDF2_HMAC_SHA1((const char *)pwd,pwd_len,salt,32,itr,32,pkey)))
	{
		perror("\n ERROR,PBKG2::");
		return 1;
	}
	
	if(!(RAND_bytes(salt_phash,32)))				/* salt to be used for pasword hash */
	{
		perror("\n ERROR,RAND_BYTES_CKS::");
		return 1;
	}

	i=0,j=0;
	while(i<64)
	{
		if(i<pwd_len)
		{
			psalt[i] = pwd[i];
		}	
		if(i>=pwd_len && i<64)				/* Specify max pwd len in pwd input MAX_PWD_SIZE 31 */
		{
			if(j<32)
			{
				psalt[i] = salt_phash[i];
				++j;
			}
			else { dbug_p("In salt_phash_CKS:I:%d: J:%d::\n",i,j);break;}
		}
		++i;
	}
	if(i <64)
	{
		psalt[i] = '\0';
	}
	else { dbug_p("ERROR,PSALT LAST NULL_CKS\n"); return 1;}

	dbug_p("II_PSALT_CKS::%d::J::%d::\n",i,j);
	dbug_p("P+SALT::%s::\n",psalt);

	if(!SHA256_Init(&sh))						/* Generating Password hash where pwd = input pwd+salt */
	{
		perror("\n ERROR,SHA256_CKS::");
		return 1;
	}
	if(!SHA256_Update(&sh,psalt,i))
	{
		perror("\n ERROR,SHA256_UPDATE_CKS::");
		return 1;
	}
	if(!SHA256_Final(md,&sh))
	{
		perror("\n ERROR,SHA256_FINAL_CKS::");
		return 1;
	}

	dbug_p("SHA256DIGLEN::%d::\n",SHA256_DIGEST_LENGTH);
	
	wr=0;
	if((wr=write(ks,md,SHA256_DIGEST_LENGTH)) != SHA256_DIGEST_LENGTH)			/* 1 item in KS:: Writing Pwd Hash to KS (32 Bytes) */
	{
		dbug_p("ERROR,WRITE_SHA256_CKS,bytes written:%d:\n",wr);
		return 1;
	}
	
	wr=0;
	if((wr=write(ks,salt_phash,32)) != 32)							/* 2 Item in KS:: SALT used for Pwd HASH (32Bytes) */
	{
		if(wr ==0) { dbug_p("PSALT_ITEM 2:ZERO BYTES written \n");}
                else
                if(wr<0) { perror("\n ERROR,write_PSALT_ITEM 2::");}
                dbug_p("BYTES wrtitten_PSALT_ITEM2_CKS::%d::\n",wr);
                return 1;
	}
		
	wr=0;
	if((wr=write(ks,salt,32)) != 32)							/* 3 Item in KS:: SALT used to gen session key 'S'(32Bytes) */
	{
		if(wr ==0) { dbug_p("ZERO BYTES written \n");}					/* Will be needed to gen S to decrypt k */
		else
		if(wr<0) { perror("\n ERROR,write::");}
		dbug_p("K BYTES wrtitten::%d::\n",wr);
		return 1;
	}

	wr=0;
        if((wr=write(ks,v,32)) != 32)                                                   	/* 4 Item in KS:: IV 'v'(32 Bytes) */
        {                                                                               	/* Will be needed to decrypt file data */
                if(wr ==0) { dbug_p("V_CKS_ZERO BYTES written \n");}
                else
                if(wr<0) { perror("\n ERROR,V_CKS_write::");}
                dbug_p("V BYTES wrtitten::%d::\n",wr);
                return 1;
        }   

	EVP_CIPHER_CTX_init(&p);

	if(!(EVP_EncryptInit_ex(&p, EVP_aes_256_cbc(), NULL, pkey, salt)))
	{
		perror("\n ERROR,ENC_INIT_KS::");
		return 1;
	}	

	if(!EVP_EncryptUpdate(&p,outbuf,&outlen,k,SIZE))
	{
		perror("\n ERROR,ENC_UPDATE_KS::");
		return 1;
	}
	dbug_p("OUTLEN_CREAT_KS_ENCUPDATE::%d::\n",outlen);
	
	if(write(ks,outbuf,outlen) != outlen)
	{
		dbug_p("ERROR,Cant write all bytes to KS\n");
		return 1;
	}
	
	if(!EVP_EncryptFinal_ex(&p,outbuf,&outlen))
	{
		perror("\n ERROR,ENC_FINAL_CKS::");
		return 1;
	}

	if(write(ks,outbuf,outlen) != outlen)						/* 5(Last) Item in KS:: Key final (?? Bytes) */
	{
		dbug_p("ERROR,Cant write bytes to KS_EFINAL\n");
		return 1;
	}
	dbug_p("outlen in ENC_FINAL::%d::\n",outlen);

	dbug_p("ENC KEY WRITTEN TO KS\n");

	close(ks);					/* Create a clean up function to be called in case of Failure that will del corup KS file */
	EVP_CIPHER_CTX_cleanup(&p);
	dbug_p("CREAT KS SUCCESS\n");
	return 0;					/* SUCCESS */
}


/***************************************************** read_keystore() *****************************************/

int read_keystore(unsigned char *pwd,unsigned int pwd_len,unsigned char* k, unsigned char *v)
{
	int ks,i,j,rd=0;
	unsigned char uname[256],/*pkey[32],*/md[SHA256_DIGEST_LENGTH],r_phash[SHA256_DIGEST_LENGTH]; //salt[32];
	char path[256]= "/home/";	

//	EVP_CIPHER_CTX dp;
	SHA256_CTX sh;

	if((getlogin_r((char *) uname,256)))
        {
                perror("\n ERROR,getlogin_r::");
                return 1;
        }
        
        i=0;
        while(path[i] != '\0') { ++i;}

        j=0;
        while(uname[j] != '\0' && i <256 && j<256)
        {
                path[i] = uname[j];
                ++i; ++j;
        }
        if(i <240)                                      /* Make the file name random */
        {       path[i] = '/';
                path[i+1] = '.';
                path[i+2] = 'k';
                path[i+3] = 's';
                path[i+4] = '.';
                path[i+5] = 't';
                path[i+6] = '\0';
        }
	
	dbug_p("READPATH::%s::",path);
	if((ks=open(path,O_RDONLY)) ==-1)
	{
		perror("\n ERROR,open ks file::");
		return 1;
	}

	rd=0;
	if((rd=read(ks,r_phash,SHA256_DIGEST_LENGTH)) != SHA256_DIGEST_LENGTH)	/* Reading Stored Pwd Hash for comparison(20Bytes) */
	{
		perror("\n ERROR,Reading_PHASH_RKS::");
		return 1;
	}
	
	SHA256_Init(&sh);

	if(!SHA256_Update(&sh,pwd,pwd_len))
	{
		perror("\n ERROR,SHA256_U_RKS::");
		return 1;
	}

	if(!SHA256_Final(md,&sh))
	{
		perror("\n ERROR,SHA256_F_RKS::");
		return 1;
	}

	rd=0;
	if((rd=read(ks,v,32)) != 32)						/* Reading IV */
	{
		perror("\n ERROR, reading iv::");
		return 1;
	}
	
	return 0;
}
