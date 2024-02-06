#include "fork.h"

caught_internal_process_status create_caught_internal_process_status(int type, int status)
{
    caught_internal_process_status new = {
        .type = type,
        .status = status,
        .status_str = NULL,
    };
    if (type == 1 && status >= 1 && status <= 31)
    {
        new.status_str = signal_names[status - 1];
    }
    else if (type == 0 && status >= 0 && status <= 1)
    {
        new.status_str = exit_status_names[status];
    }
    return new;
}
