/***//****************************************
      https://github.com/RagingWaves
----------------------------------------------
 * @file        Matrix.c/h
 * @brief       ����C��������⣬�ο���Cotex-M4��arm_math.h�Ķ����ʽ���޸ĺ궨����
 *              ӳ�亯������ֱ������������ܱ�֤��ͨ���ԡ�
 * @mark        1.  3x3��2x2�������ͨ�������ų��������Ŀ���--22.3.2
 *              2.  ����mat_free����--22.4.2
 *              3.  �޸�ָ����ƣ�ȥ��mat_free����--22.4.2
 *              4.  �޸�mat_mult���������жϴ����bug��ͨ��2x2��2x1��˵Ĳ���--22.4.4
 *              5.  �޸�mat_trans��mat_det��������޸�mat_inv�ڴ����--22.4.7
 *              6.  Cotex-M4Ӳ���������ͨ����΢���ļ�������궨��--22.4.8
 * @version     V1.2.3
 * @auther      ZYuan
 * @url	        
----------------------------------------------
****************************************//***/

#ifndef __MATRIX_H
#define __MATRIX_H

#ifndef MAT_MODE
	//�޸Ĵ˺궨��ı����㷽ʽ
	//0��C����ͨ���������
	//1��Cotex-M4Ӳ������
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
	MAT_SUCCESS        =  0,        /**< �޴��� */
	MAT_ARGUMENT_ERROR = -1,        /**< һ��������������ȷ */
	MAT_LENGTH_ERROR   = -2,        /**< ���ݻ��������Ȳ���ȷ */
	MAT_SIZE_MISMATCH  = -3,        /**< �����С����������� */
	MAT_NANINF         = -4,        /**< �����ֻ������ */
	MAT_SINGULAR       = -5,        /**< �������Ϊ��������ܷ�ת */
	MAT_TEST_FAILURE   = -6         /**< ����ʧ�� */
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
  * 1.  �������ݴ����������У�ȫ�־���Ӧ����ȫ�����顣
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

