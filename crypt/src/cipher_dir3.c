# include <stdio.h>
# include <stdlib.h>
# include <dirent.h>
# include <unistd.h>
# include <pthread.h>
# include <string.h>
# include <errno.h>
# include <error.h>

# include <sys/types.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <openssl/evp.h>

# include "cipher_dir.h"
# include "cipher_file.h"
# include "debug.h"

int cipher_dir(unsigned char* in_path, EVP_CIPHER_CTX* di_tx, int ci_flag)
{
	struct thread_arg one;
	DIR* d;
	struct dirent* r;
	struct stat chk;
	char *name = NULL;
	unsigned char f_path[4096] = {0};
	int i = 0;
	int j = 0;
	int b = 0;
	int rc = 0;

	pthread_t th;
	pthread_attr_t att;

	pthread_attr_init(&att);
	pthread_attr_setdetachstate(&att, PTHREAD_CREATE_JOINABLE);

	while ((in_path[i] != '\0') && (i < 4096))
	{
		f_path[i] = in_path[i];
		++i;
	}
	f_path[i++] = '/';
	dbug_p("PATH is:%s:\n",in_path);

	if((d = opendir(in_path)) == NULL)
	{
		perror("\n ERROR,Opendir_C_DIR::");
		return 1;
	}

	//Setting errno to 0 to diffrentiate between end of dir & ERROR
	errno = 0;
	while ((r = readdir(d)) != NULL)
	{
		j = 0;
		b = i;
		name = r->d_name;
		if ((name[0] == 46) && ((name[1] == 46) || (name[1] == '\0')))
			continue;

		while ((name[j] != '\0') && (b < 4095) && (j < 256))
		{
			f_path[b++] = name[j++];
		}
		if (b < 4095)
		{
			f_path[b] = '\0';
			dbug_p("F_PATH:%s:\n",f_path);
		}
		else
		{
			printf("\n ERROR,PATH TOO LONG_C_DIR\n");
			return 1;
		}

		if (stat(f_path,&chk) != 0)
		{
			perror("\n ERROR,stat_C_DIR::");
			return 1;
		}
		if (S_IFDIR == (chk.st_mode & S_IFMT))
		{
			strncpy(one.in_path, f_path, sizeof(f_path));
			one.di_tx = di_tx;
			one.ci_flag = ci_flag;

			rc = pthread_create(&th, &att, cipher_dir2, &one);

			if (rc)
			{
				printf("\n ERROR IN THREAD_C_D_3\n");
				exit(1);
			}
			pthread_join(th, NULL);
			pthread_attr_destroy(&att);
		}
		else
		{
			if (cipher_file(f_path, di_tx, ci_flag))
			{
				dbug_p("ERROR,CANT DO CRYPT_C_DIR\n");
				return 1;
			}
		}
	}

	if (errno && (!r))
	{
		perror("\n ERROR,readdir_C_DIR::");
		return 1;
	}

	closedir(d);
	pthread_exit(NULL);
	return 0;
}

