/*** Header file for the debug function defines a DEBUG variable which can be defined in any source file 
*** @author dZONE 
*** v1.0 
*** Apr23,2012 ***/

#ifndef DEBUG_H_INCLUDE
#define DEBUG_H_INCLUDE

//extern int DEBUG; 


# ifndef DEBUG			//if DEBUG not defines in source == *DEBUG OFF* so DEBUG_T is 0
# define DEBUG_T 0

#else
# define DEBUG_T 1 		//IF DEBUG defined in source == *DEBUG ON* so DEBUG_T is 1
#endif


/******************* MACRO declaration the debug print: **********************************/
/***Chk for more details http://stackoverflow.com/questions/1644868/c-define-macro-for-debug-printing***/

#define dbug_p(fmt,...) \
	do { if(DEBUG_T) fprintf(stderr, "%s:%d:%s():" fmt,__FILE__,\
				__LINE__,__func__,##__VA_ARGS__); } while(0)

/****************************************************************************************/
#endif
