/**** Basic *xor* implementation of encrypt and decrypt operations on files **
*** @ author dZONE v1.0 ***/

# include <stdio.h>
# include <stdlib.h>
# include <error.h>
# include <assert.h>
# include <stdint.h>
# include "debug.h"		// My own debug header v1.0

# define SIZE 4096
# define UI unsigned int	
char buf[SIZE];

void encry(FILE **,FILE **);

void encry(FILE **in, FILE **out)
{
	uint8_t key = 179;
	UI rd =0,i =0;
	
	rd = fread(buf,1,4096,(*in));
	if(rd == 0)
	{
		perror("\n ERROR:Cant read file: ");
		exit(1);
	}
	else if(rd)
	{
		(*out) = fopen("enc1.txt","a");
		if((*out) == NULL) 
		{
			perror("\n ERROR:Cant open file to write");
			exit(1);
		}
		i =0;
		while(i<rd)
		{
			buf[i] ^= key;
			++i;
		}
		fwrite(buf,1,rd,(*out));
		
	}
}

int main(void)
{
	FILE *pl = NULL,*ci = NULL;		// pl -to open a plain text file : ci -for the output file encrypted.

	pl = fopen("enc.txt","r");
	if(pl == NULL)
	{
		perror("\n*ERROR*: Cant open file to read:");
		exit(1);
	}
	else if(pl)
	{
		encry(&pl,&ci);
	}	
	
	fclose(pl);
	fclose(ci);
	return 0;
}
 
