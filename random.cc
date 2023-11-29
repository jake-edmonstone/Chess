#include <cstdlib>
#include <ctime>

int randInt(int a) {
  srand(time(0));
  return rand() % (a + 1);
}
