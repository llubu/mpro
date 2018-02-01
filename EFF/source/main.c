/* This is the main() for CrypTIt project
 *  @author dZONE
 *  DATE:(C,M) 05-13-2012	05-18-2012
 *
 *	This function takes all input from the user which are:
 *	1. Password
 *	2. Path of the file or Directory to be encrypted/Decrypted
 *	3. Choice for Encryption or Decryption
 *
 *	Also InIt & clear the Cipher Context
 */

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <termios.h>    // For  secure password input
# include <error.h>

# include <sys/types.h>
# include <sys/stat.h>
# include <openssl/evp.h>
# include <openssl/aes.h>

# include "aes_cipher.h"
# include "cipher_file.h"
# include "cipher_dir.h"
# include "debug.h"

int key_flag;

#define DO_ENCRYPTION    (1u)
#define DO_DECRYPTION    (2u)

int main()
{
	// MAX_PATH defined in /usr/include/linux/limits.h
	unsigned char in_path[4096];
	unsigned char pwd[256];
	int ci_flag = 0;
	int dir_flag = 0; // Encryption1 Decryption2 DIR: 1 FILE: 0
	unsigned int i;
	unsigned int npwd = 0;
	unsigned int upwd = 0;
	unsigned int lenpwd = 0;
	struct termios old,new; // Struct to hold old and new terminal settings
	struct stat in;
	char ch;
	EVP_CIPHER_CTX en,de;

	printf("\n Enter the path(absolute) of the file/directory\n "
		"(e.g. /home/user/plain.txt) \n");

	if ((fgets((char *)in_path, 4096, stdin)) == NULL)
	{
		perror("\n ERROR,fgets_INPATH_MAIN::");
		return 1;
	}

	i = 0;
	while ((in_path[i] != '\n') && (in_path[i] != '\0'))
		++i;

	if (in_path[i] == '\n') // Truncating the extra newline if present
	{
		in_path[i] ='\0';
		i = 0;
	}
	dbug_p("PATH::%s::\n",in_path);

	if (stat((char *)in_path,&in) != 0)
	{
		perror("\n ERROR,STAT_MAIN::");
		return 1;
	}
	if (S_IFDIR == (in.st_mode & S_IFMT))
		dir_flag = 1; // Checking for directory or file
	dbug_p("DIR::%d::\n",dir_flag);

	printf("\n Enter Choice 1: ENCRYPTION  2: DECRYPTION \n");
	scanf("%d",&ci_flag);

	if ((ci_flag != 1) && (ci_flag != 2))
		printf("\n Incorrect Choice Entered \n");

	if (ci_flag == 1)
	{
		printf("\n Enter 1 to create new keystore 2 "
			"to use existing keystore \n");
		scanf("%d", &key_flag);
	}

	// truncate any newline if present in input buffer
	while ((ch = getchar()) != '\n');

	// gets the Current setting of STDIN
	if (tcgetattr(fileno(stdin), &old) != 0)
	{
		perror("\n ERROR,TCGET_MAIN::");
		return 1;
	}
	new = old;
	new.c_lflag &= ~ECHO; // Setting ECHO off

	if (tcsetattr(fileno(stdin), TCSAFLUSH, &new) != 0)
	{
		perror("\n ERROR,TCSSET1_MAIN::");
		return 1;
	}

	printf("\n Password \n");
	if ((fgets((char *)pwd, 256, stdin)) == NULL)
	{
		perror("\n ERROR,fgets_PWD_MAIN::");
		return 1;
	}
	// Restoring STDIN to old setting
	if (tcsetattr(fileno(stdin),TCSAFLUSH,&old) != 0)
	{
		perror("\n ERROR,TCSSET2_MAIN::");
		return 1;
	}

	i = 0;
	// Password complexity Rules
	while ((pwd[i] != '\n') && (pwd[i] != '\0'))
	{
		if ((pwd[i] >= 48) && (pwd[i] <= 57))
			++npwd;
		if ((pwd[i] >= 65) && (pwd[i] <= 90))
			++upwd;
		++i;
	}

	if (pwd[i] == '\n') // truncating newline if present
	{
		pwd[i] ='\0';
	}
	lenpwd = i;

	if ((npwd < 2u) || (!upwd) || (lenpwd < 7u) || (lenpwd > 30u))
	{
		printf("\n Password Complexity Rules not satisfied:\n "
			"MIN LENGTH:-7  MAX LENGTH:-30 MIN NUMERALS:-2  "
			"MIN UpperCase:-1 \n");
		return 1;
	}
	dbug_p("PWD::%s:: LEN::%d::\n", pwd, lenpwd);

	switch (ci_flag)
	{
	case DO_ENCRYPTION: // Encryption Init
		if (aes_init(pwd, lenpwd, &en, NULL, 1))
		{
			dbug_p("ERROR,ENC_INIT_MAIN\n");
			return 1;
		}

		if (dir_flag)
		{
			if (cipher_dir(in_path, &en, ci_flag))
			{
				dbug_p("ERROR,CIPHER_DIR_MAIN\n");
				return 1;
			}
		}
		else
		{
			if (cipher_file(in_path, &en, ci_flag))
			{
				dbug_p("ERROR,CIPHER_FILE_MAIN\n");
				return 1;
			}
		}
		break;

	case DO_DECRYPTION: // Decryption Init
		if (aes_init(pwd, lenpwd, (EVP_CIPHER_CTX *)NULL, &de, 2))
		{
			dbug_p("ERROR,DEC_INIT_MAIN\n");
			return 1;
		}

		if (dir_flag)
		{
			if (cipher_dir(in_path,&de,ci_flag))
			{
				dbug_p("ERROR,D_CIPHER_DIR_MAIN\n");
				return 1;
			}
		}
		else
		{
			if(cipher_file(in_path,&de,ci_flag))
			{
				dbug_p("ERROR,D_CIPHER_FILE_MAIN\n");
				return 1;
			}
		}
		break;
	default:
		printf("\n Incorrect Choice Entered \n");
		return 1;
	}

	if (ci_flag == 1)
		EVP_CIPHER_CTX_cleanup(&en);
	if (ci_flag == 2)
		EVP_CIPHER_CTX_cleanup(&de);
	return 0;
}

