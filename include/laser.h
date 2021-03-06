/********************************************************************
 *
 *  TACC In-job Report Library
 *  Authors: Lucas Wilson (lwilson@tacc.utexas.edu
 *           Carlos Rosales (carlos@tacc.utexas.edu)
 *
 *  The TACC In-job Report Library (libreport) provides an intuitive
 *  means of generating summary report for interesting scientific or
 *  computation events that occur in-job, in order to reduce the need
 *  for post-processing on large-scale data sets.
 *   
 *******************************************************************/

#ifndef TACCREPORT_H
#define TACCREPORT_H
#include <time.h>


// EVENTTYPE DEFINES
#define LASER_EVENT_BASIC 0
#define LASER_EVENT_IMAGE 1
#define LASER_EVENT_TABLE 2

// FILETYPE DEFINES
#define LASER_OUTPUT_PDF 0
#define LASER_OUTPUT_TXT 1
#define LASER_OUTPUT_TEX 2

// AGGREGATOR OPTIONS
#define LASER_AGG_COUNT  1
#define LASER_AGG_SUM    2
#define LASER_AGG_MEAN   4
#define LASER_AGG_MIN    8
#define LASER_AGG_MAX    16
#define LASER_AGG_VAR    32
#define LASER_AGG_CUSTOM 64

typedef struct {
  char *title;            //Title for the event to be generated
  char *desc;             //Description of the event
  time_t generated;       //Time the event was generated (UNIX epcoh)
  short eventtype;        //Type of event (defines provided above)
  char *data;             //String containting the event specifics
} laser_event;

typedef struct {
  char *filename;    //Name of the file to be generated
  short filetype;    //Type of report to generate (defines provided above)
  char *filetplate;  //Name of the file containing the LaTeX template

  struct {
    char *title;     //Title for this report
    char **authors;  //Authors
    int numauths;
    char *abstract;       //A brief description of what the job generating this report is doing
    unsigned long jobID;  //Batch ID from the scheduler
    char *invocation;     //Shell invocation line for the job generating this report
    unsigned int NP;      //Number of processors for the job
    time_t walltime;      //Time requested for job
  } header;

  laser_event **events;     //The actual events that will be in the report
  int numevents;

} laser_report;

typedef struct {
  short options;
  unsigned long count;
  double values[7];
  double(*custom)(double, double, unsigned long);
} laser_reduction;

typedef struct {
  short acctype;
  unsigned long numvals;
  void **values;
} laser_accumulator;

/********************************************************************
 *
 *   FUNCTION createReport
 *   Create a report object
 *
 *   Inputs:     fname - String containing the filename to generate
 *               ftype - Type of file to create (PDF, TXT, TEX) (Use LASER_OUTPUT defines)
 *           ftemplate - LaTeX template file to use (optional, for default use NULL)
 *               title - Title of the report
 *            abstract - Description of the report
 *             authors - List of author/contributor/researcher names
 *            numauths - Number of authors
 *
 *   Outputs: Report object pointer (report_type *)
 *
 *******************************************************************/
laser_report *createReport(char *fname, short ftype, char *ftemplate, char *title, char *abstract, char **authors, int numauths);

/********************************************************************
 *
 *   FUNCTION createEvent
 *   Create an event object to add to a report
 *
 *   Inputs: title - Name for the event
 *            desc - Description of the event
 *           etype - Type of event to create (Use LASER defines)
 *            data - String containting the event specifics
 *
 *   Outputs: Event object pointer (event_type *)
 *
 *******************************************************************/
laser_event *createEvent(char *title, char *desc, short etype, char *data);

/********************************************************************
 *
 *   FUNCTION addEvent
 *   Add an event to a report
 *
 *   Inputs: report - Pointer to report object
 *            event - Pointer to event object to be added
 *
 *   Outputs: None
 *
 *******************************************************************/
void addEvent(laser_report *report, laser_event *event);


/********************************************************************
 *
 *   FUNCTION createReduction
 *   Create a reduction object for collecting statistics
 *
 *   Inputs: custom - Function pointer for custom statistics
 *
 *   Outputs: Reduction object pointer (laser_reduction *)
 *
 *******************************************************************/
laser_reduction *createReduction(double(*custom)(double, double, unsigned long));

/********************************************************************
 *
 *   FUNCTION reduce
 *   Place a new value into a reduction object
 *
 *   Inputs: reducer - Pointer to aggregate object
 *               val - Value to be aggregated
 *
 *   Outputs: None
 *
 *******************************************************************/
void reduce(laser_reduction *reducer, double val);

double getReductionValue(laser_reduction *reducer);

/********************************************************************
 *   FUNCTION createReductionEvent
 *   Generate an event from the current state of a reducer
 *
 *   Inputs: reducer - Pointer to reduction object
 *           options - Statistics to report in this event
 *             title - Name for the event
 *              desc - Description of the event
 *
 *   Outputs: Event object pointer (laser_event *)
 *******************************************************************/
laser_event *createReductionEvent(laser_reduction *reducer, short options, char *title, char *desc);

laser_accumulator *createAccumulator(short acctype);

void accumulate(laser_accumulator *accumulator, void *val);

laser_event *createAccumulatorEvent(laser_accumulator *accumulator, short options);


/********************************************************************
 *
 *   FUNCTION generateReport
 *   Generate final report from given report object
 *
 *   Inputs: report - Pointer to report object
 *
 *   Outputs: None
 *
 *******************************************************************/
void generateReport(laser_report *report);
#endif
