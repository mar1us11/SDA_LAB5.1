#include <stdio.h>
#include <stdlib.h>

int countDots(FILE *fp) {
    rewind(fp);
    int counter = 0;
    int c;
    while ((c = fgetc(fp)) != EOF) {
        if (c == '.') {
            int next = fgetc(fp);
            if (next == ' ' || next == '\n' || next == EOF) {
                counter++;
            }

            if (next != EOF) {
                ungetc(next, fp); // undo the fgetc()
            }
        }
    }
    return counter;
}


int main(void) {
    char fileName[100];
    printf("Please enter the file name: ");
    scanf("%s", fileName);
    getchar(); // clears buffer

    FILE *fp;
    fp = fopen(fileName, "w+");
    if (fp == NULL) {
        printf("File could not be opened\n");
        return 1;
    }

    char *inputStr = NULL;
    int strSize = 0;
    int ch;

    while ((ch = getchar()) != EOF) {
        char *temp = realloc(inputStr, strSize + 2);
        if (temp == NULL) {
            free(inputStr);
            return 1;
        }
        inputStr = temp;

        inputStr[strSize++] = ch;
        inputStr[strSize] = '\0';
    }

    fputs(inputStr, fp);
    free(inputStr);

    int dots = countDots(fp);
    printf("There are %d declarative sentences in the file.\n", dots);

    //int lastLen = lastSentLen(fp);


    fclose(fp);
    return 0;
}
