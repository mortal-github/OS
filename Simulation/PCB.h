#ifndef _PCB__H_
#define _PCB__H_

#include "Primitive.h"

typedef struct PCB {
	//进程标识符
	Int id;
	//struct PCB* parent;
	//ArrayList children;
	Char* name;

	//处理机状态
	//Long ir;
	//Long pswr;
	//Long* regs;
	//Short regs_size;

	//进程调度信息
	Byte status;
	double priority;
	Long arrive;
	Long terminated;
	Int length;
	Int run;

	//进程控制信息
	//Long ram;
	//Long swap;
	//Long disk;
	//同步与通信机制。
	//资源清单。
}PCB;

#define CREATING 0
#define RUNNABLE 1
#define BLOCKED 2
#define TERMINATED 3

/*
 初始化PCB。
 输入条件：NULL!=pcbm, id>=0。
 正常处理：初始化PCB。
 错误处理：若不满足输入条件，返回FALSE。
 隐式条件:
 */
Boolean PCB_init(PCB* pcb, Int id);
/*
 设置进程名。
 输入条件：NULL!=pcbm。
 正常处理：设置进程名，返回TRUE。
 错误处理：若不满足输入条件，返回FALSE。
 隐式条件: 
 */
Boolean PCB_setName(PCB* pcb, Char* name);

/*
 设置进程状态。
 输入条件：NULL!=pcbm，status是合法的值。
 正常处理：设置进程状态返回TRUE。
 错误处理：若不满足输入条件，返回FALSE;转换失败返回FALSE。
 隐式条件:
 */
Boolean PCB_setStatus(PCB* pcb, Byte status);
/*
 设置进程优先数。
 输入条件：NULL!=pcbm。
 正常处理：设置进程优先数。返回TRUE。
 错误处理：若不满足输入条件，返回FALSE。
 隐式条件:
 */
Boolean PCB_setPriority(PCB* pcb, Int priority);
/*
 设置进程到达时间。
 输入条件：NULL!=pcbm, arrive>=0。
 正常处理：设置进程到达时间，返回TRUE。
 错误处理：若不满足输入条件，返回FALSE。
 隐式条件:
 */
Boolean PCB_setArrive(PCB* pcb, Long arrive);
/*
 设置进程结束时间。
 输入条件：NULL!=pcbm, arrive>=0。
 正常处理：设置进程结束时间，返回TRUE。
 错误处理：若不满足输入条件，返回FALSE。
 隐式条件:
 */
Boolean PCB_setTerminate(PCB* pcb, Long terminate);
/*
 设置进程长度。
 输入条件：NULL!=pcbm, length>0。
 正常处理：设置进程长度，返回TRUE。
 错误处理：若不满足输入条件，返回FALSE。
 隐式条件:
 */
Boolean PCB_setLength(PCB* pcb, Int length);
/*
 增加运行时间计数。
 输入条件：NULL!=pcbm, add_time>0。
 正常处理：增加运行时间计数,返回TRUE。
 错误处理：若不满足输入条件，返回FALSE。
 隐式条件:
 */
Boolean PCB_addRun(PCB* pcb, Int time);
#endif // !_PCB__H_
