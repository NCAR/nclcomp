CC = gcc
FC = gfortran
LDFLAGS = -lgfortran
DEBUG = -g -O0

all: fortran.o libnclcomp.so

.PHONY: fortran.o
fortran.o:
	$(MAKE) -C fortran

libnclcomp.so: fortran.o
	$(CC) $(LDFLAGS) $(DEBUG) -shared -o libnclcomp.so c/*.c fortran/*.o

clean:
	-rm -rf libnclcomp.so libnclcomp.so.dSYM
	$(MAKE) -C fortran clean
