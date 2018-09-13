//
// Created by admin on 2018/9/12.
//

#ifndef UTIL_CHARS_H
#define UTIL_CHARS_H


#include <stdint.h>


/**
 * utf8单元转化成unicode单元
 * @param ptr
 * @param out
 * @return int -1:error >0: utf8单元长度
 */
int utf8unit2unicode(const char* ptr, int64_t* out);

/**
 * unicode单元转为utf8单元
 * @param unicode
 * @param buf
 * @param bufsize
 * @return int uft8单元长度
 */
int unicode2utf8unit(uint64_t unicode, char* buf, unsigned int bufsize);

/**
 * 对字符串中的每个unicode单位应用用户回调函数
 * 当用户回调返回-1时退出
 * @param ptr
 * @param len
 * @param onEachUnicode
 * @param cbdata
 */
void utf82unicodecb(
        const char* ptr,
        unsigned int len,
        int (*onEachUnicode)(int64_t out, int utf8unitlen, void* cbdata),
        void* cbdata);


#endif //UTIL_CHARS_H
