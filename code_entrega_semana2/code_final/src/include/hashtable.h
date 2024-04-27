#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "tools.h"
#include "sequence.h"

template <class Key>
class Table {
 public:
  Table(unsigned table_size) { table_size_ = table_size; }
  virtual ~Table() {}
  virtual bool Search(const Key& key, int& index) const = 0;
  virtual bool Insert(const Key& key) = 0;
  virtual bool IsFull() const = 0;
  virtual std::ostream& Write(std::ostream& out) const = 0;
 protected:
  int table_size_;
};

template <class Key, class Container = StaticSequence<Key>>
class HashTable : public Table<Key> {
 public:
  HashTable(unsigned table_size, DisperseFunction<Key>& fd, ExplorationFunction<Key>& fe, unsigned block_size);
  virtual ~HashTable();
  bool Search(const Key& key, int& index) const;
  bool Insert(const Key& key);
  bool IsFull() const;
  std::ostream& Write(std::ostream& out) const;
 private:
  DisperseFunction<Key>* fd_ = nullptr;
  ExplorationFunction<Key>* fe_ = nullptr;
  Container** table_;
  int block_size_;
};

template <class Key>
class HashTable<Key, DynamicSequence<Key>> : public Table<Key> {
 public:
  HashTable(unsigned table_size, DisperseFunction<Key>& fd);
  virtual ~HashTable();
  bool Search(const Key& key, int& index) const;
  bool Insert(const Key& key);
  bool IsFull() const;
  std::ostream& Write(std::ostream& out) const;
 private:
  DisperseFunction<Key>* fd_ = nullptr;
  DynamicSequence<Key>** table_;
};

// ================================ HASH TABLE STATIC SEQUENCE ================================ //

template<class Key, class Container>
HashTable<Key, Container>::HashTable(unsigned table_size, DisperseFunction<Key>& fd, ExplorationFunction<Key>& fe, unsigned block_size) : Table<Key>(table_size) {
  this->table_size_ = table_size;
  fd_ = &fd;
  fe_ = &fe;
  table_ = new Container*[table_size];
  block_size_ = block_size;
  for (unsigned i = 0; i < table_size; ++i) {
    table_[i] = new Container(block_size);
  }
}

template<class Key, class Container>
HashTable<Key, Container>::~HashTable() {
  delete fd_;
  delete fe_;
  for (int i = 0; i < this->table_size_; ++i) {
    delete table_[i];
  }
  delete[] table_;
}

template<class Key, class Container>
bool HashTable<Key, Container>::Search(const Key& key, int& index) const {
  index = (*fd_)(key);
  if (!table_[index]->Search(key)) {
    if (table_[(*fd_)(key)]->IsFull()) {
      int attempt = 1;
      unsigned aux_index = ((*fd_)(key) + (*fe_)(key, attempt)) % this->table_size_;
      while (!table_[aux_index]->Search(key) && table_[aux_index]->IsFull()) {
        ++attempt;
        if (attempt > this->table_size_) {
          std::cout << "All possible indexes have been tried" << std::endl;
          return false;
        }
        aux_index = ((*fd_)(key) + (*fe_)(key, attempt)) % this->table_size_;
      }
      index = aux_index;
      return true;
    }
    else {
      return false;
    }
  }
  return true;
}

template<class Key, class Container>
bool HashTable<Key, Container>::Insert(const Key& key) {
  if (!table_[(*fd_)(key)]->Insert(key)) {
    std::cout << std::setw(4) << "Collision Detected!" << std::endl << std::endl;
    int attempt = 1;
    unsigned aux_index = ((*fd_)(key) + (*fe_)(key, attempt)) % this->table_size_;
    while (!table_[aux_index]->Insert(key)) {
      std::cout << std::setw(4) << "Collision Detected!" << std::endl << std::endl;
      ++attempt;
      if (attempt > this->table_size_) {
        std::cout << "All possible indexes have been tried" << std::endl << std::endl;
        return false;
      }
      aux_index = ((*fd_)(key) + (*fe_)(key, attempt)) % this->table_size_;
    }
    return true;
  }
  return true;
}

template<class Key, class Container>
bool HashTable<Key, Container>::IsFull() const {
  for (int i = 0; i < this->table_size_; ++i) {
    if (!table_[i]->IsFull()) return false;
  }
  return true;
}

template<class Key, class Container>
std::ostream& HashTable<Key, Container>::Write(std::ostream& out) const {
  for (int i = 0; i < this->table_size_; ++i) {
    std::cout << "Table[" << i << "]: ";
    table_[i]->Write(std::cout);
    std::cout << std::endl;
  }
  return out;
}

// ================================ HASH TABLE DYNAMIC SEQUENCE ================================ // 

template<class Key>
HashTable<Key, DynamicSequence<Key>>::HashTable(unsigned table_size, DisperseFunction<Key>& fd) : Table<Key>(table_size) {
  this->table_size_ = table_size;
  fd_ = &fd;
  table_ = new DynamicSequence<Key>*[table_size];
  for (int i = 0; i < this->table_size_; ++i) {
    table_[i] = new DynamicSequence<Key>();
  }
}

template<class Key>
HashTable<Key, DynamicSequence<Key>>::~HashTable() {
  delete fd_;
  for (int i = 0; i < this->table_size_; ++i) {
    delete table_[i];
  }
  delete[] table_;
}

template<class Key>
bool HashTable<Key, DynamicSequence<Key>>::Search(const Key& key, int& index) const {
  index = (*fd_)(key);
  return table_[(*fd_)(key)]->Search(key);
}

template<class Key>
bool HashTable<Key, DynamicSequence<Key>>::Insert(const Key& key) {
  unsigned index = (*fd_)(key);
  table_[index]->Insert(key);
  return true;
}

template<class Key>
bool HashTable<Key, DynamicSequence<Key>>::IsFull() const {
  std::cerr << "The table is never full because it is dynamic" << std::endl;
  return false;
}

template<class Key>
std::ostream& HashTable<Key, DynamicSequence<Key>>::Write(std::ostream& out) const {
  for (int i = 0; i < this->table_size_; ++i) {
    std::cout << "Table[" << i << "]: ";
    table_[i]->Write(std::cout);
    std::cout << std::endl;
  }
  return out;
}

#endif