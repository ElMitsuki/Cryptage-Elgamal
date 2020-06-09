all : main main_file_encrypton main_file_decrypton

utils.o : utils.c utils.h
	gcc -Wall -c utils.c

elgamal.o : elgamal.c elgamal.h
	gcc -Wall -c elgamal.c

main : main.c utils.o elgamal.o
	gcc -Wall -o main main.c utils.o elgamal.o

main_file_encrypton : main_file_encrypton.c utils.o elgamal.o
	gcc -Wall -o main_file_encrypton main_file_encrypton.c utils.o elgamal.o

main_file_decrypton : main_file_decrypton.c utils.o elgamal.o
	gcc -Wall -o main_file_decrypton main_file_decrypton.c utils.o elgamal.o

clean:
	rm *.o main
