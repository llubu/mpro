/* This file contains helper function 
*  @author dZONE
*  DATE 05-05-2012 v1.0 */

#ifndef CIPHER_HELPER_H_INCLUDE
#define CIPHER_HELPER_H_INCLUDE

int replace(char *source,char *dest);			/* Replaces the destination file with the source file */
int file_perres(char *path,struct stat *pt);				/* Restores the file permission of the file */

#endif
