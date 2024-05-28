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
#include <iostream>
#include <thread>
#include <unistd.h>

namespace sc = boost::statechart;

struct transitionKicker : sc::event<transitionKicker> {};

struct Kicker;
struct initialStateKicker;
struct updateWorldModelKicker;
struct goToBall;
struct shoot;
struct dribble;
struct finalStateKicker;
struct undefinedStateKicker;
struct exec1;

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
  [[nodiscard]] bool getExec() const { return exec_; }
  bool& getExec() { return exec_; }
  [[nodiscard]] int getGamemode() const { return gamemode_; }
  int& getGamemode() { return gamemode_; }
  [[nodiscard]] timePoint getTimestamp() const { return lastTimestamp_; }
  timePoint& getTimestamp() { return lastTimestamp_; }

 private:
  int gamemode_{1};
  int worldModel_{1};
  int kickable_{1};
  bool canShoot_{true};
  bool updatedWorldModel_{false};
  bool exec_{false};
  timePoint lastTimestamp_{timePoint::min()};
};

struct initialStateKicker : sc::state<initialStateKicker, Kicker> {
 public:
  using reactions = sc::custom_reaction<transitionKicker>;

  explicit initialStateKicker(my_context ctx) : my_base(ctx) {
    std::cout << "Updating the kicker world model!\n";
  }

  ~initialStateKicker() override { std::cout << "Kicker World model updated\n"; }

  sc::result react(const transitionKicker& /*unused*/) { return transit<updateWorldModelKicker>(); }
};

struct updateWorldModelKicker : sc::state<updateWorldModelKicker, Kicker> {
 public:
  const long int iterationTime = 1000;
  using reactions = sc::custom_reaction<transitionKicker>;
  using clock = std::chrono::system_clock;
  using timePoint = std::chrono::time_point<std::chrono::system_clock>;

  explicit updateWorldModelKicker(my_context ctx) : my_base(ctx) {
    auto currTime = context<Kicker>().getTimestamp();

    if (context<Kicker>().getTimestamp() != timePoint::min()) {
      auto execTime
          = std::chrono::duration_cast<std::chrono::milliseconds>(clock::now() - currTime).count();
      auto wait = iterationTime - execTime;
      auto duration = std::chrono::milliseconds(wait);
      while (clock::now() - currTime < duration) {
      }
    }

    context<Kicker>().getTimestamp() = clock::now();
    std::cout << "Updating the world model!\n";
  }

  ~updateWorldModelKicker() override { std::cout << "World model updated\n"; }

  sc::result react(const transitionKicker& /*unused*/) {
    switch (static_cast<int>(context<Kicker>().getUpdatedWorldModel())) {
      case 0: context<Kicker>().getExec() = true; return transit<exec1>(); // return transit<updateWorldModelKicker>();
      case 1: return transit<j1Kicker>();
      default: return transit<undefinedStateKicker>();
    }
  }
};

struct exec1 : sc::state<exec1, Kicker> {
 public:
  using reactions = sc::custom_reaction<transitionKicker>;
  explicit exec1(my_context ctx) : my_base(ctx) { std::cout << "Entering junction exec1\n"; }

  ~exec1() override { std::cout << "Quitting junction j1\n"; }

  sc::result react(const transitionKicker& /*unused*/) {
    return transit<updateWorldModelKicker>();
  }
};

struct j1Kicker : sc::state<j1Kicker, Kicker> {
 public:
  using reactions = sc::custom_reaction<transitionKicker>;
  explicit j1Kicker(my_context ctx) : my_base(ctx) { std::cout << "Entering junction j1Kicker\n"; }

  ~j1Kicker() override { std::cout << "Quitting junction j1\n"; }

  sc::result react(const transitionKicker& /*unused*/) {
    switch (context<Kicker>().getGamemode()) {
      case 0: return transit<finalStateKicker>();
      case 1: return transit<goToBall>();
      default: return transit<undefinedStateKicker>();
    }
  }
};

