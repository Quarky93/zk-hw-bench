#include <blst.h>
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>

#ifndef N_THREADS
#define N_THREADS 32*4
#endif
#define N_ITERATIONS 10000000L

void mod_sq(blst_fp &res) {
    blst_fp x;
    const uint64_t limbs[6] = {123456789, 0, 0, 0, 0, 0};
    blst_fp_from_uint64(&x, limbs);
    for (long i = 0; i < N_ITERATIONS; i++) {
        blst_fp_sqr(&x, &x);
    }
    res = x;
}

int main() {
    std::vector<std::thread> threads(N_THREADS);
    std::vector<blst_fp> res(N_THREADS);

    auto t_start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < N_THREADS; i++) {
        threads[i] = std::thread(mod_sq, std::ref(res[i]));
    }

    for(auto& t : threads) {
        t.join();
    }
    auto t_end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(t_end - t_start);
    std::cout << "Elapsed time: " << elapsed.count() << " ms\n";
    std::cout << "Throughput: " << (N_THREADS * N_ITERATIONS / 1000000000.0) / (elapsed.count() / 1000.0) << " GOPS\n";

    return 0;
}
