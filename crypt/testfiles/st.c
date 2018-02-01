# include <stdio.h>
# include <stdlib.h>
# include <string.h>

int main()
{
	struct chk
	{
		int a;
		char m[64];
	};
	struct chk ck;

	struct test
	{
		int t;
		char msg[256];
		struct chk e;
	};

	struct test st;

	char string[64] = "ABC1";
	char string2[64] = "DEF2";

	int v=1;
	int z=2;

	st.t=1;
	
	strncpy(st.msg,string,sizeof(string));

	ck.a=z;
	strncpy(ck.m,string2,sizeof(string2));

	st.e = ck;

	printf("ST.T:%d:\n",st.t);
	printf("ST.MSG:%s:\n",st.msg);
	printf("ST.e.a:%d:\n",st.e.a);
	printf("ST.e.m:%s:\n",st.e.m);
	
	return 0;
}		
