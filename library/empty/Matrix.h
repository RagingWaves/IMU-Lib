/***//****************************************
      https://github.com/RagingWaves
----------------------------------------------
 * @file        Matrix.c/h
 * @brief       矩阵C语言运算库，参考了Cotex-M4中arm_math.h的定义格式，修改宏定义重
 *              映射函数名可直接替代，尽可能保证了通用性。
 * @mark        1.  3x3、2x2矩阵测试通过，不排除运算错误的可能--22.3.2
 *              2.  调整mat_free函数--22.4.2
 *              3.  修改指针机制，去除mat_free函数--22.4.2
 *              4.  修复mat_mult计算条件判断错误的bug，通过2x2与2x1相乘的测试--22.4.4
 *              5.  修复mat_trans、mat_det计算错误；修复mat_inv内存溢出--22.4.7
 *              6.  Cotex-M4硬件解算测试通过，微调文件引用与宏定义--22.4.8
 * @version     V1.2.3
 * @auther      ZYuan
 * @url	        
----------------------------------------------
****************************************//***/

#ifndef __MATRIX_H
#define __MATRIX_H

#ifndef MAT_MODE
	//修改此宏定义改变运算方式
	//0：C语言通用软件解算
	//1：Cotex-M4硬件解算
	#define MAT_MODE 0
#endif

#if MAT_MODE == 1
#include "arm_math.h"

#define mat_status         arm_status
#define MAT_SUCCESS        ARM_MATH_SUCCESS
#define MAT_ARGUMENT_ERROR ARM_MATH_ARGUMENT_ARROR
#define MAT_LENGTH_ERROR   ARM_MATH_LENGTH_ERROR
#define MAT_SIZE_MISMATCH  ARM_MATH_SIZE_MISMATCH
#define MAT_NANINF         ARM_MATH_NANINF
#define MAT_SINGULAR       ARM_MATH_SINGULAR
#define MAT_TEST_FAILURE   ARM_MATH_TEST_FAILURE

#define mat         arm_matrix_instance_f32
#define mat_f64     arm_matrix_instance_f64
#define mat_init    arm_mat_init_f32
#define mat_add     arm_mat_add_f32
#define mat_sub     arm_mat_sub_f32
#define mat_mult    arm_mat_mult_f32
#define mat_trans   arm_mat_trans_f32
#define mat_inv     arm_mat_inverse_f32
#define mat_inv_f64 arm_mat_inverse_f64
	
#else
#include <stdlib.h>
#include <math.h>
typedef enum
{
	MAT_SUCCESS        =  0,        /**< 无错误 */
	MAT_ARGUMENT_ERROR = -1,        /**< 一个或多个参数不正确 */
	MAT_LENGTH_ERROR   = -2,        /**< 数据缓冲区长度不正确 */
	MAT_SIZE_MISMATCH  = -3,        /**< 矩阵大小与操作不兼容 */
	MAT_NANINF         = -4,        /**< 无数字或无穷大 */
	MAT_SINGULAR       = -5,        /**< 输入矩阵为奇异矩阵不能反转 */
	MAT_TEST_FAILURE   = -6         /**< 试验失败 */
} mat_status;
typedef struct {
	unsigned int numRows;
	unsigned int numCols;
	float *pData;
} mat;
typedef struct {
	unsigned int numRows;
	unsigned int numCols;
	double *pData;
} mat_f64;

/**
  * @attention
  * 1.  矩阵数据储存于数组中，全局矩阵应搭配全局数组。
  */
void mat_init(mat *S, unsigned int nRows, unsigned int nCols, float *pData);
mat_status mat_add(const mat *A, const mat *B, mat *out);
mat_status mat_sub(const mat *A, const mat *B, mat *out);
mat_status mat_mult(const mat *A, const mat *B, mat *out);
mat_status mat_trans(const mat *A, mat *out);
mat_status mat_det(const mat *A, float *out);
mat_status mat_inv(const mat *A, mat *out);

#endif

#endif

