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

struct transitionKicker : sc::event<transitionKicker> {};
using sysClock = std::chrono::system_clock;
using timePoint = std::chrono::time_point<std::chrono::system_clock>;

struct Kicker;
struct initialStateKicker;
struct updateWorldModelKicker;
struct goToBall;
struct shoot;
struct dribble;
struct finalStateKicker;
struct undefinedStateKicker;

struct j1Kicker;
struct j2Kicker;
struct j3Kicker;
struct j4Kicker;

struct Kicker : sc::state_machine<Kicker, initialStateKicker> {
 public:
  Kicker() = default;
  ~Kicker() override = default;
  using timePoint = std::chrono::time_point<std::chrono::system_clock>;

  [[nodiscard]] int getWorldModel() const { return worldModel_; }
  int& getWorldModel() { return worldModel_; }
  [[nodiscard]] int getKickable() const { return kickable_; }
  int& getKickable() { return kickable_; }
  [[nodiscard]] bool getCanShoot() const { return canShoot_; }
  bool& getCanShoot() { return canShoot_; }
  [[nodiscard]] bool getUpdatedWorldModel() const { return updatedWorldModel_; }
  bool& getUpdatedWorldModel() { return updatedWorldModel_; }
  [[nodiscard]] int getGamemode() const { return gamemode_; }
  int& getGamemode() { return gamemode_; }
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
    this->gamemode_ = static_cast<int>(rng() % 2);
    this->worldModel_ = static_cast<int>(rng() % 2);
    this->kickable_ = static_cast<int>(rng() % 2);
    this->canShoot_ = static_cast<bool>(rng() % 2);
    this->updatedWorldModel_ = static_cast<bool>(rng() % 2);
  }

 private:
  int gamemode_{1};
  int worldModel_{1};
  int kickable_{1};
  bool canShoot_{true};
  bool updatedWorldModel_{true};
  timePoint lastTimestamp_{timePoint::min()};
};

struct initialStateKicker : sc::state<initialStateKicker, Kicker> {
 public:
  using reactions = sc::custom_reaction<transitionKicker>;
  explicit initialStateKicker(my_context ctx);
  ~initialStateKicker() override;
  sc::result react(const transitionKicker&);
};

struct updateWorldModelKicker : sc::state<updateWorldModelKicker, Kicker> {
 public:
  const long int iterationTime = 1000;
  using reactions = sc::custom_reaction<transitionKicker>;
  explicit updateWorldModelKicker(my_context ctx);
  ~updateWorldModelKicker() override;
  sc::result react(const transitionKicker&);
};

struct j1Kicker : sc::state<j1Kicker, Kicker> {
 public:
  using reactions = sc::custom_reaction<transitionKicker>;
  explicit j1Kicker(my_context ctx);
  ~j1Kicker() override;
  sc::result react(const transitionKicker&);
};

struct finalStateKicker : sc::state<finalStateKicker, Kicker> {
 public:
  using reactions = sc::custom_reaction<transitionKicker>;
  explicit finalStateKicker(my_context ctx);
  ~finalStateKicker() override;
  sc::result react(const transitionKicker&);
};

struct goToBall : sc::state<goToBall, Kicker> {
 public:
  using reactions = sc::custom_reaction<transitionKicker>;
  explicit goToBall(my_context ctx);
  ~goToBall() override;
  sc::result react(const transitionKicker&);
};

struct j2Kicker : sc::state<j2Kicker, Kicker> {
 public:
  using reactions = sc::custom_reaction<transitionKicker>;
  explicit j2Kicker(my_context ctx);
  ~j2Kicker() override;
  sc::result react(const transitionKicker&);
};

struct shoot : sc::state<shoot, Kicker> {
  using reactions = sc::custom_reaction<transitionKicker>;
  explicit shoot(my_context ctx);
  ~shoot() override;
  sc::result react(const transitionKicker&);
};

struct j3Kicker : sc::state<j3Kicker, Kicker> {
 public:
  using reactions = sc::custom_reaction<transitionKicker>;
  explicit j3Kicker(my_context ctx);
  ~j3Kicker() override;
  sc::result react(const transitionKicker&);
};

struct dribble : sc::state<dribble, Kicker> {
  using reactions = sc::custom_reaction<transitionKicker>;
  explicit dribble(my_context ctx);
  ~dribble() override;
  sc::result react(const transitionKicker&);
};

struct j4Kicker : sc::state<j4Kicker, Kicker> {
 public:
  using reactions = sc::custom_reaction<transitionKicker>;
  explicit j4Kicker(my_context ctx);
  ~j4Kicker() override;
  sc::result react(const transitionKicker&);
};

struct undefinedStateKicker : sc::state<undefinedStateKicker, Kicker> {
 public:
  using reactions = sc::custom_reaction<transitionKicker>;
  explicit undefinedStateKicker(my_context ctx);
  ~undefinedStateKicker() override;
  sc::result react(const transitionKicker&);
};

#endif