#include <gmp.h>
#include <thread>
#include <vector>
#include <chrono>
#include <iostream>

#ifndef N_THREADS
#define N_THREADS 32*4
#endif
#define N_ITERATIONS 10000000L

void mod_sq(mpz_t res) {
    mpz_t x, p, tmp;

    mpz_init_set_str(x, "123456789", 10);
    mpz_init_set_str(p, "1a0111ea397fe69a4b1ba7b6434bacd764774b84f38512bf6730d2a0f6b0f6241eabfffeb153ffffb9feffffffffaaab", 16);
    mpz_init(tmp);

    for (long i = 0; i < N_ITERATIONS; i++) {
        mpz_mul(tmp, x, x);
        mpz_mod(x, tmp, p);
    }
    mpz_set(res, x);
    mpz_clears(x, p, tmp, NULL);
}

int main() {
    std::vector<std::thread> threads(N_THREADS);
    mpz_t res[N_THREADS];
    for (int i = 0; i < N_THREADS; i++) {
        mpz_init(res[i]);
    }

    auto t_start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < N_THREADS; i++) {
        threads[i] = std::thread(mod_sq, res[i]);
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
