#include <openssl/bn.h>
#include <thread>
#include <vector>
#include <chrono>
#include <iostream>

#ifndef N_THREADS
#define N_THREADS 32*4
#endif
#define N_ITERATIONS 10000000L

void mod_sq(BIGNUM *res) {
    BN_CTX *ctx = BN_CTX_new();
    BIGNUM *x = BN_new();
    BIGNUM *p = BN_new();

    BN_dec2bn(&x, "123456789");
    BN_hex2bn(&p, "1a0111ea397fe69a4b1ba7b6434bacd764774b84f38512bf6730d2a0f6b0f6241eabfffeb153ffffb9feffffffffaaab");
    
    for (long i = 0; i < N_ITERATIONS; i++) {
        BN_mod_sqr(x, x, p, ctx);
    }
    BN_copy(res, x);
    BN_free(x);
    BN_free(p);
    BN_CTX_free(ctx);
}

int main() {
    std::vector<std::thread> threads(N_THREADS);
    BIGNUM *res[N_THREADS];
    for(int i = 0; i < N_THREADS; i++) {
        res[i] = BN_new();
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
        BN_free(res[i]);
    }

    return 0;
}
