/** This file contains function to creat and maintain the keystore respective to each logged in user
**  @author dZONE
**  DATE 05-06-2012 v1.0 **/

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <error.h>
# include <sys/types.h>
# include <fcntl.h>
# include <sys/stat.h>
# include "debug.h"
# include "keystore.h"

int creat_keystore(unsigned char *pwd,unsigned char *k,unsigned char *v)
{
	int ks,i,j,wr;
	char uname[256];
	char path[256] = "/home/" ;

	if((getlogin_r(uname,256)))
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
	
	if((ks=open(path,O_CREAT|O_RDWR,S_IREAD|S_IWRITE))== -1)
	{
		perror("\n ERROR,OPEN()::");
		return 1;
	}

	wr=0;
	if((wr=write(ks,k,32)) != 32)							/* Writing Key to keystore */
	{
		if(wr ==0) { dbug_p("ZERO BYTES written \n");}
		else
		if(wr<0) { perror("\n ERROR,write::");}
		dbug_p("K BYTES wrtitten::%d::\n",wr);
		return 1;
	}

	wr=0;
	if((wr=write(ks,v,32)) != 32) 							/* Writing IV to keystore file */
        {   
                if(wr ==0) { dbug_p("ZERO BYTES written \n");}
                else
                if(wr<0) { perror("\n ERROR,write::");}
                dbug_p("V BYTES wrtitten::%d::\n",wr);
                return 1;
        }   
	
	return 0;					/* SUCCESS */
}


/***************************************************** read_keystore() *****************************************/

int read_keystore(unsigned char* k, unsigned char *v)
{
	int ks,i,j,rd=0;
	char uname[256];
	char path[256]= "/home/";	

	if((getlogin_r(uname,256)))
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

	if((rd=read(ks,k,32)) != 32)						/* reading Key */
	{
		perror("\n ERROR, reading key::");
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
