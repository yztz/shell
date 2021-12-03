#ifndef _H_COMMON_
#define _H_COMMON_

#define INLINE static inline

#define _unused __attribute__((unused))
#define _constructor __attribute__((constructor))

/*=================color================*/
/*
bg     fg
40: 黑 30: 黑
41: 红 31: 红
42: 绿 32: 绿
43: 黄 33: 黄
44: 蓝 34: 蓝
45: 紫 35: 紫
46: 深绿 36: 深绿
47: 白色 37: 白色
*/
#define COLOR(bg, fg) "\033[" #bg ";" #fg "m"
#define COLOR_CLEAR "\033[0m"
#define RED COLOR(, 31)
#define YELLOW COLOR(, 33)
#define BLUE COLOR(, 34)

/*==============color end===============*/

#include <stdlib.h>
INLINE void safe_free(void **p) {
    if(p == NULL || *p == NULL) return;
    free(*p);
    *p = NULL;
}
#define sfree(p) safe_free((void **)p)


#endif