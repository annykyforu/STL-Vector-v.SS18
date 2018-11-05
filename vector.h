
//  vector.cpp
//  class Vector
//
//  Created by annykyforu in SS2018
//  Copyright © 2018 PRG 2. All rights reserved.
//

#ifndef vector_h
#define vector_h

#include <iostream>
#include <initializer_list>
#include <cstddef>

using namespace std;
using std::ptrdiff_t;

static constexpr size_t min_size{10};

template <typename T>
class Vector
{
public:
  using value_type = T;
  using size_type = size_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using difference_type = ptrdiff_t;
  
  ///////////*----- ConstIterator -----*///////////
  class ConstIterator
  {
  public:
    using iterator_category = std::forward_iterator_tag;
    using const_value_type = Vector::value_type;
    using const_reference = Vector::const_reference;
    using const_pointer = Vector::const_pointer;
    using difference_type = Vector::difference_type;
  private:
    const_pointer ptr;
    const Vector* vec;
  public:
    ConstIterator (const_pointer, const Vector*);
    ConstIterator& operator++ (); /*--- Increment ---*/
    ConstIterator operator++ (int); /*--- Postfix Increment ---*/
    friend bool operator== (const ConstIterator& lop, const ConstIterator& rop) { return lop.ptr == rop.ptr; }
    bool operator!= (const ConstIterator&) const;
    const_reference operator* () const;
    const_pointer operator-> () const;
    difference_type operator- (const ConstIterator&); /*--- operator- Implementation ---*/
  };
  
  ///////////*----- Iterator -----*///////////
  class Iterator
  {
  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = Vector::value_type;
    using reference = Vector::reference;
    using pointer = Vector::pointer;
    using difference_type = Vector::difference_type;
  private:
    pointer ptr;
    const Vector* vec;
  public:
    Iterator (pointer, const Vector*);
    Iterator operator++ (); /*--- Increment ---*/
    Iterator operator++ (int); /*--- Postfix Increment ---*/
    friend bool operator== (const Iterator& lop, const Iterator& rop) { return lop.ptr == rop.ptr; }
    bool operator!= (const Iterator&) const;
    reference operator* ();
    pointer operator-> ();
    difference_type operator- (const Iterator&); /*--- operator- Implementation ---*/
    operator ConstIterator() const;
  };
  
  using iterator = Iterator;
  using const_iterator = ConstIterator;
  
  ///////////*----- class Vector Implementation -----*///////////
private:
  size_type sz;
  size_type max_size;
  pointer values;
  void new_buf(size_type size)
  {
    pointer buf { new value_type[sz] };
    for (size_t i{0}; i < this->sz; ++i)
      buf[i] = values[i];
    delete[] values;
    values = buf;
    max_size = sz;
  }
  
public:
  ///////////*----- Constructors -----*///////////
  Vector();
  Vector(size_type);
  Vector(initializer_list<value_type>);
  Vector(const Vector&);
  ~Vector();
  
  ///////////*----- Methods -----*///////////
  size_type size();
  const size_type size() const;
  bool empty();
  void clear();
  void reserve(size_type);
  void shrink_to_fit();
  void push_back(value_type);
  void pop_back();

  ///////////----- Operators -----///////////
  Vector operator= (const Vector&);
  reference operator[] (size_type);
  const_reference operator[] (size_type) const;
  
  ///////////*----- Iterators -----*///////////
  iterator begin();
  iterator end();
  iterator begin() const;
  iterator end() const;
  const_iterator cbegin();
  const_iterator cend();
  
  ///////////*----- Erase, Insert from VO -----*///////////
  iterator erase (iterator);
  iterator insert (iterator, const_reference);
  const_iterator erase (const_iterator);
  const_iterator insert (const_iterator, const_reference);
  
  friend ostream& operator<< (ostream& os, const Vector& v)
  {
    if (v.size() == 0)
      os << "Nothing to display - vector is empty!";
    os << "[";
    for (size_t i {0}; i < v.size(); ++i)
    {
      os << v[i];
      if (i < v.size()-1) os << ", ";
    }
    os << "]";
    return os;
  }
};


template <typename T>
Vector<T>::Vector() : sz{0}, max_size{0}, values{nullptr} {}

template <typename T>
Vector<T>::Vector(size_type new_size) : sz{0}, max_size{new_size > min_size ? new_size : min_size}, values{new value_type[max_size]} {}

