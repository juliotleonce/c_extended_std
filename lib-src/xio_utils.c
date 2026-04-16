#include "../include/xio_utils.h"

#include <stdio.h>

#include "xstring.h"

XString *xio_read_console_line(const char *prompt) {
    printf("%s", prompt);
    fflush(stdout);

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    if ((read = getline(&line, &len, stdin)) != -1) {
        if (line[read - 1] == '\n') { line[read - 1] = '\0'; }
        XString *xstr_line = xstring_new(line);
        free(line);
        return xstr_line;
    }

    return NULL;
}
