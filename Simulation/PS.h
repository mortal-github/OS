#ifndef _PS__H_
#define _PS__H_

#include "Primitive.h"
#include "PCB.h"
#include "ArrayList.h"
#include "Heap.h"
#include "PM.h"

/*
短作业调度算法，
输入条件：NULL!=pm,
处理结果：排序就绪队列，选择最优先进程，移出该就绪队列，返回该进程。
错误处理：若不满足输入条件，返回NULL,失败返回NULL。
隐式条件：pm初始化过，且仅通过函数处理pm。
*/
PCB* SPF(ArrayList* list);
/*
 轮转调度算法。
 输入条件：NULL!=list, NULL!=pcb。
 正常处理：为轮转算法调度选择最优PCB,移除队列，并返回。
 错误处理：如果不满足输入条件，则什么都不做，返回NULL。
 隐式条件：list的元素是PCB。
*/
PCB* RR(ArrayList* list);
/*
 高响应比优先调度算法。
 输入条件：NULL!=list, NULL!=pcb, time>0。
 正常处理：为高响应比优先调度算法调度程序，添加新进程,返回TRUE。
 错误处理：如果不满足输入条件，则什么都不做，返回FALSE;添加失败，则什么都不做，返回FALSE。
 隐式条件：list的元素是PCB。
*/
PCB* HRRN(ArrayList* list, Long now_time);


//演示调度程序

#define SPF_METHOD 1
#define RR_METHOD  2
#define HRRN_METHOD  3
//选择特定调度算法来执行。
PCB* Schedule(PM* pm, Int method);
//演示特定调度算法的一次调度。
Boolean runOnce(PM* pm, Int time, Int schedule);
//生成指定范围随机数
Int random(Int min, Int max);
//*产生特定的时间出现的进程。
ArrayList runOrder(Int srand_param, Int length_min, Int length_max, Int count);
//演示特定调度算法的所有调度
void runUntil(PM* pm, Int time_max, Int schedule, ArrayList* list);
//选择调度方法，演示调度。
void Schedule_show();

#endif // !_PS__H_
