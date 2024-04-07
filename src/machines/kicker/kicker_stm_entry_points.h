#include <iostream>

namespace kickerStm {
inline bool canShootToGoal() {
  std::cout << "Checks if the kicker can shoot to goal\n";
  return true;
}
inline void doDribble() { std::cout << "Dribble\n"; }
} // namespace kickerStm