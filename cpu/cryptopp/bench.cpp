#include <crypto++/integer.h>
#include <thread>
#include <vector>
#include <chrono>
#include <iostream>

#ifndef N_THREADS
#define N_THREADS 32*4
#endif
#define N_ITERATIONS 1000000L

using CryptoPP::Integer;

void mod_sq(Integer& res) {
    Integer x("123456789");
    Integer p("0x1a0111ea397fe69a4b1ba7b6434bacd764774b84f38512bf6730d2a0f6b0f6241eabfffeb153ffffb9feffffffffaaab");
    
    for (long i = 0; i < N_ITERATIONS; i++) {
        x = a_times_b_mod_c(x, x, p);
    }
    res = x;
}

int main() {
    std::vector<std::thread> threads(N_THREADS);
    std::vector<Integer> res(N_THREADS);

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
