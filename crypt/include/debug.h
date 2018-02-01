#ifndef __DEBUG_H__
#define __DEBUG_H__

//extern int DEBUG;

# ifndef DEBUG
# define DEBUG_T 0

#else
# define DEBUG_T 1
#endif


// http://stackoverflow.com/questions/1644868/c-define-macro-for-debug-printing

#define dbug_p(fmt,...) \
	do { if(DEBUG_T) fprintf(stderr, "%s:%d:%s():" fmt,__FILE__,\
				__LINE__,__func__,##__VA_ARGS__); } while(0)

#endif

