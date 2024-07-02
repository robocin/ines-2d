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

struct TransitionKicker : sc::event<TransitionKicker> {};
using sysClock = std::chrono::system_clock;
using timePoint = std::chrono::time_point<std::chrono::system_clock>;

struct Kicker;
struct InitialStateKicker;
struct SUpdateWorldModelKicker;
struct SGoToBall;
struct SShoot;
struct SDribble;
struct FinalStateKicker;
struct UndefinedStateKicker;

struct j1Kicker;
struct j2Kicker;
struct j3Kicker;
struct j4Kicker;

struct worldModel {
  int gameMode;
  bool canShoot;
};

struct KickerStm : sc::state_machine<KickerStm, InitialStateKicker> {
 public:
  KickerStm() = default;
  ~KickerStm() override = default;
  using timePoint = std::chrono::time_point<std::chrono::system_clock>;

  [[nodiscard]] worldModel getWorldModel() const { return WorldModel; }
  worldModel& getWorldModel() { return WorldModel; }
  [[nodiscard]] int getKickable() const { return isKickable; }
  int& getKickable() { return isKickable; }
  [[nodiscard]] bool getCanShoot() const { return WorldModel.canShoot; }
  bool& getCanShoot() { return WorldModel.canShoot; }
  [[nodiscard]] bool getUpdatedWorldModel() const { return UpdateWorldModel; }
  bool& getUpdatedWorldModel() { return UpdateWorldModel; }
  [[nodiscard]] int getGamemode() const { return WorldModel.gameMode; }
  int& getGamemode() { return WorldModel.gameMode; }
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
    this->WorldModel.gameMode = static_cast<int>(rng() % 2);
    this->isKickable = static_cast<int>(rng() % 2);
    this->WorldModel.canShoot = static_cast<bool>(rng() % 2);
    this->UpdateWorldModel = static_cast<bool>(rng() % 2);
  }

 private:
  worldModel WorldModel;
  int isKickable;
  bool UpdateWorldModel;
  timePoint lastTimestamp_{timePoint::min()};
};

struct InitialStateKicker : sc::state<InitialStateKicker, KickerStm> {
 public:
  using reactions = sc::custom_reaction<TransitionKicker>;
  explicit InitialStateKicker(my_context ctx);
  ~InitialStateKicker() override;
  sc::result react(const TransitionKicker&);
};

struct SUpdateWorldModelKicker : sc::state<SUpdateWorldModelKicker, KickerStm> {
 public:
  const int iterationTime = 1000;
  using reactions = sc::custom_reaction<TransitionKicker>;
  explicit SUpdateWorldModelKicker(my_context ctx);
  ~SUpdateWorldModelKicker() override;
  sc::result react(const TransitionKicker&);
};

struct j1Kicker : sc::state<j1Kicker, KickerStm> {
 public:
  using reactions = sc::custom_reaction<TransitionKicker>;
  explicit j1Kicker(my_context ctx);
  ~j1Kicker() override;
  sc::result react(const TransitionKicker&);
};

struct FinalStateKicker : sc::state<FinalStateKicker, KickerStm> {
 public:
  using reactions = sc::custom_reaction<TransitionKicker>;
  explicit FinalStateKicker(my_context ctx);
  ~FinalStateKicker() override;
  sc::result react(const TransitionKicker&);
};

struct SGoToBall : sc::state<SGoToBall, KickerStm> {
 public:
  using reactions = sc::custom_reaction<TransitionKicker>;
  explicit SGoToBall(my_context ctx);
  ~SGoToBall() override;
  sc::result react(const TransitionKicker&);
};

struct j2Kicker : sc::state<j2Kicker, KickerStm> {
 public:
  using reactions = sc::custom_reaction<TransitionKicker>;
  explicit j2Kicker(my_context ctx);
  ~j2Kicker() override;
  sc::result react(const TransitionKicker&);
};

struct SShoot : sc::state<SShoot, KickerStm> {
  using reactions = sc::custom_reaction<TransitionKicker>;
  explicit SShoot(my_context ctx);
  ~SShoot() override;
  sc::result react(const TransitionKicker&);
};

struct j3Kicker : sc::state<j3Kicker, KickerStm> {
 public:
  using reactions = sc::custom_reaction<TransitionKicker>;
  explicit j3Kicker(my_context ctx);
  ~j3Kicker() override;
  sc::result react(const TransitionKicker&);
};

struct SDribble : sc::state<SDribble, KickerStm> {
  using reactions = sc::custom_reaction<TransitionKicker>;
  explicit SDribble(my_context ctx);
  ~SDribble() override;
  sc::result react(const TransitionKicker&);
};

struct j4Kicker : sc::state<j4Kicker, KickerStm> {
 public:
  using reactions = sc::custom_reaction<TransitionKicker>;
  explicit j4Kicker(my_context ctx);
  ~j4Kicker() override;
  sc::result react(const TransitionKicker&);
};

struct UndefinedStateKicker : sc::state<UndefinedStateKicker, KickerStm> {
 public:
  using reactions = sc::custom_reaction<TransitionKicker>;
  explicit UndefinedStateKicker(my_context ctx);
  ~UndefinedStateKicker() override;
  sc::result react(const TransitionKicker&);
};

#endif