N_HW_THREADS=$(shell nproc)
N_THREADS=$(shell expr $(N_HW_THREADS) \* 4)

bench:
	g++ -std=c++17 -march=native -O2 -DN_THREADS=$(N_THREADS) bench.cpp -lflint
	sleep 1
	./a.out