

/* /usr/src/servers/pm/table.c   */
do_setdl  /*69 = setdl*/


/* /usr/src/servers/pm/misc.c*/
PUBLIC int do_setdl()
{
        int num_sec;
        struct mproc *rmp;

        rmp = mp;

        num_sec = m_in.m1_i1;

        sys_setdl(rmp->mp_endpoint, num_sec);

        return(OK);
}


/* /usr/src/servers/pm/proto.h new prototype*/
_PROTOTYPE( int do_setdl, (void) );


/* /usr/src/include/minix/callnr.h */
#define SETDL 69

/* /usr/include/minix/callnr.h */
#define SETDL 69

/* la till fil /usr/src/lib/posix/_setdl.c*/
#include <lib.h>
#include <unistd.h>

PUBLIC int setdl(int num_sec)
{
        message m;
        m.m1_i1 = num_sec;

        return(_syscall(MM,SETDL,&m));
}

/* la in setdl libc_FILES i slutet /usr/src/lib/posix/Makefile.in */
 _setdl.c

/*Ändrade i filen /usr/src/lib/posix/setdl.c*/
#include <lib.h>
#include <unistd.h>

PUBLIC int setdl(int num_sec)
{
        message m;
        m.m1_i1 = num_sec;

        return(_syscall(MM,SETDL,&m));
}

/* /usr/src/include/minix/com.h la till kernel_call ändrade nr_sys_calls 50 till 51*/
#  define SYS_SETDL       (KERNEL_CALL + 50) /* sys_setdl() */

/* skapade metoden /usr/src/lib/syslib/sys_setdl.c  */
#include "syslib.h"

/*===========================================================================*
 *                                sys_setdl                                  *
 *===========================================================================*/
PUBLIC int sys_setdl(endpoint_t proc_ep, int seconds)
{
  message m;

  m.PR_ENDPT = proc_ep;
  m.PR_DEADLINE = seconds;

  return(_taskcall(SYSTASK, SYS_SETDL, &m));
}

/*la till prototype /usr/src/include/minix/syslib.h*/
_PROTOTYPE( int sys_setdl, (endpoint_t proc_ep, int seconds)            );

/*la till prototyp med parametrar i /usr/src/kernel/system.h */
_PROTOTYPE( int do_setdl, (message *m_ptr) );
#if ! USE_SETDL
#define do_setdl do_unused
#endif

/*mappa sys_setdl och do_setdl  /usr/src/kernel/system.c*/
  map(SYS_SETDL, do_setdl);             /* set deadline */


/*definera USE_SETDL 1  /usr/src/kernel/config.h*/
#define USE_SETDL          1    /* DEADLINE */


/*la till /usr/src/kernel/do_setdl.c*/
/* The kernel call implemented in this file:
 *   m_type:    SYS_SETDL
 *
 * The parameters for this kernel call are:
 *    m1_i1:    PR_ENDPT        process number to change priority
 *    m1_i2:    PR_PRIORITY     the new priority
 */

#include "../system.h"
#include <sys/resource.h>
#include <time.h>
#if USE_SETDL
/*===========================================================================*
 *                                do_setdl                                   *
 *===========================================================================*/
