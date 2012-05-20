/* Header file for the aes_encrypt() 
*  @author dZONE
*  ver 1.0 DATE 05-02-2012 
*/

#ifndef AES_CIPHER_H_INCLUDE
#define AES_CIPHER_H_INCLUDE

extern int key_flag;

int aes_init(unsigned char *pwd, unsigned int pwd_len,EVP_CIPHER_CTX *en,EVP_CIPHER_CTX *de,int flag);	/* INITIALIZES THE CIPHER CONEXT */
int aes_encrypt(EVP_CIPHER_CTX *e,int in,int out);							/* ENCRYPTION FUNCTION */
int aes_decrypt(EVP_CIPHER_CTX *d,int in,int out);							/* DECRYPTION FUNCTION */

#endif
