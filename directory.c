#include <dirent.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileHelpers.h"

// Function walks a directory displaying all items contained in it
char *displayDirectory(const char *directoryPath) {

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
  contents[0] = '\0';
    while ((dir = readdir(d)) != NULL) {

      if (dir->d_type == DT_REG) { // Only lists regular files for now, will
                                   // include directories
        count += strlen(dir->d_name) + 1;

        if (count >= bufferSize) {
          bufferSize += 500;
          char *newContents = realloc(contents, bufferSize);
          if (newContents == NULL) {
            free(contents);
            perror("Memory allocation error");
            return NULL;
          }
          contents = newContents;
        }
        strncat(contents, dir->d_name, bufferSize - strlen(contents) - 1);
        strncat(contents, " ", bufferSize - strlen(contents) - 1);
      }
    }
    closedir(d);

  return contents;
}

// Creates an array where each element is an item in a directory
char **fileToArray(char *listOfFiles) {
  size_t arrSize = 10;
  char **arr = malloc(arrSize * sizeof(char *));

  if (arr == NULL) {
    perror("Memory allocation error");
    return NULL;
  }

  const char *tok = strtok(listOfFiles, " ");
  int i = 0;

  while (tok != NULL) {
    if (i >= arrSize) {
      arrSize *= 2;
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
    arr[i] = strdup(tok);
    if (arr[i] == NULL) {
      perror("Memory allocation error for string");

      for (int j = 0; j < i; j++) {
        free(arr[j]);
      }
      free(arr);
      return NULL;
    }

    tok = strtok(NULL, " ");
    i++;
  }

  arr[i] = NULL;
  return arr;
}
