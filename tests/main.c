#include <stdio.h>
#include <memory.h>
#include "../src/chars.h"

int onEachUnicode1(int64_t unicode, int utf8unitlen, void* cbdata) {
    (void)cbdata;
    char buf[10] = {0};
    if (utf8unitlen == 1) {
        sprintf(buf, "%c", (char)unicode);
    } else {
        sprintf(buf, "\\u%x", (unsigned int)unicode);
    }
    printf("%s", buf);
    
    return 0;
}

int onEachUnicode2(int64_t unicode, int utf8unitlen, void* cbdata) {
    (void)cbdata;
    (void)utf8unitlen;

    char utf8unit[7] = {0};
    unicode2utf8unit(unicode, utf8unit, sizeof(utf8unit));
    printf("%s", utf8unit);

    return 0;
}

int main(int argc, char** argv) {
    char s[] = "hi你abc";

    eachUnicode(s, (unsigned int)strlen(s), onEachUnicode1, 0);
    printf("\n");
    eachUnicode(s, (unsigned int)strlen(s), onEachUnicode2, 0);
    printf("\n");

    return 0;
}
