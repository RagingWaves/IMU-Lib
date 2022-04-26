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
#include "Matrix.h"

#if MAT_MODE == 0
/**
  * @brief  ��ʼ������
  * @param  ���ʼ���ľ���
  * @param  ����������
  * @param  ����������
  * @param  �����ʼ��ֵ
  * @retval void
  * @attention  �������ݴ����������У�ȫ�־���Ӧ����ȫ�����顣
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
  * @brief  ������ӣ�A+B����������ھ���out
  * @param  �������A
  * @param  �������B
  * @param  �������
  * @retval ����ɹ�����MAT_SUCCESS
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
  * @brief  ���������A-B����������ھ���out
  * @param  �������A
  * @param  �������B
  * @param  ����������
  * @retval ����ɹ�����MAT_SUCCESS
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
  * @brief  ������ˣ�A*B����������ھ���out
  * @param  �������A
  * @param  �������B
  * @param  �������
  * @retval ����ɹ�����MAT_SUCCESS
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
  * @brief  ����ת�þ��󣬽�������ھ���out
  * @param  �������
  * @param  �������
  * @retval ����ɹ�����MAT_SUCCESS
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
  * @brief  ��������ʽֵ����������ڱ���out
  * @param  �������
  * @param  ������
  * @retval ����ɹ�����MAT_SUCCESS
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
  * @brief  ��������󣬽�������ھ���out
  * @param  �������
  * @param  �������
  * @retval ����ɹ�����MAT_SUCCESS
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

