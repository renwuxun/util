//
// Created by admin on 2018/9/12.
//

#ifndef UTIL_CHARS_H
#define UTIL_CHARS_H


#include <stdint.h>
#include <stddef.h>

struct buf_s{
    char* base;
    size_t len;
    size_t size;
};

/**
 * utf8单元转化成unicode单元
 * @param ptr
 * @param unicode
 * @return int -1:error >0: utf8单元长度
 */
int utf8unit2unicode(const char* ptr, uint32_t* unicode);

/**
 * unicode单元转为utf8单元
 * @param unicode
 * @param buf
 * @param bufsize
 * @return int uft8单元长度
 */
int unicode2utf8unit(uint32_t unicode, char* buf, size_t bufsize);

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
        size_t len,
        int (*onEachUnicode)(uint32_t unicode, int utf8unitlen, void* cbdata),
        void* cbdata);

/**
 *
 * @param s char* utf8字符串
 * @param slen unsigned int
 * @param buf char* 输出至buf
 * @param bufsize buf大小
 * @return int buf填充的长度, -1表示buf不够
 */
int str2unicode(char* s, size_t slen, char* buf, size_t bufsize);

/**
 *
 * @param s char* utf8字符串
 * @param slen unsigned int
 * @param buf char* 输出至buf
 * @param bufsize buf大小
 * @return int buf填充的长度, -1表示buf不够
 */
int str2ascii(char* s, size_t slen, char* buf, size_t bufsize);

/**
 *
 * @param s char*
 * @param slen size_t
 * @param left char
 * @param right char
 * @param onfound
 * @param onfoundData
 * @param onfinish
 * @param onfinishData
 */
void findBetween(char* s, size_t slen, char left, char right,
                void (*onfound)(char* leftPtr, char* rightPtr, void* onfoundData),
                void* onfoundData,
                void (*onfinish)(char* s, size_t slen, char* lastRightPtr, void* onfinishData),
                void* onfinishData
);

#endif //UTIL_CHARS_H