struct finalStateKicker : sc::state<finalStateKicker, Kicker> {
 public:
  using reactions = sc::custom_reaction<transitionKicker>;

  explicit finalStateKicker(my_context ctx) : my_base(ctx) {
    std::cout << "Entering kicker final state\n";
  }

  ~finalStateKicker() override { std::cout << "Finishing kicker machine\n"; }

  sc::result react(const transitionKicker& /*unused*/) { return terminate(); }
};

struct goToBall : sc::state<goToBall, Kicker> {
 public:
  using reactions = sc::custom_reaction<transitionKicker>;

  explicit goToBall(my_context ctx) : my_base(ctx) { std::cout << "Going to ball position!\n"; }

  ~goToBall() override { std::cout << "Quitting go to ball\n"; }

  sc::result react(const transitionKicker& /*unused*/) { return transit<j2Kicker>(); }
};

struct j2Kicker : sc::state<j2Kicker, Kicker> {
 public:
  using reactions = sc::custom_reaction<transitionKicker>;
  explicit j2Kicker(my_context ctx) : my_base(ctx) { std::cout << "Entering junction j2Kicker\n"; }

  ~j2Kicker() override { std::cout << "Quitting junction j2Kicker\n"; }

  sc::result react(const transitionKicker& /*unused*/) {
    switch (context<Kicker>().getKickable()) {
      case 0: kickerStm::doMove(/*ball=*/0.0); return transit<j4Kicker>();
      case 1: return transit<shoot>();
      default: return transit<undefinedStateKicker>();
    }
  }
};

struct shoot : sc::state<shoot, Kicker> {
  using reactions = sc::custom_reaction<transitionKicker>;

  explicit shoot(my_context ctx) : my_base(ctx) {
    std::cout << "Entering shoot!\n";
    context<Kicker>().getCanShoot() = kickerStm::canShootToGoal();
  }

  ~shoot() override { std::cout << "Quitting shoot\n"; }

  sc::result react(const transitionKicker& /*unused*/) { return transit<j3Kicker>(); }
};

struct j3Kicker : sc::state<j3Kicker, Kicker> {
 public:
  using reactions = sc::custom_reaction<transitionKicker>;
  explicit j3Kicker(my_context ctx) : my_base(ctx) { std::cout << "Entering junction j3Kicker\n"; }

  ~j3Kicker() override { std::cout << "Quitting junction j3Kicker\n"; }

  sc::result react(const transitionKicker& /*unused*/) {
    switch (static_cast<int>(context<Kicker>().getCanShoot())) {
      case 0: return transit<j4Kicker>();
      case 1: kickerStm::doShoot(); return transit<dribble>();
      default: return transit<undefinedStateKicker>();
    }
  }
};

struct dribble : sc::state<dribble, Kicker> {
  using reactions = sc::custom_reaction<transitionKicker>;

  explicit dribble(my_context ctx) : my_base(ctx) {
    std::cout << "Entering dribble!\n";
    kickerStm::doDribble();
  }

  ~dribble() override { std::cout << "Quitting dribble\n"; }

  sc::result react(const transitionKicker& /*unused*/) { return transit<updateWorldModelKicker>(); }
};

struct j4Kicker : sc::state<j4Kicker, Kicker> {
 public:
  using reactions = sc::custom_reaction<transitionKicker>;
  explicit j4Kicker(my_context ctx) : my_base(ctx) { std::cout << "Entering junction j4Kicker\n"; }

  ~j4Kicker() override { std::cout << "Quitting junction j4Kicker\n"; }

  sc::result react(const transitionKicker& /*unused*/) { return transit<updateWorldModelKicker>(); }
};

struct undefinedStateKicker : sc::state<undefinedStateKicker, Kicker> {
 public:
  using reactions = sc::custom_reaction<transitionKicker>;
  explicit undefinedStateKicker(my_context ctx) : my_base(ctx) {
    std::cout << "Deadlock detected\n";
  }

  ~undefinedStateKicker() override = default;

  sc::result react(const transitionKicker& /*unused*/) { return transit<undefinedStateKicker>(); }
};