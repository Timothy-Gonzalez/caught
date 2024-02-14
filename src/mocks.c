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

void MOCK_STDIN(char *input)
{
    if (caught_internal_state.original_stdin != -1)
    {
        caught_output_errorf("Cannot mock already mocked stdin");
    }
    caught_internal_state.original_stdin = dup(STDIN_FILENO);

    if (caught_internal_state.original_stdin == -1)
    {
        caught_output_perrorf("Failed to dup stdin before mocking");
    }

    int stdin_pipe[2];
    if (pipe(stdin_pipe) == -1)
    {
        caught_output_perrorf("Failed to create pipe for mocked stdin");
    }
    fflush(NULL);

    if (dup2(stdin_pipe[0], STDIN_FILENO) == -1)
    {
        caught_output_perrorf("Failed to mock stdin with pipe");
    }
    if (write(stdin_pipe[1], input, strlen(input)) == -1)
    {
        caught_output_perrorf("Failed to write to stdin mock");
    }
    fflush(NULL);
    close(stdin_pipe[0]);
    close(stdin_pipe[1]);
}

void RESTORE_STDIN()
{
    if (caught_internal_state.original_stdin == -1)
    {
        caught_output_errorf("Cannot restore not mocked stdin");
    }

    fflush(NULL);

    if (dup2(caught_internal_state.original_stdin, STDIN_FILENO) == -1)
    {
        caught_output_perrorf("Failed to restore stdin");
    }

    caught_internal_state.original_stdin = -1;
}

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
        caught_output_errorf("Cannot restore not mocked stdout");
    }

    fflush(NULL);

    close(caught_internal_state.mocked_stdout_pipe[1]);

    if (dup2(caught_internal_state.original_stdout, STDOUT_FILENO) == -1)
    {
        caught_output_perrorf("Failed to restore stdout");
    }

    char *result = malloc(CAUGHT_MOCK_BUFFER_SIZE);
    int result_size = 0;
    int result_capacity = CAUGHT_MOCK_BUFFER_SIZE;

    char *buffer = malloc(CAUGHT_MOCK_BUFFER_SIZE);

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
        ssize_t size = read(caught_internal_state.mocked_stdout_pipe[0], buffer, CAUGHT_MOCK_BUFFER_SIZE);

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

        if (result_size + CAUGHT_MOCK_BUFFER_SIZE >= result_capacity)
        {
            result_capacity *= CAUGHT_MOCK_RESULT_GROW_RATE;
            result = realloc(result, result_capacity);
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
