CC = gcc
FC = gfortran
LDFLAGS = -lgfortran
DEBUG = -g -O0


libnclcomp.so: c fortran
	$(CC) $(LDFLAGS) $(DEBUG) -shared -o libnclcomp.so c/*.c fortran/*.o

all: libnclcomp.so
