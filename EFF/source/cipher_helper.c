/* This file contains the helper functions
** @author dZONE
** DATE 05-05-2012 v1.0 */

# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include "debug.h"
# include "cipher_helper.h"

int replace(char * source,char * dest)			/* Replaces the char* source file with char *dest file  ***improve child process handling */
{
        int status;
        pid_t pid;

        if(source == NULL || dest == NULL)
        {   
                dbug_p("ERROR, Corrupted arguments to func \n");
                return 1;
        }   

        pid = fork();
        if(pid <0) 
        {   
                perror("\n ERROR,Cant fork::");
                return 1;
        }   
        else
        if(pid ==0)
        {   
                if((execl("/bin/cp","/bin/cp",source,dest,(char*) 0)) <0)
		{
			perror("\n ERROR, CPEXECL::");
			return 1; 
		}
	}
	else{
		wait(&status);
		dbug_p("STATUS:%d:",status);
	}   
        return 0;
}

/************************************ CLEANUP *********************************************************************/

int cleanup(char *path)
{
	int status;
	pid_t pid;
	
	if(path == NULL)
	{
		dbug_p("Error, Corrupted path passed in argument \n");
		return 1;
	}
	
	pid = fork();
	if(pid <0)
	{
		perror("\n ERROR,Cant fork::");
		return 1;
	}
	else
	if(pid ==0)
	{
		if((execl("/bin/rm","/bin/rm",path,(char*) 0)) <0)
		{
			perror("\n ERROR,CLEANEXCV::");
			return 1;
		}
	}
	else{
		wait(&status);
		dbug_p("STATUS:%d",status);
	}
	return 0;
}
