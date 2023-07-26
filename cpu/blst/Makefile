N_HW_THREADS=$(shell nproc)
N_THREADS=$(shell expr $(N_HW_THREADS) \* 4)

bench:
	g++ -march=native -O2 -DN_THREADS=$(N_THREADS) -I../../blst/bindings/ -L../../blst/ bench.cpp -lblst
	sleep 1
	./a.out