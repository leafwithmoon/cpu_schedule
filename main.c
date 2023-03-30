/*
 * Author: Montana Esguerra
 * Filename: main.c
 * Description: Test bed for cpu scheduler
 */

#include <stdio.h>
#include <stdlib.h>
#include "oslabs.h"


// Enqueues a PCB
int enqueue_pcb(struct PCB ready_queue[], int* queue_cnt, struct PCB* to_enqueue)
{
    // Queue is Full - return 0 as error code
    if((*queue_cnt) >= QUEUEMAX)
        return 0;

    ready_queue[(*queue_cnt)] = (*to_enqueue);
    (*queue_cnt)++;

    return 1;
}

void set_pcb(struct PCB *to_set, int pid, int ats, int tbt, int est, int eet, int rbt, int pp)
{
    to_set->process_id = pid;
    to_set->arrival_timestamp = ats;
    to_set->total_bursttime = tbt;
    to_set->execution_starttime = est;
    to_set->execution_endtime = eet;
    to_set->remaining_bursttime = rbt;
    to_set->process_priority = pp;
}

void display_pcb(struct PCB *pcb)
{
    printf("Process ID: %d\nArrival Timestamp: %d\nTotal Burst Time: %d\nExecution Start Time: %d\nExecution End Time: %d\nRemaining Burst Time: %d\nProcess Priority: %d\n",
            pcb->process_id, pcb->arrival_timestamp, pcb->total_bursttime, pcb->execution_starttime, pcb->execution_endtime, pcb->remaining_bursttime, pcb->process_priority);
}



int main( int argc, char* argv[])
{
    struct PCB ready_queue[QUEUEMAX];// empty ready queue
    int queue_cnt = 0;
    struct PCB process1;
    struct PCB process2;
    // struct PCB process3;
    int timestamp = 2;
    //int time_quantum = 10;

    set_pcb(&process1, 1, 1, 4, 1, 5, 4, 6);
    set_pcb(&process2, 2, 2, 3, 0, 0, 3, 8);
    // set_pcb(&process3, 3, 1, 24, 0, 0, 24, 0);

    // enqueue_pcb(ready_queue, &queue_cnt, &process1);
    // enqueue_pcb(ready_queue, &queue_cnt, &process2);
    // enqueue_pcb(ready_queue, &queue_cnt, &process3);
    
    printf("\n BEFORE\n\n");

    printf("queue_cnt=%d\n", queue_cnt);
    printf("ready_queue:\n");

    for(int i = 0; i < queue_cnt; ++i)
    {
        display_pcb(&ready_queue[i]);
        printf("\n===================\n");
    }

    printf("\n AFTER\n\n");
    
    struct PCB result = handle_process_arrival_pp(ready_queue, &queue_cnt, process1, process2, timestamp);

    for(int i = 0; i < queue_cnt; ++i)
    {
        display_pcb(&ready_queue[i]);
        printf("\n===================\n");
    }

    printf("queue_cnt=%d\n", queue_cnt);
    printf("ready_queue:\n");

    printf("\n EARLIEST TIME ARRIVAL\n");
    display_pcb(&result);





    return 0;
}