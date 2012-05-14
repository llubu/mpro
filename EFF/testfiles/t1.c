# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <dirent.h>
# include <errno.h>
# include <error.h>
# include <limits.h>

int main(int argc, char **argv)
{
	if(argc !=2)
	{
		printf("\n ENTR PATH ALSO");
		return 1;
	}

	DIR *d;
	struct dirent *r;

	printf("\n MAX PATH SIZE::%d::\n",MAX_PATH);
	if((d=opendir(argv[1])) == NULL)
	{
		perror("\n ERROR,opendir::");
		return 1;
	}
	
	errno =0;
	
	printf("\n DIR CONTENTS\n");
	while((r=readdir(d)) != NULL)
	{
		printf("\n FILE NAME IS:%s:\n",r->d_name);			/* Only POSIX compliant strut member of dirent */

	}
	if(errno && !r)
	{
		perror("\n ERROR,readdir::");	
		return 1;
	}
	else {	
		printf("\n END OF DIR\n");
	}

	closedir(d);	
	return 0;
}	
	
