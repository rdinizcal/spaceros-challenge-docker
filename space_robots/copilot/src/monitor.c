#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "monitor_types.h"
#include "monitor.h"

//static int64_t input_signal_cpy;
static float input_signal_float_cpy;
//static double input_signal_double_cpy;

static bool handlerBatteryCopilot_guard(void) {
  return !(input_signal_float_cpy >= ((float)(0.3f)));
}

void step(void){
  (input_signal_float_cpy) = (input_signal_float);

  //if ((handlerBatteryCopilot_guard)()) {
    {(handlerBatteryCopilot)(handlerBatteryCopilot_guard());}
  //};
}
