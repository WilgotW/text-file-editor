#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include "functions.h"

int readExistingFiles();
int setup();
int createFile();
int editFile();
int printoutFiles();
int readFile();
int pressToContinue();
int printStorage();
int setTextColor();
int resetTextColor();

struct FileData
{
    char name[100];
    int index;
};

struct FileData files[100];
int filesCreated;
int *filesCreatedPointer = &filesCreated;



int main() {
    resetTextColor();
    readExistingFiles();

    while (TRUE)
    {
        printf("\n what do you want to do?");
        printf("\n 1. create new file. \n 2. edit existing file \n 3. read existing file. \n 4. exit program \n");

        int select;
        scanf("%d", &select);
        getchar();

        switch (select){
            case 1:
                createFile();
                break;
            case 2:
                editFile();
                break;
            case 3:
                readFile();
                break;
            case 4:
                exit(0);
                break;
        }
        printf("\n");
    }
    return 0;
}

int readExistingFiles(){
    FILE *recordFile = fopen("file_record.txt", "r");
    if (recordFile != NULL) {
        while (fscanf(recordFile, "%s", files[filesCreated].name) != EOF) {
            files[filesCreated].index = filesCreated;
            (*filesCreatedPointer)++;
        }
        fclose(recordFile);
    }else {
        setup();
    }
    return 0;
}

int setup(){
    //record file doesn't exist, create file
    FILE *recordFilePointer;
    recordFilePointer = fopen("file_record.txt", "w");

    //check if files folder exists
    const char *folderPath = "files";
    if (!CreateDirectory(folderPath, NULL)) {
        if (GetLastError() == ERROR_ALREADY_EXISTS) {
            printf("Folder already exists.\n");
        } else {
            printf("Failed to create folder.\n");
        }
    } else {
        printf("Folder created successfully.\n");
    }

    return 0;
}
int createFile() {
    char input[100];
    printf("enter file name: ");
    scanf("%99[^\n]", input);


    FILE *filePointer;
    char *folderPath = "files";
    char *extension = ".txt";

    // Combine filename and extension
    char filenameWithExtension[100];
    snprintf(filenameWithExtension, sizeof(filenameWithExtension), "%s%s", input, extension);

    char fullPath[100];
    snprintf(fullPath, sizeof(fullPath), "%s/%s", folderPath, filenameWithExtension);

    filePointer = fopen(fullPath, "w");

    if (filePointer == NULL) {
        printf("File could not be created.\n");
        return 1;
    }

    //add file to files structure
    strcpy(files[filesCreated].name, input);
    files[filesCreated].index = filesCreated;

    *filesCreatedPointer += 1;

    printf("File created successfully.\n");

    fclose(filePointer);

    //update record file
    FILE *recordFile = fopen("file_record.txt", "a");
    if (recordFile != NULL) {
        fprintf(recordFile, "%s\n", files[filesCreated - 1].name);
        fclose(recordFile);
    }

    return 0;
}

int editFile() {
    // Print out files:
    int sp = printStorage();
    if(sp){
        printf("no files found \n");
        pressToContinue();
        return 0;
    }
    printf("What file do you want to edit?\n");

    char input[100];
    scanf("%99[^\n]", input);
    getchar();

    char *path = combineStrings("files/", input);
    char *fileName = combineStrings(path, ".txt");


    FILE *openFile = fopen(fileName, "w");
    if (openFile == NULL) {
        perror("Error opening file");
        return 1;
    }

    printf("%s\n", combineStrings("\n Opened file: ", fileName));
    printf("Enter new text: ");

    char textInput[250];
    scanf(" %249[^\n]", textInput);
    getchar();

    fputs(textInput, openFile);
    printf("Edited file\n");
    fclose(openFile);

    free(path);
    free(fileName);

    return 0;
}


int readFile(){
    int sp = printStorage();
    if(sp){
        printf("no files found \n");
        pressToContinue();
        return 0;
    }
    printf("what file do you want to read? \n");

    char input[100];
    scanf("%99[^\n]", input);
    getchar();

    char *path = combineStrings("files/", input);
    char *fileName = combineStrings(path, ".txt");

    FILE *openFile = fopen(fileName, "r");

    if(openFile == NULL){
        perror("invalid file name");
        return 1;
    }

    printf("content: \n");
    setTextColor();
    char ch;
    while ((ch = fgetc(openFile)) != EOF){
        putchar(ch);
    }
    resetTextColor();

    pressToContinue();
    fclose(openFile);

    return 0;
}

int printStorage(){
    if(filesCreated < 1){
        return 1;
    }
    printf("files in storage: \n");
    for(int i = 0; i < filesCreated; i++){
        printf("*  -");
        printf(files[i].name);
        printf("\n");
    }
    return 0;
}

int pressToContinue(){
    printf("\npress any key to continue... \n \n");
    getch();

    return 0;
}

int setTextColor(){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
    return 0;
}
int resetTextColor(){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    return 0;
}