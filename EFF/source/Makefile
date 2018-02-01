CC = gcc
CFLAGS = -ggdb -Wall -Werror -DDEBUG

IDIR = /home/abhi/mpro/EFF/include
SDIR = /home/abhi/mpro/EFF/source

LIBS = lcrypto -lpthread

SBU: main.o cipher_file.o cipher_dir2.o cipher_dir3.o cipher_helper.o aes_cipher.o keystore.o
	$(CC) $(CFLAGS) main.o cipher_file.o cipher_dir2.o cipher_dir3.o cipher_helper.o aes_cipher.o keystore.o -$(LIBS) -I$(IDIR) -o SBU

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -I$(IDIR)

cipher_file.o: cipher_file.c
	$(CC) $(CFLAGS) -c cipher_file.c -I$(IDIR)

cipher_dir2.o: cipher_dir2.c
	$(CC) $(CFLAGS) -c cipher_dir2.c -I$(IDIR)

cipher_dir3.o: cipher_dir3.c
	$(CC) $(CFLAGS) -c cipher_dir3.c -I$(IDIR)

cipher_helper.o: cipher_helper.c
	$(CC) $(CFLAGS) -c cipher_helper.c -I$(IDIR)

aes_cipher.o: aes_cipher.c
	$(CC) $(CFLAGS) -c aes_cipher.c -I$(IDIR)

keystore.o: keystore.c
	$(CC) $(CFLAGS) -c keystore.c -I$(IDIR)

clean:
	rm SBU *.o

