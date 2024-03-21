#include <thread>
#include <unistd.h>
#include "machines/kicker_stm.h"

void trigger(Kicker& machine) {
  constexpr int ktransitions = 20;

  for (int i = 0; i < ktransitions; i++) {
    machine.process_event(transition());
  }
}

int main() {
  Kicker machine;
  machine.initiate();
  std::thread triggerEvent(trigger, std::ref(machine));
  triggerEvent.join();
  return 0;
}