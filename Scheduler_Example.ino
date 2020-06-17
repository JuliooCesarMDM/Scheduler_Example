#include "SCHEDULER.h"

/********************INFO**************************/
//AUTOR:JULIO CESAR MATIAS
//DATA:10/02/2019
/**************************************************/

//****************MODO DE USO*********************************************************//
//AVRTIME.SchedulerMicros()               >> FUNÇÃO MICROS
//AVRTIME.SchedulerMillis()               >> FUNÇÃO MILLIS
//AVRTIME.SchedulerSleep(T)               >> FUNÇÃO DELAY (T = TEMPO)
//AVRTIME.SchedulerMicroSecondsSleep(T)   >> FUNÇÃO DELAYMICROSECONDS (T = TEMPO)
//************************************************************************************//

void Setup() {
  pinMode(13, OUTPUT);
  Serial.begin(115200);
  AVRTIME.SchedulerSleep(1000);             //ESPERA 1 SEGUNDO
  AVRTIME.SchedulerMicroSecondsSleep(1000); //ESPERA 1 MICROSEGUNDO
}

void Loop() {
  static uint32_t Store;
  static bool     Toogle;
  if (AVRTIME.SchedulerMillis() - Store >= 1000) {
    Toogle = !Toogle;
    if (Toogle)Serial.println("Toogle"), digitalWrite(13, HIGH); else digitalWrite(13, LOW);
    Store = AVRTIME.SchedulerMillis();
  }
}

int main(void) {
  AVRTIME.SchedulerInit();
  Setup();
  for (;;)Loop();
  return 0;
}
