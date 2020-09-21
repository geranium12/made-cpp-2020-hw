#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>



const int EXP_SIZE = 11;
const int DOUBLE_SIZE = 64;

/**
 * Library-level functions.
 * You should use them in the main sections.
 */

uint64_t convertToUint64 (double number) {
    return *((uint64_t *)(&number));
}

bool getBit (const uint64_t number, const uint8_t index) {
    uint64_t mask = 1;
    mask <<= index;
    uint64_t bit = number & mask;
    bit >>= index;
    return bit;
}


/**
 * Checkers here:
 */

bool checkForPlusZero (uint64_t number) {
    return number == 0x0000000000000000;
}

bool checkForMinusZero (uint64_t number) {
    return number == 0x8000000000000000;
}

bool checkForPlusInf (uint64_t number) {
    return number == 0x7FF0000000000000;
}

bool checkForMinusInf (uint64_t number) {
    return number == 0xFFF0000000000000;
}

bool checkSignBit (uint64_t number) {
    return getBit(number, DOUBLE_SIZE - 1);
}

bool checkForNormal (uint64_t number) {
    bool is0 = true;
    bool is1 = true;
    for (int i = 1; i <= EXP_SIZE; i++) {
        is0 &= !getBit(number, DOUBLE_SIZE - i - 1);
        is1 &= getBit(number, DOUBLE_SIZE - i - 1);
    }

    return !(is0 | is1);
}

bool checkForPlusNormal (uint64_t number) {
    return checkForNormal(number) & !checkSignBit(number);
}

bool checkForMinusNormal (uint64_t number) {
    return checkForNormal(number) & checkSignBit(number);
}

bool checkForDenormal (uint64_t number) {
    for (int i = 1; i <= EXP_SIZE; i++) {
        if (getBit(number, DOUBLE_SIZE - i - 1)) {
            return false;
        }
    }

    return !(checkForPlusZero(number) | checkForMinusZero(number));
}

bool checkForPlusDenormal (uint64_t number) {
    return checkForDenormal(number) & !checkSignBit(number);
}

bool checkForMinusDenormal (uint64_t number) {
    return checkForDenormal(number) & checkSignBit(number);
}

bool checkForNan (uint64_t number) {
    for (int i = 1; i <= EXP_SIZE; i++) {
        if (!getBit(number, DOUBLE_SIZE - i - 1)) {
            return false;
        }
    }
    return !checkForPlusInf(number) & !checkForMinusInf(number);
}

bool checkForFirstFractionBit (uint64_t number) {
    return getBit(number, DOUBLE_SIZE - EXP_SIZE - 2);
}

bool checkForSignalingNan (uint64_t number) {
    return checkForNan(number) & !checkForFirstFractionBit(number);
}

bool checkForQuietNan (uint64_t number) {
    return checkForNan(number) & checkForFirstFractionBit(number);
}


void classify (double number) {
    if (checkForPlusZero(convertToUint64(number))) {
        printf("Plus zero\n");
    }

    else if (checkForMinusZero(convertToUint64(number))) {
        printf("Minus zero\n");
    }

    else if (checkForPlusInf(convertToUint64(number))) {
        printf("Plus inf\n");
    }

    else if (checkForMinusInf(convertToUint64(number))) {
        printf("Minus inf\n");
    }

    else if (checkForPlusNormal(convertToUint64(number))) {
        printf("Plus normal\n");
    }

    else if (checkForMinusNormal(convertToUint64(number))) {
        printf("Minus normal\n");
    }

    else if (checkForPlusDenormal(convertToUint64(number))) {
        printf("Plus Denormal\n");
    }

    else if (checkForMinusDenormal(convertToUint64(number))) {
        printf("Minus Denormal\n");
    }

    else if (checkForSignalingNan(convertToUint64(number))) {
        printf("Signaling NaN\n");
    }

    else if (checkForQuietNan(convertToUint64(number))) {
        printf("Quiet NaN\n");
    }

    else {
        printf("Error.\n");
    }
}
