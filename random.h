#ifndef RANDOM_H
#define RANDOM_H

#include <vector>

// randInt(a) returns a random integer in the interval [0, a]
int randInt(int a);

// getRandomElement(vec) returns a random element from the vector
template <typename T> T getRandomElement(const std::vector<T> &vec) {
  if (vec.empty()) throw std::out_of_range("cannot randomly select from empty vector");
  int index = vec.size() - 1;
  return vec[index];
}

#endif
