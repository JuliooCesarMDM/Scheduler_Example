#ifndef SCHEDULER_h
#define SCHEDULER_h
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
class AVRTIMECLASS
{
  public:
    void     SchedulerInit(void);
    void     SchedulerSleep(uint16_t MillisSeconds);
    void     SchedulerMicroSecondsSleep(uint16_t MicroSeconds);
    uint32_t SchedulerMillis(void);
    uint32_t SchedulerMicros(void);
};
extern AVRTIMECLASS AVRTIME;
#endif
