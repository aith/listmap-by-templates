// $Id: listmap.h,v 1.24 2020-10-22 16:49:46-07 - - $

#ifndef __LISTMAP_H__
#define __LISTMAP_H__

#include "debug.h"
#include "xless.h"
#include "xpair.h"

#define SHOW_LINK(FLAG,PTR) { \
           DEBUGB (FLAG, #PTR << "=" << PTR \
                   << ": next=" << PTR->next \
                   << ", prev=" << PTR->prev); \
        }

template <typename key_t, typename mapped_t, class less_t=xless<key_t>>
class listmap {
   public:
      using key_type = key_t;
      using mapped_type = mapped_t;
      using value_type = xpair<const key_type, mapped_type>;
   private:
      less_t less;
      struct node;
      struct link {
         // boundary tag
         node* next{};
         node* prev{};
         // link ctor taking pointers
         link (node* next_, node* prev_): next(next_), prev(prev_){}
      };
      struct node: link {
         value_type value{};
         // node ctor (leveragng the link ctor) that takes the 
         // boundary tag pointers and a value.
         node (node* next_, node* prev_, const value_type& value_):
               link (next_, prev_), value(value_){}
      };
      node* anchor() { return static_cast<node*> (&anchor_); }
      // ?? creates an anchor boundary tag
      // The list itself
      link anchor_ {anchor(), anchor()};
   public:
      class iterator;
      // default ctor
      listmap(){};
      // copy ctor
      listmap (const listmap&);
      // copy assignment
      listmap& operator= (const listmap&);
      // dtor
      ~listmap();
      // insert a value type and return an iterator to that
      iterator insert (const value_type&);
      // update a new node's neighor pointers
      void insertBounds(node* newnode);
      void removeBounds(node* newnode);
      // find a key type and return iterator to it
      iterator find (const key_type&);
      // erase an iterator and return iterator following it
      iterator erase (iterator position);
      iterator begin() { return anchor()->next; }
      iterator end() { return anchor(); }
      void print(iterator position); 
      // checks if list itself (the anchor) is empty
      bool empty() const { return anchor_.next == &anchor_; }
      operator bool() const { return not empty(); }
};


template <typename key_t, typename mapped_t, class less_t>
class listmap<key_t,mapped_t,less_t>::iterator {
   private:
      // let the list map access all of the iterator's elements
      friend class listmap<key_t,mapped_t,less_t>; 
      /* where
            the actual pointer object, the address it points to.
      */
      listmap<key_t,mapped_t,less_t>::node* where {nullptr}; 
      /* ctor
            takes a node pointer and sets the where to it
      */
      iterator (node* where_): where(where_){};
   public:
      iterator() {}
      value_type& operator*() {
         SHOW_LINK ('b', where);
         return where->value;
      }
      value_type* operator->() { return &(where->value); }
      iterator& operator++() { where = where->next; return *this; }
      iterator& operator--() { where = where->prev; return *this; }
      // TODO
      void erase();
      bool operator== (const iterator& that) const {
         return this->where == that.where;
      }
      bool operator!= (const iterator& that) const {
         return this->where != that.where;
      }
};

#include "listmap.tcc"
#endif

