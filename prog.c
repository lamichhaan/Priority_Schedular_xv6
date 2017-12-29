 struct proc *p;
  struct proc *temp;
  int seed_h, seed_l;
  uint begin_ticks, end_ticks;
  seed_h = 0;
  seed_l = 0;
  for(;;){
back:
	// Enable interrupts on this processor.
	sti();
    	int  cnt_hqueue;
   
	// traverse process table
	// check initial high-priority queue
	cnt_hqueue = 0;
	acquire(&ptable.lock);
	for(p = ptable.proc; p < &ptable.proc[NPROC]; p++) {

  	if(p->state != RUNNABLE || p->priority != 2) {
    continue;
  	}
  	cnt_hqueue++;
	} 
	if(cnt_hqueue != 0) {
  	// exist high priority queue
  	while(cnt_hqueue > 0) {
      
    	// keep seed [0, NPROC - 1]
   
    	for(temp = ptable.proc; temp < &ptable.proc[NPROC]; temp++) {
   	 p = temp;
   	 
      if(temp->state != RUNNABLE || temp->priority != 2) {
    	continue;
      }
      cnt_hqueue++;
    }
    	if (cnt_hqueue == 0) {
      	release(&ptable.lock);
      goto back;
    }
    seed_h %= NPROC;
    	p = &ptable.proc[seed_h];

    if(p -> state == RUNNABLE && p->priority == 2) {
      	proc = p;
      	switchuvm(p);
      	p->state = RUNNING;
         	begin_ticks = ticks;      	
     	swtch(&cpu->scheduler, proc->context);
      	switchkvm();
      	end_ticks = ticks;

      if(p->priority == 2) {
    	p->high_tick += end_ticks - begin_ticks;
      }
      else if (p->priority == 1){
    	p->low_tick += end_ticks - begin_ticks;
      }
      proc = 0;
      cnt_hqueue --;
    	}
    	seed_h ++;
  	}
  	release(&ptable.lock);
  	goto back;
	}
// no high priority queue
	else {
  	int find_one, count_lqueue;
  	find_one = 0;
  	count_lqueue = 0;
	 

  	while(find_one == 0 && count_lqueue < NPROC) {
    	seed_l %= NPROC;
   		 p = &ptable.proc[seed_l];
       		if(p->state == RUNNABLE && p -> priority == 2) {
      break;
    		}
else if(p->state == RUNNABLE) {     
      	begin_ticks = ticks;      	
proc = p;
      	switchuvm(p);   	
      	p->state = RUNNING;
      	swtch(&cpu->scheduler, proc->context);
      	switchkvm();
      	end_ticks = ticks;
      if(p->priority == 2) {
    	p->high_tick += end_ticks - begin_ticks;
      }
      else if(p->priority == 1) {
        	p->low_tick += end_ticks - begin_ticks;
      }
      	proc = 0;
      find_one = 1;
    } else {
   
    };
  
    seed_l++;
    count_lqueue ++;
  	}
}
  	release(&ptable.lock);
  }
}
