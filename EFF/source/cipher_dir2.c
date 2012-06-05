/* This contains cipher_dir2() which handles the crypt operation using threads for each dir which is called from cipher_dir.c
*  @author dZONE
*  DATE(C,M) 05-15-2012		05-31-2012
*/

# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <dirent.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <string.h>
# include <pthread.h>
# include <openssl/evp.h>
# include <errno.h>
# include <error.h>
# include "cipher_dir.h"
# include "cipher_file.h"
# include "debug.h"

void* cipher_dir2(void * st)
{

		struct thread_arg * th_pt;
		th_pt = (struct thread_arg *) st;					/* Pointer to the thread argument structure */
		
		DIR *d;
		struct dirent *r;
		struct stat chk;
		char *name = NULL;						/* File name */
		unsigned char f_path[4096]={0};
		int i=0,j=0,b=0,rc1=0;

		pthread_t th1;
		pthread_attr_t attr1;
	
		pthread_attr_init(&attr1);
		pthread_attr_setdetachstate(&attr1, PTHREAD_CREATE_JOINABLE);		
		
//		sleep(70);
//		printf("\n After 1 sleep\n");

		while(th_pt->in_path[i] != '\0' && i<4096)
		{
			f_path[i] = th_pt->in_path[i];
			++i;
		}
		f_path[i++] = '/';
		dbug_p("PATH is:%s:\n",th_pt->in_path);
	
		if((d=opendir((char *)th_pt->in_path)) == NULL)
		{
			perror("\n ERROR,Opendir_C_DIR::");
			exit(1);
		}
		
		errno=0;						/* Setting errno to 0 to diffrentiate between end of dir & ERROR */
		while((r=readdir(d)) != NULL)
		{
			j=0,b=i;
			name = r->d_name;
			if(name[0] == 46 && (name[1] == 46 || name[1] == '\0')) continue;	/* handling '.' & '..' */
			
			while(name[j] != '\0' && b <4095 && j<256)
			{
				f_path[b++] =	name[j++];
			}
			if(b<4095)
			{	
				f_path[b] = '\0';
				dbug_p("F_PATH:%s:\n",f_path);
			}
			else { printf("\n ERROR,PATH TOO LONG_C_DIR\n"); exit (1);}

			if(stat((char *)f_path,&chk) !=0)				/* ?? improve handling nested directories */
			{
				perror("\n ERROR,stat_C_DIR::");
				exit(1);
			}
			if(S_IFDIR == (chk.st_mode & S_IFMT))         		/* Checking for directory or file */
			{
				struct thread_arg chk;
				
				strncpy((char *)chk.in_path,(char *)f_path,sizeof(f_path));
				dbug_p("PATH copied to struct:%s:\n",chk.in_path);
				chk.di_tx = th_pt->di_tx;			/* Copying add of cipher context */
				chk.ci_flag = th_pt->ci_flag;
 
//				sleep(70);
//				printf("\n After 2 SLeep\n");
				rc1=pthread_create(&th1,&attr1,cipher_dir2,(void *) &chk);
				if(rc1)
				{
					printf("\n ERROR in thread_cre_c_d_2\n");
					exit(1);
				}
				pthread_join(th1,NULL);
				pthread_attr_destroy(&attr1);
//				pthread_exit(NULL);

/*				if(cipher_dir(f_path,di_tx,ci_flag))
				{
					dbug_p("ERROR,RECURSIVE_C_DIR\n");
					return 1;
				} */
				
			}
			else
			if(cipher_file(f_path,th_pt->di_tx,th_pt->ci_flag))	/* Calling cipher_file() to do crypt ope. on files in dec */
			{
				dbug_p("ERROR,CANT DO CRYPT_C_DIR\n");
				exit(1);
			}
		}
		
		if(errno && !r)
		{
			perror("\n ERROR,readdir_C_DIR::");
			exit(1);
		}

		closedir(d);				/* Closing Directory stream */
		pthread_exit(NULL);
	return (void *) 1;					/* SUCCESS */
}
