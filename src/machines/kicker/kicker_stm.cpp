#include "kicker_stm.h"

#include <boost/statechart/result.hpp>

InitialStateKicker::InitialStateKicker(my_context ctx) : my_base(ctx) {
  std::cout << "Update the kicker world model\n";
}

InitialStateKicker::~InitialStateKicker() { std::cout << "Kicker World model updated\n"; }

sc::result InitialStateKicker::react(const TransitionKicker& /*unused*/) {
  return transit<SUpdateWorldModelKicker>();
}

/* ====================================================================================== */

SUpdateWorldModelKicker::SUpdateWorldModelKicker(my_context ctx) : my_base(ctx) {
  context<KickerStm>().getTimestamp()
      = context<KickerStm>().waitCycleTime(context<KickerStm>().getTimestamp());
  std::cout << "Updating the world model!\n";
}

SUpdateWorldModelKicker::~SUpdateWorldModelKicker() { std::cout << "World model updated\n"; }

sc::result SUpdateWorldModelKicker::react(const TransitionKicker& /*unused*/) {
  switch (static_cast<int>(context<KickerStm>().getUpdatedWorldModel())) {
    case 0: return transit<SUpdateWorldModelKicker>();
    case 1: return transit<j1Kicker>();
    default: return transit<UndefinedStateKicker>();
  }
}

/* ====================================================================================== */

j1Kicker::j1Kicker(my_context ctx) : my_base(ctx) { std::cout << "Entering junction j1Kicker\n"; }

j1Kicker::~j1Kicker() { std::cout << "Quitting junction j1\n"; }

sc::result j1Kicker::react(const TransitionKicker& /*unused*/) {
  switch (context<KickerStm>().getGamemode()) {
    case 0: return transit<FinalStateKicker>();
    case 1: return transit<SGoToBall>();
    default: return transit<UndefinedStateKicker>();
  }
}

/* ====================================================================================== */

FinalStateKicker::FinalStateKicker(my_context ctx) : my_base(ctx) {
  std::cout << "Entering kicker final state\n";
}

FinalStateKicker::~FinalStateKicker() { std::cout << "Finishing kicker machine\n"; }

sc::result FinalStateKicker::react(const TransitionKicker& /*unused*/) { return terminate(); }

/* ====================================================================================== */

SGoToBall::SGoToBall(my_context ctx) : my_base(ctx) { std::cout << "Going to ball position!\n"; }

SGoToBall::~SGoToBall() { std::cout << "Quitting go to ball\n"; }

sc::result SGoToBall::react(const TransitionKicker& /*unused*/) { return transit<j2Kicker>(); }

/* ====================================================================================== */

j2Kicker::j2Kicker(my_context ctx) : my_base(ctx) { std::cout << "Entering junction j2Kicker\n"; }

j2Kicker::~j2Kicker() { std::cout << "Quitting junction j2Kicker\n"; }

sc::result j2Kicker::react(const TransitionKicker& /*unused*/) {
  switch (context<KickerStm>().getKickable()) {
    case 0: kickerStm::doMove(0.0); return transit<j4Kicker>();
    case 1: return transit<SShoot>();
    default: return transit<UndefinedStateKicker>();
  }
}

/* ====================================================================================== */

SShoot::SShoot(my_context ctx) : my_base(ctx) {
  std::cout << "Entering shoot!\n";
  context<KickerStm>().getCanShoot() = kickerStm::canShootToGoal();
}

SShoot::~SShoot() { std::cout << "Quitting shoot\n"; }

sc::result SShoot::react(const TransitionKicker& /*unused*/) { return transit<j3Kicker>(); }

/* ====================================================================================== */

j3Kicker::j3Kicker(my_context ctx) : my_base(ctx) { std::cout << "Entering junction j3Kicker\n"; }

j3Kicker::~j3Kicker() { std::cout << "Quitting j3Kicker\n"; }

sc::result j3Kicker::react(const TransitionKicker& /*unused*/) {
  switch (static_cast<int>(context<KickerStm>().getCanShoot())) {
    case 0: return transit<j4Kicker>();
    case 1: kickerStm::doShoot(); return transit<SDribble>();
    default: return transit<UndefinedStateKicker>();
  }
}

/* ====================================================================================== */

SDribble::SDribble(my_context ctx) : my_base(ctx) {
  std::cout << "Entering dribble!\n";
  kickerStm::doDribble();
}

SDribble::~SDribble() { std::cout << "Quitting dribble\n"; }

sc::result SDribble::react(const TransitionKicker& /*unused*/) {
  return transit<j4Kicker>();
}

/* ====================================================================================== */

j4Kicker::j4Kicker(my_context ctx) : my_base(ctx) { std::cout << "Entering junction j4Kicker\n"; }

j4Kicker::~j4Kicker() { std::cout << "Quitting junction j4Kicker\n"; }

sc::result j4Kicker::react(const TransitionKicker& /*unused*/) {
  context<KickerStm>().exec();
  return transit<SUpdateWorldModelKicker>();
}

/* ====================================================================================== */

UndefinedStateKicker::UndefinedStateKicker(my_context ctx) : my_base(ctx) {
  std::cout << "Deadlock detected\n";
}

UndefinedStateKicker::~UndefinedStateKicker() = default;

sc::result UndefinedStateKicker::react(const TransitionKicker& /*unused*/) {
  return transit<UndefinedStateKicker>();
}

/* ====================================================================================== */