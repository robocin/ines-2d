#include <iostream>

namespace goalieStm {
inline void doCatch() { std::cout << "Catch\n"; }
inline void doClearBall() { std::cout << "Clear Ball\n"; }
inline void doTackle() { std::cout << "Tackle\n"; }
inline void doBodyIntercept() { std::cout << "Intercept\n"; }
inline void doMove(double point) { std::cout << "Do move to " << point << "\n"; }
} // namespace goalieStm