#include "laser.h"
#include <stdlib.h>
#include <stdio.h>

laser_reduction *createReduction(double(*custom)(double, double, unsigned long)) {
  laser_reduction *tmp = (laser_reduction*)malloc(sizeof(laser_reduction));

  tmp->custom = custom;
  tmp->count = 0;

  int i;
  for(i=0; i<7; i++) tmp->values[i]=0.0;

  return tmp;
}

void reduce(laser_reduction *reducer, double val) {
  double delta;

  //COUNT
  reducer->count++;

  //SUM
  reducer->values[0] += val;  

  //MIN
  if(val < reducer->values[2]) reducer->values[2] = val;

  //MAX
  if(val > reducer->values[3]) reducer->values[3] = val;

  //MEAN & VARIANCE (From Knuth, 1998)
  delta = val - reducer->values[1];
  reducer->values[1] += delta / reducer->count;
  reducer->values[6] += delta * (val - reducer->values[1]);
  reducer->values[4] = reducer->values[6] / reducer->count;

  //CUSTOM
  if(reducer->custom != NULL) {
    reducer->values[5] = reducer->custom(reducer->values[5], val, reducer->count);
  }
}

laser_event *createReductionEvent(laser_reduction *reducer, short options, char *title, char *desc) {
  char datastr[1024] = "";

  if(options & LASER_AGG_COUNT) sprintf(datastr, "*Count, %ld;", reducer->count);
  if(options & LASER_AGG_SUM) sprintf(datastr, "%s *Sum, %lf;", datastr, reducer->values[0]);
  if(options & LASER_AGG_MEAN) sprintf(datastr, "%s *Mean, %lf;", datastr, reducer->values[1]);
  if(options & LASER_AGG_MIN) sprintf(datastr, "%s *Min, %lf;", datastr, reducer->values[2]);
  if(options & LASER_AGG_MAX) sprintf(datastr, "%s *Max, %lf;", datastr, reducer->values[3]);
  if(options & LASER_AGG_VAR) sprintf(datastr, "%s *Variance, %lf;", datastr, reducer->values[4]);
  if(options & LASER_AGG_CUSTOM) sprintf(datastr, "%s *Custom, %lf;", datastr, reducer->values[5]);

  return createEvent(title, desc, LASER_EVENT_TABLE, datastr);
}
