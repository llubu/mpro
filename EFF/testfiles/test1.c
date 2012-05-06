# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <fcntl.h>

struct stat a;

int main()
{
	int in=0;
	int u_read=0,u_write=0,u_x=0,g_read=0,g_write=0,g_x=0,o_read=0,o_write=0,o_x=0,u_s=0,g_s=0,sticky=0;

	if((in=open("plain.txt",O_RDONLY)) <0)
	{
		perror("\n ERROR opening file::");
		return 1;
	}
	if(fstat(in,&a))
	{
		perror("\n ERROR,fstat");
		return 1;
	}

	if(a.st_mode & S_IREAD) u_read=1;
	if(a.st_mode & S_IWRITE) u_write=1;
	if(a.st_mode & S_IEXEC) u_x=1;

	if(a.st_mode & S_IRGRP) g_read=1;
	if(a.st_mode & S_IWGRP) g_write=1;
	if(a.st_mode & S_IXGRP) g_x=1;
	
	if(a.st_mode & S_IROTH) o_read=1;
	if(a.st_mode & S_IWOTH) o_write=1;
	if(a.st_mode & S_IXOTH) o_x=1;

	if(a.st_mode & S_ISUID) u_s=1;
	if(a.st_mode & S_ISGID) g_s=1;
	if(a.st_mode & S_ISVTX) sticky=1;

	printf("\n ur:%d: uw:%d: ux:%d: suid:%d: gr:%d: gw:%d: gx:%d: sgid:%d: or:%d: ow:%d: ox:%d: sticky:%d:\n",u_read,u_write,u_x,u_s,g_read,g_write,g_x,g_s,o_read,o_write,o_x,sticky);
	printf("\n FILE PERMISSION \n");
	if(u_read)
	{
		printf("-r");
	}
	else {printf("--");}
	
	if(u_write)
	{
		printf("w");
	}
	else {printf("-");}
	
	if(u_x && u_s)
	{
		printf("s");
	}
	else
		if(u_x)
		{
			printf("x");
		}
		else
			if(u_s)
			{
				printf("S");
			}
			else
				if((!u_x) && (!u_s))
				{
					printf("-");
				}
	
	if(g_read)
	{
		printf("r");
	}
	else {printf("-");}

	if(g_write)
	{
		printf("w");
	}
	else { printf("-");}

	if(g_x && g_s)
	{
		printf("s");
	}
	else
		if(g_x)
		{
			printf("x");
		}
		else
			if(g_s)
			{
				printf("S");
			}
			else
				if((!g_x) && (!g_s))
				{
					printf("-");
				}

	if(o_read)
	{
		printf("r");
	}
	else { printf("-");}
	
	if(o_write)
	{
		printf("w");
	}
	else { printf("-");}
	
	if(o_x && sticky)
	{
		printf("t");
	}
	else
		if(o_x)
		{
			printf("x");
		}
		else
			if(sticky)
			{
				printf("T");
			}
			else
				if((!o_x) && (!sticky))
				{
					printf("-");
				}
	return 0;
}
