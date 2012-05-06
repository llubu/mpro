CC = gcc
CFLAGS = -ggdb -Wall -Werror -DDEBUG

IDIR = /home/abhi/mpro/EFF/include
SDIR = /home/abhi/mpro/EFF/source

LIBS = lcrypto

test: cipher_file.o cipher_helper.o aes_cipher.o 
	$(CC) $(CFLAGS) cipher_file.o cipher_helper.o aes_cipher.o -$(LIBS) -I$(IDIR) -o test

cipher_file.o: cipher_file.c
	$(CC) $(CFLAGS) -c cipher_file.c -I$(IDIR)

cipher_helper.o: cipher_helper.c cipher_helper.h
	$(CC) $(CFLAGS) -c cipher_helper.c -I$(IDIR)

aes_cipher.o: aes_cipher.c aes_cipher.h
	$(CC) $(CFLAGS) -c aes_cipher.c -I$(IDIR)

clean:
	rm test *.o
