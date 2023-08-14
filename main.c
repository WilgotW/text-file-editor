#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <conio.h>

int readExistingFiles();
int setup();
int createFile();
int editFile();
int printoutFiles();
int readFile();

struct FileData
{
    char name[100];
    int index;
};

struct FileData files[100];
int filesCreated;
int *filesCreatedPointer = &filesCreated;

int main() {

    readExistingFiles();

    while (TRUE)
    {
        printf("what do you want to do?");
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

int editFile(){
    //print out files:
    printoutFiles();

    return 0;
}

int printoutFiles(){
    printf("files in storage: \n");
    for(int i = 0; i < filesCreated; i++){
        printf("*  -");
        printf(files[i].name);
        printf("\n");
    }
    printf("press any key to continue...");
    getch();
    return 0;
}

int readFile(){


    return 0;
}