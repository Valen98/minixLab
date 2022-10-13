
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

  printf("Deadline for rp: %d \n", rp->p_deadline);

  while(*c_proc != NIL_PROC && (*c_proc)->p_deadline < rp->p_deadline) {
    printf("This process is smaller than in queue c_proc:  %d \n", (*c_proc)->p_deadline);
    c_proc = &(*c_proc)->p_nextready;
  }

  printf("c_proc now has deadline:  %d \n", (*c_proc)->p_deadline);

  rp->p_nextready = *c_proc;
  *c_proc = rp;

  printf("after switch \n");
  printf("rp has deadline: %d \n", rp->p_deadline);

/*  printf("rp->nextready has deadline: %d \n", (*(rp->p_nextready))->p_deadline);*/

  /* Checks if process enter empty queue */
  if(rp->p_nextready == NIL_PROC)  {
    rdy_tail[q] = rp;
  }
}
