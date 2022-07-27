#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdint.h>

enum STATE{
  LAUNCHPAD = 0,
  FLIGHT,
  AIRBRAKE_EJECTION,
  LANDING,
  ONGROUND,
};

typedef struct{
  enum STATE state;
  uint8_t state_change;
} state_machine_t;


void state_machine_init(state_machine_t *sm);

uint8_t state_machine_change_state(state_machine_t *sm, enum STATE state);

uint8_t state_machine_check_state_change(state_machine_t *sm);

//inline enum STATE state_machine_get_state(state_machine *sm);

void launchpad_loop(state_machine_t *sm);


#endif