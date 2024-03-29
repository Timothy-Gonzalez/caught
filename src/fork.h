#ifndef CAUGHT_FORK
#define CAUGHT_FORK

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "output.h"

typedef struct caught_internal_process_status
{
    int type;               // 0 for exit status, 1 for signal status
    int status;             // The exit status or signal, depending on above
    const char *status_str; // The string signal, if there is one
} caught_internal_process_status;

static const char *exit_status_names[] = {"EXIT_SUCCESS", "EXIT_FAILURE"};

static const char *signal_names[] = {
    "SIGHUP", "SIGINT", "SIGQUIT", "SIGILL", "SIGTRAP", "SIGABRT", "SIGBUS",
    "SIGFPE", "SIGKILL", "SIGUSR1", "SIGSEGV", "SIGUSR2", "SIGPIPE", "SIGALRM",
    "SIGTERM", "SIGSTKFLT", "SIGCHLD", "SIGCONT", "SIGSTOP", "SIGTSTP", "SIGTTIN",
    "SIGTTOU", "SIGURG", "SIGXCPU", "SIGXFSZ", "SIGVTALRM", "SIGPROF", "SIGWINCH",
    "SIGIO", "SIGPWR", "SIGSYS"};

caught_internal_process_status create_caught_internal_process_status(int type, int status);

#define CAUGHT_INTERNAL_FORK(child_execute_block)                                              \
    caught_internal_process_status caught_internal_fork_child_status = {};                     \
    fflush(NULL);                                                                              \
    pid_t caught_internal_pid = fork();                                                        \
    if (caught_internal_pid == -1)                                                             \
    {                                                                                          \
        caught_output_perrorf("Failed to fork\n");                                             \
    }                                                                                          \
    if (caught_internal_pid == 0)                                                              \
    {                                                                                          \
        caught_internal_cleanup_state(false);                                                  \
        child_execute_block                                                                    \
                                                                                               \
            caught_output_perrorf("Fork segment must call exit to prevent fork bombs\n");      \
    }                                                                                          \
    else                                                                                       \
    {                                                                                          \
        int caught_internal_status = 0;                                                        \
        waitpid(caught_internal_pid, &caught_internal_status, 0);                              \
        if (WIFEXITED(caught_internal_status))                                                 \
        {                                                                                      \
            caught_internal_fork_child_status =                                                \
                create_caught_internal_process_status(0, WEXITSTATUS(caught_internal_status)); \
        }                                                                                      \
        else if (WIFSIGNALED(caught_internal_status))                                          \
        {                                                                                      \
            caught_internal_fork_child_status =                                                \
                create_caught_internal_process_status(1, WTERMSIG(caught_internal_status));    \
        }                                                                                      \
    }

#endif
