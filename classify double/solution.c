#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>



/**
 * Library-level functions.
 * You should use them in the main sections.
 */

uint64_t convertToUint64 (double number) {
    return *((uint64_t *)(&number));
}

bool getBit (const uint64_t number, const uint8_t index) {
    /// Your code here...
    uint64_t mask = (uint64_t)1 << index; // a mask of interested bit
    uint64_t temp = mask & number; // reveal interested bit
    // temp = temp >> index; // to first bit
    return temp; // need just bool
}

/**
 * Checkers here:
 */

uint64_t getExponent (uint64_t number) {
    return (( number << 1 ) >> 53) << 52; // clear sign, clear fraction
}

bool isPositiveSign (uint64_t number) {
    return !( number & 0x8000000000000000 );
}

bool isZeroExponent (uint64_t number) {
    return !( number & 0x7FF0000000000000 );
}

bool isNotZeroFraction (uint64_t number) {
    return number & 0x000FFFFFFFFFFFFF;
}

bool checkForPlusZero (uint64_t number) {
    /// Your code here.
    return number == 0x0000000000000000;
}

bool checkForMinusZero (uint64_t number) {
    return number == 0x8000000000000000;
}

bool checkForPlusInf (uint64_t number) {
    /// Your code here.
    return number == 0x7FF0000000000000;
}

bool checkForMinusInf (uint64_t number) {
    /// Your code here.
    return number == 0xFFF0000000000000;
}

bool checkForPlusNormal (uint64_t number) {
    /// Your code here.
    uint64_t exp = getExponent(number);
    return isPositiveSign(number) && (exp >= 0x0010000000000000) && (exp <= 0x7FE0000000000000 );
}

bool checkForMinusNormal (uint64_t number) {
    /// Your code here.
    uint64_t exp = getExponent(number);
    return !isPositiveSign(number) && (exp >= 0x0010000000000000) && (exp <= 0x7FE0000000000000 );
}

bool checkForPlusDenormal (uint64_t number) {
    /// Your code here.
    return isPositiveSign(number) && isZeroExponent(number) && isNotZeroFraction(number);
}

bool checkForMinusDenormal (uint64_t number) {
    /// Your code here.
    return !isPositiveSign(number) && isZeroExponent(number) && isNotZeroFraction(number);
}

bool checkForSignalingNan (uint64_t number) {
    /// Your code here.
    uint64_t temp = (( number << 1 ) >> 52) << 51; //clear sign, clear fraction without last bit
    return (temp ==  0x7FF0000000000000) && (number & 0x0007FFFFFFFFFFFF); // all singles in exponent and no zero fraction
}

bool checkForQuietNan (uint64_t number) {
    /// Your code here.
    uint64_t temp = (( number << 1 ) >> 52) << 51; //clear sign, clear fraction without last bit
    return (temp ==  0x7FF8000000000000); // all singles in exponent and in last bit of fraction
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
        printf("Signailing NaN\n");
    }

    else if (checkForQuietNan(convertToUint64(number))) {
        printf("Quiet NaN\n");
    }

    else {
        printf("Error.\n");
    }
}
