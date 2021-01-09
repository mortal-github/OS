#ifndef _DISK_SCHEDULE__H_
#define _DISK_SCHEDULE__H_

#include "Primitive.h"
#include "ArrayList.h"

/*
 先来先服务磁盘调度算法，磁头刚从last磁道移到now磁道,request是按时间排序的请求。
 方法按先来先服务调整请求的顺序。
 输入条件：last>0,now>0,now!=last,NULL!=request。
 正常处理：先来先服务磁盘调度算法调整request的次序，返回平均寻道距离。
 错误处理：若不满足输入条件，则什么都不做，返回-1.
*/
double DiskSchedule_FCFS(Int last, Int now, ArrayList* request);
/*
 最短寻道时间优先磁盘调度算法，磁头刚从last磁道移到now磁道,request是按时间排序的请求。
 方法按最短寻道时间优先调整请求的顺序。
 输入条件：last>0,now>0,now!=last,NULL!=request。
 正常处理：最短寻道时间优先磁盘调度算法调整request的次序，返回平均寻道距离。
 错误处理：若不满足输入条件，则什么都不做，返回-1.
*/
double DiskSchedule_SSTF(Int last, Int now, ArrayList* request);

/*
 最短寻道时间优先磁盘调度算法，磁头刚从last磁道移到now磁道,request是按时间排序的请求。
 方法按最短寻道时间优先调整请求的顺序。
 输入条件：last>0,now>0,now!=last,NULL!=request。
 正常处理：最短寻道时间优先磁盘调度算法调整request的次序，返回平均寻道距离。
 错误处理：若不满足输入条件，则什么都不做，返回-1.
*/
double DiskSchedule_SSTF(Int last, Int now, ArrayList* request);
/*
 扫描磁盘调度算法，磁头刚从last磁道移到now磁道,request是按时间排序的请求。
 方法按扫描优先调整请求的顺序。
 输入条件：last>0,now>0,now!=last, NULL!=request。
 正常处理：扫描优先磁盘调度算法调整request的次序，返回平均寻道距离。
 错误处理：若不满足输入条件，则什么都不做，返回-1.
*/
double DiskSchedule_SCAN(Int last, Int now, ArrayList* request);

/*
 循环扫描磁盘调度算法，磁头刚从last磁道移到now磁道,request是按时间排序的请求。
 方法按循环扫描优先调整请求的顺序。
 输入条件：last>0,now>0,now>last, NULL!=request。
 正常处理：循环扫描优先磁盘调度算法调整request的次序，返回平均寻道距离。
 错误处理：若不满足输入条件，则什么都不做，返回-1.
*/
double DiskSchedule_CSCAN(Int last, Int now, ArrayList* request);

void DiskSchedule_test();

#endif // !_DISK_SCHEDULE__H_
