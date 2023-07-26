N_HW_THREADS=$(shell nvidia-settings -q CUDACores -t)
N_THREADS=$(shell expr $(N_HW_THREADS) \* 4)

bench:
	nvcc -O2 -DN_THREADS=$(N_THREADS) -I../../blst/bindings/ -I../../blst/src/ -I../../sppark/ff/ -L../../blst/ bench.cu -lblst
	sleep 1
	./a.out