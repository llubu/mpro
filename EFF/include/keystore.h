/** This header file contains functions to create and maintain a keystore
**  @author dZONE
**  DATE 06-05-2012 v1.0 **/

#ifndef KEYSTORE_H_INCLUDE
#define KEYSTORE_H_INCLUDE

int creat_keystore(unsigned char *pwd,unsigned int, unsigned char *k,unsigned char *v);				/* Creates the Keystore for a user */
int read_keystore(unsigned char *pwd,unsigned int pwd_len,unsigned char *k,unsigned char *v);			/* Read KS */

#endif
