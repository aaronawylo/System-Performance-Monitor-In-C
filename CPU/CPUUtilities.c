#include <stdio.h>
#include <math.h>

int removeLeftMostValue(int original, int originalLength){
    int firstDigit = original / pow(10, originalLength - 1);
    int multiplier = pow(10, originalLength-1);
    return original - firstDigit * multiplier ;
}

int getLeftMostValue(int original, int originalLength){
    int firstDigit = original / pow(10, originalLength - 1);
    return firstDigit;
}

int getOddPositionedDigits(int original){
    int oddPositionedDigits = 0;
    int originalLength = floor(log10(abs(original))) + 1;
    int truncatedNumber = removeLeftMostValue(original, originalLength);
    if (originalLength <= 0){
        return 0;
    }
    if (originalLength %2 != 0){
        int multiplier;
        if (originalLength > 1){
            multiplier = pow(10, originalLength/2);
        }
        else {
            multiplier = 1;
        }
        int increment = getLeftMostValue(original, originalLength)*multiplier;
        oddPositionedDigits += increment;
    }
    return oddPositionedDigits += getOddPositionedDigits(truncatedNumber);
}

int main() {
    int original = 89201;
    printf("%d\n", getOddPositionedDigits(original));
    original = 123;
    printf("%d\n", getOddPositionedDigits(original));
    original = 20;
    printf("%d\n", getOddPositionedDigits(original));
    original = 87654321;
    printf("%d\n", getOddPositionedDigits(original));
    return 0;
}