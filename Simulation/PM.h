#ifndef _PM__H_
#define _PM__H_

#include "PCB.h"
#include "ArrayList.h"
#include "LinkList.h"

/*进程管理。*/
typedef struct PM {
	/*创建进程*/
	ArrayList creating;
	/*就绪进程*/
	ArrayList runnable;
	/*阻塞进程*/
	ArrayList blocked;
	/*终止进程*/
	ArrayList terminated;
	/*运行进程*/
	PCB* run;

	/*现在时间*/
	Long time;
	/*进程id*/
	Int nextId;
}PM;

/*
 初始化进程管理。
 输入条件：NULL!=pm,NULL！=schedule。
 正常处理：初始化PM,返回TRUE。
 错误处理：若不满足输入条件，什么都不处理，返回NULL;若初始化失败，则什么都不处理，返回NULL。
 隐式条件：所保存的信息已经无效，否则可能丢失信息；schedule确实从就绪队列移除一个进程。
*/
Boolean PM_init(PM* pm);
/*
 更新进程时间控制信息。
 输入条件：NULL!=pm,now_time>pm->time。
 正常处理：更新进程的时间控制信息，返回TRUE。
 错误处理：若不满足输入条件,则什么都不做，返回FALSE。
 隐式条件：pm初始化过，且指通过函数写。
 */
Boolean PM_updateTime(PM* pm, Long now_time);
/*
 将当前进程移出运行状态，插入就绪队列队尾。
 输入条件：NULL!=pm。
 正常处理：将当前进程移出运行状态，插入就绪队列队尾，返回TRUE。
 错误处理：若不满足输入条件,则什么都不做，返回FALSE。
 隐式条件：pm初始化过，且指通过函数写。
 */
Boolean PM_removeRun(PM* pm);
/*
 设置当前进程。
 输入条件：NULL!=pm，RUNNABLE=pcb->status。
 正常处理：设置当前进程，返回TRUE。
 错误处理：若不满足输入条件,则什么都不做，返回FALSE。
 隐式条件：pm初始化过，且指通过函数写；pcb已经移出就绪队列，且不能存在其他队列中。
 */
Boolean PM_setRun(PM* pm, PCB* pcb);

/*
 申请空白PCB。
 输入条件：NULL!=pm。
 正常处理：返回一个空白初始化的PCB。
 错误处理：若不满足输入条件，或申请失败，则什么都不做，返回NULL。
 隐式条件：pm已经初始化过，且只通过函数处理pm。
*/
PCB* PM_apply(PM* pm);
/*激活进程。
输入条件: NULL!=pm, NULL!=pcb,pcb!=pm->run,pcb是CREATING或BLOCKED。
正常处理：把一个PCB激活，即加入就绪队列，返回TRUE。
错误处理：若不满足输入条件或激活失败，则什么都不做，返回FALSE。
隐式条件：pm已经初始化过，且只通过函数处理pm；pcb已经初始化过，且只通过函数处理pcb。
*/
Boolean PM_active(PM* pm, PCB* pcb);
/*阻塞进程。
输入条件: NULL!=pm, NULL!=pcb,pcb!=pm->run,pcb是RUNNABLE。
正常处理：阻塞进程，即加入阻塞队列，返回TRUE。
错误处理：若不满足输入条件或阻塞失败，则什么都不做，返回FALSE。
隐式条件：pm已经初始化过，且只通过函数处理pm；pcb已经初始化过，且只通过函数处理pcb。
*/
Boolean PM_block(PM* pm, PCB* pcb);
/*终止进程。
输入条件: NULL!=pm,NULL!=pcb,pcb!=pm->run,pcb不是terminated。
正常处理：终止进程，即加入终止队列，返回TRUE。
错误处理：若不满足输入条件或终止失败，则什么都不做，返回FALSE。
隐式条件：pm已经初始化过，且只通过函数处理pm；pcb已经初始化过，且只通过函数处理pcb。
*/
Boolean PM_terminate(PM* pm, PCB* pcb);

void PM_print(PM* pm);
#endif // !_PM__H_
