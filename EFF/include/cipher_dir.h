#ifndef __CIPHER_DIR_H__
#define __CIPHER_DIR_H__

struct thread_arg
{
	unsigned char in_path[4096];
	EVP_CIPHER_CTX *di_tx;
	int ci_flag;
};

int cipher_dir(unsigned char* in_path, EVP_CIPHER_CTX* di_tx, int ci_flag);
void* cipher_dir2(void* st);

#endif

