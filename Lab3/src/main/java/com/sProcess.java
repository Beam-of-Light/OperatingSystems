package com;

public class sProcess {
  public int cputime;
  public int cpudone;
  public int delay;
  public int numblocked;

  public sProcess (int cputime, int cpudone, int delay, int numblocked) {
    this.cputime = cputime;
    this.cpudone = cpudone;
    this.delay = delay;
    this.numblocked = numblocked;
  } 	
}
