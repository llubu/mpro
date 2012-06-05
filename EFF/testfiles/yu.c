# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include<sys/types.h>
# include<fcntl.h>

int main()
{
	int inlen=0,in=0,out=0;
	char buf[1024];
	
	if((in=open("/tmp/do_dec",O_RDONLY)) <0)
	{
		printf("\n error opening source file \n");
		return 1;
	}
	if((out=open("df.txt",O_CREAT|O_RDWR,0400|0200)) <0)
	{
		printf("\n error opening destfile \n");
		return 1;
	}

	while((inlen=read(in,buf,1024)) >0)
	{
		if(write(out,buf,inlen) != inlen)
		{
			printf("\n error \n");
		}
	}
		
	return 0;
}	
