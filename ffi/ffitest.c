#include <stdio.h>
#include "ffitest.h"

reader *createReader(readHandlerFunctions *fn) {
    reader *r = malloc(sizeof(*r));
    r->fn = fn;
    return r;
}

// Totally and completely contrived example just to illustrate the question
void getReply(const reader *r, void **reply) {
    void *root, *obj;
    readTask *task;

    task = malloc(sizeof(*task));
    task->parent = NULL;

    // Root level array response
    root = r->fn->createArray(task, 1);
    task->parent = root;

    obj = r->fn->createArray(task, 1);
    task->parent = obj;

    // Sub array
    obj = r->fn->createArray(task, 3);
    task->parent = obj;

    // Add three integers
    r->fn->createInteger(task, 42);
    r->fn->createInteger(task, 43);
    r->fn->createInteger(task, 44);

    free(task);

    // Set the "root" reply
    *reply = root;
}

void freeReader(reader *reader) {
    if (reader != NULL) {
        free(reader);
    }
}
