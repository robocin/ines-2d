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

 private:
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
    std::cout << "Updating the world model!\n";
  }

  ~initialStateGoalie() override { std::cout << "World model updated\n"; }

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
    return transit<doCatch>();
  }
};

struct doCatch : sc::state<doCatch, Goalie> {
 public:
  using reactions = sc::custom_reaction<transitionGoalie>;

  explicit doCatch(my_context ctx) : my_base(ctx) {
    std::cout << "Going to catch\n";
    context<Goalie>().getInsideGoalieArea() = goalieStm::isInOurPenaltyArea(/*ballPos=*/0.0);
  }

  ~doCatch() override { std::cout << "Quitting catch state\n"; }

  sc::result react(const transitionGoalie& /*unused*/) {
    if (context<Goalie>().getCatchable() && context<Goalie>().getInsideGoalieArea()) {
      goalieStm::doCatch();
      return transit<updateWorldModelGoalie>();
    }
    return transit<clearBall>();
  }
};

struct clearBall : sc::state<clearBall, Goalie> {
 public:
  using reactions = sc::custom_reaction<transitionGoalie>;

  explicit clearBall(my_context ctx) : my_base(ctx) { std::cout << "entering clear ball state\n"; }

  ~clearBall() override { std::cout << "Quitting clear ball\n"; }

  sc::result react(const transitionGoalie& /*unused*/) {
    if (context<Goalie>().getKickable()) {
      goalieStm::doClearBall();
      return transit<updateWorldModelGoalie>();
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

  sc::result react(const transitionGoalie& /*unused*/) {
    if (context<Goalie>().getTacklePossible()) {
      goalieStm::doTackle();
      return transit<updateWorldModelGoalie>();
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

  sc::result react(const transitionGoalie& /*unused*/) {
    if (context<Goalie>().getBodyInterceptAct()) {
      goalieStm::doBodyIntercept();
    } else {
      goalieStm::doMove(context<Goalie>().getBlockPoint());
    }
    return transit<updateWorldModelGoalie>();
  }
};