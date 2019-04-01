//
// Created by admin on 2018/9/12.
//

#ifndef UTIL_CHARS_H
#define UTIL_CHARS_H


#include <stdint.h>

struct buf_s{
    char* base;
    unsigned int len;
    unsigned int size;
};

/**
 * utf8单元转化成unicode单元
 * @param ptr
 * @param unicode
 * @return int -1:error >0: utf8单元长度
 */
int utf8unit2unicode(const char* ptr, int64_t* unicode);

/**
 * unicode单元转为utf8单元
 * @param unicode
 * @param buf
 * @param bufsize
 * @return int uft8单元长度
 */
int unicode2utf8unit(int64_t unicode, char* buf, unsigned int bufsize);

/**
 * 对字符串中的每个unicode单位应用用户回调函数
 * 当用户回调返回-1时退出
 * @param ptr
 * @param len
 * @param onEachUnicode
 * @param cbdata
 * @return int
 */
int eachUnicode(
        const char* ptr,
        unsigned int len,
        int (*onEachUnicode)(int64_t unicode, int utf8unitlen, void* cbdata),
        void* cbdata);

/**
 *
 * @param s char* utf8字符串
 * @param slen unsigned int
 * @param buf char* 输出至buf
 * @param bufsize buf大小
 * @return int buf填充的长度, -1表示buf不够
 */
int str2unicode(char* s, unsigned int slen, char* buf, unsigned int bufsize);

#endif //UTIL_CHARS_H
