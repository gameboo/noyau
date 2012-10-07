CFLAGS=-Wall -g

all : usersched.o testusersched

usersched.o : usersched.c
	gcc -c usersched.c $(CFLAGS)

testmyjmp : testmyjmp.c usersched.o
	gcc testmyjmp.c usersched.o -o testmyjmp $(CFLAGS)

testusersched : testusersched.c usersched.o
	gcc testusersched.c usersched.o -o testusersched $(CFLAGS)

.PHONY : clean

clean :
	rm -f *.o
	rm -f testusersched
	rm -f testmyjmp
