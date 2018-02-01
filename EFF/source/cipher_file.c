# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <error.h>
# include <string.h>
# include <errno.h>

# include <sys/types.h>
# include <sys/stat.h>
# include <openssl/evp.h>

# include "aes_cipher.h"
# include "cipher_helper.h"
# include "debug.h"
# include "cipher_file.h"

#define DO_ENCRYPTION    (1u)
#define DO_DECRYPTION    (2u)

int cipher_file(unsigned char* in_path, EVP_CIPHER_CTX* tx, int ci_flag)
{
	char enc_path[] = "/tmp/do_encr";
	char dec_path[] = "/tmp/do_dec";
	struct stat a; // Strores the original file/owner permission
	int in;
	int out;

	if ((in = open(in_path, O_RDONLY)) < 0)
	{
		perror("\n ERROR,OPEN_INFILE_C_FILE::");
		return 1;
	}

	switch (ci_flag)
	{
	case DO_ENCRYPTION:
		if ((out = open(enc_path, (O_CREAT | O_RDWR),
			(0400 | 0200))) < 0)
		{
			perror("\n ERROR,CANT create temp file_C_FILE::");
			return 1;
		}
		if (fstat(in, &a) == -1)
		{
			perror("\n ERROR,CANT COPY FILE PERMISSION_C_FILE::");
			return 1;
		}
		if (aes_encrypt(tx, in, out))
		{
			dbug_p("ERROR,ENCRYPTING_C_FILE\n");
			return 1;
		}
		dbug_p("ENCRYPTED \n");

		close(in);
		close(out);
		// replacing original file
		if (replace(enc_path, in_path))
		{
			dbug_p("ERROR,REPLACING FILE \n");
			return 1;
		}
		if ((unlink(enc_path)) < 0)
		{
			perror("\n ERROR, Cant unink enc file_C_FILE::");
			return 1;
                }

		if (file_perres(in_path, &a)) // Restoring File permission
		{
			dbug_p("ERROR,in restoring file per EE\n");
			return 1;
		}
		break;
	case DO_DECRYPTION:
		if ((in = open(in_path, O_RDONLY)) < 0)
		{
			perror("\n ERROR,opening encrypted file_C_FILE::");
			return 1;
		}
		if ((out = open(dec_path, (O_CREAT | O_RDWR),
			(0400 | 0200))) < 0)
		{
			perror("\n ERROR,craeting dec file_C_FILE::");
			return 1;
		}

		if (aes_decrypt(tx, in, out))
		{
			dbug_p("ERROR,decrypting file_C_FILE\n");
			return 1;
		}
		dbug_p("DECRYPTED \n");

		close(in);
		close(out);

		if (replace(dec_path, in_path))
		{
			dbug_p("ERROR,REPLACING DEC FILE \n");
			return 1;
		}

		if ((unlink(dec_path)) < 0)
		{
			perror("\n ERROR, Cant unink enc file_C_FILE::");
                        return 1;
		}
		break;
	default:
		printf("\n INCORRECT CHOICE ENTERED \n");
		return 1;
	}
	return 0;
}

