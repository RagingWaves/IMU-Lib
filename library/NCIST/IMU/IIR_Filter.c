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

#include "IIR_Filter.h"
#include "IIR_fdacoefs.h"

#if (MWSPT_NSEC/2) > IIR_SECTIONS_BUF
//缓存区过小，打开IIR_Filter.h修改宏定义IIR_SECTIONS_BUF
#error	The buffer is too small！
#endif

#define IIR_SECTIONS	MWSPT_NSEC/2

/**
  * @brief  初始化滤波器
  * @param  需初始化的滤波器
  * @retval void
  */
void IIR_Filter_Init(IIR_Filter_t *filter) {
	int i, j;
	
	for(i = 0; i < IIR_SECTIONS; i++) {
		for(j = 0; j < 3; j++) {
			filter->s_buf[i][j] = 0;
		}
	}
}

/**
  * @brief  滤波
  * @param  所使用的滤波器
  * @param  输入数据
  * @retval 输出数据
  */
double IIR_Filter_Calc(IIR_Filter_t *filter, double in) {
	int i, j;
	
	filter->in = in;
	for(i = IIR_SECTIONS; i >= 0; i--) {
		for(j = 2; j > 0; j--) {
			filter->s_buf[i][j] = filter->s_buf[i][j-1];
		}
	}
	filter->s_buf[0][0] = in;
	for(i = 0, j = 0; i < IIR_SECTIONS; i++, j+=2) {
		//filter->n_buf[i][0] = NUM[i][0]*NUM[i+1][0]*filter->x_buf[0];
		//y(n)=bg*(b0*x(n)+b1*x(n-1)+b2*x(n-2))-ag*(a1*y(n-1)+a2*y(n-2))
		//程序已做简化，默认a增益、a0为1
		filter->s_buf[i+1][0] = NUM[j][0]*                          //b_gain
		                      (	NUM[j+1][0]*filter->s_buf[i][0]     //b0*x(n)
		                      +	NUM[j+1][1]*filter->s_buf[i][1]     //b1*x(n-1)
		                      +	NUM[j+1][2]*filter->s_buf[i][2])    //b2*x(n-2)
		                      -	DEN[j+1][1]*filter->s_buf[i+1][1]   //a1*y(n-1)
		                      -	DEN[j+1][2]*filter->s_buf[i+1][2] ; //a2*y(n-2)
	}
	filter->out = filter->s_buf[IIR_SECTIONS][0];
	
	return filter->out;
}

