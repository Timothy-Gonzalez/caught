#ifndef CAUGHT_MOCKS
#define CAUGHT_MOCKS

static const int RESTORE_STDOUT_BUFFER_SIZE = 1024;
static const int RESTORE_STDOUT_RESULT_GROW_RATE = 2;

void MOCK_STDOUT();
char *RESTORE_STDOUT();

#endif
