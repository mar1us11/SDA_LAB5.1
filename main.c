#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int charCount;
    int wordCount;
} SentenceInfo;


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


SentenceInfo lastSentLen(FILE *fp) {
    int counter = 0;
    fseek(fp, 0, SEEK_END);
    long pos = ftell(fp) - 1;

    while (counter < 1 && pos > 0) {
        fseek(fp, pos - 1, SEEK_SET);
        int ch2 = fgetc(fp);
        int ch  = fgetc(fp);
        if ((ch2 == '.') && (ch == ' ' || ch == '\n'))
            counter++;
        pos--;
    }

    while (counter < 2 && pos > 0) {
        fseek(fp, pos - 1, SEEK_SET);
        int ch2 = fgetc(fp);
        int ch  = fgetc(fp);
        if ((ch2 == '.' || ch2 == '!' || ch2 == '?') && (ch == ' ' || ch == '\n'))
            counter++;
        pos--;
    }

    if (counter < 2) {
        pos = 0;
    }
    else {
        pos += 2;
    }

    fseek(fp, pos, SEEK_SET);

    int counterChar = 0;
    counter = 0;
    int inWord = 0;

    int c = fgetc(fp);

    while (c == '\n' || c == '\t' || c == ' '){
        c = fgetc(fp);
    }

    while ((c != EOF) &&(c != '?') && (c != '.') && (c != '!')){
        counterChar++;


        if (c != ' ' && c != '\n' && c != '\t') {
            if (!inWord) {
                counter++;
                inWord = 1;
            }
        }
        else {
            inWord = 0;
        }


        c = fgetc(fp);
    }


    SentenceInfo result;
    result.charCount = counterChar;
    result.wordCount = counter;

    return result;
}


int main(void) {
    char fileName[100];
    printf("Please enter the file name: ");
    scanf("%99s", fileName);
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

    if (inputStr != NULL) {
        fputs(inputStr, fp);
    }
    else {
        printf("There is nothing to input!\n");
        return 1;
    }

    free(inputStr);

    int dots = countDots(fp);
    printf("There are %d declarative sentences in the file.\n", dots);

    SentenceInfo sentenceInfo = lastSentLen(fp);
    printf("There are %d characters in the last sentence and %d words.", sentenceInfo.charCount, sentenceInfo.wordCount);


    fclose(fp);
    return 0;
}
