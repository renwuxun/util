#include <stdio.h>
#include <memory.h>
#include "chars.h"

int onEachUnicode(int64_t out, int utf8unitlen, void* cbdata) {
    (void)cbdata;
    char buf[10] = {0};
    int buflen;
    if (utf8unitlen == 1) {
        buflen = sprintf(buf, "%c", (char)out);
    } else {
        buflen = sprintf(buf, "\\u%x", (unsigned int)out);
    }
    printf("%s", buf);
    return 0;
}

int main() {
    char s[] = "hi你abc";

    eachUnicode(s, (unsigned int)strlen(s), onEachUnicode, 0);

    return 0;
}