// William McLaughlin
// Project 4 Part 3

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

// This program opens a file, reads a word from it, then
// forks a child process that unlinks the file.
// The outcome is as expected. When attempting to read
// from the file that no longer exists, my program throws
// an error "Failed to open the file". If I don't check for
// file existing, it will segfault. The filepointer is pointing
// to either nothing or something else than before


// This function opens the file and reads the first word
// into value.
void parsePage(FILE *fp, char *value) {

    if (fp) {
        char *p;
        char buf[1024];

        while (fgets(buf, sizeof(buf), fp) != NULL) {
            p = strtok(buf, " ");
            strcpy(value, p);
            break;
        }
    } else {
        printf("Failed to open the file\n");;
    }
}


// This function forks a child process who unlinks the file
void unlinkFile(char* file) {
    switch(fork()) {
        case -1:
            printf("\nError creating fork\n");
        case 0:
        {
            unlink(file);
        }
    }

    while ( wait(NULL) != -1)
        ;
}


int main(int argc, char **argv) {
    char *filename;
    char value[80];

    if (argc < 2) {
        printf("Missing Filename\n");
    } else {
        filename = argv[1];
        FILE *fp;

        fp = fopen(filename, "r");

        parsePage(fp, value); // Get first word of file
        printf("\nValue:\t%s\n", value);

        unlinkFile(filename); // Delete file

        parsePage(fp, value); // Attempt to get first word again
        printf("\nValue:\t%s\n", value);
    }
    return 0;
}