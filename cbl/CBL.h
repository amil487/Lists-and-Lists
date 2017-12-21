#ifndef _CBL_H_
#define _CBL_H_
#include "List.h"
#include <stdexcept>

namespace cop3530{

    template <typename E>
    class CBL : public List <E>{
    public:
        CBL(size_t size){                         //constructor and destructor
            list = new E[size];
            arraySize = size;
            head = -1;
            tail = 0;
            start = size;

            for(size_t i = 0; i < size; i++)
            {
                list[i] = 0;
            }
        }
        CBL(){
            list = new E[50];
            arraySize = 50;
            head = -1;
            tail = 0;
            start = 50;
        }
        ~CBL() override {
            clear();
            delete [] list;
        }

        CBL(const CBL &src){          //copy constructor
            list = new E[src.arraySize];
            arraySize = src.arraySize;
            tail = 0;
            head = -1;
            start = src.start;

            for(size_t i = 0, j = src.head; i < ((src.arraySize - src.head + src.tail + 1) % src.arraySize); i++ ,j++)
            {
                push_back(src.list[j % src.arraySize]);
            }
        }

        CBL &operator=(const CBL &src){       //copy-assignment operator
            if(this != &src){
                delete [] list;

                list = new E[src.arraySize];
                arraySize = src.arraySize;
                tail = 0;
                head = -1;
                start = src.start;

                for(size_t i = 0, j = src.head; i < ((src.arraySize - src.head + src.tail + 1) % src.arraySize); i++ ,j++)
                {
                    push_back(src.list[j % src.arraySize]);
                }
            }
            return *this;
        }

        CBL(CBL&& src){           //move constructor
            list = src.list;        //steal src's stuff
            tail = src.tail;
            head = src.head;
            arraySize = src.arraySize;
            start = src.start;

            src.list = nullptr;     //leave src with nothing
            src.tail = 0;
            src.head = -1;
            src.arraySize = 0;
            src.start = 0;
        }

        CBL &operator=(CBL&& src){
            if(this != &src){
                delete [] list;

                list = src.list;        //steal src's stuff
                tail = src.tail;
                head = src.head;
                arraySize = src.arraySize;
                start = src.start;

                src.list = nullptr;     //leave src with nothing
                src.tail = 0;
                src.head = -1;
                src.arraySize = 0;
                src.start = 0;
            }
            return *this;
        }

        void insert(E element,size_t position) override;        //function declarations
        void push_back(E element) override;
        void push_front(E element) override;
        E replace(E element, size_t position) override;
        E remove(size_t position) override;
        E pop_back(void) override;
        E pop_front(void) override;
        E& item_at(size_t position) override;
        E& peek_back(void) override;
        E& peek_front(void) override;
        bool is_empty(void) override;
        bool is_full(void) override;
        size_t length(void) override;
        void clear(void) override;
        bool contains(E element, bool (*equals_function)(const E&,const E&)) override;
        void print(std::ostream& stream_name) override;
        E* contents(void) override;
        void resizer();

    private:
        E * list= nullptr;
        size_t tail;
        size_t head;
        size_t arraySize;
        size_t start;
//********************************Iterator class************************************************************//

    public:
        template <typename DataT>
        class CBL_Iter {

        public:
            // type aliases required for C++ iterator compatibility
            using value_type = E;
            using reference = E&;
            using pointer = E*;
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::forward_iterator_tag;

            // type aliases for prettier code
            using self_type = CBL_Iter;
            using self_reference = CBL_Iter &;

        private:
            E * here;
            size_t take;

        public:
            explicit CBL_Iter(pointer start = nullptr, size_t position = 0) : here(start), take(position){}

            CBL_Iter(const CBL_Iter &src) : here(src.here),take(src.take) {}

            reference operator*() const {
                return here[take];
            }

            pointer operator->() const {
                return &(operator*());
            }

            self_reference operator=(CBL_Iter<DataT> const &src) {
                if (this == &src)
                    return (*this);
                here = src.here;
                return *this;
            }

            self_reference operator++() { // preincrement
                take++;
                return *this;
            }

            self_type operator++(int) { // postincrement
                self_type tmp(*this);
                ++(*this);
                return tmp;
            }

            bool operator==(CBL_Iter<DataT> const &rhs) const {
                return (take == rhs.take && here == rhs.here);
            }

            bool operator!=(CBL_Iter<DataT> const &rhs) const {
                return (take != rhs.take || here != rhs.here);
            }
        };

