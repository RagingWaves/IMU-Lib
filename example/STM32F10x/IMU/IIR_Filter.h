/***//****************************************
      https://github.com/RagingWaves
----------------------------------------------
 * @file        IIR_Filter.c/h
 * @dependent   1.  IIR_fdacoefs.h （MATLAB生成的参数文件，重命名加上前缀"IIR_"避免文件冲突）
 *              2.  tmwtypes.h （具体看fdacoefs.h内的说明）
 * @brief       IIR滤波器，对应使用MATLAB的fdatool工具生成的参数文件fdacoefs.h，
 *              阶数任意
 *              修改滤波器参数：
 *              使用MATLAB的fdatool生成新的.h文件直接替代即可，
 *              具体方法可参考下面的文章：
 *              https://blog.csdn.net/zht9961020/article/details/6980981
 * @mark        1.  已测试4阶2节、10阶5节Butterworth，均正确运算--22.3.29
 *              2.  修正函数名--22.4.2
 *              3.  修正头文件引用，增加IIR_SECTIONS_BUF宏定义
 * @version     V1.0.2
 * @auther      ZYuan
 * @url         
----------------------------------------------
****************************************//***/

#ifndef __IIR_FILTER_H
#define __IIR_FILTER_H

/**
  * @attention  缓存大小等于节数，若缓存过小则编译报错“The buffer is too small！”
  *             这样是为了避免重复引用此文件时导致fdacoefs.h里的数组重定义。
  */
#define IIR_SECTIONS_BUF	10

typedef struct {
	double s_buf[IIR_SECTIONS_BUF+1][2+1];
	double in;
	double out;
}IIR_Filter_t;

void IIR_Filter_Init(IIR_Filter_t *filter);
double IIR_Filter_Calc(IIR_Filter_t *filter, double in);

#endif

