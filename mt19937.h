// (c) Copyright 2015 Lev Barash and Maria Guskova, Science Center in Chernogolovka, Russian Academy of Sciences
// This is supplement to the paper:
// M.S. Guskova, L.Yu. Barash, L.N. Shchur, "RNGAVXLIB: Program library for random number generation, AVX realization."
// e-mail: barash @ itp.ac.ru (remove space)

#define mt19937_N 624

typedef struct{
  unsigned mt_aligned[3*mt19937_N+9] __attribute__ ((aligned(32)));
  unsigned out[3*mt19937_N+9] __attribute__ ((aligned(32)));
  unsigned *mt;
  int mti;
} mt19937_state;

unsigned int mt19937_generate_(mt19937_state* state);

unsigned int mt19937_ansi_generate_(mt19937_state* state);

unsigned int mt19937_sse_generate_(mt19937_state* state);

unsigned int mt19937_avx_generate_(mt19937_state* state);

float mt19937_generate_uniform_float_(mt19937_state* state);

float mt19937_ansi_generate_uniform_float_(mt19937_state* state);

float mt19937_sse_generate_uniform_float_(mt19937_state* state);

float mt19937_avx_generate_uniform_float_(mt19937_state* state);

void mt19937_skipahead_(mt19937_state* state, unsigned long long offset0, unsigned offset_log);

void mt19937_init_(mt19937_state* state);

void mt19937_init_sequence_(mt19937_state* state, unsigned long long SequenceNumber);

void mt19937_print_state_(mt19937_state* state);
