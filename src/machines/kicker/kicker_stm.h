#include "kicker_stm_entry_points.h"
#include "kicker_stm_transitions.h"

#include <boost/statechart/custom_reaction.hpp>
#include <boost/statechart/event.hpp>
#include <boost/statechart/result.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/transition.hpp>
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

struct j1Kicker;
struct j2Kicker;
struct j3Kicker;
struct j4Kicker;

struct Kicker : sc::state_machine<Kicker, initialStateKicker> {
 public:
  Kicker() = default;
  ~Kicker() override = default;

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

 private:
  int gamemode_{2};
  int worldModel_{1};
  int kickable_{1};
  bool canShoot_{true};
  bool updatedWorldModel_{true};
};

struct initialStateKicker : sc::state<initialStateKicker, Kicker> {
 public:
  using reactions = sc::custom_reaction<transitionKicker>;

  explicit initialStateKicker(my_context ctx) : my_base(ctx) {
    std::cout << "Updating the world model!\n";
  }

  ~initialStateKicker() override { std::cout << "World model updated\n"; }

  sc::result react(const transitionKicker& /*unused*/) { return transit<updateWorldModelKicker>(); }
};

struct updateWorldModelKicker : sc::state<updateWorldModelKicker, Kicker> {
 public:
  using reactions = sc::custom_reaction<transitionKicker>;

  explicit updateWorldModelKicker(my_context ctx) : my_base(ctx) {
    std::cout << "Updating the world model!\n";
  }

  ~updateWorldModelKicker() override { std::cout << "World model updated\n"; }

  sc::result react(const transitionKicker& /*unused*/) {
    if (!context<Kicker>().getUpdatedWorldModel()) {
      return transit<updateWorldModelKicker>();
    }
    return transit<j1Kicker>();
  }
};
struct j1Kicker : sc::state<j1Kicker, Kicker> {
 public:
  using reactions = sc::custom_reaction<transitionKicker>;
  explicit j1Kicker(my_context ctx) : my_base(ctx) { std::cout << "Entering junction j1Kicker\n"; }

  ~j1Kicker() override { std::cout << "Quitting junction j1\n"; }

  sc::result react(const transitionKicker& /*unused*/) {
    if (context<Kicker>().getGamemode() == 1) {
      return transit<goToBall>();
    }
    return transit<finalStateKicker>();
  }
};

struct finalStateKicker : sc::state<finalStateKicker, Kicker> {
 public:
  using reactions = sc::custom_reaction<transitionKicker>;

  explicit finalStateKicker(my_context ctx) : my_base(ctx) { std::cout << "Entering kicker final state\n"; }

  ~finalStateKicker() override { std::cout << "Finishing kicker machine\n"; }

  sc::result react(const transitionKicker& /*unused*/) { 
    return terminate();
  }
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
    if (context<Kicker>().getKickable() == 1) {
      return transit<shoot>();
    }
    kickerStm::doMove(/*ball=*/0.0);
    return transit<j4Kicker>();
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
    if (context<Kicker>().getCanShoot()) {
      kickerStm::doShoot();
      return transit<j4Kicker>();
    }
    return transit<dribble>();
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