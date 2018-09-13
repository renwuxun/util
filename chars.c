//
// Created by admin on 2018/9/12.
//

#include "chars.h"


/**
 * utf8单元转化成unicode单元
 * @param ptr
 * @param out
 * @return int -1:error >0: utf8单元长度
 */
int utf8unit2unicode(const char* ptr, int64_t* out) {
    *out = 0;

    int tmp = 0x80;
    int utf8UnitLen = 0;
    char c = *ptr;
    for (; tmp & c; tmp = tmp >> 1) {
        utf8UnitLen++;
    }
    switch (utf8UnitLen) {
        case 0:
            *out = *ptr;
            utf8UnitLen++;
            break;
        case 2:
            *out |= (*ptr & 0x1f)<<6;
            *out |= *(ptr+1) & 0x3f;
            break;
        case 3:
            *out |= (*ptr & 0xf)<<12;
            *out |= (*(ptr+1) & 0x3f)<<6;
            *out |= *(ptr+2) & 0x3f;
            break;
        case 4:
            *out |= (*ptr & 0x7)<<18;
            *out |= (*(ptr+1) & 0x3f)<<12;
            *out |= (*(ptr+2) & 0x3f)<<6;
            *out |= *(ptr+3) & 0x3f;
            break;
        case 5:
            *out |= (*ptr & 0x3)<<24;
            *out |= (*(ptr+1) & 0x3f)<<18;
            *out |= (*(ptr+2) & 0x3f)<<12;
            *out |= (*(ptr+3) & 0x3f)<<6;
            *out |= *(ptr+4) & 0x3f;
            break;
        case 6:
            *out |= (*ptr & 0x1)<<30;
            *out |= (*(ptr+1) & 0x3f)<<26;
            *out |= (*(ptr+2) & 0x3f)<<18;
            *out |= (*(ptr+3) & 0x3f)<<12;
            *out |= (*(ptr+4) & 0x3f)<<6;
            *out |= *(ptr+5) & 0x3f;
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
int unicode2utf8unit(uint64_t unicode, char* buf, unsigned int bufsize) {
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
 */
void eachUnicode(
        const char* ptr,
        unsigned int len,
        int (*onEachUnicode)(int64_t, int, void*),
        void* cbdata) {
    int64_t out = 0;
    int utf8unitlen = 0;
    const char* maxptr = ptr+len;
    for (;ptr < maxptr;) {
        utf8unitlen = utf8unit2unicode(ptr, &out);
        if (utf8unitlen != -1) {
            ptr += utf8unitlen;
            if (-1 == onEachUnicode(out, utf8unitlen, cbdata)) {
                return;
            }
        }
    }
}