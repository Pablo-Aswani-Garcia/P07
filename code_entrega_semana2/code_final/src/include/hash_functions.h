#ifndef HASH_FUNCTIONS_H
#define HASH_FUNCTIONS_H

#include "tools.h"

template <class Key>
class DisperseFunction {
 public:
  DisperseFunction(unsigned table_size) : table_size_(table_size) {}
  virtual ~DisperseFunction() {}
  virtual unsigned operator()(const Key& key) const = 0;
 protected:
  int table_size_;
};

template <class Key>
class ModFunction : public DisperseFunction<Key> {
 public:
  ModFunction(unsigned table_size) : DisperseFunction<Key>(table_size) {}
  unsigned operator()(const Key& key) const { return long(key) % this->table_size_; }
};

template <class Key>
class SumFunction : public DisperseFunction<Key> {
 public:
  SumFunction(unsigned table_size) : DisperseFunction<Key>(table_size) {}
  unsigned operator()(const Key& key) const {
    unsigned copy_key = long(key), summatory = 0;
    while (copy_key != 0) {
      summatory += copy_key % 10;
      copy_key /= 10;
    }
    return summatory % this->table_size_;
  }
};

template <class Key>
class RandFunction : public DisperseFunction<Key> {
 public:
  RandFunction(unsigned table_size) : DisperseFunction<Key>(table_size) {}
  unsigned operator()(const Key& key) const { srand(long(key)); return rand() % this->table_size_; }
};

template <class Key>
class ExplorationFunction {
 public:
  ExplorationFunction(unsigned table_size) : table_size_(table_size) {}
  virtual ~ExplorationFunction() {}
  virtual unsigned operator()(const Key& key, unsigned attempt) const = 0;
 protected:
  int table_size_;
};

template <class Key>
class LinearFunction : public ExplorationFunction<Key> {
 public:
  LinearFunction(unsigned table_size) : ExplorationFunction<Key>(table_size) {}
  // The operator simply returns the attempt as a new position
  unsigned operator()(const Key& key, unsigned attempt) const { return attempt; }
};

template <class Key>
class QuadraticFunction : public ExplorationFunction<Key> {
 public:
  QuadraticFunction(unsigned table_size) : ExplorationFunction<Key>(table_size) {}
  // The operator returns a position based by the attempt squared
  unsigned operator()(const Key& key, unsigned attempt) const { return attempt * attempt; }
};

template <class Key>
class DoubleDisperseFunction : public ExplorationFunction<Key> {
 public:
  DoubleDisperseFunction(unsigned table_size, DisperseFunction<Key>* fd) : ExplorationFunction<Key>(table_size) { fd_ = fd; }
  virtual ~DoubleDisperseFunction() { delete fd_; }
  // The operator uses a disperse function to get a new position by multiplying it to attempt
  unsigned operator()(const Key& key, unsigned attempt) const { return attempt * (*fd_)(key); }
 private:
  DisperseFunction<Key>* fd_;
};

template <class Key>
class RedispersionFunction : public ExplorationFunction<Key> {
 public:
  RedispersionFunction(unsigned table_size) : ExplorationFunction<Key>(table_size) {}
  // The operator uses a random function that iterates attempt times to get a new position
  unsigned operator()(const Key& key, unsigned attempt) const { 
    srand(long(key));
    if (attempt == 1) return rand();
    for (unsigned i = 0; i < attempt - 2; ++i) {
      rand();
    }
    return rand();
  }
};

#endif