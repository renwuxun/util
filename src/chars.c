//
// Created by admin on 2018/9/12.
//

#include <stdio.h>
#include <memory.h>
#include "chars.h"


/**
 * utf8单元转化成unicode单元
 * @param ptr
 * @param unicode
 * @return int -1:error >0: utf8单元长度
 */
int utf8unit2unicode(const char* ptr, uint32_t* unicode) {
    *unicode = 0;

    int tmp = 0x80;
    int utf8UnitLen = 0;
    char c = *ptr;
    for (; tmp & c; tmp = tmp >> 1) {
        utf8UnitLen++;
    }
    switch (utf8UnitLen) {
        case 0:
            *unicode = (unsigned int)*ptr;
            utf8UnitLen++;
            break;
        case 2:
            *unicode |= (*ptr & 0x1f)<<6;
            *unicode |= *(ptr+1) & 0x3f;
            break;
        case 3:
            *unicode |= (*ptr & 0xf)<<12;
            *unicode |= (*(ptr+1) & 0x3f)<<6;
            *unicode |= *(ptr+2) & 0x3f;
            break;
        case 4:
            *unicode |= (*ptr & 0x7)<<18;
            *unicode |= (*(ptr+1) & 0x3f)<<12;
            *unicode |= (*(ptr+2) & 0x3f)<<6;
            *unicode |= *(ptr+3) & 0x3f;
            break;
        case 5:
            *unicode |= (*ptr & 0x3)<<24;
            *unicode |= (*(ptr+1) & 0x3f)<<18;
            *unicode |= (*(ptr+2) & 0x3f)<<12;
            *unicode |= (*(ptr+3) & 0x3f)<<6;
            *unicode |= *(ptr+4) & 0x3f;
            break;
        case 6:
            *unicode |= (*ptr & 0x1)<<30;
            *unicode |= (*(ptr+1) & 0x3f)<<26;
            *unicode |= (*(ptr+2) & 0x3f)<<18;
            *unicode |= (*(ptr+3) & 0x3f)<<12;
            *unicode |= (*(ptr+4) & 0x3f)<<6;
            *unicode |= *(ptr+5) & 0x3f;
            break;
        default:
            return -1;
    }

    return utf8UnitLen;
}

/**
 * unicode单元转为utf8单元
 * @param unicode
 * @param buf
 * @param bufsize
 * @return int uft8单元长度
 */
int unicode2utf8unit(uint32_t unicode, char* buf, size_t bufsize) {
    if (unicode <= 0xffff) {
        if (unicode <= 0x7ff) {
            if (unicode <= 0x7f) { //abc
                if (bufsize < 1) {
                    return -1;
                }
                *buf = (char)unicode;
                return 1;
            } else { //2ge1
                if (bufsize < 2) {
                    return -1;
                }
                *buf = (char)(0xc0 | (unicode & 0xfc0)>>6);
                *(buf+1) = (char)(0x80 | (unicode & 0x3f));
                return 2;
            }
        } else { // 3ge1
            if (bufsize < 3) {
                return -1;
            }
            *buf = (char)(0xe0 | (unicode & 0x3f000)>>12);
            *(buf+1) = (char)(0x80 | (unicode & 0xfc0)>>6);
            *(buf+2) = (char)(0x80 | (unicode & 0x3f));
            return 3;
        }
    } else {
        if (unicode <= 0x3ffffff) {
            if (unicode <= 0x1fffff) { //4ge1
                if (bufsize < 4) {
                    return -1;
                }
                *buf = (char)(0xf0 | (unicode & 0xfc0000)>>18);
                *(buf+1) = (char)(0x80 | (unicode & 0x3f000)>>12);
                *(buf+2) = (char)(0x80 | (unicode & 0xfc0)>>6);
                *(buf+3) = (char)(0x80 | (unicode & 0x3f));
                return 4;
            } else { //5ge1
                if (bufsize < 5) {
                    return -1;
                }
                *buf = (char)(0xf8 | (unicode & 0x3f000000)>>24);
                *(buf+1) = (char)(0x80 | (unicode & 0xfc0000)>>18);
                *(buf+2) = (char)(0x80 | (unicode & 0x3f000)>>12);
                *(buf+3) = (char)(0x80 | (unicode & 0xfc0)>>6);
                *(buf+4) = (char)(0x80 | (unicode & 0x3f));
                return 5;
            }
        } else { //6ge1
            if (bufsize < 6) {
                return -1;
            }
            *buf = (char)(0xfc | (unicode & 0xfc0000000)>>30);
            *(buf+1) = (char)(0x80 | (unicode & 0x3f000000)>>24);
            *(buf+2) = (char)(0x80 | (unicode & 0xfc0000)>>18);
            *(buf+3) = (char)(0x80 | (unicode & 0x3f000)>>12);
            *(buf+4) = (char)(0x80 | (unicode & 0xfc0)>>6);
            *(buf+5) = (char)(0x80 | (unicode & 0x3f));
            return 6;
        }
    }
}

