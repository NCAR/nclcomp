CC = gcc
FC = gfortran
LDFLAGS = "-lgfortran"

libnclcomp.so: c fortran
	$(CC) $(LDFLAGS) -shared -o libnclcomp.so c/*.c fortran/*.o

all: libnclcomp.so
