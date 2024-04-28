#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

// INITIAL PERMUTATION TABLE
static char IP[] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};

// FINAL PERMUTATION TABLE
static char FP[] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25
};

// EXPLANSION TABLE
static char E[] = {
    32, 1, 2, 3, 4, 5,
    4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 1
};

// PERMUTATION TABLE
static char P[] = {
    16, 7, 20, 21,
    29, 12, 28, 17,
    1, 15, 23, 26,
    5, 18, 31, 10,
    2, 8, 24, 14,
    32, 27, 3, 9,
    19, 13, 30, 6,
    22, 11, 4, 25
};

// SUBSTITUTION S-BOXES
static char S[8][64] = {
    {
        14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
        0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
        4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
        15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13
    },
    {
        15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
        3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
        0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
        13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9
    },
    {
        10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
        13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
        13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
        1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12
    },
    {
        7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
        13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
        10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
        3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14
    },
    {
        2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
        14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
        4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
        11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3
    },
    {
        12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
        10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
        9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
        4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13
    },
    {
        4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
        13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
        1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
        6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12
    },
    {
        13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
        1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
        7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
        2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
    }
};

// PERMUTED CHOICE 1 TABLE
static char PC1[] = {
    57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,

    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4
};

// PERMUTED CHOICE 2 TABLE
static char PC2[] = {
    14, 17, 11, 24, 1, 5,
    3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

// KEY SCHEDULE SHIFTS ARRAY
static char key_schedule_shift[] = {
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

uint64_t initial_permutation(const uint64_t input) {
    uint64_t result = 0;

    for (int i = 0; i < 64; i++) {
        result <<= 1;
        result |= (input >> (64 - IP[i])) & 0x0000000000000001;
    }

    return result;
}

uint64_t final_permutation(const uint64_t input) {
    uint64_t result = 0;

    for (int i = 0; i < 64; i++) {
        result <<= 1;
        result |= (input >> (64 - FP[i])) & 0x0000000000000001;
    }

    return result;
}

uint64_t *key_schedule(const uint64_t key) {
    uint64_t pc1_temp = 0;
    uint64_t pc2_temp = 0;

    uint64_t *sub_keys = malloc(16 * sizeof(uint64_t));
    for (int i = 0; i < 16; i++) {
        sub_keys[i] = 0;
    }
    uint32_t C = 0;
    uint32_t D = 0;

    // 64 bits -> 56 bits
    for (int i = 0; i < 56; i++) {
        pc1_temp <<= 1;
        pc1_temp |= (key >> (64 - PC1[i])) & 0x0000000000000001;
    }

    // left 28 bits
    C = (uint32_t) ((pc1_temp >> 28) & 0x000000000fffffff);
    // right 28 bits
    D = (uint32_t) (pc1_temp & 0x000000000fffffff);

    // Subkeys
    for (int i = 0; i < 16; i++) {
        // shift
        for (int j = 0; j < key_schedule_shift[i]; j++) {
            C = (C << 1) & 0x0fffffff | (C >> 27) & 0x00000001;
            D = (D << 1) & 0x0fffffff | (D >> 27) & 0x00000001;
        }

        pc2_temp = 0;
        pc2_temp = (((uint64_t) C) << 28) | (uint64_t) D;

        sub_keys[i] = 0;

        for (int j = 0; j < 48; j++) {
            sub_keys[i] <<= 1;
            sub_keys[i] |= (pc2_temp >> (56 - PC2[j])) & 0x0000000000000001;
        }
    }

    return sub_keys;
}

uint32_t permutation(uint32_t input) {
    uint32_t result = 0;

    for (int j = 0; j < 32; j++) {
        result <<= 1;
        result |= (input >> (32 - P[j])) & 0x00000001;
    }
    return result;
}

uint32_t substitution(const uint64_t input) {
    uint32_t result = 0;

    for (int j = 0; j < 8; j++) {
        /*
          0000 0000 0000 0000 |  1010 11  00 1010   0110 01  10 1010   1101 01  01 1111   1010 01  10 1001 (example 48 bits)
          0000 0000 0000 0000 |  1000 01  00 0000   0000 00  00 0000   0000 00  00 0000   0000 00  00 0000
                                 8	  4
          0000 0000 0000 0000 |  0111 10  00 0000   0000 00  00 0000   0000 00  00 0000   0000 00  00 0000
                                 7    8
         */
        // GET THE 6 BITS ALL THE WAY TO THE RIGHT (LSB)
        char row = ((input & (0x0000840000000000 >> 6 * j)) >> 42 - 6 * j);
        // GET THE 2 BITS NEXT TO ONE ANOTHER TO FORM A NUMBER
        row = (row >> 4) | row & 0x01;

        char column = ((input & (0x0000780000000000 >> 6 * j)) >> 43 - 6 * j);

        result <<= 4;
        result |= (uint32_t) (S[j][16 * row + column] & 0x0f);
    }
    return result;
}

uint64_t explansion(const uint32_t input) {
    uint64_t result = 0;

    for (int j = 0; j < 48; j++) {
        result <<= 1;
        result |= (uint64_t) ((input >> (32 - E[j])) & 0x00000001);
    }

    return result;
}

uint64_t des(const uint64_t input, const uint64_t key, const char mode) {
    uint64_t init_perm_res = 0;
    uint64_t *sub_keys = key_schedule(key);
    uint32_t L = 0;
    uint32_t R = 0;
    uint64_t sub_input_block = 0;
    uint32_t sub_output_block = 0;
    uint32_t final_block = 0;
    uint32_t temp = 0;
    uint64_t final_per_input = 0;
    uint64_t result = 0;

    // INITIAL PERMUTATION
    init_perm_res = initial_permutation(input);

    // GETTING LEFT AND RIGHT BLOCKS
    L = (uint32_t) (init_perm_res >> 32) & 0x00000000ffffffff;
    R = (uint32_t) init_perm_res & 0x00000000ffffffff;

    // MAIN LOGIC
    for (int i = 0; i < 16; i++) {
        // FEISTEL FUNCTION
        sub_input_block = 0;

        // EXPANSION
        sub_input_block = explansion(R);

        // XOR
        if (mode == 'd') {
            // decryption
            sub_input_block = sub_input_block ^ sub_keys[15 - i];
        } else {
            // encryption
            sub_input_block = sub_input_block ^ sub_keys[i];
        }

        // SUBSTITUTION
        sub_output_block = substitution(sub_input_block);

        // PERMUTATION
        final_block = permutation(sub_output_block);

        temp = R;
        R = L ^ final_block;
        L = temp;
    }

    free(sub_keys);

    temp = R;
    R = L;
    L = temp;

    final_per_input = (((uint64_t) L) << 32) | (uint64_t) R;

    // FINAL PERMUTATION
    result = final_permutation(final_per_input);

    return result;
}

uint64_t generateRandomHex() {
    srand(time(NULL));

    const unsigned int lower = rand();
    const unsigned int upper = rand();

    const uint64_t result = ((uint64_t) upper << 32) | lower;

    return result;
}

int main() {
    const uint64_t input = generateRandomHex();
    const uint64_t key = generateRandomHex();
    uint64_t result = 0;

    printf("E: %016llx\n", input);

    result = des(input, key, 'e');
    printf("E: %016llx\n", result);

    result = des(result, key, 'd');
    printf("D: %016llx\n", result);

    exit(0);
}
