/***//****************************************
      https://github.com/RagingWaves
----------------------------------------------
 * @file        IIR_Filter.c/h
 * @dependent   1.  IIR_fdacoefs.h ��MATLAB���ɵĲ����ļ�������������ǰ׺"IIR_"�����ļ���ͻ��
 *              2.  tmwtypes.h �����忴fdacoefs.h�ڵ�˵����
 * @brief       IIR�˲�������Ӧʹ��MATLAB��fdatool�������ɵĲ����ļ�fdacoefs.h��
 *              ��������
 *              �޸��˲���������
 *              ʹ��MATLAB��fdatool�����µ�.h�ļ�ֱ��������ɣ�
 *              ���巽���ɲο���������£�
 *              https://blog.csdn.net/zht9961020/article/details/6980981
 * @mark        1.  �Ѳ���4��2�ڡ�10��5��Butterworth������ȷ����--22.3.29
 *              2.  ����������--22.4.2
 *              3.  ����ͷ�ļ����ã�����IIR_SECTIONS_BUF�궨��
 * @version     V1.0.2
 * @auther      ZYuan
 * @url         
----------------------------------------------
****************************************//***/

#include "IIR_Filter.h"
#include "IIR_fdacoefs.h"

#if (MWSPT_NSEC/2) > IIR_SECTIONS_BUF
//��������С����IIR_Filter.h�޸ĺ궨��IIR_SECTIONS_BUF
#error	The buffer is too small��
#endif

#define IIR_SECTIONS	MWSPT_NSEC/2

/**
  * @brief  ��ʼ���˲���
  * @param  ���ʼ�����˲���
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
  * @brief  �˲�
  * @param  ��ʹ�õ��˲���
  * @param  ��������
  * @retval �������
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
		//���������򻯣�Ĭ��a���桢a0Ϊ1
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

