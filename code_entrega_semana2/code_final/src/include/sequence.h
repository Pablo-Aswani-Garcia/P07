#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "hash_functions.h"

template <class Key>
class Sequence {
 public:
  virtual bool Search(const Key& key) const = 0;
  virtual bool Insert(const Key& key) = 0;
  virtual bool Delete(const Key& key) = 0;
  virtual Key GetKey(const int& index) const = 0;
  virtual std::ostream& Write(std::ostream& out) const = 0;
};

template<class Key> 
class DynamicSequence: public Sequence<Key> {
 public:
  DynamicSequence() {}
  virtual ~DynamicSequence();
  bool Search(const Key& key) const;
  bool Insert(const Key& key);
  bool Delete(const Key& key);
  Key GetKey(const int& index) const { return *block_[index]; }
  int GetSize() const { return block_.size(); }
  std::ostream& Write(std::ostream& out) const;
 private:
  std::vector<Key*> block_;
};

template<class Key> 
class StaticSequence: public Sequence<Key> {
 public:
  StaticSequence(const int& block_size);
  virtual ~StaticSequence();
  bool Search(const Key& key) const;
  bool Insert(const Key& key);
  bool Delete(const Key& key);
  virtual bool IsFull() const;
  Key GetKey(const int& index) const {
    if (block_[index] == nullptr) return Book();
    return *block_[index]; 
  }
  std::ostream& Write(std::ostream& out) const;
 private:
  int block_size_;
  int top_ = 0;
  Key** block_;
};

// ================================ DYNAMIC SEQUENCE ================================ //

/** @brief Destructor of the DynamicSequence class */
template<class Key>
DynamicSequence<Key>::~DynamicSequence() {
  for (unsigned i = 0; i < block_.size(); ++i) {
    delete block_[i];
  }
}

/** @brief Searchs a key in the sequence
 *  @param[in] key. The key to search.
 *  @return True if the key is in the sequence, false otherwise.
 */
template<class Key>
bool DynamicSequence<Key>::Search(const Key& key) const {
  for (unsigned i = 0; i < block_.size(); ++i) {
    if (*block_[i] == key) return true;
  }
  return false;
}

/** @brief Inserts a key in the sequence
 *  @param[in] key. The key to insert.
 *  @return True if the key has been inserted, false otherwise.
 */
template<class Key>
bool DynamicSequence<Key>::Insert(const Key& key) {
  block_.push_back(new Key(key));
  return true;
}

template <class Key>
bool DynamicSequence<Key>::Delete(const Key& key) {
  for (unsigned i = 0; i < block_.size(); ++i) {
    if (*block_[i] == key) {
      block_.erase(block_.begin() + i);
      return true;
    }
  }
  return false;
}

/** @brief Writes the key in the sequence
 *  @param[in] out. The output stream.
 *  @param[in] index. The index of the key.
 *  @return The output stream.
 */
template <class Key>
std::ostream& DynamicSequence<Key>::Write(std::ostream& out) const {
  for (unsigned i = 0; i < block_.size(); ++i) {
    if (block_[i] != nullptr) {
      out << std::string(*block_[i]) << " | ";
    }
  }
  return out;
}

// ================================ STATIC SEQUENCE ================================ //

/** @brief Constructor of the StaticSequence class
 *  @param[in] block_size. The size of the sequence.
 */
template<class Key>
StaticSequence<Key>::StaticSequence(const int& block_size) {
  block_size_ = block_size;
  block_ = new Key*[block_size];
  for (int i = 0; i < block_size; ++i) {
    block_[i] = nullptr;
  }
}

/** @brief Destructor of the StaticSequence class */
template<class Key>
StaticSequence<Key>::~StaticSequence() { 
  for (int i = 0; i < block_size_; ++i) {
    delete block_[i];
  }
  delete[] block_; 
}

/** @brief Searchs a key in the sequence
 *  @param[in] key. The key to search.
 *  @return True if the key is in the sequence, false otherwise.
 */
template<class Key>
bool StaticSequence<Key>::Search(const Key& key) const {
  for (int i = 0; i < block_size_; ++i) {
    if (block_[i] != nullptr && long(*block_[i]) == long(key)) return true;
  }
  return false;
}

/** @brief Inserts a key in the sequence
 *  @param[in] key. The key to insert.
 *  @return True if the key has been inserted, false otherwise.
 */
template<class Key>
bool StaticSequence<Key>::Insert(const Key& key) {
  if (IsFull()) {
    return false;
  }
  block_[top_] = new Key(key);
  ++top_;
  return true;
}

template <class Key>
bool StaticSequence<Key>::Delete(const Key& key) {
  for (int i = 0; i < block_size_; ++i) {
    if (block_[i] != nullptr && long(*block_[i]) == long(key)) {
      delete block_[i];
      block_[i] = nullptr;
      return true;
    }
  }
  return false;
}

/** @brief Checks if the sequence is full
 *  @return True if the sequence is full, false otherwise.
 */
template<class Key>
bool StaticSequence<Key>::IsFull() const { 
  for (int i = 0; i < block_size_; ++i) {
    if (block_[i] == nullptr) return false;
  }
  return true;
}

/** @brief Writes the key in the sequence
 *  @param[in] out. The output stream.
 *  @param[in] index. The index of the key.
 *  @return The output stream.
 */
template <class Key>
std::ostream& StaticSequence<Key>::Write(std::ostream& out) const {
  for (int i = 0; i < block_size_; ++i) {
    if (block_[i] != nullptr) {
      out << std::string(*block_[i]) << " | ";
    }
  }
  return out;
}

#endif