#include "goalie_stm_entry_points.h"
#include "goalie_stm_transitions.h"

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

struct transitionGoalie : sc::event<transitionGoalie> {};
struct Goalie;
struct initialStateGoalie;
struct updateWorldModelGoalie;
struct doCatch;
struct clearBall;
struct doTackle;
struct bodyIntercept;
struct finalStateGoalie;

struct j1Goalie;
struct j2Goalie;
struct j3Goalie;
struct j4Goalie;
struct j5Goalie;
struct j6Goalie;

struct Goalie : sc::state_machine<Goalie, initialStateGoalie> {
 public:
  Goalie() = default;
  ~Goalie() override = default;

  [[nodiscard]] int getWorldModel() const { return worldModel_; }
  int& getWorldModel() { return worldModel_; }
  [[nodiscard]] bool getUpdatedWorldModel() const { return updatedWorldModel_; }
  bool& getUpdatedWorldModel() { return updatedWorldModel_; }
  [[nodiscard]] bool getCatchable() const { return catchable_; }
  bool& getCatchable() { return catchable_; }
  [[nodiscard]] bool getInsideGoalieArea() const { return insideGoalieArea_; }
  bool& getInsideGoalieArea() { return insideGoalieArea_; }
  [[nodiscard]] bool getKickable() const { return kickable_; }
  bool& getKickable() { return kickable_; }
  [[nodiscard]] bool getTacklePossible() const { return tacklePossible_; }
  bool& getTacklePossible() { return tacklePossible_; }
  [[nodiscard]] bool getBodyInterceptAct() const { return bodyInterceptAct_; }
  bool& getBodyInterceptAct() { return bodyInterceptAct_; }
  [[nodiscard]] double getBlockPoint() const { return blockPoint_; }
  double& getBlockPoint() { return blockPoint_; }
  [[nodiscard]] int getGamemode() const { return gamemode_; }
  int& getGamemode() { return gamemode_; }

 private:
  int gamemode_{2};
  int worldModel_{1};
  bool updatedWorldModel_{true};
  bool catchable_{true};
  bool insideGoalieArea_{true};
  bool kickable_{true};
  bool tacklePossible_{true};
  bool bodyInterceptAct_{true};
  double blockPoint_{2.0};
};

struct initialStateGoalie : sc::state<initialStateGoalie, Goalie> {
 public:
  using reactions = sc::custom_reaction<transitionGoalie>;

  explicit initialStateGoalie(my_context ctx) : my_base(ctx) {
    std::cout << "Entering initial state!\n";
  }

  ~initialStateGoalie() override { std::cout << "Quitting initial state\n"; }

  sc::result react(const transitionGoalie& /*unused*/) { return transit<updateWorldModelGoalie>(); }
};

struct updateWorldModelGoalie : sc::state<updateWorldModelGoalie, Goalie> {
 public:
  using reactions = sc::custom_reaction<transitionGoalie>;

  explicit updateWorldModelGoalie(my_context ctx) : my_base(ctx) {
    std::cout << "Updating the world model!\n";
  }

  ~updateWorldModelGoalie() override { std::cout << "World model updated\n"; }

  sc::result react(const transitionGoalie& /*unused*/) {
    if (!context<Goalie>().getUpdatedWorldModel()) {
      return transit<updateWorldModelGoalie>();
    }
    return transit<j1Goalie>();
  }
};

struct j1Goalie : sc::state<j1Goalie, Goalie> {
 public:
  using reactions = sc::custom_reaction<transitionGoalie>;
  explicit j1Goalie(my_context ctx) : my_base(ctx) { std::cout << "Entering junction j1Goalie\n"; }

  ~j1Goalie() override { std::cout << "Quitting junction j1Goalie\n"; }

  sc::result react(const transitionGoalie& /*unused*/) {
    if (context<Goalie>().getGamemode() == 1) {
      return transit<doCatch>();
    }
    return transit<finalStateGoalie>();
  }
};

struct finalStateGoalie : sc::state<finalStateGoalie, Goalie> {
 public:
  using reactions = sc::custom_reaction<transitionGoalie>;
  explicit finalStateGoalie(my_context ctx) : my_base(ctx) {
    std::cout << "Entering final state\n";
  }

  ~finalStateGoalie() override { std::cout << "Ending goalie machine\n"; }

  sc::result react(const transitionGoalie& /*unused*/) { return terminate(); }
};

struct doCatch : sc::state<doCatch, Goalie> {
 public:
  using reactions = sc::custom_reaction<transitionGoalie>;

  explicit doCatch(my_context ctx) : my_base(ctx) {
    std::cout << "Going to catch\n";
    context<Goalie>().getInsideGoalieArea() = goalieStm::isInOurPenaltyArea(/*ballPos=*/0.0);
  }

  ~doCatch() override { std::cout << "Quitting catch state\n"; }

