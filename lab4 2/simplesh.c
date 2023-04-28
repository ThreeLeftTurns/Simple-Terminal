// Michael Cali
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include "argset.h"

void external_command(arg_t *arg)
{
    execvp(arg->argv[0], arg->argv);
    perror("execvp");
    exit(EXIT_FAILURE);
}

void piped_command(argset_t *argset)
{
    int pipefd[2];

    pid_t pid_1;
    pid_t pid_2;

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return;
    }

    pid_1 = fork();
    if (pid_1 == -1)
    {
        perror("fork");
        return;
    }

    if (pid_1 == 0)
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        external_command(&argset->sets[0]);

        exit(EXIT_SUCCESS);
    }

    pid_2 = fork();
    if (pid_2 == -1)
    {
        perror("fork");
        return;
    }

    if (pid_2 == 0)
    {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);

        external_command(&argset->sets[1]);

        exit(EXIT_SUCCESS);
    }

    close(pipefd[0]);
    close(pipefd[1]);

    waitpid(pid_1, NULL, 0);
    waitpid(pid_2, NULL, 0);
}

int main(int argc, char **argv)
{

    char *line = readline("--Welcome to my first terminal--- \nPress enter...");
    char *holder = line;


    while (holder != NULL)
    {
        line = readline("$ ");
        argset_t* argset = build_argset(line);


        if (argset->count == 1)
        {

            arg_t *args = &argset->sets[0];

            if (strncmp(args->argv[0], "version", 5) == 0)
            {
                printf("Simplesh version 0.1 - Michael Cali\n");
            }
            else if (strncmp(args->argv[0], "echo", 4) == 0)
            {
                for (int i = 1; i < args->argc; i++)
                {
                    printf("%s ", args->argv[i]);
                }
                printf("\n");
            }
            else if (strncmp(args->argv[0], "ls", 2) == 0)
            { 
                if( args->argv[1] == NULL)
                {
                    printf("File1\nFile2\nFile3 \n");
                    
                }
                
            }
            else if (strncmp(args->argv[0], "exit", 4) == 0)
            {
                exit(9);
            }
        }
        else if (argset->count == 2 && strncmp(argset->sets[1].argv[0], "rev", 3) == 0)
        {
            piped_command(argset);
        }
    }
    free(line);
    free(holder);
}
