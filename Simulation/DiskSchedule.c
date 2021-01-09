#include "DiskSchedule.h"
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include "Heap.h"

double DiskSchedule_FCFS(Int last, Int now, ArrayList* request)
{
    if (last <= 0 || now <= 0 || NULL == request) {
        return FALSE;
    }

    if (request->size == 0) {
        return 0;
    }
    assert(request->size > 0);
   
    Int next;
    Int total = 0;
    for (Int i = 0; i < request->size; i++) {
        next = (Int)ArrayList_get(request, i);
        if (next >= now) {
            total += next - now;
        }
        else {
            total += now - next;
        }
        now = next;
    }
    return (double)total / (double)request->size;
}

double DiskSchedule_SSTF(Int last, Int now, ArrayList* request)
{
    if (last <= 0 || now <= 0 || NULL == request) {
        return -1;
   }
    if (request->size == 0) {
        return 0;
    }
    Int next;
    Int min;
    Int distance = 0;
    Int distance2 = 0;
    Int total = 0;
    for (Int i = 0; i < request->size; i++) {
        min = i;
        next = (Int)ArrayList_get(request, i);
        distance = abs(next - now);
        //找到离当前最短的请求。
        for (Int j = i+1; j < request->size; j++) {
            next = (Int)ArrayList_get(request, j);
            distance2 = abs(next - now);
            if (distance2 < distance) {
                min = j;
                distance = distance2;
            }
        }
        next = (Int)ArrayList_remove(request, min);
        ArrayList_add(request, i, (Element)next);
        now = next;
        total += distance;
    }
    
    return (double)total / (double)request->size;
}

Int DiskSchedule_compare(Element left, Element right) {
    if (left < right) {
        return -1;
    }
    else if (left > right) {
        return 1;
    }
    else {
        return 0;
    }
}

double DiskSchedule_SCAN(Int last, Int now, ArrayList* request)
{
    if (last <= 0 || now <= 0 || last == now || NULL == request) {
        return -1;
    }
    Int tag;
    if (last > now) {//向内
        tag = -1;
    }
    else {//向外
        tag = 1;
    }
    //排序
    Heap heap;
    Heap_init(&heap, request, tag, DiskSchedule_compare);
    Heap_adjust(&heap);
    Heap_sort(&heap);
    //向外则找到大者，向内则找到小者
    Int i = 0;
    for (i; i < request->size; i++) {
        Int temp = (Int)ArrayList_get(request, i);
        if (tag == DiskSchedule_compare((Element)temp, (Element)now)) {
            break;
        }
    }
    //i之前元素插入size-j的位置。
    for (Int j = 0; j < i; j++) {
        Int temp = (Int)ArrayList_remove(request, 0);
        ArrayList_add(request, request->size - j, (Element)temp);
    }
    //计算寻道距离
    Int distance = 0;
    for (Int k = 0; k < request->size; k++) {
        Int temp = (Int)ArrayList_get(request, k);
        if (temp > now) {
            distance += temp - now;
        }
        else {
            distance += now - temp;
        }
        now = temp;
    }

    return (double)distance / (double)request->size;
}

double DiskSchedule_CSCAN(Int last, Int now, ArrayList* request)
{
    if (last <= 0 || now <= 0 || last == now || NULL == request) {
        return -1;
    }
    //排序
    Heap heap;
    Heap_init(&heap, request, 1, DiskSchedule_compare);
    Heap_adjust(&heap);
    Heap_sort(&heap);
    //查找下一个
    Int i = 0;
    for (i; i < request->size; i++) {
        Int temp = (Int)ArrayList_get(request, i);
        if (temp >= now) {
            break;
        }
    }
    //把i之前的全部插入后续
    for (Int j = 0; j < i; j++) {
        Int temp = (Int)ArrayList_remove(request, 0);
        ArrayList_add(request, request->size, (Element)temp);
    }
    //计算寻道距离
    Int distance = 0;
    for (Int k = 0; k < request->size; k++) {
        Int temp = (Int)ArrayList_get(request, k);
        if (temp > now) {
            distance += temp - now;
        }
        else {
            distance += now - temp;
        }
        now = temp;
    }

    return (double)distance / (double)request->size;
}

