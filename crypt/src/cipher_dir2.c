# include <stdio.h>
# include <stdlib.h>
# include <dirent.h>
# include <unistd.h>
# include <string.h>
# include <pthread.h>
# include <errno.h>
# include <error.h>

# include <sys/types.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <openssl/evp.h>

# include "cipher_dir.h"
# include "cipher_file.h"
# include "debug.h"

void* cipher_dir2(void* st)
{
	struct thread_arg* th_pt = (struct thread_arg*)st;

	DIR* d;
	struct dirent* r;
	struct stat chk;
	char* name = NULL;
	unsigned char f_path[4096] = {0};
	int i = 0;
	int j = 0;
	int b = 0;
	int rc1 = 0;

	pthread_t th1;
	pthread_attr_t attr1;

	pthread_attr_init(&attr1);
	pthread_attr_setdetachstate(&attr1, PTHREAD_CREATE_JOINABLE);

	while ((th_pt->in_path[i] != '\0') && (i < 4096))
	{
		f_path[i] = th_pt->in_path[i];
		++i;
	}
	f_path[i++] = '/';
	dbug_p("PATH is:%s:\n",th_pt->in_path);

	if ((d = opendir(th_pt->in_path)) == NULL)
	{
		perror("\n ERROR,Opendir_C_DIR::");
		exit(1);
	}

	//Setting errno to 0 to diffrentiate between end of dir & ERROR
	errno = 0;
	while ((r = readdir(d)) != NULL)
	{
		j = 0;
		b = i;
		name = r->d_name;
		if ((name[0] == 46) && ((name[1] == 46) || (name[1] == '\0')))
			continue; //handling '.' & '..'

		while ((name[j] != '\0') && (b < 4095) && (j < 256))
			f_path[b++] = name[j++];

		if(b < 4095)
		{
			f_path[b] = '\0';
			dbug_p("F_PATH:%s:\n",f_path);
		}
		else
		{
			printf("\n ERROR,PATH TOO LONG_C_DIR\n");
			exit(1);
		}

		if (stat(f_path, &chk) != 0)
		{
			perror("\n ERROR,stat_C_DIR::");
			exit(1);
		}
		if (S_IFDIR == (chk.st_mode & S_IFMT))
		{
			struct thread_arg chk;

			strncpy(chk.in_path, f_path, sizeof(f_path));
			dbug_p("PATH copied to struct:%s:\n",chk.in_path);
			chk.di_tx = th_pt->di_tx;
			chk.ci_flag = th_pt->ci_flag;

			rc1 = pthread_create(&th1, &attr1, cipher_dir2, &chk);
			if (rc1)
			{
				printf("\n ERROR in thread_cre_c_d_2\n");
				exit(1);
			}
			pthread_join(th1, NULL);
			pthread_attr_destroy(&attr1);
		}
		else
			if (cipher_file(f_path, th_pt->di_tx, th_pt->ci_flag))
			{
				dbug_p("ERROR,CANT DO CRYPT_C_DIR\n");
				exit(1);
			}
	}

	if (errno && (!r))
	{
		perror("\n ERROR,readdir_C_DIR::");
		exit(1);
	}

	closedir(d);
	pthread_exit(NULL);
	return (void *)1;
}

