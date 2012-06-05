# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include "a.h"

int main()
{
	pthread_t th[2];

	pthread_attr_t att;
	int rc,i;

	pthread_attr_init(&att);
	pthread_attr_setdetachstate(&att, PTHREAD_CREATE_JOINABLE);


	struct t_data arr[2];
	
	arr[0].i = 1;
	arr[0].msg = "one th";

	arr[1].i=2;
	arr[1].msg = "two th";

	for(i=0;i<2;i++)
	{
		rc = pthread_create(&th[i],&att,chk,(void *) &arr[i]);
		if(rc)
		{
			printf("\n ERROR in thread%d\n",i);
			return 1;
		}
	}


	for(i=0;i<2;i++)
	{
		pthread_join(th[i],NULL);
	}
	for(i=0;i<2;i++)
	{
		printf("\nMain Th%d::%s::\n",arr[i].i,arr[i].msg);
	}

	pthread_attr_destroy(&att);
	pthread_exit(NULL);	
	return 0;
}
