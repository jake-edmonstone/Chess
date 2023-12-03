#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include "random.h"

using namespace std;

int randInt(int a) {
  static bool seeded = false;
  if (!seeded) {
    srand(time(0));
    seeded = true;
  }
  return rand() % (a + 1);
}
