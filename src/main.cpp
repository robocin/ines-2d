#include "machines/goalie/goalie_stm.h"
#include "machines/kicker/kicker_stm.h"

#include <thread>
#include <unistd.h>

void triggerKicker(Kicker& machine) {
  constexpr int ktransitions = 20;

  for (int i = 0; i < ktransitions; i++) {
    machine.process_event(transitionKicker());
  }
}

void triggerGoalie(Goalie& machine) {
  constexpr int ktransitions = 20;

  for (int i = 0; i < ktransitions; i++) {
    machine.process_event(transitionGoalie());
  }
}

int main() {
  Kicker machineKicker;
  machineKicker.initiate();
  std::thread triggerEventKicker(triggerKicker, std::ref(machineKicker));
  triggerEventKicker.join();

  Goalie machineGoalie;
  machineGoalie.initiate();
  std::thread triggerEventGoalie(triggerGoalie, std::ref(machineGoalie));
  triggerEventGoalie.join();

  return 0;
}