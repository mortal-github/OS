#ifndef _DISK_SCHEDULE__H_
#define _DISK_SCHEDULE__H_

#include "Primitive.h"
#include "ArrayList.h"

/*
 �����ȷ�����̵����㷨����ͷ�մ�last�ŵ��Ƶ�now�ŵ�,request�ǰ�ʱ�����������
 �����������ȷ�����������˳��
 ����������last>0,now>0,NULL!=request��
 �������������ȷ�����̵����㷨����request�Ĵ��򣬷���ƽ��Ѱ�����롣
 ��������������������������ʲô������������-1.
*/
Int DiskSchedule_FCFS(Int last, Int now, ArrayList* request);

/*
 ���Ѱ��ʱ�����ȴ��̵����㷨����ͷ�մ�last�ŵ��Ƶ�now�ŵ�,request�ǰ�ʱ�����������
 ���������Ѱ��ʱ�����ȵ��������˳��
 ����������last>0,now>0,NULL!=request��
 �����������Ѱ��ʱ�����ȴ��̵����㷨����request�Ĵ��򣬷���ƽ��Ѱ�����롣
 ��������������������������ʲô������������-1.
*/
Int DiskSchedule_SSTF(Int last, Int now, ArrayList* request);
#endif // !_DISK_SCHEDULE__H_
