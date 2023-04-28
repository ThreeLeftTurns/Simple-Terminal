//
// Created by William Katsak on 8/27/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "argset.h"

argset_t *build_argset(char *line) {
    char *token = strtok(line, " ");
    if (token == NULL) {
        return NULL;
    }

    int _argc = 0;
    char **_argv = malloc(sizeof(char *) * (MAX_LINE_TOKENS + 1));

    argset_t *argset = malloc(sizeof(argset_t));
    argset->count = 0;

    while(token != NULL && _argc < MAX_LINE_TOKENS) {
        _argv[_argc] = malloc(strlen(token) + 1);
        strcpy(_argv[_argc], token);
        _argc++;
        token = strtok(NULL, " ");
    }

    // Count pipes in cmd args.
    for(int i=0; i<_argc; i++) {
        if(strcmp("|", _argv[i]) == 0 | i == (_argc-1)) {
            argset->count++;
        }
    }
    argset->sets = calloc(argset->count, sizeof(arg_t));

    // Keep track of current set.
    int current_set = 0;
    // Keep track of current position in current argv.
    int current_offset = 0;

    // Step over original argv.
    for (int i=0; i<_argc; i++) {
        // If the current token is a |, go to the next set.
        if (strcmp("|", _argv[i]) == 0) {
            current_set++;
            current_offset = 0;
            continue;
        }

        argset->sets[current_set].argc++;
        argset->sets[current_set].argv[current_offset] = _argv[i];
        current_offset++;
    }

    free(_argv);
    return argset;
}

void free_argset(argset_t *argset) {
    for (int i=0; i<argset->count; i++) {
        for (int j=0; j<argset->sets[i].argc; j++) {
            free(argset->sets[i].argv[j]);
        }
    }
    free(argset->sets);
    free(argset);
}

void print_argset(argset_t *argset) {
    for (int i=0; i<argset->count; i++) {
        printf("argset: %d\n", i);
        printf("- argc = %d\n", argset->sets[i].argc);
        for (int j=0; j<argset->sets[i].argc; j++) {
            printf("- argv[%d] = %s\n", j, argset->sets[i].argv[j]);
        }
    }
}
