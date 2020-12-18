// William McLaughlin
// Project 4 Part 1 + 2

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <math.h>

// This is my struct for each virtual address
// in the pagefile.
struct address {
    char valid[80];
    char permission[80];
    char physPage[80];
    char useBit[80];
    int size;

};

struct clock {
    char useBit[80];
    char physPage[80];
    int virtPage;
    int size;
};

// This function interprets the input file.
// It gets the values for the size of virtual
// addresses and physical, the size of a page
// and builds an array of virtual addresses
int parsePage(char *filename, struct address *page, struct clock *clockTable,
               char *virtBit, char *physBit, char *pageByte) {
    FILE *fp;

    fp = fopen(filename, "r");

    int lineCount = 0;
    int clockCount;

    if (fp) {
        int charCount = 0;
        char *p;
        char buf[1024];

        while (fgets(buf, sizeof(buf), fp) != NULL) {
            charCount = 0;
            p = strtok(buf, " ");

            if (lineCount == 0) { // The first line of the file
                while (p != NULL) {
                    switch (charCount) {
                        case 0:                 // The first word of the first line
                            strcpy(virtBit, p);
                            break;
                        case 1:                 // The second word
                            strcpy(physBit, p);
                            break;
                        case 2:                 // The third word
                            strcpy(pageByte, p);
                            break;
                        default:
                            break;
                    }
                    charCount++;
                    p = strtok(NULL, " ");
                }
            } else { // Not the first line

                int valid = 0;
                while (p != NULL) {
                    switch (charCount) { // Go through each line one word at a time
                        case 0:
                            strcpy(page[lineCount-1].valid, p);
                            if (*p == '1') {
                                clockCount++;
                                valid = 1;
                            }
                            break;
                        case 1:
                            strcpy(page[lineCount-1].permission, p);
                            if (valid == 1) {
                                clockTable[clockCount - 1].virtPage = lineCount - 1;
                            }

                            break;
                        case 2:
                            strcpy(page[lineCount-1].physPage, p);
                            if (valid == 1)
                                strcpy(clockTable[clockCount-1].physPage, p);
                            break;
                        case 3:
                            strcpy(page[lineCount-1].useBit, p);
                            if (valid == 1)
                                strcpy(clockTable[clockCount - 1].useBit, p);
                            break;
                        default:
                            break;
                    }
                    charCount++;
                    p = strtok(NULL, " ");
                }
            }
            lineCount++;
        }
    } else {
        printf("Failed to open the file\n");
        return 1;
    }

    page[0].size = lineCount; // Number of virtual addresses
    clockTable[0].size = clockCount;

    return 0;
}


// This function converts hexadecimal to binary
int hexToBin(char *binAddress, char *address){
    int i, j;

    for(j = i = 0; i < strlen(address) - 1; ++i, j += 4){

        switch(tolower(address[i])){
            case '0': strcpy(binAddress + j, "0000");
                break;
            case '1': strcpy(binAddress + j, "0001");
                break;
            case '2': strcpy(binAddress + j, "0010");
                break;
            case '3': strcpy(binAddress + j, "0011");
                break;
            case '4': strcpy(binAddress + j, "0100");
                break;
            case '5': strcpy(binAddress + j, "0101");
                break;
            case '6': strcpy(binAddress + j, "0110");
                break;
            case '7': strcpy(binAddress + j, "0111");
                break;
            case '8': strcpy(binAddress + j, "1000");
                break;
            case '9': strcpy(binAddress + j, "1001");
                break;
            case 'a': strcpy(binAddress + j, "1010");
                break;
            case 'b': strcpy(binAddress + j, "1011");
                break;
            case 'c': strcpy(binAddress + j, "1100");
                break;
            case 'd': strcpy(binAddress + j, "1101");
                break;
            case 'e': strcpy(binAddress + j, "1110");
                break;
            case 'f': strcpy(binAddress + j, "1111");
                break;
            default:
                printf("invalid character %c\n", address[i]);
                strcpy(binAddress + j, "0000");
                return 0;
        }
    }
    return 1;
}


// This function does exponential math because blue doesn't have cmath
int power(int a, int b) {

    for (int i = 0; i < b - 1; ++i) {
        a *= a;
    }

    if (b == 0)
        a = 1;
    return a;
}


// This function converts from binary back into hex
int convertBinaryToDecimal(char* n)
{
    int decimalNumber = 0, i = 0, remainder;
    char* end;
    int temp = strtol(n, &end, 10);

    while (temp !=0)
    {
        remainder = temp % 10;
        temp /= 10;

        decimalNumber += remainder*power(2,i);
        ++i;
    }
    return decimalNumber;
}


