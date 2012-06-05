/* cipher_file() encryptes a file, this function can be called from two places:
*  1. main() directly for a single file when in_path points to a valid file
*  2. cipher_dir() to encrypt files in a directory when in_path points to a valid directory
*  @author dZONE
*  DATE(C,M) 05-04-2012 06-01-2012 
*/

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <openssl/evp.h>
# include <error.h>
# include <string.h>
# include <errno.h>
# include "aes_cipher.h"
# include "cipher_helper.h"
# include "debug.h"
# include "cipher_file.h"					

int cipher_file(unsigned char *in_path,EVP_CIPHER_CTX *tx,int ci_flag)					
{
	char enc_path[] ="/tmp/do_encr";
	char dec_path[] ="/tmp/do_dec";
	struct stat a;						/* Strores the original file/owner permission */
	int in,out;	

	if((in = open((char *)in_path,O_RDONLY)) <0)			/* Opening the input file  */
	{
		perror("\n ERROR,OPEN_INFILE_C_FILE::");
		return 1;	
	}

	switch (ci_flag)
	{
		case 1:
			if((out = open(enc_path,O_CREAT|O_RDWR,0400|0200)) <0)				/* Creating tmp file for writing */
			{
				perror("\n ERROR,CANT create temp file_C_FILE::");
				return 1;
			}
			
			if(fstat(in,&a) == -1)								/* Fetching file permission for file opened with fd *in* */
			{
				perror("\n ERROR,CANT COPY FILE PERMISSION_C_FILE::");
				return 1;
			}
				
			if(aes_encrypt(tx,in,out))							/* ENCRYPTING */
			{
				dbug_p("ERROR,ENCRYPTING_C_FILE\n");
				return 1;
			}
			dbug_p("ENCRYPTED \n");

			close(in);
			close(out);
								/* Replacing original file */
			if(replace(enc_path,(char *)in_path))
			{
				dbug_p("ERROR,REPLACING FILE \n");
				return 1;
			}

			if((unlink(enc_path)) <0) 
                        {   
                                perror("\n ERROR, Cant unink enc file_C_FILE::");
                                return 1;
                        }    

			if(file_perres((char *)in_path,&a))							/* Restoring File permission */
			{
				dbug_p("ERROR,in restoring file per EE\n");
				return 1;
			}
		
//			close(in);
//			close(out);
		break;							/* Case 1 ENDS */	
	

		case 2:		/* DECRYPTION CASE */
			if((in =open((char *)in_path,O_RDONLY))<0)							/* Opening encrypted file for decryption */
			{
				perror("\n ERROR,opening encrypted file_C_FILE::");
				return 1;
			}
			if((out =open(dec_path,O_CREAT|O_RDWR,0400|0200)) <0)					/* Opening temp file */
			{
				perror("\n ERROR,craeting dec file_C_FILE::");
				return 1;
			}

			if(aes_decrypt(tx,in,out))								/* Decrypting */
			{
				dbug_p("ERROR,decrypting file_C_FILE\n");
				return 1;
			}
			dbug_p("DECRYPTED \n");

			close(in);
			close(out);

			if(replace(dec_path,(char *)in_path))								/* Replacing files */
			{
				dbug_p("ERROR,REPLACING DEC FILE \n");
				return 1;
			}

			if((unlink(dec_path)) <0) 
                        {   
                                 perror("\n ERROR, Cant unink enc file_C_FILE::");
                                 return 1;
                        }    

//			close(in);
//			close(out);
		break;				/* CASE 2 ENDS */

		default:			/* Default case for the incorrect choice entered */	
			printf("\n INCORRECT CHOICE ENTERED \n");	
			return 1;	
		break;
	}

	return 0;		/* SUCCESS */
}
