#include <NTL/ZZ.h>
#include <thread>
#include <vector>
#include <chrono>
#include <iostream>

#ifndef N_THREADS
#define N_THREADS 32*4
#endif
#define N_ITERATIONS 10000000L

void mod_sq(NTL::ZZ &res) {
    NTL::ZZ x, p;

    NTL::conv(x, "123456789");
    NTL::conv(p, "4002409555221667393417789825735904156556882819939007885332058136124031650490837864442687629129015664037894272559787");
    
    for (long i = 0; i < N_ITERATIONS; i++) {
        NTL::SqrMod(x, x, p);
    }
    res = x;
}

int main() {
    std::vector<std::thread> threads(N_THREADS);
    NTL::ZZ res[N_THREADS];

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
