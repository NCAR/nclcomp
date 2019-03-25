CC = gcc
FC = gfortran
LDFLAGS = -lgfortran
DEBUG = -g -O0

all: fortran.o libncomp.so

.PHONY: fortran.o
fortran.o:
	$(MAKE) -C fortran

libncomp.so: fortran.o
	$(CC) $(LDFLAGS) $(DEBUG) -fPIC -shared -o libncomp.so c/*.c fortran/*.o c/ncomp.h

clean:
	-rm -rf libncomp.so libncomp.so.dSYM
	$(MAKE) -C fortran clean
