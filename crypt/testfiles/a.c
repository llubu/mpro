# include <stdio.h>
# include <pthread.h>
# include "a.h"

int test(int n)
{
	printf("\n fg in test::%d::\n",n);
	return 0;
}

void * chk(void * st)
{

	struct t_data *d;
	d = (struct t_data *) st;
	int t,fg=0;
	printf("\n Th%d:: %s::\n",d->i,d->msg);

	fg = d->i;
	fg +=2;
	t = test(fg);
	printf("\n FG in chk::%d::",fg);

	if(d->i == 1)
	{
		d->msg = "O_th";
	}
	else
	{
		d->msg = "T_th";
	}	
	printf("\n The%d::%s::\n",d->i,d->msg);

	pthread_exit(NULL);
}
