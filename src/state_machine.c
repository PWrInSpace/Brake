#include "../include/state_machine.h"


void state_machine_init(state_machine_t *sm){
  sm->state = LAUNCHPAD;
  sm->state_change = 1;
}

uint8_t state_machine_change_state(state_machine_t *sm, enum STATE state){
  if(state - 1 != sm->state){
    return 0;
  }

  sm->state = state;
  sm->state_change = 1;

  return 1;
}

uint8_t state_machine_check_state_change(state_machine_t *sm){
  if(sm->state_change){
    sm->state_change = 0;
    return 1;
  }

  return 0;
}

void launchpad_loop(state_machine_t *sm){
  //check liftoff detection
  /*
  if(gpio_get_level(LIFTOFF_DETECTION_PIN) == 0){
    sm->state = FLIGHT;
  }*/

}