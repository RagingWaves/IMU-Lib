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
#include "Matrix.h"

#if MAT_MODE == 0
/**
  * @brief  初始化矩阵
  * @param  需初始化的矩阵
  * @param  矩阵行数量
  * @param  矩阵列数量
  * @param  矩阵初始数值
  * @retval void
  * @attention  矩阵数据储存于数组中，全局矩阵应搭配全局数组。
  */
void mat_init(mat *S, unsigned int nRows, unsigned int nCols, float *pData) {
	int c, r;
	if(nRows == 0 || nCols == 0)
		return;
	S->numRows = nRows;
	S->numCols = nCols;
	S->pData = pData;
	for(c = 0; c < nCols; c++)
		for(r = 0; r < nRows; r++)
			S->pData[c*S->numRows+r] = pData[c*nRows+r];
}

/**
  * @brief  矩阵相加：A+B，结果储存于矩阵out
  * @param  输入矩阵A
  * @param  输入矩阵B
  * @param  输出矩阵
  * @retval 运算成功返回MAT_SUCCESS
  */
mat_status mat_add(const mat *A, const mat *B, mat *out) {
	int c, r;
	if(A->numRows != B->numRows || A->numCols != B->numCols)
		return MAT_SIZE_MISMATCH;
	if(A->numRows != out->numRows || A->numCols != out->numCols)
		return MAT_LENGTH_ERROR;
	for(r = 0; r < out->numRows; r++)
		for(c = 0; c < out->numCols; c++)
			out->pData[r*out->numCols+c] = A->pData[r*A->numCols+c] + B->pData[r*B->numCols+c];
	return MAT_SUCCESS;
}

/**
  * @brief  矩阵相减：A-B，结果储存于矩阵out
  * @param  输入矩阵A
  * @param  输入矩阵B
  * @param  结果输出矩阵
  * @retval 运算成功返回MAT_SUCCESS
  */
mat_status mat_sub(const mat *A, const mat *B, mat *out) {
	int c, r;
	if(A->numRows != B->numRows || A->numCols != B->numCols)
		return MAT_SIZE_MISMATCH;
	if(A->numRows != out->numRows || A->numCols != out->numCols)
		return MAT_LENGTH_ERROR;
	for(r = 0; r < out->numRows; r++)
		for(c = 0; c < out->numCols; c++)
			out->pData[r*out->numCols+c] = A->pData[r*A->numCols+c] - B->pData[r*B->numCols+c];
	return MAT_SUCCESS;
}

/**
  * @brief  矩阵相乘：A*B，结果储存于矩阵out
  * @param  输入矩阵A
  * @param  输入矩阵B
  * @param  输出矩阵
  * @retval 运算成功返回MAT_SUCCESS
  */
mat_status mat_mult(const mat *A, const mat *B, mat *out) {
	int c, r, nn;
	if(A->numCols != B->numRows)
		return MAT_SIZE_MISMATCH;
	if(A->numRows != out->numRows || B->numCols != out->numCols)
		return MAT_LENGTH_ERROR;
	for(r = 0; r < out->numRows; r++)
		for(c = 0; c < out->numCols; c++) {
			out->pData[r*out->numCols+c] = 0;
			for(nn = 0; nn < A->numCols; nn++)
				out->pData[r*out->numCols+c] += A->pData[r*A->numCols+nn] * B->pData[nn*B->numCols+c];
		}
	return MAT_SUCCESS;
}

/**
  * @brief  计算转置矩阵，结果储存于矩阵out
  * @param  输入矩阵
  * @param  输出矩阵
  * @retval 运算成功返回MAT_SUCCESS
  */
mat_status mat_trans(const mat *A, mat *out) {
	int i, j;
	if((A->numCols != out->numRows) || (A->numRows != out->numCols))
		return MAT_SIZE_MISMATCH;
	for(i = 0; i < A->numRows; i++)
		for(j = 0; j < A->numCols; j++)
			out->pData[j*out->numCols+i] = A->pData[i*A->numCols+j];
	return MAT_SUCCESS;
}

/**
  * @brief  计算行列式值，结果储存于变量out
  * @param  输入矩阵
  * @param  输出结果
  * @retval 运算成功返回MAT_SUCCESS
  */
mat_status mat_det(const mat *A, float *out) {
	int max_row, swap_count = 0, i, j, k;
	float max, t;
	float *pData;
	if(A->numCols != A->numRows)
		return MAT_SIZE_MISMATCH;
	pData = (float *)malloc(sizeof(float) * A->numRows * A->numCols);
	j = A->numRows*A->numCols;
	for(i = 0; i < j; i++) {
		pData[i] = A->pData[i];
	}
	for(i = 0; i < A->numRows-1; i++) {
		max = fabsf(pData[i*A->numCols+i]);
		max_row = i;
		for(j = i+1; j < A->numRows; j++) {
			if(fabsf(pData[j*A->numCols+i]) > max) {
				max = pData[j*A->numCols+i];
				max_row = j;
			}
		}
		if(i != max_row) {
			for(k = 0; k < A->numCols; k++) {
				t = pData[i*A->numCols+k];
				pData[i*A->numCols+k] = pData[max_row*A->numCols+k];
				pData[max_row*A->numCols+k] = t;
			}
			swap_count++;
		}
		for(j = i+1; j < A->numRows; j++) {
			if(pData[j*A->numCols+i] != 0) {
				t = pData[j*A->numCols+i]/pData[i*A->numCols+i];
				for(k = 0; k < A->numCols; k++) {
					pData[j*A->numCols+k] -= pData[i*A->numCols+k] * t;
				}
			}
		}
	}
	*out = swap_count%2==1 ? -1 : 1;
	for(i = 0; i < A->numCols; i++)
		*out *= pData[i*A->numCols+i];
	free(pData);
	return MAT_SUCCESS;
}

/**
  * @brief  计算逆矩阵，结果储存于矩阵out
  * @param  输入矩阵
  * @param  输出矩阵
  * @retval 运算成功返回MAT_SUCCESS
  */
mat_status mat_inv(const mat *A, mat *out) {
	int i, j, k, di, dj;
	float A_det, t;
	float *mat_A_d;
	mat A_d;
	mat_det(A, &A_det);
	if(A_det == 0)
		return MAT_SINGULAR;	
	mat_A_d = (float *)malloc(sizeof(float) * (A->numRows-1) * (A->numCols-1));
	mat_init(&A_d, A->numRows-1, A->numCols-1, mat_A_d);
	for(i = 0; i < A->numRows; i++)
		for(j = 0; j < A->numCols; j++) {
			for(di = 0, k = 0; di < A->numRows; di++) {
				if(di == i)
					continue;
				for(dj = 0; dj < A->numCols; dj++) {
					if(dj == j)
						continue;
					A_d.pData[k++] = A->pData[di*A->numCols+dj];
				}
			}
			if(i%2)
				k = 1;
			else
				k = 0;
			mat_det(&A_d, &t);
			out->pData[i*A->numCols+j] = t/A_det;
			if((j+k)%2)
				out->pData[i*A->numCols+j] *= -1;
			else
				out->pData[i*A->numCols+j] *= 1;
		}
	free(mat_A_d);
	return MAT_SUCCESS;
}
#endif

