CC = gcc
FC = gfortran
LDFLAGS = -lgfortran
DEBUG = -g -O0

all: vinth2p_dp.o areaAve.o dtrend_dp.o runave_dp.o

areaAve.o: areaAve.f
	$(FC) $(DEBUG) -c -fpic areaAve.f

dtrend_dp.o: dtrend_dp.f
	$(FC) $(DEBUG) -c -fpic dtrend_dp.f

runave_dp.o: runave_dp.f
	$(FC) $(DEBUG) -c -fpic runave_dp.f

vinth2p_dp.o: vinth2p_dp.f
	$(FC) $(DEBUG) -c -fpic vinth2p_dp.f

clean:
	-rm -f areaAve.o vinth2p_dp.o dtrend_dp.o runave_dp.o
