MPICC = mpiicc
FC = ifort
CC = icc
CFLAGS = -g
OPTFLAGS = -O3 -xHost
OMPFLAGS = -qopenmp
STREAMFLAGS = -opt-streaming-stores always
LDFLAGS_MKL_SEQ = -L$(MKLROOT)/lib/intel64 -lmkl_intel_lp64 -lmkl_sequential -lmkl_core
LDFLAGS_MKL_OMP = -L$(MKLROOT)/lib/intel64 -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5

default: all

clean:
	rm -f *.x *.o *~
