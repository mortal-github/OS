#ifndef _DISK_SCHEDULE__H_
#define _DISK_SCHEDULE__H_

#include "Primitive.h"
#include "ArrayList.h"

/*
 �����ȷ�����̵����㷨����ͷ�մ�last�ŵ��Ƶ�now�ŵ�,request�ǰ�ʱ�����������
 �����������ȷ�����������˳��
 ����������last>0,now>0,now!=last,NULL!=request��
 �������������ȷ�����̵����㷨����request�Ĵ��򣬷���ƽ��Ѱ�����롣
 ��������������������������ʲô������������-1.
*/
double DiskSchedule_FCFS(Int last, Int now, ArrayList* request);
/*
 ���Ѱ��ʱ�����ȴ��̵����㷨����ͷ�մ�last�ŵ��Ƶ�now�ŵ�,request�ǰ�ʱ�����������
 ���������Ѱ��ʱ�����ȵ��������˳��
 ����������last>0,now>0,now!=last,NULL!=request��
 �����������Ѱ��ʱ�����ȴ��̵����㷨����request�Ĵ��򣬷���ƽ��Ѱ�����롣
 ��������������������������ʲô������������-1.
*/
double DiskSchedule_SSTF(Int last, Int now, ArrayList* request);

/*
 ���Ѱ��ʱ�����ȴ��̵����㷨����ͷ�մ�last�ŵ��Ƶ�now�ŵ�,request�ǰ�ʱ�����������
 ���������Ѱ��ʱ�����ȵ��������˳��
 ����������last>0,now>0,now!=last,NULL!=request��
 �����������Ѱ��ʱ�����ȴ��̵����㷨����request�Ĵ��򣬷���ƽ��Ѱ�����롣
 ��������������������������ʲô������������-1.
*/
double DiskSchedule_SSTF(Int last, Int now, ArrayList* request);
/*
 ɨ����̵����㷨����ͷ�մ�last�ŵ��Ƶ�now�ŵ�,request�ǰ�ʱ�����������
 ������ɨ�����ȵ��������˳��
 ����������last>0,now>0,now!=last, NULL!=request��
 ��������ɨ�����ȴ��̵����㷨����request�Ĵ��򣬷���ƽ��Ѱ�����롣
 ��������������������������ʲô������������-1.
*/
double DiskSchedule_SCAN(Int last, Int now, ArrayList* request);

/*
 ѭ��ɨ����̵����㷨����ͷ�մ�last�ŵ��Ƶ�now�ŵ�,request�ǰ�ʱ�����������
 ������ѭ��ɨ�����ȵ��������˳��
 ����������last>0,now>0,now>last, NULL!=request��
 ��������ѭ��ɨ�����ȴ��̵����㷨����request�Ĵ��򣬷���ƽ��Ѱ�����롣
 ��������������������������ʲô������������-1.
*/
double DiskSchedule_CSCAN(Int last, Int now, ArrayList* request);

void DiskSchedule_test();

#endif // !_DISK_SCHEDULE__H_
