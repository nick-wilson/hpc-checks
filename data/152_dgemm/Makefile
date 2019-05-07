include make.inc

EXE = dgemm_mkl.sequential

all: dgemm_mkl.sequential.x dgemm_mkl.threaded.x dgemm_netlib.x

dgemm_mkl.sequential.x: dgemm_mkl.f
	. $(INCLUDE)/profile && $(FC) $(OPTFLAGS) $^ -o $@ $(LDFLAGS_MKL_SEQ)

dgemm_mkl.threaded.x: dgemm_mkl.f
	. $(INCLUDE)/profile && $(FC) $(OPTFLAGS) $(OMPFLAGS) $^ -o $@ $(LDFLAGS_MKL_OMP)

dgemm_netlib.x: dgemm_netlib.f
	. $(INCLUDE)/profile && $(FC) $(OPTFLAGS) $^ -o $@
