#include <stdio.h>
#include <memory.h>
#include "../src/chars.h"

int onEachUnicode1(unsigned int unicode, int utf8unitlen, void* cbdata) {
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

int onEachUnicode2(unsigned int unicode, int utf8unitlen, void* cbdata) {
    (void)cbdata;
    (void)utf8unitlen;

    char utf8unit[7] = {0};
    unicode2utf8unit(unicode, utf8unit, sizeof(utf8unit));
    printf("%s", utf8unit);

    return 0;
}

void onfound(char* leftptr, char* rightptr, void* data) {
    (void)data;
    size_t len = (size_t)(rightptr-leftptr-1);
    char buf[100] = {0};
    memcpy(buf, leftptr+1, len);
    printf("%s\n", buf);
}

int main(int argc, char** argv) {
    char s[] = "hii你aiaidcabc";

    eachUnicode(s, (unsigned int)strlen(s), onEachUnicode1, 0);
    printf("\n");
    eachUnicode(s, (unsigned int)strlen(s), onEachUnicode2, 0);
    printf("\n");

    char buf[100] = {0};
    if (0 < str2unicode(s, (unsigned int)strlen(s), buf, sizeof(buf))){
        printf("%s\n", buf);
    }

    memset(buf, 0, 100);
    if (0 < str2ascii(s, (unsigned int)strlen(s), buf, sizeof(buf))){
        printf("%s\n", buf);
    }

    findBetween(s, (unsigned int)strlen(s), 'i', 'a', onfound, NULL, NULL, NULL);

    return 0;
}
/*
hii\u4f60aiaidcabc
hii你aiaidcabc
hii\u4f60aiaidcabc
&#104;&#105;&#105;&#20320;&#97;&#105;&#97;&#105;&#100;&#99;&#97;&#98;&#99;
你

dc
*/