#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int charCount;
    int wordCount;
} SentenceInfo;

// writes results to output.txt
void writeToOutput(int dots, SentenceInfo *sentenceInfo) {
    FILE *file = fopen("output.txt", "w");
    if (file == NULL) {
        printf("Error opening file\n");
        return;
    }

    fprintf(file, "%d\n", dots);
    fprintf(file, "nr of characters: %d, nr of words: %d", sentenceInfo->charCount, sentenceInfo->wordCount);
    fclose(file);
}

// counts sentences ending in '.'
int countDots(FILE *fp) {
    rewind(fp);
    int counter = 0;
    int c;
    while ((c = fgetc(fp)) != EOF) {
        if (c == '.') {
            counter++;
        }
    }
    return counter;
}

// finds char/word count of the last sentence
SentenceInfo lastSentLen(FILE *fp) {
    int counter = 0;
    fseek(fp, 0, SEEK_END);
    long pos = ftell(fp) - 1;

    // find the last declarative sentence ending
    while (counter < 1 && pos > 0) {
        fseek(fp, pos, SEEK_SET);
        int ch = fgetc(fp);
        if (ch == '.') {
            counter++;
        }
        pos--;
    }

    // find the one before it to locate start of last sentence
    while (counter < 2 && pos > 0) {
        fseek(fp, pos, SEEK_SET);
        int ch = fgetc(fp);
        if (ch == '.' || ch == '?' || ch == '!') {
            counter++;
        }
        pos--;
    }

    // if only one sentence, start from beginning
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

    // skip leading whitespace
    while (c != EOF && (c == '\n' || c == '\t' || c == ' ')){
        c = fgetc(fp);
    }

    // count chars and words until end of sentence
    while ((c != EOF) && (c != '?') && (c != '.') && (c != '!')){
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
    result.charCount = counterChar + 1; // +1 to include punctuation
    result.wordCount = counter;

    return result;
}

int main(void) {
    char fileName[100];
    printf("Please enter the file name: ");
    scanf("%99s", fileName);
    getchar();

    FILE *fp;
    fp = fopen(fileName, "w+");
    if (fp == NULL) {
        printf("File could not be opened\n");
        return 1;
    }

    char *inputStr = NULL;
    int strSize = 0;
    int ch;

    printf("enter the text: ");

    // read input char by char, growing buffer as needed
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
        fputc('\n', fp);
    }
    else {
        printf("There is nothing to input!\n");
        return 1;
    }

    free(inputStr);

    int dots = countDots(fp);
    printf("There are %d declarative sentences in the file.\n", dots);

    SentenceInfo sentenceInfo = lastSentLen(fp);
    printf("There are %d characters in the last declarative sentence and %d words.", sentenceInfo.charCount, sentenceInfo.wordCount);

    writeToOutput(dots, &sentenceInfo);
    fclose(fp);
    return 0;
}