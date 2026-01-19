#ifndef BENCHMARKS_HACKS_HPP
#define BENCHMARKS_HACKS_HPP

// Both functions are taken from https://youtu.be/nXaxk27zwlk?si=wEvzEe4KTWFB5b7d&t=2753
// Note: could be outdated

// Prevent the compiler from optimizing away a variable
static void escape(void* p) {
  asm volatile("" : : "g"(p) : "memory");
}

// Prevent the compiler from optimizing across this point
static void clobber() {
  asm volatile("" : : : "memory");
}

#endif // BENCHMARKS_HACKS_HPP
