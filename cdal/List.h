#ifndef _LIST_H_
#define _LIST_H_
#include <cstdlib>
#include<iostream>
#include <iterator>
namespace cop3530{

    template<typename E>
    class List{
    public:
        virtual void insert(E element, size_t position) = 0;
        virtual void push_back(E element) = 0;
        virtual void push_front(E element) = 0;
        virtual E replace(E element, size_t position) = 0;
        virtual E remove(size_t position) = 0;
        virtual E pop_back(void) = 0;
        virtual E pop_front(void) = 0;
        virtual E& item_at(size_t position) = 0;
        virtual E& peek_back(void) = 0;
        virtual E& peek_front(void) = 0;
        virtual bool is_empty(void) = 0;
        virtual bool is_full(void) = 0;
        virtual size_t length(void) = 0;
        virtual void clear(void) = 0;
        virtual bool contains(E element, bool (*equals_function)(const E&,const E&)) = 0;
        virtual void print(std::ostream& stream_name) = 0;
        virtual E* contents(void) = 0;

        virtual ~List(){};

    };
}
#endif
