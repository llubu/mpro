# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>

# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>

# include "debug.h"
# include "cipher_helper.h"

int replace(char* source, char* dest)
{
	int status;
	pid_t pid;

	if ((source == NULL) || (dest == NULL))
	{
		dbug_p("ERROR, Corrupted arguments to func \n");
		return 1;
	}

	pid = fork();
	if (pid < 0)
	{
		perror("\n ERROR,Cant fork::");
		return 1;
	}
	else
	{
		if (pid == 0)
		{
			if ((execl("/bin/cp", "/bin/cp", source,
				dest, (char*) 0)) < 0)
			{
				perror("\n ERROR, CPEXECL::");
				return 1;
			}
		}
		else
		{
			wait(&status);
			dbug_p("STATUS:%d:\n",status);
		}
	}
	return 0;
}

int file_perres(char* path, struct stat* pt)
{
	if ((chmod(path, (*pt).st_mode)) == -1)
	{
		perror("\n ERROR in chmod::");
		return 1;
	}

	if ((chown(path, (*pt).st_uid, (*pt).st_gid)) == -1)
	{
		perror("\n ERROR in chown::");
		return 1;
	}
	return 0;
}

