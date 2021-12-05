#ifndef _H_COMMON_
#define _H_COMMON_

#define INLINE static inline

#define _unused __attribute__((unused))
#define _constructor __attribute__((constructor))

#define max(a, b) ((a) > (b) ? (a) : (b))

/*=================color================*/
/*
\033[显示方式;前景色;背景色m ....... \33[0m
bg     fg
40: 黑 30: 黑
41: 红 31: 红
42: 绿 32: 绿
43: 黄 33: 黄
44: 蓝 34: 蓝
45: 紫 35: 紫
46: 深绿 36: 深绿
47: 白色 37: 白色

0                    终端默认设置
1                    高亮加粗显示
4                    使用下划线
*/
#define ATTR(attr) "\033["#attr"m"
#define COLOR_CLEAR ATTR(0)
#define RED ATTR(31)
#define YELLOW ATTR(33)
#define BLUE ATTR(34)
#define WHITE ATTR(37)
#define GREEN ATTR(32)
#define BOLD ATTR(1)

/*==============color end===============*/

#include <stdlib.h>
INLINE void safe_free(void **p) {
    if(p == NULL || *p == NULL) return;
    free(*p);
    *p = NULL;
}
#define sfree(p) safe_free((void **)p)


#endif