CC = gcc
FC = gfortran
LDFLAGS = -lgfortran
DEBUG = -g -O0

vinth2p_dp.o: vinth2p_dp.f
	$(FC) $(DEBUG) -c -fpic vinth2p_dp.f

all: *.o
