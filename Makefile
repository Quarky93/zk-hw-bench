.PHONY: cpu gpu
cpu:
	make -C cpu/ark/
	make -C cpu/blst/
	make -C cpu/boost/
	make -C cpu/cryptopp/
	make -C cpu/flint/
	make -C cpu/gmp/
	make -C cpu/ntl/
	make -C cpu/openssl/

gpu:
	make -C gpu/sppark/
