#include "DiskSchedule.h"
#include <math.h>
#include <assert.h>

Int DiskSchedule_FCFS(Int last, Int now, ArrayList* request)
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
    return total / request->size;
}

Int DiskSchedule_SSTF(Int last, Int now, ArrayList* request)
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
        for (Int j = i+1; i < request->size; j++) {
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
    
    return total / request->size;
}