void DiskSchedule_test()
{
    ArrayList list;
    ArrayList_init(&list);
    Int last = 99;
    Int now = 100;
    ArrayList_add(&list, list.size, (Element)55);
    ArrayList_add(&list, list.size, (Element)58);
    ArrayList_add(&list, list.size, (Element)39);
    ArrayList_add(&list, list.size, (Element)18);
    ArrayList_add(&list, list.size, (Element)90);
    ArrayList_add(&list, list.size, (Element)160);
    ArrayList_add(&list, list.size, (Element)150);
    ArrayList_add(&list, list.size, (Element)38);
    ArrayList_add(&list, list.size, (Element)184);

    double avg = DiskSchedule_SCAN(now, last, &list);
    printf_s("avg = %f", avg);
    for (Int i = 0; i < list.size; i++) {
        Int temp = (Int)ArrayList_get(&list, i);
        printf_s(", %d", temp);
    }
}

Boolean DiskSchedule_show_print(Element element) {
    printf_s(", %d", (Int)element);
    return TRUE;
}
void DiskSchedule_show_clone(ArrayList* origin, ArrayList* copy) {
    copy->size = 0;
    for (Int i = 0; i < origin->size; i++) {
        Element element = ArrayList_get(origin, i);
        ArrayList_add(copy, copy->size, element);
    }
}
void DiskSchedule_show()
{
    printf_s("程序简介：\n");
    printf_s("	3118005434， 钟景文，2018级别信息安全2班，计算机学院，广东工业大学\n");
    printf_s("	仿真各种磁盘调度算法：\n");
    printf_s("1 ：先来先服务算法\n");
    printf_s("2 : 最短寻道算法\n");
    printf_s("3 : 扫描算法\n");
    printf_s("4 ：循环扫描算法\n");

    ArrayList request;
    ArrayList_init(&request);

    Int last = 0;
    Int now = 0;
    do {
        printf_s("请输入磁头上一次所在的位置：");
        scanf_s("%d", &last);
        printf_s("请输入磁头当前所在的位置  ：");
        scanf_s("%d", &now);
        if (last == now) {
            printf_s("两次位置不能相等！\n");
        }
        else {
            break;
        }
    } while (1 == 1);

    Int i = 1;
    Int req = 0;
    printf_s("请输入磁盘请求，要求数字大于0，0表示停止输入\n");
    do {
        printf_s("第% 2d个请求: ", i);
        scanf_s("%d", &req);
        if (0 == req) {
            break;
        }
        ArrayList_add(&request, request.size, (Element)req);
        i++;
    } while (1 == 1);

    double avg;
    ArrayList copy;
    ArrayList_init(&copy);
    //先来先服务
    DiskSchedule_show_clone(&request, &copy);
    avg  = DiskSchedule_FCFS(80, 100, &copy);
    printf_s("==============================\n");
    printf_s("先来先服务：\n");
    printf_s("平均寻道距离：%f\n", avg);
    printf_s("寻道顺序：");
    ArrayList_forEach(&copy, DiskSchedule_show_print);
    printf_s("\n");

    //最短寻道时间优先算法
    DiskSchedule_show_clone(&request, &copy);
    avg = DiskSchedule_SSTF(80, 100, &copy);
    printf_s("==============================\n");
    printf_s("最短寻道时间优先算法：\n");
    printf_s("平均寻道距离：%f\n", avg);
    printf_s("寻道顺序：");
    ArrayList_forEach(&copy, DiskSchedule_show_print);
    printf_s("\n");
    

    //扫描算法
    DiskSchedule_show_clone(&request, &copy);
    avg = DiskSchedule_SCAN(80, 100, &copy);
    printf_s("==============================\n");
    printf_s("扫描算法：\n");
    printf_s("平均寻道距离：%f\n", avg);
    printf_s("寻道顺序：");
    ArrayList_forEach(&copy, DiskSchedule_show_print);
    printf_s("\n");


    //循环扫描算法
    DiskSchedule_show_clone(&request, &copy);
    avg = DiskSchedule_CSCAN(80, 100, &copy);
    printf_s("==============================\n");
    printf_s("循环扫描算法：\n");
    printf_s("平均寻道距离：%f\n", avg);
    printf_s("寻道顺序：");
    ArrayList_forEach(&copy, DiskSchedule_show_print);
    printf_s("\n");

    ArrayList_destroy(&request);
    ArrayList_destroy(&copy);
}
