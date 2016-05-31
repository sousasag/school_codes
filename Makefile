SHELL = /bin/bash

all:
	cd ARES; make
	cd interpol_models_marcs; make
	cd MOOG2014; make -f Makefile.rh64silent
	cd TMCALC; make

clean:
	cd ARES; make clean
	cd interpol_models_marcs; make clean
	cd MOOG2014; rm *.o MOOGSILENT
	cd TMCALC; make
