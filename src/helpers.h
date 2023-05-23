#include <errno.h>
#include <sys/stat.h>

int createDirectory(const char* path);
char* getDirectoryFromPath(const char* inPath);
int mkdirRecursive(const char* path);
void writeLinesToFile(const char* filePath, const char* lines[], int numLines);

int createDirectory(const char* path) {
    int status = mkdir(path, 0777);

    if (status == 0) {
        printf("Directory created: %s\n", path);
        return 1;
    } else if (errno == EEXIST) {
        //printf("Directory already exists: %s\n", path);
        return 1;
    } else {
        if (mkdirRecursive(path) == 1)
            return createDirectory(path);
        else
            return 0;
    }
}

char* getDirectoryFromPath(const char* inPath) {
    size_t length = strlen(inPath);
    if (length == 1) {
        char* directory = malloc(1);
        directory[0] = '\0';
        return directory;
    } else {
        size_t lastSlashIndex = length - 2;
        while (lastSlashIndex > 0 && inPath[lastSlashIndex] != '/') {
            lastSlashIndex--;
        }
        size_t start = (inPath[lastSlashIndex] == '/') ? lastSlashIndex + 1 : lastSlashIndex;
        size_t directoryLength = length - start - 1;
        char* directory = malloc(directoryLength + 1);
        strncpy(directory, inPath + start, directoryLength);
        directory[directoryLength] = '\0';
        return directory;
    }
}

int mkdirRecursive(const char* path) {
    char* pathCopy = strdup(path);
    char* dirName = strtok(pathCopy, "/");
    char dirPath[256] = {'\0'};

    while (dirName != NULL) {
        strcat(dirPath, dirName);
        strcat(dirPath, "/");

        int status = mkdir(dirPath, 0777);

        if (status != 0 && errno != EEXIST && errno != 17) {
            printf("Failed to create directory: %s\n", dirPath);
            free(pathCopy);
            return 0;
        }

        dirName = strtok(NULL, "/");
    }

    free(pathCopy);
    return 1;
}

void writeLinesToFile(const char* filePath, const char* lines[], int numLines) {
    FILE* file = fopen(filePath, "w");
    if (file == NULL) {
        printf("Failed to open file: %s\n", filePath);
        return;
    }

    for (int i = 0; i < numLines; i++) {
        if (lines[i][0] != '\0') {
            fprintf(file, "%s\n", lines[i]);
        } else {
            fprintf(file, "\n");
        }
    }

    fclose(file);
    printf("Successfully written lines to file: %s\n", filePath);
}
