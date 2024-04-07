#include <iostream>

namespace goalieStm {
inline bool isInOurPenaltyArea(double ballPos) {
  std::cout << "Checking if ball is inside the goalie area\n";
  std::cout << "Ball pos is " << ballPos << "\n";
  return true;
}
inline bool isTacklePossible(double tackleProbability) {
  std::cout << "Checking tackle\n";
  return tackleProbability >= 0.3;
}
inline bool checkBody(double ball) {
  std::cout << "Checking body\n";
  return ball >= 1.0;
}
inline int calculateBlockPoint(double ball, double ballPos) {
  std::cout << "Calculating block point\n";
  std::cout << ball << " " << ballPos << "\n";
  return 1;
}
} // namespace goalieStm