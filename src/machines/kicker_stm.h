#include <boost/statechart/custom_reaction.hpp>
#include <boost/statechart/event.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/transition.hpp>
#include <iostream>
#include <thread>
#include <unistd.h>

namespace sc = boost::statechart;

struct transition : sc::event<transition> {};

struct Kicker;
struct updateWorldModel;
struct goToBall;
struct shoot;
struct dribble;

struct Kicker : sc::state_machine<Kicker, updateWorldModel> {
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

struct updateWorldModel : sc::state<updateWorldModel, Kicker> {
 public:
  using reactions = sc::custom_reaction<transition>;

  explicit updateWorldModel(my_context ctx) : my_base(ctx) {
    std::cout << "Updating the world model!\n";
  }

  ~updateWorldModel() override { std::cout << "World model updated\n"; }

  sc::result react(const transition& /*unused*/) {
    if (!context<Kicker>().getUpdatedWorldModel()) {
      return transit<updateWorldModel>();
    }
    return transit<goToBall>();
  }
};


struct goToBall : sc::state<goToBall, Kicker> {
 public:
  using reactions = sc::custom_reaction<transition>;

  explicit goToBall(my_context ctx) : my_base(ctx) { std::cout << "Going to ball position!\n"; }

  ~goToBall() override { std::cout << "Quitting go to ball\n"; }

  sc::result react(const transition& /*unused*/) {
    if (context<Kicker>().getKickable() == 1) {
      return transit<shoot>();
    }
    return transit<updateWorldModel>();
  }
};

struct shoot : sc::state<shoot, Kicker> {
  using reactions = sc::custom_reaction<transition>;

  explicit shoot(my_context ctx) : my_base(ctx) { std::cout << "Entering shoot!\n"; }

  ~shoot() override { std::cout << "Quitting shoot\n"; }

  sc::result react(const transition& /*unused*/) {
    if (context<Kicker>().getCanShoot()) {
      return transit<updateWorldModel>();
    }
    return transit<dribble>();
  }

};

struct dribble : sc::state<dribble, Kicker> {
  using reactions = sc::custom_reaction<transition>;

  explicit dribble(my_context ctx) : my_base(ctx) { std::cout << "Entering dribble!\n"; }

  ~dribble() override { std::cout << "Quitting dribble\n"; }

  sc::result react(const transition& /*unused*/) { return transit<updateWorldModel>(); }
};