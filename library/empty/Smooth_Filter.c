/***//****************************************
      https://github.com/RagingWaves
----------------------------------------------
 * @file        Smooth_Filter.c/h
 * @brief       ͨ�û���ƽ���˲���
 * @mark        1.  ���--22.4.8
 * @version     V1.0.0
 * @auther      ZYuan
 * @url         
----------------------------------------------
****************************************//***/

#include "Smooth_Filter.h"

/**
  * @brief  �˲�����ʼ��
  * @param  �˲���
  * @param  �˲����������
  * @retval 1����ʼ��ʧ�ܣ�  0����ʼ���ɹ�
  */
int SmoothFilter_Init(SmoothFilter_t *smooth_filter, int dp) {
	int i;
#if SMOOTH_FILTER_DATA_DP > 8
	if(dp > SMOOTH_FILTER_DATA_DP)
		return 1;
#else
	if(dp > 8)
		return 1;
#endif
	smooth_filter->dp      = dp;
	smooth_filter->pointer = 0;
	for(i = 0; i < 4; i++) {
		smooth_filter->raw_data[i] = 0;
	}
	return 0;
}

/**
  * @brief  �˲�
  * @param  �˲���
  * @param  ԭʼ����
  * @retval �˲����
  */
float SmoothFilter_Calc(SmoothFilter_t *smooth_filter, float data) {
	int i;
	float ave = 0;
	smooth_filter->raw_data[smooth_filter->pointer] = data;
	if(++smooth_filter->pointer == smooth_filter->dp)
		smooth_filter->pointer = 0;
	for(i = 0; i < smooth_filter->dp; i++) {
		ave += smooth_filter->raw_data[i];
	}
	ave /= smooth_filter->dp;
	return ave;
}

