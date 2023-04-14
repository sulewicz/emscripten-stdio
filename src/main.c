#include <stdio.h>
#include <stdlib.h>

#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"

#if defined(EMSCRIPTEN)
#include <emscripten/emscripten.h>
#endif

#if defined(EMSCRIPTEN)
extern char *__real_fgets(char *str, int num, FILE *stream);
extern int __real_fflush(FILE *stream);

EM_JS(void, _flushstdout, (), {
    window._STDIO._flushstdout();
});

EM_JS(void, _flushstderr, (), {
    window._STDIO._flushstderr();
});

EM_ASYNC_JS(void, _wait_for_stdin, (), {
    await window._STDIO._flushstdin();
});

char *__wrap_fgets(char * str, int num, FILE * stream) {
    _wait_for_stdin();
    return __real_fgets(str, num, stream);
}

int __wrap_fflush(FILE *stream) {
    int ret = __real_fflush(stream);
    if (stream == stdout) {
        _flushstdout();
    } else if (stream == stderr) {
        _flushstderr();
    }
    return ret;
}
#endif

int main(int argc, char *argv[])
{
    char buffer[256];
    printf("Type something.\n");
    fflush(stdout);
    while (1)
    {
        if (fgets(buffer, 256, stdin) == NULL) {
            return 0;
        }
        printf("%s[stdout]%s Echo: %s", KGRN, KNRM, buffer);
        fflush(stdout);
        fprintf(stderr, "%s[stderr]%s Echo: %s", KRED, KNRM, buffer);
        fflush(stderr);
    }
    return 0;
}
