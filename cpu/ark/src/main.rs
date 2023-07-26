use ark_test_curves::bls12_381::Fq as F;
use ark_ff::{BigInt, PrimeField};
use rayon::prelude::*;
use std::time::Instant;

const N_THREADS: i64 = 32*4;
const N_ITERATIONS: i64 = 10000000;

fn mod_sq() -> F {
    let mut x = F::new(BigInt::<6>([123456789, 0, 0, 0, 0, 0]));
    for _ in 0..N_ITERATIONS {
        x = x * x;
    }
    x
}

fn main() {
    let start = Instant::now();

    let modulus = <F as PrimeField>::MODULUS;
    println!("Modulus: {}", modulus);
    let results: Vec<_> = (0..N_THREADS).into_par_iter().map(|_| mod_sq()).collect();

    for (i, x) in results.iter().enumerate() {
        println!("x[{}] = {}", i, x);
    }

    let duration = start.elapsed();
    let duration_in_seconds = duration.as_secs_f64();
    let total_ops = (N_THREADS * N_ITERATIONS) as f64;
    let gops_per_second = total_ops / duration_in_seconds / 1000000000.0;

    println!("Time elapsed is: {:?}", duration);
    println!("Throughput: {:} GOP/s", gops_per_second);
}
