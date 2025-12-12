#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <path_to_bumi_file>\n", argv[0]);
        return 1;
    }

    char *bumiFile = argv[1];
    char bisonCommand[512];
    snprintf(bisonCommand, sizeof(bisonCommand),
             "check.exe \"%s\"", bumiFile);
    if (system(bisonCommand) != 0) return 1;

    char translatorCommand[512];
    snprintf(translatorCommand, sizeof(translatorCommand),
             "translator.exe \"%s\"", bumiFile);

    // printf("Running translator: %s\n", translatorCommand);
    if (system(translatorCommand) != 0) return 1;

    char cFile[512];
    strncpy(cFile, bumiFile, sizeof(cFile));
    char *dot = strrchr(cFile, '.');
    if (dot != NULL) strcpy(dot, ".c");
    else strcat(cFile, ".c");

    char exeFile[512];
    strncpy(exeFile, cFile, sizeof(exeFile));
    char *exeDot = strrchr(exeFile, '.');
    if (exeDot != NULL) strcpy(exeDot, ".exe");
    else strcat(exeFile, ".exe");

    char compileCommand[1024];
    snprintf(compileCommand, sizeof(compileCommand),
             "gcc \"%s\" -o \"%s\"", cFile, exeFile);

    // printf("Compiling C file: %s\n", compileCommand);
    if (system(compileCommand) != 0) return 1;

    char runCommand[512];
    snprintf(runCommand, sizeof(runCommand), "\"%s\"", exeFile);
    // printf("Running executable: %s\n", runCommand);

    // printf("================================================= \n\n");
    if (system(runCommand) != 0) return 1;

    return 0;
}
