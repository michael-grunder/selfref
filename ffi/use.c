#include "ffitest.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TYPE_ARRAY 0
#define TYPE_INTEGER 1

typedef struct customReply {
    int type;
    size_t len;
    union {
        struct customReply **arr;
        int ival;
    };
} customReply;

void appendArrayElement(customReply *parent, customReply *value) {
    parent->len++;
    parent->arr = realloc(parent->arr, sizeof(*parent->arr) * parent->len);
    parent->arr[parent->len-1] = value;
}

void *attach(const readTask *task, customReply *r) {
    if (task->parent != NULL) {
        customReply *parent = (customReply*)task->parent;
        appendArrayElement(parent, r);
    }

    return r;
}

void *createArray(const readTask *task, size_t elements) {
    customReply *r = calloc(1, sizeof(*r));
    r->type = TYPE_ARRAY;

    return attach(task, r);
}

void *createInteger(const readTask *task, int v) {
    customReply *r = calloc(1, sizeof(*r));
    r->type = TYPE_INTEGER;
    r->ival = v;
    return attach(task, r);
}

void printReply(customReply *reply, int indent) {
    for(int i = 0; i < indent; i++) {
        printf(" ");
    }
    if (reply->type == TYPE_ARRAY) {
        printf("Array\n");
        for(int j = 0; j < reply->len; j++) {
            printReply(reply->arr[j], indent + 2);
        }
    } else {
        printf("Integer: %d\n", reply->ival);
    }
}

void freeReply(customReply *r) {
    if (r->type == TYPE_ARRAY) {
        for (int i = 0; i < r->len; i++) {
            freeReply(r->arr[i]);
        }
        free(r->arr);
        free(r);
    } else if (r->type == TYPE_INTEGER) {
        free(r);
    } else {
        fprintf(stderr, "Error:  Invalid reply type!\n");
        exit(-1);
    }
}

int main(void) {
    customReply *reply;
    reader *reader;

    readHandlerFunctions fns = {
        createArray,
        createInteger
    };

    reader = createReader(&fns);

    getReply(reader, (void**)&reply);
    printReply(reply, 0);

    freeReply(reply);
    freeReader(reader);
}