template <typename T>
Vector<T>::Vector(initializer_list<value_type> l)
{
  values = l.size() > 0 ? new value_type[l.size()]: nullptr;
  max_size = l.size();
  sz = 0;
  for (const auto& value : l)
    values[sz++] = value;
}

template <typename T>
Vector<T>::Vector(const Vector& copy)
{
  max_size = copy.max_size;
  sz = copy.sz;
  values = new value_type[copy.sz];
  for (size_t i {0}; i < copy.size(); ++i)
    values[i] = copy.values[i];
}

template <typename T>
Vector<T>::~Vector(){ delete[] values; }

///////////----- Vector Methods -----///////////
template <typename T>
typename Vector<T>::size_type Vector<T>::size() { return sz; }

template <typename T>
const typename Vector<T>::size_type Vector<T>::size() const { return sz; }

template <typename T>
bool Vector<T>::empty() { return sz == 0; }

template <typename T>
void Vector<T>::clear() {sz = 0;}

template <typename T>
void Vector<T>::reserve(size_type new_size)
{
  if (new_size <= max_size) return;
  if (new_size < sz)
    throw runtime_error("Error Reserve! Buffer too small.");
  pointer new_buf = new value_type[new_size];
  for (size_type i{0}; i < sz; ++i)
    new_buf[i] = values[i];
  delete[] values;
  max_size = new_size;
  values = new_buf;
}

template <typename T>
void Vector<T>::shrink_to_fit() { new_buf(sz); }

template <typename T>
void Vector<T>::push_back(value_type val)
{
  if (sz == max_size) {reserve(max_size+1);}
  values[sz++] = val;
}

template <typename T>
void Vector<T>::pop_back()
{
  if (sz == 0) return;
  --sz;
}

///////////----- Vector Methods -----///////////
template <typename T>
Vector<T> Vector<T>::operator= (const Vector& v)
{
  std::swap(max_size, v.max_size);
  std::swap(sz, v.sz);
  std::swap(values, v.values);
  return *this;
}

template <typename T>
typename Vector<T>::reference Vector<T>::operator[] (size_type index)
{
  if (index >= sz) throw runtime_error("Index is Out of Bounds!");
  return values[index];
}

template <typename T>
typename Vector<T>::const_reference Vector<T>::operator[] (size_type index) const
{
  if (index >= sz) throw runtime_error("Index is Out of Bounds!");
  return values[index];
}

///////////----- Vector Iterators -----///////////
template <typename T>
typename Vector<T>::iterator Vector<T>::begin() { return Iterator(values, this); }

template <typename T>
typename Vector<T>::iterator Vector<T>::end() { return Iterator(values+sz, this); }

template <typename T>
typename Vector<T>::iterator Vector<T>::begin() const { return Iterator(values, this); }

template <typename T>
typename Vector<T>::iterator Vector<T>::end() const { return Iterator(values+sz, this); }

template <typename T>
typename Vector<T>::const_iterator Vector<T>::cbegin() { return ConstIterator(values, this); }

template <typename T>
typename Vector<T>::const_iterator Vector<T>::cend() { return ConstIterator(values+sz, this); }

///////////----- Vector Erase, Insert from VO -----///////////
template <typename T>
typename Vector<T>::iterator Vector<T>::erase (iterator pos)
{
  auto diff = pos - begin();
  if (diff < 0 || static_cast<size_t>(diff) >= sz)
    throw runtime_error("Error Erase! Iterator is out of bounds.");
  
  size_t current { static_cast<size_t>(diff) };
  for (; current < sz-1; ++current)
    values[current] = values[current + 1];
  --sz;
  return iterator{values+current, this};
}

template <typename T>
typename Vector<T>::iterator Vector<T>::insert (iterator pos, typename Vector<T>::const_reference val)
{
  auto diff = pos - begin();
  if (diff < 0 || static_cast<size_t>(diff) >= sz)
    throw runtime_error("Error Insert! Iterator is out of bounds.");
  
  size_t current { static_cast<size_t>(diff) };
  if (sz >= max_size)
    reserve(max_size*2);  //max_sz*2+10, wenn Ihr Container max_sz==0 erlaubt
  
  for (size_t i{sz}; i-->current;)
    values[i+1] = values[i];
  values[current] = val;
  ++sz;
  return iterator{values+current, this};
}

