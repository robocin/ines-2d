#include "kicker_stm.h"

#include <boost/statechart/result.hpp>

InitialState::InitialState(my_context ctx) : my_base(ctx) {
  std::cout << "Update the kicker world model\n";
}

InitialState::~InitialState() { std::cout << "Kicker World model updated\n"; }

sc::result InitialState::react(const Transition& /*unused*/) {
  return transit<SUpdateWorldModel>();
}

/* ====================================================================================== */

SUpdateWorldModel::SUpdateWorldModel(my_context ctx) : my_base(ctx) {
  context<KickerStm>().getTimestamp()
      = context<KickerStm>().waitCycleTime(context<KickerStm>().getTimestamp());
  std::cout << "Updating the world model!\n";
}

SUpdateWorldModel::~SUpdateWorldModel() { std::cout << "World model updated\n"; }

sc::result SUpdateWorldModel::react(const Transition& /*unused*/) {
  switch (static_cast<int>(context<KickerStm>().getUpdatedWorldModel())) {
    case 0: return transit<SUpdateWorldModel>();
    case 1: return transit<J1>();
    default: return transit<UndefinedState>();
  }
}

/* ====================================================================================== */

J1::J1(my_context ctx) : my_base(ctx) { std::cout << "Entering junction J1\n"; }

J1::~J1() { std::cout << "Quitting junction j1\n"; }

sc::result J1::react(const Transition& /*unused*/) {
  switch (context<KickerStm>().getGamemode()) {
    case 0: return transit<FinalState>();
    case 1: return transit<SGoToBall>();
    default: return transit<UndefinedState>();
  }
}

/* ====================================================================================== */

FinalState::FinalState(my_context ctx) : my_base(ctx) {
  std::cout << "Entering kicker final state\n";
}

FinalState::~FinalState() { std::cout << "Finishing kicker machine\n"; }

sc::result FinalState::react(const Transition& /*unused*/) { return terminate(); }

/* ====================================================================================== */

SGoToBall::SGoToBall(my_context ctx) : my_base(ctx) { 
  std::cout << "Going to ball position!\n";
}

SGoToBall::~SGoToBall() { std::cout << "Quitting go to ball\n"; }

sc::result SGoToBall::react(const Transition& /*unused*/) { return transit<J2>(); }

/* ====================================================================================== */

J2::J2(my_context ctx) : my_base(ctx) { std::cout << "Entering junction J2\n"; }

J2::~J2() { std::cout << "Quitting junction J2\n"; }

sc::result J2::react(const Transition& /*unused*/) {
  switch (static_cast<int>(context<KickerStm>().getKickable())) {
    case 0: kickerStm::doMove(0.0); return transit<J4>();
    case 1: return transit<SShoot>();
    default: return transit<UndefinedState>();
  }
}

/* ====================================================================================== */

SShoot::SShoot(my_context ctx) : my_base(ctx) {
  std::cout << "Entering shoot!\n";
  context<KickerStm>().getCanShoot() = kickerStm::canShootToGoal();
}

SShoot::~SShoot() { std::cout << "Quitting shoot\n"; }

sc::result SShoot::react(const Transition& /*unused*/) { return transit<J3>(); }

/* ====================================================================================== */

J3::J3(my_context ctx) : my_base(ctx) { std::cout << "Entering junction J3\n"; }

J3::~J3() { std::cout << "Quitting J3\n"; }

sc::result J3::react(const Transition& /*unused*/) {
  switch (static_cast<int>(context<KickerStm>().getCanShoot())) {
    case 0: return transit<SDribble>();
    case 1: kickerStm::doShoot(); return transit<J4>();
    default: return transit<UndefinedState>();
  }
}

/* ====================================================================================== */

SDribble::SDribble(my_context ctx) : my_base(ctx) {
  std::cout << "Entering dribble!\n";
  kickerStm::doDribble();
}

SDribble::~SDribble() { std::cout << "Quitting dribble\n"; }

sc::result SDribble::react(const Transition& /*unused*/) {
  return transit<J4>();
}

/* ====================================================================================== */

J4::J4(my_context ctx) : my_base(ctx) { std::cout << "Entering junction J4\n"; }

J4::~J4() { std::cout << "Quitting junction J4\n"; }

sc::result J4::react(const Transition& /*unused*/) {
  context<KickerStm>().exec();
  return transit<SUpdateWorldModel>();
}

/* ====================================================================================== */

UndefinedState::UndefinedState(my_context ctx) : my_base(ctx) {
  std::cout << "Deadlock detected\n";
}

UndefinedState::~UndefinedState() = default;

sc::result UndefinedState::react(const Transition& /*unused*/) {
  return transit<UndefinedState>();
}

/* ====================================================================================== */