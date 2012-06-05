/* This file contains the helper functions
** @author dZONE
** DATE 05-05-2012 06-01-2012
v1.0 */

# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>
# include <fcntl.h>
# include "debug.h"
# include "cipher_helper.h"

int replace(char * source,char * dest)			/* Replaces the char* source file with char *dest file  ***improve child process handling */
{
        int d=0,s=0;
        int inlen=0;
	char *rep_buf=NULL;

        if(source == NULL || dest == NULL)
        {   
                dbug_p("ERROR, Corrupted arguments to func \n");
                return 1;
        }   
	printf("\n INSIDE REPLACE \n");
	if((d= open(dest,O_CREAT|O_RDWR|O_TRUNC,0400|0200)) <0)
	{
		perror("\n ERROR,Cant create file in dest to replace::");
		return 1;
	}
        
	dbug_p("DEST PATH:%s:\n",dest);
	dbug_p("SOURCE PATH:%s:\n",source);

	if((s= open(source,O_RDONLY)) <0)
	{
		perror("\n ERROR, Cant open temp file to read::");
		return 1;
	}
	
	if((rep_buf=malloc(1024 * sizeof(char))) == NULL)
	{
		perror("\n Cant Allocate Memory for rep buf::");
		return 1;
	}
 
	while((inlen= read(s,rep_buf,1024)) >0)
	{
		if((write(d,rep_buf,inlen)) != inlen)
		{
			perror("\n ERROR, Cant write to replace::");
			return 1;
		}
		inlen=0;
	}	  
	free(rep_buf);      
	return 0;
}

/************************************** restore original file permission ***************************************************/
/* Called two times once after copying the encrypted file and then after decrypted file */

int file_perres(char *path, struct stat *pt)
{
	if((chmod(path,(*pt).st_mode)) ==-1)
	{
		perror("\n ERROR in chmod::");
		return 1;
	}
	
	if((chown(path,(*pt).st_uid,(*pt).st_gid)) ==-1)		/* Needs *ROOT* privilege to change the owner */
	{
		perror("\n ERROR in chown::");
		return 1;
	}			
	
	return 0;
}
