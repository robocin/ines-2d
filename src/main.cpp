#include "machines/goalie/goalie_stm.h"
#include "machines/kicker/kicker_stm.h"

#include <thread>
#include <unistd.h>

void triggerKicker(Kicker& machine) {
  while(!machine.terminated()) {
    machine.process_event(transitionKicker());
  }
  std::cout << "Terminate kicker machine\n";
}

void triggerGoalie(Goalie& machine) {
  while(!machine.terminated()) {
    machine.process_event(transitionGoalie());
  }
  std::cout << "Terminate goalie machine\n";
}

int main() {
  /*Kicker machineKicker;
  machineKicker.initiate();
  std::thread triggerEventKicker(triggerKicker, std::ref(machineKicker));
  triggerEventKicker.join();*/

  Goalie machineGoalie;
  machineGoalie.initiate();
  std::thread triggerEventGoalie(triggerGoalie, std::ref(machineGoalie));
  triggerEventGoalie.join();

  return 0;
}