#ifndef KICKER_STM_H
#define KICKER_STM_H

#include "kicker_stm_entry_points.h"
#include "kicker_stm_transitions.h"

#include <boost/statechart/custom_reaction.hpp>
#include <boost/statechart/event.hpp>
#include <boost/statechart/result.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/transition.hpp>
#include <chrono>
#include <ctime>
#include <random>
#include <unistd.h>

namespace sc = boost::statechart;

struct Transition : sc::event<Transition> {};
using sysClock = std::chrono::system_clock;
using timePoint = std::chrono::time_point<std::chrono::system_clock>;

struct Kicker;
struct InitialState;
struct SUpdateWorldModel;
struct SGoToBall;
struct SShoot;
struct SDribble;
struct FinalState;
struct UndefinedState;

struct J1;
struct J2;
struct J3;
struct J4;

struct worldModel {
  int gameMode;
  bool canShoot;
};

struct KickerStm : sc::state_machine<KickerStm, InitialState> {
 public:
  KickerStm() = default;
  ~KickerStm() override = default;
  using timePoint = std::chrono::time_point<std::chrono::system_clock>;

  [[nodiscard]] worldModel getWorldModel() const { return wm; }
  worldModel& getWorldModel() { return wm; }
  [[nodiscard]] bool getKickable() const { return isKickable; }
  bool& getKickable() { return isKickable; }
  [[nodiscard]] bool getCanShoot() const { return wm.canShoot; }
  bool& getCanShoot() { return wm.canShoot; }
  [[nodiscard]] bool getUpdatedWorldModel() const { return UpdateWorldModel; }
  bool& getUpdatedWorldModel() { return UpdateWorldModel; }
  [[nodiscard]] int getGamemode() const { return wm.gameMode; }
  int& getGamemode() { return wm.gameMode; }
  [[nodiscard]] timePoint getTimestamp() const { return lastTimestamp_; }
  timePoint& getTimestamp() { return lastTimestamp_; }

  timePoint waitCycleTime(timePoint time) {
    const int iterationTime = 1000;
    auto currTime = time;

    if (time != timePoint::min()) {
      auto execTime
          = std::chrono::duration_cast<std::chrono::milliseconds>(sysClock::now() - currTime)
                .count();
      auto waitTime = iterationTime - execTime;
      auto waitDuration = std::chrono::milliseconds(waitTime);
      while (sysClock::now() - currTime < waitDuration) {
      }
    }

    return sysClock::now();
  }

  void exec() {
    std::cout << "Start Exec\n";
    writeOutputs();
    this->lastTimestamp_ = waitCycleTime(this->lastTimestamp_);
    readInputs();
  }

  void writeOutputs() { std::cout << "Writing outputs\n"; }

  void readInputs() {
    std::cout << "Reading inputs\n";
    std::random_device rng;
    this->wm.gameMode = static_cast<int>(rng() % 2);
    this->isKickable = static_cast<bool>(rng() % 2);
    this->wm.canShoot = static_cast<bool>(rng() % 2);
    this->UpdateWorldModel = static_cast<bool>(rng() % 2);
  }

 private:
  worldModel wm;
  bool isKickable;
  bool UpdateWorldModel;
  timePoint lastTimestamp_{timePoint::min()};
};

struct InitialState : sc::state<InitialState, KickerStm> {
 public:
  using reactions = sc::custom_reaction<Transition>;
  explicit InitialState(my_context ctx);
  ~InitialState() override;
  sc::result react(const Transition&);
};

struct SUpdateWorldModel : sc::state<SUpdateWorldModel, KickerStm> {
 public:
  const int iterationTime = 1000;
  using reactions = sc::custom_reaction<Transition>;
  explicit SUpdateWorldModel(my_context ctx);
  ~SUpdateWorldModel() override;
  sc::result react(const Transition&);
};

struct J1 : sc::state<J1, KickerStm> {
 public:
  using reactions = sc::custom_reaction<Transition>;
  explicit J1(my_context ctx);
  ~J1() override;
  sc::result react(const Transition&);
};

struct FinalState : sc::state<FinalState, KickerStm> {
 public:
  using reactions = sc::custom_reaction<Transition>;
  explicit FinalState(my_context ctx);
  ~FinalState() override;
  sc::result react(const Transition&);
};

struct SGoToBall : sc::state<SGoToBall, KickerStm> {
 public:
  using reactions = sc::custom_reaction<Transition>;
  explicit SGoToBall(my_context ctx);
  ~SGoToBall() override;
  sc::result react(const Transition&);
};

struct J2 : sc::state<J2, KickerStm> {
 public:
  using reactions = sc::custom_reaction<Transition>;
  explicit J2(my_context ctx);
  ~J2() override;
  sc::result react(const Transition&);
};

struct SShoot : sc::state<SShoot, KickerStm> {
  using reactions = sc::custom_reaction<Transition>;
  explicit SShoot(my_context ctx);
  ~SShoot() override;
  sc::result react(const Transition&);
};

struct J3 : sc::state<J3, KickerStm> {
 public:
  using reactions = sc::custom_reaction<Transition>;
  explicit J3(my_context ctx);
  ~J3() override;
  sc::result react(const Transition&);
};

struct SDribble : sc::state<SDribble, KickerStm> {
  using reactions = sc::custom_reaction<Transition>;
  explicit SDribble(my_context ctx);
  ~SDribble() override;
  sc::result react(const Transition&);
};

struct J4 : sc::state<J4, KickerStm> {
 public:
  using reactions = sc::custom_reaction<Transition>;
  explicit J4(my_context ctx);
  ~J4() override;
  sc::result react(const Transition&);
};

struct UndefinedState : sc::state<UndefinedState, KickerStm> {
 public:
  using reactions = sc::custom_reaction<Transition>;
  explicit UndefinedState(my_context ctx);
  ~UndefinedState() override;
  sc::result react(const Transition&);
};

#endif