  sc::result react(const transitionGoalie& /*unused*/) { return transit<j2Goalie>(); }
};

struct j2Goalie : sc::state<j2Goalie, Goalie> {
 public:
  using reactions = sc::custom_reaction<transitionGoalie>;
  explicit j2Goalie(my_context ctx) : my_base(ctx) { std::cout << "Entering junction j2Goalie\n"; }

  ~j2Goalie() override { std::cout << "Quitting junction j2Goalie\n"; }

  sc::result react(const transitionGoalie& /*unused*/) {
    if (context<Goalie>().getCatchable() && context<Goalie>().getInsideGoalieArea()) {
      goalieStm::doCatch();
      return transit<j6Goalie>();
    }
    return transit<clearBall>();
  }
};

struct clearBall : sc::state<clearBall, Goalie> {
 public:
  using reactions = sc::custom_reaction<transitionGoalie>;

  explicit clearBall(my_context ctx) : my_base(ctx) { std::cout << "entering clear ball state\n"; }

  ~clearBall() override { std::cout << "Quitting clear ball\n"; }

  sc::result react(const transitionGoalie& /*unused*/) { return transit<j3Goalie>(); }
};

struct j3Goalie : sc::state<j3Goalie, Goalie> {
 public:
  using reactions = sc::custom_reaction<transitionGoalie>;
  explicit j3Goalie(my_context ctx) : my_base(ctx) { std::cout << "Entering junction j3Goalie\n"; }

  ~j3Goalie() override { std::cout << "Quitting junction j3Goalie\n"; }

  sc::result react(const transitionGoalie& /*unused*/) {
    if (context<Goalie>().getKickable()) {
      goalieStm::doClearBall();
      return transit<j6Goalie>();
    }
    return transit<doTackle>();
  }
};

struct doTackle : sc::state<doTackle, Goalie> {
 public:
  using reactions = sc::custom_reaction<transitionGoalie>;

  explicit doTackle(my_context ctx) : my_base(ctx) {
    std::cout << "Entering do tackle state\n";
    context<Goalie>().getTacklePossible() = goalieStm::isTacklePossible(/*tackleProbability=*/0.3);
  }

  ~doTackle() override { std::cout << "Quitting do tackle\n"; }

  sc::result react(const transitionGoalie& /*unused*/) { return transit<j4Goalie>(); }
};

struct j4Goalie : sc::state<j4Goalie, Goalie> {
 public:
  using reactions = sc::custom_reaction<transitionGoalie>;
  explicit j4Goalie(my_context ctx) : my_base(ctx) { std::cout << "Entering junction j4Goalie\n"; }

  ~j4Goalie() override { std::cout << "Quitting junction j4Goalie\n"; }

  sc::result react(const transitionGoalie& /*unused*/) {
    if (context<Goalie>().getTacklePossible()) {
      goalieStm::doTackle();
      return transit<j6Goalie>();
    }
    return transit<bodyIntercept>();
  }
};

struct bodyIntercept : sc::state<bodyIntercept, Goalie> {
 public:
  using reactions = sc::custom_reaction<transitionGoalie>;

  explicit bodyIntercept(my_context ctx) : my_base(ctx) {
    std::cout << "Entering body intercept\n";
    context<Goalie>().getBodyInterceptAct() = goalieStm::checkBody(/*ball=*/0.0);
    context<Goalie>().getBlockPoint()
        = goalieStm::calculateBlockPoint(/*ball=*/1.0, /*ballPos=*/1.0);
  }

  ~bodyIntercept() override { std::cout << "Quitting body intercept\n"; }

  sc::result react(const transitionGoalie& /*unused*/) { return transit<j5Goalie>(); }
};

struct j5Goalie : sc::state<j5Goalie, Goalie> {
 public:
  using reactions = sc::custom_reaction<transitionGoalie>;
  explicit j5Goalie(my_context ctx) : my_base(ctx) { std::cout << "Entering junction j5Goalie\n"; }

  ~j5Goalie() override { std::cout << "Quitting junction j5Goalie\n"; }

  sc::result react(const transitionGoalie& /*unused*/) {
    if (context<Goalie>().getBodyInterceptAct()) {
      goalieStm::doBodyIntercept();
    }

    if (!context<Goalie>().getBodyInterceptAct()) {
      goalieStm::doMove(context<Goalie>().getBlockPoint());
    }

    return transit<j6Goalie>();
  }
};

struct j6Goalie : sc::state<j6Goalie, Goalie> {
 public:
  using reactions = sc::custom_reaction<transitionGoalie>;
  explicit j6Goalie(my_context ctx) : my_base(ctx) { std::cout << "Entering junction j6Goalie\n"; }

  ~j6Goalie() override { std::cout << "Quitting junction j6Goalie\n"; }

  sc::result react(const transitionGoalie& /*unused*/) { return transit<updateWorldModelGoalie>(); }
};
