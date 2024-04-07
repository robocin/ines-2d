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

 private:
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

    sc::result react(const transitionKicker& /*unused*/) {
      return transit<updateWorldModelKicker>();
    }
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
    return transit<goToBall>();
  }
};


struct goToBall : sc::state<goToBall, Kicker> {
 public:
  using reactions = sc::custom_reaction<transitionKicker>;

  explicit goToBall(my_context ctx) : my_base(ctx) { std::cout << "Going to ball position!\n"; }

  ~goToBall() override { std::cout << "Quitting go to ball\n"; }

  sc::result react(const transitionKicker& /*unused*/) {
    if (context<Kicker>().getKickable() == 1) {
      return transit<shoot>();
    }
    return transit<updateWorldModelKicker>();
  }
};

struct shoot : sc::state<shoot, Kicker> {
  using reactions = sc::custom_reaction<transitionKicker>;

  explicit shoot(my_context ctx) : my_base(ctx) { std::cout << "Entering shoot!\n"; }

  ~shoot() override { std::cout << "Quitting shoot\n"; }

  sc::result react(const transitionKicker& /*unused*/) {
    if (context<Kicker>().getCanShoot()) {
      return transit<updateWorldModelKicker>();
    }
    return transit<dribble>();
  }

};

struct dribble : sc::state<dribble, Kicker> {
  using reactions = sc::custom_reaction<transitionKicker>;

  explicit dribble(my_context ctx) : my_base(ctx) { std::cout << "Entering dribble!\n"; }

  ~dribble() override { std::cout << "Quitting dribble\n"; }

  sc::result react(const transitionKicker& /*unused*/) { return transit<updateWorldModelKicker>(); }
};