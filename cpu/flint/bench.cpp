#include <flint/flint.h>
#include <flint/fmpz.h>
#include <thread>
#include <vector>
#include <chrono>
#include <iostream>

#ifndef N_THREADS
#define N_THREADS 32*4
#endif
#define N_ITERATIONS 10000000L

void mod_sq(fmpz_t res) {
    fmpz_t x, p;
    fmpz_init(x);
    fmpz_init(p);

    fmpz_set_str(x, "123456789", 10);
    fmpz_set_str(p, "1a0111ea397fe69a4b1ba7b6434bacd764774b84f38512bf6730d2a0f6b0f6241eabfffeb153ffffb9feffffffffaaab", 16);
    
    for (long i = 0; i < N_ITERATIONS; i++) {
        fmpz_mul(x, x, x);
        fmpz_mod(x, x, p);
    }

    fmpz_set(res, x);
    fmpz_clear(x);
    fmpz_clear(p);
}

int main() {
    std::vector<std::thread> threads(N_THREADS);
    fmpz_t res[N_THREADS];
    for(int i = 0; i < N_THREADS; i++) {
        fmpz_init(res[i]);
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

    for(int i = 0; i < N_THREADS; i++) {
        fmpz_clear(res[i]);
    }

    return 0;
}
