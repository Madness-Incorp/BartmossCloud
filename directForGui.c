#include <ctype.h>
#include <dirent.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isEmptyOrWhitespace(const char *filename);

// Function walks a directory displaying all items contained in it
char *GUIdisplayDirectory(const char *directoryPath) {
    struct dirent *dir;
    const char *path = directoryPath;
    DIR *d = opendir(path);

    if (d == NULL) {
        perror("Failed to open directory");
        return NULL;
    }

    int bufferSize = 500;
    size_t count = 0;

    char *contents = malloc(bufferSize * sizeof(char));
    if (contents == NULL) {
        perror("Memory allocation error");
        return NULL;
    }
    contents[0] = '\0'; // Initialize contents as an empty string

    while ((dir = readdir(d)) != NULL) {
        // Check for regular files that are not hidden and not empty
        if (dir->d_type == DT_REG) {
            const char *fileName = dir->d_name;

            printf("Checking file: '%s'\n", fileName);

            // Check if the file is hidden or empty
            if (fileName[0] != '.' || !isEmptyOrWhitespace(fileName)) {
                count += strlen(fileName) + 1;

                if (count >= bufferSize) {
                    bufferSize += 500;
                    char *newContents = realloc(contents, bufferSize);
                    if (newContents == NULL) {
                        free(contents);
                        perror("Memory allocation error");
                        closedir(d);
                        return NULL;
                    }
                    contents = newContents;
                }

                // Append the valid file name and a space
                strncat(contents, fileName, bufferSize - strlen(contents) - 1);
                strncat(contents, " ", bufferSize - strlen(contents) - 1);
            }
        }
    }

    closedir(d);
    return contents;
}

// Function to check if a filename is empty or consists only of whitespace
int isEmptyOrWhitespace(const char *filename) {
    if (filename == NULL || strlen(filename) == 0) {
        return 1; // Empty filename
    }

    // Check if the filename consists only of whitespace characters
    for (const char *p = filename; *p; ++p) {
        if (!isspace(*p)) {
            return 0; // Found a non-whitespace character
        }
    }

    return 1; // Only whitespace characters
}

// Creates an array where each element is an item in a directory
char **GUIfileToArray(char *listOfFiles, size_t lofSize) {
    size_t arrSize = 10;
    char **arr = malloc(arrSize * sizeof(char *));

    if (arr == NULL) {
        perror("Memory allocation error");
        return NULL;
    }

    const char *tok = strtok(listOfFiles, " ");
    int i = 0;

    while (tok != NULL) {
        // Check if the token is empty or starts with a dot
        if (tok[0] == '.' || isEmptyOrWhitespace(tok)) {
            tok = strtok(NULL, " ");
            continue;
        }

        // Resize the array if necessary
        if (i >= arrSize) {
            arrSize *= 2; // Double the size
            char **newArr = realloc(arr, arrSize * sizeof(char *));
            if (newArr == NULL) {
                perror("Memory allocation error");

                for (int j = 0; j < i; j++) {
                    free(arr[j]);
                }
                free(arr);
                return NULL;
            }
            arr = newArr;
        }

        // Duplicate the token into the array
        arr[i] = strdup(tok);
        if (arr[i] == NULL) {
            perror("Memory allocation error for string");

            // Free previously allocated strings before returning
            for (int j = 0; j < i; j++) {
                free(arr[j]);
            }
            free(arr);
            return NULL;
        }

        tok = strtok(NULL, " ");
        i++;
    }

    arr[i] = NULL; // Null-terminate the array
    return arr; // Return the array of valid filenames
}

char **GUIlistFiles(const char *directoryPath) {
    char *directoryString = GUIdisplayDirectory(directoryPath);
    if (directoryString == NULL) {
        return NULL;
    }

    char **arrayofFiles = GUIfileToArray(directoryString, strlen(directoryString));
    free(directoryString);

    return arrayofFiles;
}