    public:
        //type aliases
        using value_type = E;
        using iterator = CBL_Iter<E>;
        using const_iterator = CBL_Iter<E const>;

        // iterators over a non-const List
        iterator begin(){
            return CBL_Iter<E>(list,head);
        }
        iterator end(){
            return CBL_Iter<E>(list, tail + 1);
        }

        // iterators over a const List
        const_iterator begin() const {
            return CBL_Iter<const E>(list, head);
        }
        const_iterator end() const {
            return CBL_Iter<const E>(list, tail + 1);
        }
    };

//***********************************Operations*********************************************************//

    template <typename E>
    void
    CBL<E>::insert(E element, size_t position){        //insert a node at a valid location

        size_t realPosition = (head + position) % arraySize;

        if (is_empty() && position != 0)       //if the list is empty
            throw std::runtime_error("SDAL<E>.insert(): cannot put element in position");
        else if (realPosition > (tail + 1) % arraySize && realPosition < (head - 1)% arraySize)
            throw std::runtime_error("SDAL<E>.insert(): position is invalid");

        if(realPosition == head)       //if the node is wanted at the first position
            push_front(element);
        else if(realPosition == (tail + 1) % arraySize )
            push_back(element);
        else
        {
            if (is_full())       //if the list is full, let's make it bigger
            {
                size_t currentSize = arraySize;
                E * temp = new E[currentSize];

                for(size_t i = 0; i < currentSize; i++ )
                    temp[i] = list[i];

                size_t newsize = currentSize * 1.5;

                list = new E[newsize];

                for(size_t i = 0; i < newsize; i++)
                    list[i] = 0;

                for(size_t i = 0; i < currentSize; i++ )
                    list[i] = temp[i];

                arraySize = newsize;
            }

            E temp = list[realPosition];

            for(size_t i = (head % arraySize); i != realPosition; )
            {
                if(i == 0)
                    i = arraySize;
                list[(i-1) % arraySize] = list[i % arraySize];
                i = ((i+1) % arraySize);
            }
            list[realPosition-1] = element;
            if(head == 0)
                head = arraySize - 1;
            else
                head--;
        }
    }

    template <typename E>               //function to push node at the end of the list
    void
    CBL<E>::push_back(E element){

        if(is_full()){
            size_t currentSize = arraySize;
            E * temp = new E[currentSize];

            for(size_t i = 0; i < currentSize; i++ )
                temp[i] = list[i];

            size_t newsize = currentSize * 1.5;

		delete [] list;

            list = new E[newsize];

            for(size_t i = 0; i < newsize; i++)
                list[i] = 0;

            for(size_t i = 0; i < currentSize; i++ )
                list[i] = temp[i];

            arraySize = newsize;
            delete temp;
        }
        if(head == -1)
            head = tail = 0;
        else if(tail == arraySize - 1)
            tail = 0;
        else
            tail++;
        list[tail] = element;
    }

    template <typename E>                   //function that pushes the node to the front of list
    void
    CBL<E>::push_front(E element){
        if(is_full()){
            size_t currentSize = arraySize;
            E * temp = new E[currentSize];

            for(size_t i = 0; i < currentSize; i++ )
                temp[i] = list[i];

            size_t newsize = currentSize * 1.5;

            list = new E[newsize];

            for(size_t i = 0; i < newsize; i++)
                list[i] = 0;

            for(size_t i = 0; i < currentSize; i++ )
                list[i] = temp[i];

            arraySize = newsize;
            delete temp;
        }
        if(head == -1)
            head = tail = 0;
        else if(head == 0)
            head = arraySize - 1;
        else
            head--;
        list[head] = element;
    }

    template <typename E>                   //function to replace the data of an existing node
    E
    CBL<E>::replace(E element, size_t position){

        size_t realPosition = (head + position) % arraySize;

        if (is_empty())
            throw std::runtime_error("CBL<E>.replace(): list is empty");
        else if((tail < position && head < position) ||(tail > position && head > position))
            throw std::runtime_error("CBL<E>.replace(): invalid position");
        else
        {
            E temp = list[realPosition];
            list[realPosition] = element;
            return temp;
        }
    }

    template <typename E>           //function to remove existing nodes
    E
    CBL<E>::remove(size_t position){
        E returnable;

        size_t realPosition = (head + position) % arraySize;

        if (is_empty())
            throw std::runtime_error("CBL<E>.remove(): list is empty");
        else if(realPosition > (tail + 1) % arraySize && realPosition < (head - 1)% arraySize)
            throw std::runtime_error("CBL<E>.remove(): position is invalid");

        else
            returnable = list[realPosition];
        for(size_t i = realPosition; i != tail % arraySize; )
        {
            list[(i) % arraySize] = list[(i+1) % arraySize];
            i = ((i+1) % arraySize);
        }
        if(tail == 0)
            tail = arraySize -1;
        else
            tail--;

        resizer();
        return returnable;
    }

