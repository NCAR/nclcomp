CC = gcc
FC = gfortran
CPPFLAGS = -I..
LDFLAGS = -L.. -lnclcomp -lgfortran -Wl,-rpath,$(PWD)/..

test: ../libnclcomp.so
	$(CC) $(CPPFLAGS) $(LDFLAGS) test.c -o test
	DYLD_LIBRARY_PATH=.. ./test

all: test
