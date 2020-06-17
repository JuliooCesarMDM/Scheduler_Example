#include "SCHEDULER.h"

AVRTIMECLASS AVRTIME;

volatile uint32_t    Timer0_OverFlow         = 0;
volatile uint32_t    Timer0_Scheduler_Millis = 0;
static unsigned char Timer0_Fraction         = 0;

ISR(TIMER0_OVF_vect) {
  uint32_t MillisCount        = Timer0_Scheduler_Millis;
  unsigned char FractionCount = Timer0_Fraction;
  MillisCount       += 1;
  FractionCount     += (1024 % 1000) >> 3;
  if (FractionCount >= 1000 >> 3) {
    FractionCount   -= 1000 >> 3;
    MillisCount     += 1;
  }
  Timer0_Fraction         = FractionCount;
  Timer0_Scheduler_Millis = MillisCount;
  Timer0_OverFlow++;
}

uint32_t AVRTIMECLASS::SchedulerMillis() {
  uint32_t MillisCount;
  uint8_t oldSREG = SREG;
  cli();
  MillisCount = Timer0_Scheduler_Millis;
  SREG = oldSREG;
  return MillisCount;
}

uint32_t AVRTIMECLASS::SchedulerMicros() {
  uint32_t MillisCount;
  uint8_t oldSREG = SREG, TCNTCount;
  cli();
  MillisCount = Timer0_OverFlow;
  TCNTCount   = TCNT0;
  if ((TIFR0 & _BV(TOV0)) && (TCNTCount < 255))MillisCount++;
  SREG = oldSREG;
  return ((MillisCount << 8) + TCNTCount) * (64 / 16);
}

void AVRTIMECLASS::SchedulerSleep(uint16_t MillisSeconds) {
  uint32_t Start = AVRTIME.SchedulerMicros();
  while (MillisSeconds > 0) {
    AVRTIME.SchedulerMicros();
    AVRTIME.SchedulerMillis();
    while ((AVRTIME.SchedulerMicros() - Start) >= 1000) {
      MillisSeconds--;
      if (MillisSeconds == 0)break;
      Start += 1000;
    }
  }
}

void AVRTIMECLASS::SchedulerMicroSecondsSleep(uint16_t MicroSeconds) {
  if (--MicroSeconds == 0)return;
  MicroSeconds <<= 2;
  MicroSeconds -= 2;
  __asm__ __volatile__ ("1: sbiw %0,1" "\n\t" "brne 1b" : "=w" (MicroSeconds) : "0" (MicroSeconds));
}

void AVRTIMECLASS::SchedulerInit() {
  //TODAS AS CONFIGURAÇÕES DE REGISTRADORES ABAIXO SERVEM APENAS PARA OS MICROCONTROLADORES ATMEGA 328/1280 E 2560
  sei();
  _SFR_BYTE(TCCR0A) |= _BV(WGM01);
  _SFR_BYTE(TCCR0A) |= _BV(WGM00);
  _SFR_BYTE(TCCR0B) |= _BV(CS01);
  _SFR_BYTE(TCCR0B) |= _BV(CS00);
  _SFR_BYTE(TIMSK0) |= _BV(TOIE0);
  TCCR1B = 0;
  _SFR_BYTE(TCCR1B) |= _BV(CS11);
  _SFR_BYTE(TCCR1B) |= _BV(CS10);
  _SFR_BYTE(TCCR1A) |= _BV(WGM10);
  _SFR_BYTE(TCCR2B) |= _BV(CS22);
  _SFR_BYTE(TCCR2A) |= _BV(WGM20);

  //REGISTRADORES PARA O MEGA 1280 E 2560
#if defined(TCCR3B) && defined(CS31) && defined(WGM30)
  _SFR_BYTE(TCCR3B) |= _BV(CS31);
  _SFR_BYTE(TCCR3B) |= _BV(CS30);
  _SFR_BYTE(TCCR3A) |= _BV(WGM30);
#endif

#if defined(TCCR4B) && defined(CS41) && defined(WGM40)
  _SFR_BYTE(TCCR4B) |= _BV(CS41);
  _SFR_BYTE(TCCR4B) |= _BV(CS40);
  _SFR_BYTE(TCCR4A) |= _BV(WGM40);
#endif

#if defined(TCCR5B) && defined(CS51) && defined(WGM50)
  _SFR_BYTE(TCCR5B) |= _BV(CS51);
  _SFR_BYTE(TCCR5B) |= _BV(CS50);
  _SFR_BYTE(TCCR5A) |= _BV(WGM50);
#endif

  _SFR_BYTE(ADCSRA) |= _BV(ADPS2);
  _SFR_BYTE(ADCSRA) |= _BV(ADPS1);
  _SFR_BYTE(ADCSRA) |= _BV(ADPS0);
  _SFR_BYTE(ADCSRA) |= _BV(ADEN);
}
