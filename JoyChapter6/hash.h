#pragma

#include <stdint.h>

typedef uint32_t (*HashFunc)    (uint32_t state, uint32_t input);

uint32_t identity_hash          (uint32_t state, uint32_t input);
uint32_t additive_hash          (uint32_t state, uint32_t input);
uint32_t rotating_hash          (uint32_t state, uint32_t input);
uint32_t rotating_hash_rev      (uint32_t state, uint32_t input);
uint32_t one_at_a_time_hash     (uint32_t state, uint32_t input);
uint32_t one_at_a_time_hash_rev (uint32_t state, uint32_t input);
uint32_t jenkins_hash           (uint32_t state, uint32_t input);
