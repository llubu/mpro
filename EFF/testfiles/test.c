# include <stdio.h>
# include <stdlib.h>
# include <openssl/evp.h>
# include <openssl/aes.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
# include <string.h>
# include <assert.h>
# include <error.h>
# include "debug.h"

//# define DEBUG 
int main()
{
	dbug_p();
	int i=0,a=1;
	if((a=i++)) printf("\n A=%d::I=%d\n",a,i);
	printf("\n$$ A=%d::I=%d\n",a,i);
	dbug_p("%d%d",a,i);
	return 0;
}

