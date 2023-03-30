#include "oslabs.h"

// struct PCB {
//        int process_id;
//        int arrival_timestamp;
//        int total_bursttime;
//        int execution_starttime;
//        int execution_endtime;
//        int remaining_bursttime;
//        int process_priority;
//    };

struct PCB NULLPCB = {
    0,0,0,0,0,0,0
};

int is_null_pcb(struct PCB pcb) {
    if (pcb.process_id != 0|| pcb.arrival_timestamp != 0 || pcb.total_bursttime != 0 ||
    pcb.execution_starttime != 0 || pcb.execution_endtime != 0 || pcb.remaining_bursttime != 0
     || pcb.process_priority != 0 ) {
        return 0;
    } 
    else {
        return 1;
    };
};




struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp) {
    // If there is no currently-running process (i.e., the third argument is the NULLPCB), then the method returns the PCB of the newly-arriving process, 
    // indicating that it is the process to execute next. In this case, the PCB of the new process is modified so that the execution start time is set to the current timestamp, 
    // the execution end time is set to the sum of the current timestamp and the total burst time and the remaining burst time is set to the total burst time. 
    if (is_null_pcb(current_process))
    {
     new_process.execution_starttime = timestamp;
     new_process.execution_endtime = timestamp + new_process.total_bursttime;
     new_process.remaining_bursttime = new_process.total_bursttime;

     return new_process;
     
    } 

    // If there is a currently-running process, the method compares the priority of the newly-arriving process with the currently-running process. 
    if (current_process.process_priority <= new_process.process_priority) {
        // If the new process has equal or lower priority (smaller integers for the priority field in the PCB indicate higher priority), then its PCB is simply 
        // added to the ready queue and the return value is the PCB of the currently-running process. As the newly-arriving process is added to the ready queue, its execution 
        // start time and execution end time are set to 0, and the remaining burst time is the same as its total burst time. 
        new_process.execution_endtime = 0;
        new_process.execution_starttime = 0;
        new_process.remaining_bursttime = new_process.total_bursttime;
        ready_queue[*queue_cnt] = new_process;
        *queue_cnt = *queue_cnt + 1;

        return current_process;
    } else {
        // If the new process has a higher priority, then the PCB of the currently-running process is added to the ready queue and the return value is the PCB of the new process. 
        // In this case, the PCB of the new process is modified so that the execution start time is set to the current timestamp, the execution end time is set to the sum of t
        // he current timestamp and the total burst time and the remaining burst time is set to the total burst time. Also, the PCB of the currently-running process is added to the 
        // ready queue after marking its execution end time as 0, and adjusting its remaining burst time. 
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;


        current_process.remaining_bursttime = current_process.execution_endtime - timestamp; 
        current_process.execution_endtime = 0;
        ready_queue[*queue_cnt] =current_process;
        *queue_cnt = *queue_cnt + 1;

        return new_process;
    };
};

struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp) {
    // If the ready queue is empty, the method returns the NULLPCB, indicating that there is no process to execute. 
    if (*queue_cnt == 0) {

        return NULLPCB;
    }
    
    // Otherwise, the method finds the PCB of the process in the ready queue with the highest priority (smaller integers for the priority field in the PCB mean higher priorities), 
    // removes this PCB from the ready queue and returns it. Before returning the PCB of the next process to execute, it is modified to set the execution start time as the current timestamp and 
    // the execution end time as the sum of the current timestamp and the remaining burst time. 
    struct PCB *max_pcb = &NULLPCB;
    int index =0;
    // find the max priority PCB
    for (int i=0;i<*queue_cnt;i++) {
        if (is_null_pcb(*max_pcb)){
            *max_pcb = ready_queue[i];
            continue;
        }
        if (ready_queue[i].process_priority < (*max_pcb).process_priority) {
            *max_pcb = ready_queue[i];
            index = i;
        }
    }

    // remove selected PCB
    for (int i=index;i<*queue_cnt-1;i++) {
        ready_queue[i] = ready_queue[i+1];
    }
    (*queue_cnt)--;

    // modify selected PCB
    (*max_pcb).execution_starttime = timestamp;
    (*max_pcb).execution_endtime = timestamp + (*max_pcb).remaining_bursttime;
    return *max_pcb;
    


};


struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int time_stamp) {
    //If there is no currently-running process (i.e., the third argument is the NULLPCB), then the method returns the PCB of the newly-arriving process, indicating that it is the process to execute next. In this case, the PCB of the new process is modified so that the execution start time is set to the current timestamp, the execution end time is set to the sum of the current timestamp and the total burst time and the remaining burst time is set to the total burst time. 
    if (is_null_pcb(current_process)) {
        new_process.execution_starttime = time_stamp;
        new_process.execution_endtime = time_stamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;

        return new_process;
     
    }


    if (current_process.remaining_bursttime <= new_process.total_bursttime) {
        // If there is a currently-running process, the method compares the remaining burst time of the currently-running process and the total burst time of the 
        // newly-arriving process. If the new process does not have a shorter burst time, then its PCB is simply added to the ready queue and the return value is the PCB of 
        // the currently running process. As the newly-arriving process is added to the ready queue, its execution start time and execution end time are set to 0, and the 
        // remaining burst time is set to the total burst time. 
        new_process.execution_endtime = 0;
        new_process.execution_starttime = 0;
        new_process.remaining_bursttime = new_process.total_bursttime;
        ready_queue[*queue_cnt] = new_process;
        *queue_cnt = *queue_cnt + 1;;

        return current_process;
    } else {
        // If the new process has a shorter burst time, then the PCB of the currently-running process is added to the ready queue and the return value is the PCB of 
        // the new process. In this case, the PCB of the new process is modified so that the execution start time is set to the current timestamp, the execution end time 
        // is set to the sum of the current timestamp and the total burst time and the remaining burst time is set to the total burst time. Also, the PCB of the 
        // currently-running process is added to the ready queue, after marking its execution start time and execution end time as 0, and adjusting its remaining burst time.  
        new_process.execution_starttime = time_stamp;
        new_process.execution_endtime = time_stamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;

        current_process.remaining_bursttime = current_process.execution_endtime - time_stamp;
        current_process.execution_endtime = 0;
        ready_queue[*queue_cnt] =current_process;
        *queue_cnt = *queue_cnt + 1;;

        return new_process;
    };

};




struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp) {
    // If the ready queue is empty, the method returns the NULLPCB, indicating that there is no process to execute next. 
    if (*queue_cnt == 0) {
        return NULLPCB;
    }


    // Otherwise, the method finds the PCB of the process in the ready queue with the smallest remaining burst time, removes this PCB from the ready queue and returns it. 
    // Before returning the PCB of the next process to execute, it is modified to set the execution start time as the current timestamp and the execution end time 
    // as the sum of the current timestamp and the remaining burst time. 
    struct PCB *max_pcb = &NULLPCB;
    int index =0;
    // find the max priority PCB
    for (int i=0;i<*queue_cnt;i++) {
        if (is_null_pcb(*max_pcb)){
            *max_pcb = ready_queue[i];
            continue;
        }
        if (ready_queue[i].remaining_bursttime < (*max_pcb).remaining_bursttime) {
            *max_pcb = ready_queue[i];
            index = i;
        }
    }

    // remove selected PCB
    for (int i=index;i<*queue_cnt-1;i++) {
        ready_queue[i] = ready_queue[i+1];
    }
    (*queue_cnt)--;

    // modify selected PCB
    (*max_pcb).execution_starttime = timestamp;
    (*max_pcb).execution_endtime = timestamp + (*max_pcb).remaining_bursttime;
    return *max_pcb;
    

};



struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp, int time_quantum) {
    // If there is no currently-running process (i.e., the third argument is the NULLPCB), then the method returns the PCB of the newly-arriving process, 
    // indicating that it is the process to execute next. In this case, the PCB of the new process is modified so that the execution start time is set to the current timestamp,
    // the execution end time is set to the sum of the current timestamp and the smaller of the time quantum and the total burst time. 
    // The remaining burst time is set to the total burst time. 
    if (is_null_pcb(current_process)) {
        new_process.execution_starttime = timestamp;
        new_process.remaining_bursttime = new_process.total_bursttime;

        int time_to_add = time_quantum;
        if (new_process.remaining_bursttime < time_quantum) {
            time_to_add = new_process.remaining_bursttime;
        }
        new_process.execution_endtime = timestamp + time_to_add;

        return new_process;     
    }

    // If there is a currently-running process, the method simply adds the PCB of the newly-arriving process to the ready queue and the return value is the PCB of the 
    // currently running process. As the newly-arriving process is added to the ready queue, its execution start time and execution end time are set to 0, and the remaining
    //  burst time is set to the total burst time. 
    new_process.execution_endtime = 0;
    new_process.execution_starttime = 0;
    new_process.remaining_bursttime = new_process.total_bursttime;
    ready_queue[*queue_cnt] = new_process;
    (*queue_cnt)++;

    return current_process;
};




struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int time_stamp, int time_quantum) {
    // If the ready queue is empty, the method returns the NULLPCB, indicating that there is no process to execute next. 
    if (*queue_cnt == 0) {

        return NULLPCB;
    }
    
    // Otherwise, the method finds the PCB of the process in the ready queue with the earliest arrival time, removes this PCB from the ready queue and returns it. 
    // Before returning this PCB, it is modified to set the execution start time as the current timestamp and the execution end time as the sum of the current 
    // timestamp and the smaller of the time quantum and the remaining burst time. 
    struct PCB *max_pcb = &NULLPCB;
    int index =0;
    // find the max priority PCB
    for (int i=0;i<*queue_cnt;i++) {
        if (is_null_pcb(*max_pcb)){
            *max_pcb = ready_queue[i];
            continue;
        }
        if (ready_queue[i].arrival_timestamp < (*max_pcb).arrival_timestamp) {
            *max_pcb = ready_queue[i];
            index = i;
        }
    }

    // remove selected PCB
    for (int i=index;i<*queue_cnt-1;i++) {
        ready_queue[i] = ready_queue[i+1];
    }
    (*queue_cnt)--;

    // modify selected PCB
    (*max_pcb).execution_starttime = time_stamp;

    int time_to_add = time_quantum;
        if ((*max_pcb).remaining_bursttime < time_quantum) {
            time_to_add = (*max_pcb).remaining_bursttime;
        }
    (*max_pcb).execution_endtime = time_stamp + time_to_add;
    return *max_pcb;
}; 
