#ifndef __READ_H
#define __READ_H
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct readTask {
    void *parent;
} readTask;

typedef void *(*createArrayCallback)(const readTask*, size_t);
typedef void *(*createIntegerCallback)(const readTask*, int);

typedef struct readHandlerFunctions {
    createArrayCallback createArray;
    createIntegerCallback createInteger;
} readHandlerFunctions;

typedef struct reader {
    readHandlerFunctions *fn;
} reader;

reader *createReader(readHandlerFunctions *fn);
void getReply(const reader *reader, void **reply);
void freeReader(reader *reader);

#ifdef __cplusplus
}
#endif

#endif
