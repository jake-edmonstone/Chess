#include <cstdlib>
#include <ctime>
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

template <typename T> T getRandomElement(const vector<T> &vec) {
  if (vec.empty()) throw out_of_range("cannot randomly select from empty vector");
  int index = vec.size() - 1;
  return vec[index];
}