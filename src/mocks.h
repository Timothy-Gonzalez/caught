#ifndef CAUGHT_MOCKS
#define CAUGHT_MOCKS

static const int CAUGHT_MOCK_BUFFER_SIZE = 1024;
static const int CAUGHT_MOCK_RESULT_GROW_RATE = 2;

void MOCK_STDIN(char *input);
void RESTORE_STDIN();

void MOCK_STDOUT();
char *RESTORE_STDOUT();

#endif
