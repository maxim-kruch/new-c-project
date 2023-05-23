#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helpers.h"

void createCMakeLists(const char* directoryPath, const char* name);
void createGitIgnore(const char* directoryPath);
void createREADME(const char* directoryPath, const char* name);
void createSrcMain(const char* directoryPath);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Wrong amount of arguments!\n");
         return -1;
    }

    char path[1024];
    char name[1024];
    char gitCommand[1024];

    strcpy(path, argv[1]);
    if (path[strlen(path) - 1] != '/') {
        strcat(path, "/");
    }

    strcpy(name, getDirectoryFromPath(path));

    if (createDirectory(path)) {
        printf("Directory creation successful.\n");
    } else {
        printf("Directory creation failed.\n");
        return -1;
    }

    sprintf(gitCommand, "cd %s && git init . && git add . && git commit -m \"project '%s' initialized\"", path, name);

    printf("The new project will be called \"%s\".\n", name);

    createSrcMain(path);
    createGitIgnore(path);
    createCMakeLists(path, name);
    createREADME(path, name);
    system(gitCommand);

    return 0;
}

void createCMakeLists(const char* directoryPath, const char* name) {
    const char* CMakeListsPath = "CMakeLists.txt";    
    const char* lines[] = {
        "cmake_minimum_required(VERSION 3.26.3)",
        "\0",
        "#set(CMAKE_C_COMPILER clang)",
        "#set(CMAKE_CXX_COMPILER clang++)",
        "#add_compile_options(-Wall -Wextra -pedantic -Werror)",
        "\0",
        "set(PROJECT_NAME ",
        "project(${PROJECT_NAME})",
        "\0",
        "#set(LIBRARY_NAME foo)",
        "#add_subdirectory(${LIBRARY_NAME})",
        "\0",
        "add_executable(${PROJECT_NAME} src/main.c)",
        "#target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBRARY_NAME})",
        "#target_include_directories(${PROJECT_NAME} PUBLIC \"${PROJECT_SOURCE_DIR}/${LIBRARY_NAME}\")"
    };

    size_t numLines = sizeof(lines) / sizeof(lines[0]);

    for (size_t i = 0; i < numLines; i++) {
        if (strcmp(lines[i], "set(PROJECT_NAME ") == 0) {
            char* updatedLine = malloc(strlen(lines[i]) + strlen(name) + 1);
            sprintf(updatedLine, "set(PROJECT_NAME %s)", name);
            lines[i] = updatedLine;
        }
    }

    char filePath[256];
    snprintf(filePath, sizeof(filePath), "%s%s", directoryPath, CMakeListsPath);

    writeLinesToFile(filePath, lines, 15);
    printf("Created 'CMakeLists.txt' file inside: %s\n", directoryPath);
}

void createGitIgnore(const char* directoryPath) {
    const char* gitignorePath = ".gitignore";    
    const char* lines[] = {
        "/build"
    };

    char filePath[256];
    snprintf(filePath, sizeof(filePath), "%s%s", directoryPath, gitignorePath);

    writeLinesToFile(filePath, lines, 1);
    printf("Created '.gitignore' file inside: %s\n", directoryPath);
}

void createREADME(const char* directoryPath, const char* name) {
    const char* READMEPath = "README.md";    
    const char* lines[] = {
        "# NAME",
        "A simple C program, that does stuff.",
        "\0",
        "## Building and Running",
        "\0",
        "### Requirements",
        "- cmake",
        "- make",
        "- a working c compiler",
        "\0",
        "### Building:",
        "```bash",
        "mkdir build && cd build",
        "cmake ..",
        "make",
        "```",
        "\0",
        "### Running",
        "```bash",
        "./NAME",
        "```"
    };

    size_t numLines = sizeof(lines) / sizeof(lines[0]);

    for (size_t i = 0; i < numLines; i++) {
        if (strcmp(lines[i], "# NAME") == 0) {
            char* updatedLine = malloc(strlen(lines[i]) + strlen(name) + 1);
            sprintf(updatedLine, "# %s", name);
            lines[i] = updatedLine;
        }
    }

    for (size_t i = 0; i < numLines; i++) {
        if (strcmp(lines[i], "./NAME") == 0) {
            char* updatedLine = malloc(strlen(lines[i]) + strlen(name) + 1);
            sprintf(updatedLine, "./%s", name);
            lines[i] = updatedLine;
        }
    }

    char filePath[256];
    snprintf(filePath, sizeof(filePath), "%s%s", directoryPath, READMEPath);

    writeLinesToFile(filePath, lines, 21);
    printf("Created 'README.md' file inside: %s\n", directoryPath);
}

void createSrcMain(const char* directoryPath) {
    // Create "src" directory
    char srcPath[256];
    snprintf(srcPath, sizeof(srcPath), "%ssrc", directoryPath);
    mkdir(srcPath, 0777);
    
    // Create "main.c" file inside "src" directory
    char mainPath[256];
    snprintf(mainPath, sizeof(mainPath), "%s/main.c", srcPath);
    
    const char* lines[] = {
        "#include <stdio.h>",
        "\0",
        "int main(int argc, char *argv[]) {",
        "    printf(\"Hello World!\\n\");",
        "    return 0;",
        "}"
    };

    writeLinesToFile(mainPath, lines, 6);
    printf("Created directory 'src' and 'main.c' file inside: %s\n", directoryPath);
}
