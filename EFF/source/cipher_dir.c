/* This contains cipher_dir() which handles the crypt operation on directories which is called from main()
*  @author dZONE
*  DATE(C,M) 05-15-2012		05-15-2012
*/

# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <dirent.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <openssl/evp.h>
# include <errno.h>
# include <error.h>
# include "cipher_dir.h"
# include "cipher_file.h"
# include "debug.h"

int cipher_dir(unsigned char *in_path,EVP_CIPHER_CTX *di_tx,int ci_flag)
{
		DIR *d;
		struct dirent *r;
		struct stat chk;
		char *name = NULL;						/* File name */
		char *f_path[4096];
		int i=0,j=0,b=0;

		while(in_path[i] != '\0' && i<4096)
		{
			f_path[i] = in_path[i++];
		}
		f_path[i++] = '/';
		dbug_p("PATH is:%s:\n",in_path);
	
		if((d=opendir(in_path)) == NULL)
		{
			perror("\n ERROR,Opendir_C_DIR::");
			return 1;
		}
		
		errno=0;						/* Setting errno to 0 to diffrentiate between end of dir & ERROR */
		while((r=readdir(d)) != NULL)
		{
			j=0,b=i;
			name = r->d_name;
			if(name[0] == 46 && (name[1] == 46 || name[1] == '\0')) continue;	/* handling '.' & '..' */
			
			while(name[j] != '\0' && b <4095 && j<256)
			{
				f_path[b++] =	name[j++];
			}
			if(b<4095)
			{	
				f_path[b] = '\0';
				dbug_p("F_PATH:%s:\n",f_path);
			}
			else { printf("\n ERROR,PATH TOO LONG_C_DIR\n"); return 1;}

			if(stat(f_path,&chk) !=0)				/* ?? improve handling nested directories */
			{
				perror("\n ERROR,stat_C_DIR::");
				return 1;
			}
			if(S_IFDIR == (in.st_mode & S_IFMT))         		/* Checking for directory or file */
			{
				if(cipher_dir(f_path,di_tx,ci_flag))
				{
					dbug_p("ERROR,RECURSIVE_C_DIR\n");
					return 1;
				}
			}
			else
			if(cipher_file(f_path,di_tx,ci_flag))				/* Calling cipher_file() to do crypt ope. on files in dec */
			{
				dbug_p("ERROR,CANT DO CRYPT_C_DIR\n");
				return 1;
			}
		}
		
		if(errno && !r)
		{
			perror("\n ERROR,readdir_C_DIR::");
			return 1;
		}

		closedir(d);				/* Closing Directory stream */

	return 0;					/* SUCCESS */
}
