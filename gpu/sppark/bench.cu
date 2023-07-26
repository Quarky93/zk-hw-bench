#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
#include <bls12-381.hpp>

#ifndef N_THREADS
#define N_THREADS 16384
#endif
#define N_ITERATIONS 10000000L
#define WARP_SIZE 32

__global__ void mod_sqr(fp_t res[N_THREADS]) {
    const unsigned long idx = blockIdx.x * blockDim.x + threadIdx.x;
    const uint64_t limbs[6] = {123456789 + idx, 0, 0, 0, 0, 0};
    fp_t x = *((fp_t *) limbs);
    for (long i = 0; i < N_ITERATIONS; i++) {
        x = x * x;
    }
    res[idx] = x;
}

int main() {
    fp_t *h_res = (fp_t *) malloc(sizeof(fp_t) * N_THREADS);
    fp_t *d_res;
    cudaMalloc(&d_res, sizeof(fp_t) * N_THREADS);

    auto t_start = std::chrono::high_resolution_clock::now();
    mod_sqr<<<N_THREADS/WARP_SIZE, WARP_SIZE>>>(d_res);
    cudaDeviceSynchronize();
    auto t_end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(t_end - t_start);
    cudaMemcpy(h_res, d_res, sizeof(fp_t) * N_THREADS, cudaMemcpyDeviceToHost);
    std::cout << "Elapsed time: " << elapsed.count() << " ms\n";
    std::cout << "Throughput: " << (N_THREADS * N_ITERATIONS / 1000000000.0) / (elapsed.count() / 1000.0) << " GOPS\n";

    return 0;
}