    template <typename E>           //remove last element
    E
    CBL<E>::pop_back(void){
        if (is_empty())
            throw std::runtime_error("CBL<E>.pop_back(): list is empty");

        E returnable = list[tail];

        if (head == tail)
        {
            head = -1;
            tail = -1;
        }
        else if (tail == 0)
            tail = arraySize - 1;
        else
            tail--;

        resizer();

        return returnable;
    }

    template <typename E>           //remove front element
    E
    CBL<E>::pop_front(void){
        if (is_empty())
            throw std::runtime_error("CBL<E>.pop_front(): list is empty");

        E returnable = list[head];

        if(head == tail)
            head = tail = -1;
        else{
            if(head == arraySize -1)
                head = 0;
            else
                head++;
        }

        resizer();

        return returnable;
    }

    template <typename E>           //returns data without messing with list
    E&
    CBL<E>::item_at(size_t position){
        size_t realPosition = (head + position) % arraySize;

        if (is_empty())
            throw std::runtime_error("CBL<E>.item_at(): list is empty");
        else if(realPosition > arraySize - 1) //this needs more
            throw std::runtime_error("CBL<E>.item_at(): invalid postion");
        else
            return list[realPosition];
    }

    template <typename E>       //check the last node
    E&
    CBL<E>::peek_back(void){
        if (is_empty())
            throw std::runtime_error("CBL<E>.peek_back(): list is empty");
        else
            return list[tail];
    }

    template <typename E>       //check the first node
    E&
    CBL<E>::peek_front(void){
        if (is_empty())
            throw std::runtime_error("CBL<E>.peek_front(): list is empty");
        else
            return list[head];
    }

    template <typename E>       //check if list is empty
    bool
    CBL<E>::is_empty(void){
        if (head == -1)
            return true;
        else
            return false;
    }

    template <typename E>       //check if list is full
    bool
    CBL<E>::is_full(void){
        if (tail == head - 1 || (head == 0 && tail == arraySize - 1))
            return true;
        else
            return false;
    }

    template <typename E>       //return length of list
    size_t
    CBL<E>::length(void){
        return (arraySize - head + tail + 1) % arraySize;
    }

    template <typename E>       //clear the list
    void
    CBL<E>::clear(void){

        if(!is_empty())
        {
            delete [] list;

            list = new E[arraySize];

            for(size_t i = 0; i < arraySize; i++)
                list[i] = 0;

            tail = 0;
            head = -1;
        }
        else
            throw std::runtime_error("CBL<E>.clear(): list is already empty");
    }

    template <typename E>           //check if the list contains an element
    bool
    CBL<E>::contains(E element, bool(*equals_function)(const E&,const E&)){
        for(size_t i = 0; i < length(); i++)
        {
            if(equals_function(element, list[i]))
                return true;
        }
        return false;
    }

    template<typename E>            //print the list
    void
    CBL<E>::print(std::ostream& stream_name){

        size_t curr = head;

        stream_name << "[";

        while (curr != tail) {
            stream_name << list[curr] << ",";
            curr = (curr + 1) % arraySize;
        }

        stream_name << list[tail] << "]\n";
    }


    template <typename E>
    void
    CBL<E>::resizer() {
        if(arraySize >= start*2 && length() < arraySize*.5)
        {
            size_t currentSize = arraySize;
            E * temp = new E[currentSize];

            for(size_t i = 0; i < currentSize; i++ )
                temp[i] = list[i];

            size_t newsize = currentSize * 0.75;

		delete []list;

            list = new E[newsize];

            for(size_t i = 0; i < newsize; i++)
                list[i] = 0;

            for(size_t i = 0; i < newsize; i++ )
                list[i] = temp[i];

            arraySize = newsize;
            delete [] temp;
        }
    }

    template <typename E>           //return contents of list in an array
    E*
    CBL<E>::contents(){

        E* returnable = new E[length()];
        size_t curr = head;
        size_t i = 0;

        while (curr != tail) {
            returnable[i] = list[curr];
            curr = (curr + 1) % arraySize;
            i++;
        }

        returnable[length()-1] = list[curr];
        return  returnable;
    }

}
#endif



