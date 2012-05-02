# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <error.h>
# include <string.h>
# include "debug.h"
# include <errno.h>

struct stat fd;
struct stat fn;

int main()
{
	int in,out;
	char inpath[1024];
	int rf=0,i=0,sym=0,j=0;
	char path[16];
	char filename[20];	
	EVP_CIPHER_CTX en,de;

	if((in = open("plain.txt",O_RDONLY)) <0)
	{
		perror("\n ERROR,OPEN IN::");
		exit (1);
	}
	
	if((out = open("/tmp/do11.txt",O_RDWR|O_CREAT,0400|0200)) <0)
	{
		perror("\n ERROR,OPEN TMP::");
		exit (1);
	}	
	if(in <10) i =1;
	if(in >9 && in <100) i =2;
	if(in >99) i =3;

	dbug_p("\n IN:%d\n",in);
	snprintf(path,15+i,"/proc/self/fd/%d",in);
	dbug_p("\n%s\n",path);
	if((rf = readlink(path,inpath,1024)) <0)
	{
		dbug_p("\n errno%d:\n",errno);
		perror("\n ERROR<readlin::");
		exit (1);
	}
	dbug_p("\nSTE::**%s**::\n",inpath);
	i =0;sym=0,j =0;
	while(inpath[i] != '\0')
	{
		if(inpath[i] == '/') sym =i;;
		++i;
	}
	++sym;
	while(inpath[sym] != '\0')
	{
		filename[j] = inpath[sym];
		++j;++sym;
	}
	filename[j] = '\0';
	dbug_p("\n FILENAME:%s:\n",filename);
	if((stat(inpath,&fn)) <0) 
	{
		perror("\n Error,stat::");
		exit (1);
	}
	if((fstat(in,&fd)) <0) 
	{
		perror("\n ERROR,fstat::");
		exit (1);
	}
	if((fn.st_dev) == (fd.st_dev))
	{
		if((fd.st_ino) == (fd.st_ino))
		{
			dbug_p("\n REPLACE IN FILE with TMP FILE *HERE*\n");	/* Replace the in and temo file here */
		}	
	}
/*** start encrypting in file **************************************************************************************/
	aes_init(	

	close(in);
	return 0;
}