/**
 * 对字符串中的每个unicode单位应用用户回调函数
 * @param ptr
 * @param len
 * @param onEachUnicode
 * @param cbdata
 * @return int
 */
int eachUnicode(
        const char* ptr,
        size_t len,
        int (*onEachUnicode)(uint32_t, int, void*),
        void* cbdata)
{
    unsigned int out = 0;
    int utf8unitlen = 0;
    const char* maxptr = ptr+len;
    for (;ptr < maxptr;) {
        utf8unitlen = utf8unit2unicode(ptr, &out);
        if (utf8unitlen != -1) {
            ptr += utf8unitlen;
            if (-1 == onEachUnicode(out, utf8unitlen, cbdata)) {
                return -1;
            }
        }
    }
    return 0;
}

static int toUnicodeCB(unsigned int unicode, int utf8unitlen, void* cbdata) {
    struct buf_s* b = (struct buf_s*)cbdata;
    char buf[10] = {0};
    int buflen;
    if (utf8unitlen == 1) {
        buflen = sprintf(buf, "%c", (char)unicode);
    } else {
        buflen = sprintf(buf, "\\u%x", unicode);
    }
    if (buflen <= (b->size - b->len)) {
        memcpy(b->base+b->len, buf, (size_t)buflen);
        b->len += buflen;
    } else {
        return -1;
    }

    return 0;
}

static int toAsciiCB(unsigned int unicode, int utf8unitlen, void* cbdata) {
    (void)utf8unitlen;
    struct buf_s* b = (struct buf_s*)cbdata;
    char buf[10] = {0};
    int buflen = sprintf(buf, "&#%d;", (int)unicode);
    if (buflen <= (b->size - b->len)) {
        memcpy(b->base+b->len, buf, (size_t)buflen);
        b->len += buflen;
    } else {
        return -1;
    }

    return 0;
}

int str2unicode(char* s, size_t slen, char* buf, size_t bufsize) {
    struct buf_s b;
    b.base = buf;
    b.len = 0;
    b.size = bufsize;
    if (0 == eachUnicode(s, slen, toUnicodeCB, &b)) {
        return (int)b.len;
    }
    return -1;
}

int str2ascii(char* s, size_t slen, char* buf, size_t bufsize) {
    struct buf_s b;
    b.base = buf;
    b.len = 0;
    b.size = bufsize;
    if (0 == eachUnicode(s, slen, toAsciiCB, &b)) {
        return (int)b.len;
    }
    return -1;
}

void findBetween(char* s, size_t slen, char left, char right,
                 void (*onfound)(char*, char*, void*),
                 void* onfoundData,
                 void (*onfinish)(char*, size_t, char*, void*),
                 void* onfinishData
){
    char* leftPtr = NULL;
    char* rightPtr = NULL;
    char* lastRightPtr = NULL;
    for (int i = 0; i < slen; ++i) {
        if (s[i] == left) {
            leftPtr = s+i;
        } else if (s[i] == right && NULL != leftPtr) {
            rightPtr = s+i;
            lastRightPtr = rightPtr;
            onfound(leftPtr, rightPtr, onfoundData);
            leftPtr = NULL;
        }
    }
    if (NULL != lastRightPtr) {
        if (NULL != onfinish) {
            onfinish(s, slen, lastRightPtr, onfinishData);
        }
    }
}
