#include <iostream>
#include <cstdint>

/*
Calculate hash using a randomly seeded murmur finalizer
*/
inline uint64_t hashFunction(uint64_t key, uint64_t seed){
    uint64_t h = key + seed;
    h ^= h >> 33;
    h *= UINT64_C(0xff51afd7ed558ccd);
    h ^= h >> 33;
    h *= UINT64_C(0xc4ceb9fe1a85ec53);
    h ^= h >> 33;
    return h;
}

/*
Compute fingerprint 
*/
template<typename FingerprintType>
inline FingerprintType compute_fingerprint(uint64_t key, uint64_t seed){
    seed <<= 32;
    uint64_t h = hashFunction(key, seed);
    return  static_cast<FingerprintType>(h ^ (h >> 32));
}


/*
Calculte a hash and limit it to a range for xor filter
*/
inline uint32_t compute_hash(uint64_t key, uint64_t seed, int index, int c){
    uint64_t h = hashFunction(key, seed);
    h = (uint32_t) h;
    uint32_t rangeMin = index * (c/3);
    uint32_t rangeMax = (c/3 * (index +1)) -1;
    if (index == 2) {
        rangeMax += c % 3;
    }
    h = h % (rangeMax - rangeMin + 1) + rangeMin;
    
    return (uint32_t)h;
}

/*
Compute 32 bit hash for bloom filter
*/
inline uint32_t compute_hash(uint64_t key, uint64_t seed){
    uint64_t h = hashFunction(key, seed);
    h = (uint32_t) h;
    return (uint32_t)h;
}