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

#ifndef __IIR_FILTER_H
#define __IIR_FILTER_H

/**
  * @attention  �����С���ڽ������������С����뱨��The buffer is too small����
  *             ������Ϊ�˱����ظ����ô��ļ�ʱ����fdacoefs.h��������ض��塣
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

