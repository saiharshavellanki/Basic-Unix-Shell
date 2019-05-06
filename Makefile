#Build an executable for shell program
shell : break.o commands.o list.o shell.o
	gcc break.o commands.o list.o shell.o -o shell

break.o : headers.h break.h break.c
	gcc break.c -c -o break.o

commands.o : headers.h commands.h commands.c 
	gcc commands.c -c -o commands.o

list.o : headers.h list.h list.c
	gcc list.c -c -o list.o

shell.o : headers.h break.h shell.c
	gcc shell.c -c -o shell.o

.PHONY : clean

clean : 
	rm -f break.o commands.o list.o shell.o