template <typename T>
typename Vector<T>::const_iterator Vector<T>::erase (const_iterator pos)
{
  auto diff = pos - cbegin();
  if (diff < 0 || static_cast<size_t>(diff) >= sz)
    throw runtime_error("Error Erase! Iterator is out of bounds.");
  
  size_t current { static_cast<size_t>(diff) };
  for (; current < sz-1; ++current)
    values[current] = values[current + 1];
  --sz;
  return const_iterator{values+current, this};
}

template <typename T>
typename Vector<T>::const_iterator Vector<T>::insert (const_iterator pos, const_reference val)
{
  auto diff = pos - cbegin();
  if (diff < 0 || static_cast<size_t>(diff) >= sz)
    throw runtime_error("Error Insert! Iterator is out of bounds.");
  
  size_t current { static_cast<size_t>(diff) };
  if (sz >= max_size)
    reserve(max_size*2+10);  //max_sz*2+10, wenn Ihr Container max_sz==0 erlaubt
  
  for (size_t i{sz}; i-->current;)
    values[i+1] = values[i];
  values[current] = val;
  ++sz;
  return const_iterator{values+current, this};
}



///////////----- ConstIterator -----///////////
template <typename T>
Vector<T>::ConstIterator::ConstIterator (const_pointer Ptr, const Vector* Vec) : ptr{Ptr}, vec{Vec} {}

template <typename T> //--- Increment ---
typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator++ ()
{
  if (ptr >= (*vec).values + (*vec).sz)
    throw runtime_error("Error Increment! Reached end of Vector.");
  ++ptr;
  return *this;
}

template <typename T> //--- Postfix Increment ---
typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator++ (int)
{
  if (ptr >= (*vec).values + (*vec).sz)
    throw runtime_error("Error Increment! Reached end of Vector.");
  const_iterator temp(*this);
  operator++();
  return temp;
}

template <typename T>
bool Vector<T>::ConstIterator::operator!= (const ConstIterator& rop) const {return ptr != rop.ptr;}

template <typename T>
typename Vector<T>::ConstIterator::const_reference Vector<T>::ConstIterator::operator* () const
{
  if (ptr < (*vec).values || ptr >= (*vec).values+(*vec).sz)
    throw runtime_error("Error Reference! Out of Bounds.");
  return *ptr;
}

template <typename T>
typename Vector<T>::ConstIterator::const_pointer Vector<T>::ConstIterator::operator-> () const { return ptr; }

template <typename T> //--- operator- Implementation ---
typename Vector<T>::ConstIterator::difference_type Vector<T>::ConstIterator::operator- (const ConstIterator& rop) { return ptr-rop.ptr; }



///////////----- Iterator -----///////////
template <typename T>
Vector<T>::Iterator::Iterator (pointer Ptr, const Vector* Vec) : ptr{Ptr}, vec{Vec} {}

template <typename T>
typename Vector<T>::Iterator Vector<T>::Iterator::operator++ ()
{
  if (ptr >= (*vec).values+(*vec).sz)
    throw runtime_error("Error Increment! Reached end of Vector.");
  ++ptr;
  return *this;
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::Iterator::operator++ (int)
{
  if (ptr >= (*vec).values+(*vec).sz)
    throw runtime_error("Error Increment! Reached end of Vector.");
  iterator temp(*this);
  ++ptr;
  return temp;
}

template <typename T>
bool Vector<T>::Iterator::operator!= (const Iterator& rop) const { return ptr != rop.ptr; }

template <typename T>
typename Vector<T>::Iterator::reference Vector<T>::Iterator::operator* ()
{
  if (ptr < (*vec).values || ptr >= (*vec).values+(*vec).sz)
    throw runtime_error("Error Reference! Out of Bounds.");
  return *ptr;
}

template <typename T>
typename Vector<T>::Iterator::pointer Vector<T>::Iterator::operator-> () { return ptr; }

template <typename T>
typename Vector<T>::Iterator::difference_type Vector<T>::Iterator::operator- (const Iterator& rop) { return ptr-rop.ptr; }

template <typename T> //--- implizite Typumwandlung von iterator nach const_iterator ---
Vector<T>::Iterator::operator ConstIterator() const  { return const_iterator {ptr, vec}; }


#endif /*vector_h*/
