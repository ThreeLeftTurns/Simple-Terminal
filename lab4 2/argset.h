//
// Created by William Katsak on 8/27/21.
//

// An 'argset' is a set of individual argc/argv pairs.
// Each entry in an argset represents one command in a pipe expression.

#ifndef SIMPLESH_ARGSET_H
#define SIMPLESH_ARGSET_H

#define MAX_LINE_TOKENS 50

typedef struct {
    int argc;
    char *argv[MAX_LINE_TOKENS+1];
} arg_t;

typedef struct {
    arg_t *sets;
    int count;
} argset_t;

argset_t *build_argset(char *line);
void free_argset(argset_t *argset);
void print_argset(argset_t *argset);

#endif //SIMPLESH_ARGSET_H
