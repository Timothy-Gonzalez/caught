#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <poll.h>
#ifndef __USE_POSIX
#define __USE_POSIX
#endif
#include <stdio.h>
#include <string.h>

#include "mocks.h"
#include "state.h"
#include "output.h"

void MOCK_STDOUT()
{
    if (caught_internal_state.original_stdout != -1)
    {
        caught_output_errorf("Cannot mock already mocked stdout");
    }

    int pipe_status = pipe(caught_internal_state.mocked_stdout_pipe);

    if (pipe_status == -1)
    {
        caught_output_perrorf("Failed to create pipe for mocked stdout");
    }

    caught_internal_state.original_stdout = dup(STDOUT_FILENO);

    fflush(NULL);

    if (caught_internal_state.original_stdout == -1)
    {
        caught_output_perrorf("Failed to dup stdout before mocking stdout");
    }

    caught_internal_state.original_stdout_file = fdopen(caught_internal_state.original_stdout, "w");

    if (caught_internal_state.original_stdout_file == NULL)
    {
        caught_output_perrorf("Failed to open file descriptor to original stdout");
    }

    if (dup2(caught_internal_state.mocked_stdout_pipe[1], STDOUT_FILENO) == -1)
    {
        caught_output_perrorf("Failed to dup2 pipe to stdout");
    }
}

char *RESTORE_STDOUT()
{
    if (caught_internal_state.original_stdout == -1)
    {
        caught_output_errorf("Cannot restore mock not mocked stdout");
    }

    fflush(NULL);

    close(caught_internal_state.mocked_stdout_pipe[1]);

    if (dup2(caught_internal_state.original_stdout, STDOUT_FILENO) == -1)
    {
        caught_output_perrorf("Failed to restore stdout");
    }

    char *result = malloc(RESTORE_STDOUT_BUFFER_SIZE);
    int result_size = 0;
    int result_capacity = RESTORE_STDOUT_BUFFER_SIZE;

    char *buffer = malloc(RESTORE_STDOUT_BUFFER_SIZE);

    struct pollfd poll_fds = {
        .fd = caught_internal_state.mocked_stdout_pipe[0],
        .events = POLLIN,
    };
    int res = poll(&poll_fds, 1, 0);

    if (res < 0 || poll_fds.revents & (POLLERR | POLLNVAL))
    {
        caught_output_perrorf("Failed to poll mocked output pipe");
    }

    bool has_data_to_read = poll_fds.revents & POLLIN;

    while (has_data_to_read)
    {
        ssize_t size = read(caught_internal_state.mocked_stdout_pipe[0], buffer, RESTORE_STDOUT_BUFFER_SIZE);

        if (size == -1)
        {
            caught_output_perrorf("Failed to read pipe");
        }

        if (size == 0)
        {
            break;
        }

        memcpy(result + result_size, buffer, size);

        result_size += size;

        if (result_size + RESTORE_STDOUT_BUFFER_SIZE >= result_capacity)
        {
            result_capacity *= RESTORE_STDOUT_RESULT_GROW_RATE;
            buffer = realloc(buffer, result_capacity);
        }
    }

    free(buffer);
    buffer = NULL;

    result[result_size] = '\0';

    close(caught_internal_state.mocked_stdout_pipe[0]);

    caught_internal_state.mocked_stdout_pipe[0] = -1;
    caught_internal_state.mocked_stdout_pipe[1] = -1;
    caught_internal_state.original_stdout = -1;

    fclose(caught_internal_state.original_stdout_file);
    caught_internal_state.original_stdout_file = NULL;

    return result;
}