PUBLIC int do_setdl(message *m_ptr)
{
/* Change process priority or stop the process. */
  int proc_nr, deadline, new_q ;
  register struct proc *rp;

  /* Extract the message parameters and do sanity checking. */
  if(!isokendpt(m_ptr->PR_ENDPT, &proc_nr)) return EINVAL;
  if (iskerneln(proc_nr)) return(EPERM);
  deadline = m_ptr->PR_DEADLINE;
  rp = proc_addr(proc_nr);
  rp->p_deadline = get_uptime() + deadline;

/*La till i objects do_setdl.o och längst ner /usr/src/kernel/system/Makefile  */
$(SYSTEM)(do_setdl.o)

$(SYSTEM)(do_setdl.o):        do_setdl.c
        $(CC) do_setdl.c


/*La till en if i enqueue för att kolla om det är i q 8 och att det är rp->p_deadline  /usr/src/kernel/proc.c */
/*===========================================================================*
 *                              enqueue                                      *
 *===========================================================================*/
PUBLIC void enqueue(rp)
register struct proc *rp;       /* this process is now runnable */
{
/* Add 'rp' to one of the queues of runnable processes.  This function is
 * responsible for inserting a process into one of the scheduling queues.
 * The mechanism is implemented here.   The actual scheduling policy is
 * defined in sched() and pick_proc().
 */
  int q;                                        /* scheduling queue to use */
  int front;                                    /* add to front or back */

  NOREC_ENTER(enqueuefunc);

#if DEBUG_SCHED_CHECK
  if(!intr_disabled()) { minix_panic("enqueue with interrupts enabled", NO_NUM); }
  if (rp->p_ready) minix_panic("enqueue already ready process", NO_NUM);
#endif

  /* Determine where to insert to process. */
  sched(rp, &q, &front);

  vmassert(q >= 0);

 if(q == 8 && rp->p_deadline > 0){
   edf(rp);
  }else {
  /* Now add the process to the queue. */
  if (rdy_head[q] == NIL_PROC) {                /* add to empty queue */
      rdy_head[q] = rdy_tail[q] = rp;           /* create a new queue */
      rp->p_nextready = NIL_PROC;               /* mark new end */
  }
  else if (front) {                             /* add to head of queue */
      rp->p_nextready = rdy_head[q];            /* chain head of queue */
      rdy_head[q] = rp;                         /* set new queue head */
  }
  else {                                        /* add to tail of queue */
      rdy_tail[q]->p_nextready = rp;            /* chain tail of queue */
      rdy_tail[q] = rp;                         /* set new queue tail */
      rp->p_nextready = NIL_PROC;               /* mark new end */
  }
 }


#if DEBUG_SCHED_CHECK
  rp->p_ready = 1;
  CHECK_RUNQUEUES;
#endif

  /*
   * enqueueing a process with a higher priority than the current one, it gets
   * preempted. The current process must be preemptible. Testing the priority
   * also makes sure that a process does not preempt itself
   */
  vmassert(proc_ptr);
  if ((proc_ptr->p_priority > rp->p_priority) &&
                  (priv(proc_ptr)->s_flags & PREEMPTIBLE))
     RTS_SET(proc_ptr, RTS_PREEMPTED); /* calls dequeue() */

#if DEBUG_SCHED_CHECK
  CHECK_RUNQUEUES;
#endif

  NOREC_RETURN(enqueuefunc, );
}


/* Kollar om current deadline är mindre än nästa deadline i kön.*/

/*---------
  edf
----------*/
PRIVATE void edf(rp)
register struct proc *rp;
{
  struct proc **c_proc; /* current process */
  int q;
  q = rp->p_priority;
  c_proc = &rdy_head[q];

/*  printf("Deadline for rp: %d \n", rp->p_deadline); */

  while(*c_proc != NIL_PROC && (*c_proc)->p_deadline < rp->p_deadline) {
 /* printf("This process is smaller than in queue c_proc:  %d \n", (*c_proc)->p_deadline);*/
    c_proc = &(*c_proc)->p_nextready;
  }

 /*  printf("c_proc now has deadline:  %d \n", (*c_proc)->p_deadline); */

  rp->p_nextready = *c_proc;
  *c_proc = rp;

 /* printf("after switch \n"); */
 /* printf("rp has deadline: %d \n", rp->p_deadline); */

/*  printf("rp->nextready has deadline: %d \n", (*(rp->p_nextready))->p_deadline); */

 /* Checks if process enter empty queue */
  if(rp->p_nextready == NIL_PROC)  {
    rdy_tail[q] = rp;
  }
}
