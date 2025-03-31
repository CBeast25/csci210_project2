#include "types.h"
#include <string.h>

extern struct NODE* root;
extern struct NODE* cwd;

struct NODE *findDirectory(struct NODE *start, char *dirname) {
    struct NODE *current = start->childPtr;
    while (current) {
        if (strcmp(current->name, dirname) == 0 && current->fileType == 'D') {
            return current;
        }
        current = current->siblingPtr;
    }
    return NULL;
}

struct NODE *splitPath(char *pathName, char *baseName, char *dirName) {
    if (strcmp(pathName, "/") == 0) {
        strcpy(dirName, "/");
        strcpy(baseName, "");
        return root;
    }

    char pathCopy[128];
    strcpy(pathCopy, pathName);
    
    char *lastSlash = strrchr(pathCopy, '/');
    if (lastSlash) {
        *lastSlash = '\0';
        strcpy(baseName, lastSlash + 1);
        strcpy(dirName, (lastSlash == pathCopy) ? "/" : pathCopy);
    } else {
        strcpy(baseName, pathCopy);
        strcpy(dirName, "");
    }
    
    struct NODE *current = (dirName[0] == '/') ? root : cwd;
    char *token = strtok(dirName, "/");
    while (token) {
        current = findDirectory(current, token);
        if (!current) {
            printf("ERROR: directory %s does not exist\n", token);
            return NULL;
        }
        token = strtok(NULL, "/");
    }
    return current;
}

void mkdir(char *pathName) {
    if (strcmp(pathName, "/") == 0 || strlen(pathName) == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    char baseName[64], dirName[128];
    struct NODE *parent = splitPath(pathName, baseName, dirName);
    if (!parent) return;

    // Check if directory already exists
    if (findDirectory(parent, baseName)) {
        printf("MKDIR ERROR: directory %s already exists\n", pathName);
        return;
    }

    // Create new directory node
    struct NODE *newNode = (struct NODE *)malloc(sizeof(struct NODE));
    strcpy(newNode->name, baseName);
    newNode->fileType = 'D';
    newNode->parentPtr = parent;
    newNode->childPtr = NULL;
    newNode->siblingPtr = NULL;
    
    // Insert into parent's child list
    if (!parent->childPtr) {
        parent->childPtr = newNode;
    } else {
        struct NODE *sibling = parent->childPtr;
        while (sibling->siblingPtr) {
            sibling = sibling->siblingPtr;
        }
        sibling->siblingPtr = newNode;
    }
    printf("MKDIR SUCCESS: node %s successfully created\n", pathName);
}
