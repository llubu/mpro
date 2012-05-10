# include <stdio.h>
# include <magic.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <fcntl.h>
# include <error.h>

int main(int argc,char **argv)
{
	magic_t a;
/*	int in,wr=0;
	char w[] ="wsould i do to make it working";

	if((in=open("w.pdf",O_CREAT|O_RDWR,0400|0200)) ==-1)
	{
		perror("\n ERROR::opne::");
	}	
	if((wr=write(in,w,10)) ==-1)
	{
		perror("\n ERROR,write::");
	}
		printf("\n WW::%d::\n",wr);
*/	
	a=magic_open(MAGIC_MIME);
	if(a==NULL)
	{
		perror("\n ERROR IN COOKIE::");
		return 1;
	}
	if (magic_load(a, NULL) != 0) 
	{
		printf("cannot load magic database - %s\n", magic_error(a));
       	        magic_close(a);
                return 1;
        }	
	printf("\n FF::%s::\n",magic_file(a,argv[1]));

//	printf("\n FF::%s::\n",f);



	printf("\n ERROR if any::%s::\n",magic_error(a));
	printf("\n ERRORNO if any::%d::\n",magic_errno(a));

	magic_close(a);
//	close(in);
	return 0;
}
