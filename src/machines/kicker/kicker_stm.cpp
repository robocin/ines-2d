#include "kicker_stm.h"

#include <boost/statechart/result.hpp>

initialStateKicker::initialStateKicker(my_context ctx) : my_base(ctx) {
  std::cout << "Update the kicker world model\n";
}

initialStateKicker::~initialStateKicker() { std::cout << "Kicker World model updated\n"; }

sc::result initialStateKicker::react(const transitionKicker& /*unused*/) {
  return transit<updateWorldModelKicker>();
}

/* ====================================================================================== */

updateWorldModelKicker::updateWorldModelKicker(my_context ctx) : my_base(ctx) {
  context<Kicker>().getTimestamp()
      = context<Kicker>().waitCycleTime(context<Kicker>().getTimestamp());
  std::cout << "Updating the world model!\n";
}

updateWorldModelKicker::~updateWorldModelKicker() { std::cout << "World model updated\n"; }

sc::result updateWorldModelKicker::react(const transitionKicker& /*unused*/) {
  switch (static_cast<int>(context<Kicker>().getUpdatedWorldModel())) {
    case 0: return transit<updateWorldModelKicker>();
    case 1: return transit<j1Kicker>();
    default: return transit<undefinedStateKicker>();
  }
}

/* ====================================================================================== */

j1Kicker::j1Kicker(my_context ctx) : my_base(ctx) { std::cout << "Entering junction j1Kicker\n"; }

j1Kicker::~j1Kicker() { std::cout << "Quitting junction j1\n"; }

sc::result j1Kicker::react(const transitionKicker& /*unused*/) {
  switch (context<Kicker>().getGamemode()) {
    case 0: return transit<finalStateKicker>();
    case 1: return transit<goToBall>();
    default: return transit<undefinedStateKicker>();
  }
}

/* ====================================================================================== */

finalStateKicker::finalStateKicker(my_context ctx) : my_base(ctx) {
  std::cout << "Entering kicker final state\n";
}

finalStateKicker::~finalStateKicker() { std::cout << "Finishing kicker machine\n"; }

sc::result finalStateKicker::react(const transitionKicker& /*unused*/) { return terminate(); }

/* ====================================================================================== */

goToBall::goToBall(my_context ctx) : my_base(ctx) { std::cout << "Going to ball position!\n"; }

goToBall::~goToBall() { std::cout << "Quitting go to ball\n"; }

sc::result goToBall::react(const transitionKicker& /*unused*/) { return transit<j2Kicker>(); }

/* ====================================================================================== */

j2Kicker::j2Kicker(my_context ctx) : my_base(ctx) { std::cout << "Entering junction j2Kicker\n"; }

j2Kicker::~j2Kicker() { std::cout << "Quitting junction j2Kicker\n"; }

sc::result j2Kicker::react(const transitionKicker& /*unused*/) {
  switch (context<Kicker>().getKickable()) {
    case 0: kickerStm::doMove(0.0); return transit<j4Kicker>();
    case 1: return transit<shoot>();
    default: return transit<undefinedStateKicker>();
  }
}

/* ====================================================================================== */

shoot::shoot(my_context ctx) : my_base(ctx) {
  std::cout << "Entering shoot!\n";
  context<Kicker>().getCanShoot() = kickerStm::canShootToGoal();
}

shoot::~shoot() { std::cout << "Quitting shoot\n"; }

sc::result shoot::react(const transitionKicker& /*unused*/) { return transit<j3Kicker>(); }

/* ====================================================================================== */

j3Kicker::j3Kicker(my_context ctx) : my_base(ctx) { std::cout << "Entering junction j3Kicker\n"; }

j3Kicker::~j3Kicker() { std::cout << "Quitting j3Kicker\n"; }

sc::result j3Kicker::react(const transitionKicker& /*unused*/) {
  switch (static_cast<int>(context<Kicker>().getCanShoot())) {
    case 0: return transit<j4Kicker>();
    case 1: kickerStm::doShoot(); return transit<dribble>();
    default: return transit<undefinedStateKicker>();
  }
}

/* ====================================================================================== */

dribble::dribble(my_context ctx) : my_base(ctx) {
  std::cout << "Entering dribble!\n";
  kickerStm::doDribble();
}

dribble::~dribble() { std::cout << "Quitting dribble\n"; }

sc::result dribble::react(const transitionKicker& /*unused*/) {
  return transit<j4Kicker>();
}

/* ====================================================================================== */

j4Kicker::j4Kicker(my_context ctx) : my_base(ctx) { std::cout << "Entering junction j4Kicker\n"; }

j4Kicker::~j4Kicker() { std::cout << "Quitting junction j4Kicker\n"; }

sc::result j4Kicker::react(const transitionKicker& /*unused*/) {
  context<Kicker>().exec();
  return transit<updateWorldModelKicker>();
}

/* ====================================================================================== */

undefinedStateKicker::undefinedStateKicker(my_context ctx) : my_base(ctx) {
  std::cout << "Deadlock detected\n";
}

undefinedStateKicker::~undefinedStateKicker() = default;

sc::result undefinedStateKicker::react(const transitionKicker& /*unused*/) {
  return transit<undefinedStateKicker>();
}

/* ====================================================================================== */