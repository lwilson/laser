#include "laser.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

laser_event *createEvent(char *title, char *desc, short etype, char *data) {
  laser_event *evt = (laser_event*)malloc(sizeof(laser_event));

  evt->title = (char*)malloc(strlen(title)+1);
  strcpy(evt->title, title);

  evt->desc = (char*)malloc(strlen(desc)+1);
  strcpy(evt->desc, desc);

  evt->generated = time(0);

  evt->eventtype = etype;

  evt->data = (char*)malloc(strlen(data)+1);
  strcpy(evt->data, data);

  return evt;
}

char *createChart(int ncols, int nrows, double *data) {
  
  //Write the contents of the matrix to a temporary file
  //Set the propoer gnuPlot commands
  //Return the filename
}

void addEvent(laser_report *report, laser_event *event) {
  laser_event **tmp = malloc(sizeof(laser_event*)*(report->numevents+1));
  if(report->events != NULL) {
    memcpy(tmp, report->events, sizeof(laser_event*)*report->numevents);
    free(report->events);
  }
  report->events = tmp;
    
  report->events[report->numevents++] = event;
}
