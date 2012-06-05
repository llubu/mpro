/* Header file for cipher_dir() 
*  @author dZONE
*  DATE(C,M) 05-15-2012		05-15-2012
*/


#ifndef CIPHER_DIR_H_INCLUDE
#define CIPHER_DIR_H_INCLUDE

struct thread_arg
{
	unsigned char in_path[4096];
	EVP_CIPHER_CTX *di_tx;
	int ci_flag;
};

int cipher_dir(unsigned char *in_path,EVP_CIPHER_CTX *di_tx,int ci_flag);
void * cipher_dir2(void * st);							/* used for POSIX thread handling of Directory */

#endif

