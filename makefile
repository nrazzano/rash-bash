CC=g++
CFLAGS=-std=c++11 -lreadline -g

make: rash.cc
	$(CC) rash.cc main.cc -o rash $(CFLAGS)
clean:
	rm rash