// This function is called indefinitely to get input from the user
int getInput(char *str, struct address *page, struct clock *clockTable, int count){

    printf("\n\n\n>> ");

    if (fgets(str, sizeof(str), stdin) == NULL) {
        printf("\nFailed to read input\n");
        return count;
    }

    if(strcmp(str, "exit") == 0) // Check if we're trying to exit
        return 0;


    if (str[0] == '0' && str[1] == 'x') { // Check if user gave leading '0x'
        char temp[strlen(str) - 2];
        for (int i = 2; i < strlen(str); ++i) { // Remove it!
            temp[i-2] = str[i];
        }

        strcpy(str, temp);
        str[strlen(str)] = '\0'; //strcpy doesn't copy the null terminator?
    }

    char original[32];
    strcpy(original, str); // Hold original input


#ifdef PROB1
#else
    for (int i = 0; i < clockTable[0].size - 1; ++i) { // Check if virtual page is already mapped
        if (clockTable[i].virtPage == original[0] - '0') {

            original[0] = clockTable[i].physPage[0];
            original[2] = '\0';
            printf("\nPAGE FAULT\nPhysical Address: 0x%s", original);

            return count;
        }

    }
#endif

    int tempSize = ((sizeof(str) + 1) * 4) +1;
    char bin[tempSize];
     if (hexToBin(bin, str) == 0) // Convert input to binary
         return count;  // Bad input

    char temp2[8];
    int tempCount = 0;
    for (int j = strlen(bin) - 8; j < strlen(bin); ++j) { // Truncate binary to 8bits
        temp2[tempCount] = bin[j];
        tempCount++;
    }
    temp2[tempCount] = '\0';
    strcpy(bin, temp2);

    char virtAddress[5];
    for (int i = 0; i < 4; ++i) { // Get the first 4 bits of the binary
        virtAddress[i] = bin[i];
    }

    int pageNum = convertBinaryToDecimal(virtAddress); // Get the decimal value of the address

    char *physPageNo;

    if (pageNum <= page[0].size) // Make sure we're not looking for a value that's out of range
        physPageNo = page[pageNum].physPage;
    else
        printf("\nPage number doesn't exist?");

    int temp = atoi(physPageNo);

    char hex[80];
    sprintf(hex, "%X", temp); // int to hex EZ

    original[0] = hex[0]; // just swap the first char of the original with the new

    printf("\n\n");

    int clockCounter = 0;

    for (int i = 0; i < pageNum; ++i) {
        clockCounter++;
    }

    if (*physPageNo == '0' || *page[pageNum].valid == '0' ||
                                *page[pageNum].useBit == '1') { // Check for conflict
#ifdef PROB1 // For problem 1
        if (*page[count].permission == '0')
            printf("\nSEGFAULT\n");
        else
            printf("\nPhysical Address: %s", "DISK");
#else

        while (*clockTable[count].useBit != '0') { // Algorithm is looking for a valid and unused address

            if (*clockTable[count].useBit == '1') // If it's in use, make it 0
                *clockTable[count].useBit = '0';

            if (count == clockTable[0].size - 1) // Loop back to the top
                count = 0;
            else
                count++; // Increment clock
        }

        *clockTable[count].useBit = '1';
        physPageNo = clockTable[count].physPage;
        clockTable[count].virtPage = str[0] - '0';
        temp = atoi(physPageNo);

        sprintf(hex, "%X", temp);

        original[0] = hex[0];

        printf("\nPAGE FAULT\nPhysical Address: 0x%s", original);
        if (count == clockTable[0].size - 1)
            count = 0;
        else
            count++;
#endif

    } else {

        for (int j = 0; j < clockTable[0].size - 1; ++j) {
            if (strcmp(clockTable[j].physPage, physPageNo) == 0) {
                *clockTable[j].useBit = '1';
                clockTable[j].virtPage = pageNum;
            }
        }

        printf("\nPhysical Address: 0x%s", original);
    }
    printf("\n\n");
    return count;
}

int main(int argc, char **argv) {
    char *filename;
    struct address page[256];
    struct clock clockTable[256];
    char virtBit[80], physBit[80], pageByte[80];
    int count = 0;

    if (argc < 2) {
        printf("Missing Filename\n");
        return 1;
    } else {
        filename = argv[1];
        if (parsePage(filename, page, clockTable, virtBit, physBit, pageByte) == 1)
            return 1;

    }

    char str[32];

    printf("\nEnter a virtual address in hex.\n");
    printf("'exit' to quit.\n\n");

    printf("\n");
    printf("Virtual address size: %s\n", virtBit);
    printf("Physical address size: %s\n", physBit);
    printf("Bytes in a page: %s\n", pageByte);
    printf("Number of table entries: %d\n", page[0].size - 1);

    while(strcmp(str, "exit") != 0) {
        #ifdef PROB1 // For problem 1
        #else
        for (int i = 0; i < clockTable[0].size ; ++i) { // Update Page Table
            for (int j = 0; j < page[0].size - 1; ++j) {
                if (*page[j].physPage == *clockTable[i].physPage) {
                    *page[j].useBit = *clockTable[i].useBit;
                }
            }
        }
        #endif

        printf("\n\n");
        printf("\tPAGE TABLE\n");
        for (int k = 0; k < 31; ++k) {
            printf("-");
        }
        printf("\n  v  |  perm  |  ppn  |  use\n");
        for (int k = 0; k < 31; ++k) {
            printf("-");
        }
        printf("\n");
        for (int l = 0; l < page[0].size - 1; ++l) {
            printf("  %s\t  %s\t  %s\t%s\n", page[l].valid, page[l].permission,
                   page[l].physPage, page[l].useBit);
        }
        for (int k = 0; k < 31; ++k) {
            printf("-");
        }

        #ifdef PROB1 // For problem 1
        #else
        printf("\n\n");
        printf("\tCLOCK TABLE\n");
        for (int k = 0; k < 31; ++k) {
            printf("-");
        }
        printf("\n  use  |  phy  |  virt\n");
        for (int k = 0; k < 31; ++k) {
            printf("-");
        }
        printf("\n");
        for (int l = 0; l < clockTable[0].size; ++l) {
            if (l == count)
                printf("  %s\t   %s\t   %d\t<-- CLOCK\n", clockTable[l].useBit,
                       clockTable[l].physPage, clockTable[l].virtPage);
            else
                printf("  %s\t   %s\t   %d\n", clockTable[l].useBit,
                       clockTable[l].physPage, clockTable[l].virtPage);
        }
        for (int k = 0; k < 31; ++k) {
            printf("-");
        }
        #endif

        count = getInput(str, page, clockTable, count);

    }
    return 0;
}