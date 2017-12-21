#ifndef _SDAL_H_
#define _SDAL_H_
#include "List.h"
#include <stdexcept>

namespace cop3530{

    template <typename E>
    class SDAL : public List <E>{
    public:
        SDAL(size_t size){                         //constructor and destructor
            list = new E[size];
            arraySize = size;
            tail = 0;
            start = size;

            for(size_t i = 0; i < size; i++)
            {
                list[i] = 0;
            }
        }
        SDAL(){
            list = new E[50];
            arraySize = 50;
            tail = 0;
            start = 50;
        }
        ~SDAL() override {
            clear();
            delete [] list;
        }

        SDAL(const SDAL &src){          //copy constructor
            list = new E[src.arraySize];
            arraySize = src.arraySize;
            tail = 0;
            start = src.start;

            for(size_t i = 0; i < src.tail; i++)
            {
                push_back(src.list[i]);
            }
        }

        SDAL &operator=(const SDAL &src){       //copy-assignment operator
            if(this != &src){
                delete [] list;

                list = new E[src.arraySize];
                arraySize = src.arraySize;
                tail = 0;
                start = src.start;

                for(size_t i = 0; i < src.tail; i++)
                {
                    push_back(src.list[i]);
                }
            }
            return *this;
        }

        SDAL(SDAL&& src){           //move constructor
            list = src.list;        //steal src's stuff
            tail = src.tail;
            arraySize = src.arraySize;
            start = src.start;

            src.list = nullptr;     //leave src with nothing
            src.tail = 0;
            src.arraySize = 0;
            src.start = 0;
        }

        SDAL &operator=(SDAL&& src){
            if(this != &src){
                delete [] list;

                list = src.list;        //steal src's stuff
                tail = src.tail;
                arraySize = src.arraySize;
                start = src.start;

                src.list = nullptr;     //leave src with nothing
                src.tail = 0;
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
        size_t atestsize();

    private:
        E * list= nullptr;          //"Neither class shall have a head data member"
        size_t tail;            //"the tail data member shall contain the index where the next item appended to the list would go"
        size_t arraySize;
        size_t start;
//********************************Iterator class************************************************************//

    public:
        template <typename DataT>
        class SDAL_Iter {

        public:
            // type aliases required for C++ iterator compatibility
            using value_type = E;
            using reference = E&;
            using pointer = E*;
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::forward_iterator_tag;

            // type aliases for prettier code
            using self_type = SDAL_Iter;
            using self_reference = SDAL_Iter &;

        private:
            E * here;
            size_t take;

        public:
            explicit SDAL_Iter(pointer start = nullptr, size_t position = 0) : here(start), take(position){}

            SDAL_Iter(const SDAL_Iter &src) : here(src.here),take(src.take) {}

            reference operator*() const {
                return here[take];
            }

            pointer operator->() const {
                return &(operator*());
            }

            self_reference operator=(SDAL_Iter<DataT> const &src) {
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

            bool operator==(SDAL_Iter<DataT> const &rhs) const {
                return (take == rhs.take && here == rhs.here);
            }

            bool operator!=(SDAL_Iter<DataT> const &rhs) const {
                return (take != rhs.take || here != rhs.here);
            }
        };

    public:
        //type aliases
        using value_type = E;
        using iterator = SDAL_Iter<E>;
        using const_iterator = SDAL_Iter<E const>;

        // iterators over a non-const List
        iterator begin(){
            return SDAL_Iter<E>(list,0);
        }
        iterator end(){
            return SDAL_Iter<E>(list, tail);
        }

        // iterators over a const List
        const_iterator begin() const {
            return SDAL_Iter<const E>(list,0);
        }
        const_iterator end() const {
            return SDAL_Iter<const E>(list,tail);
        }
    };

//***********************************Operations*********************************************************//

    template <typename E>
    void
    SDAL<E>::insert(E element, size_t position){        //insert a node at a valid location

        if (is_empty() && position != 0)       //if the list is empty
            throw std::runtime_error("SDAL<E>.insert(): cannot put element in position");
        else if (length() < position || position < 0)
            throw std::runtime_error("SDAL<E>.insert(): position is invalid");

        if(position == 0)       //if the node is wanted at the first position
        {
            push_front(element);
        }
        else if (position == length())      //if the node is wanted at the end
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

                delete [] list;

                list = new E[newsize];

                for(size_t i = 0; i < newsize; i++)
                    list[i] = 0;

                for(size_t i = 0; i < currentSize; i++ )
                    list[i] = temp[i];

                arraySize = newsize;
                delete [] temp;
            }

            for(size_t i = tail; i >= position; i--)
            {
                list[i + 1] = list[i];
            }
            list[position] = element;
            tail++;
        }
    }

    template <typename E>               //function to push node at the end of the list
    void
    SDAL<E>::push_back(E element){

        if(tail == 0)
        {
            list[0] = element;
            tail++;
        }
        else if(!is_full())
        {
            list[tail] = element;
            tail++;
        }
        else
        {
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

            list[tail] = element;
            tail++;
            arraySize = newsize;
            delete [] temp;
        }
    }

    template <typename E>                   //function that pushes the node to the front of list
    void
    SDAL<E>::push_front(E element){

        if( tail == 0)
        {
            list[0] = element;
            tail++;
        }
        else if(!is_full() && tail != arraySize)
        {
            for(size_t i = tail - 1; i < length(); i--)
                list[i + 1] = list[i];

            list[0] = element;
            tail++;
        }
        else
        {
            size_t currentSize = arraySize;
            E * temp = contents();

            size_t newsize = currentSize * 1.5;

            delete [] list;

            list = new E[newsize];

            for(size_t i = 0; i < currentSize; i++ )
            {
                list[i] = temp[i];
            }

            for(size_t i = tail-1; i > 0; i--)
                list[i + 1] = list[i];

            list[0] = element;
            tail++;
            arraySize = newsize;
            delete [] temp;
        }

    }

    template <typename E>                   //function to replace the data of an existing node
    E
    SDAL<E>::replace(E element, size_t position){

        if (is_empty())
            throw std::runtime_error("SDAL<E>.replace(): list is empty");
        else if(position > tail)
            throw std::runtime_error("SDAL<E>.replace(): invalid position");
        else
        {
            E temp = list[position];
            list[position] = element;
            return temp;
        }
    }

    template <typename E>           //function to remove existing nodes
    E
    SDAL<E>::remove(size_t position){
        E returnable;

        if (is_empty())
            throw std::runtime_error("SDAL<E>.remove(): list is empty");
        else if(position > length() - 1 || position < 0)
            throw std::runtime_error("SDAL<E>.remove(): position is invalid");
        else{
            returnable = list[position];
            for(size_t i = position + 1; i < length(); i++)
                list[i - 1] = list[i];
        }

        if(tail > 0)
            tail--;

        resizer();
        return returnable;
    }

    template <typename E>           //remove last element
    E
    SDAL<E>::pop_back(void){
        if (is_empty())
            throw std::runtime_error("SDAL<E>.pop_back(): list is empty");

        E returnable = list[tail];

        if(tail > 0)
            tail--;

        resizer();

        return returnable;
    }

    template <typename E>           //remove front element
    E
    SDAL<E>::pop_front(void){
        if (is_empty())
            throw std::runtime_error("SDAL<E>.pop_front(): list is empty");

        E returnable = list[0];

        for(size_t i = 0; i < length(); i++ )
            list[i] = list[i + 1];

        if(tail > 0)
            tail--;

        resizer();

        return returnable;
    }

    template <typename E>           //returns data without messing with list
    E&
    SDAL<E>::item_at(size_t position){
        if (is_empty())
            throw std::runtime_error("SDAL<E>.remove(): list is empty");
        else if(position > length() - 1 || position < 0)
            throw std::runtime_error("SDAL<E>.remove(): invalid postion");
        else
            return list[position];
    }

    template <typename E>       //check the last node
    E&
    SDAL<E>::peek_back(void){
        if (is_empty())
            throw std::runtime_error("SDAL<E>.peek_back(): list is empty");
        else
            return list[tail - 1];
    }

    template <typename E>       //check the first node
    E&
    SDAL<E>::peek_front(void){
        if (is_empty())
            throw std::runtime_error("SDAL<E>.peek_front(): list is empty");
        else
            return list[0];
    }

    template <typename E>       //check if list is empty
    bool
    SDAL<E>::is_empty(void){
        if (tail == 0)
            return true;
        else
            return false;
    }

    template <typename E>       //check if list is full
    bool
    SDAL<E>::is_full(void){
        if (arraySize == length())
            return true;
        else
            return false;
    }

    template <typename E>       //return length of list
    size_t
    SDAL<E>::length(void){
        return tail;
    }

    template <typename E>       //clear the list
    void
    SDAL<E>::clear(void){

        if(!is_empty())
        {
            delete [] list;

            list = new E[arraySize];

            for(size_t i = 0; i < arraySize; i++)
                list[i] = 0;

            tail = 0;
        }
        else
            throw std::runtime_error("SDAL<E>.clear(): list is already empty");
    }

    template <typename E>           //check if the list contains an element
    bool
    SDAL<E>::contains(E element, bool(*equals_function)(const E&,const E&)){
        for(size_t i = 0; i < length(); i++)
        {
            if(equals_function(element, list[i]))
                return true;
        }
        return false;
    }

    template<typename E>            //print the list
    void
    SDAL<E>::print(std::ostream& stream_name){

        stream_name << "[";
        for(size_t i = 0; i < length(); i++ )
        {
            stream_name << list[i];
            if(i + 1 < length())
                stream_name << ", ";
        }
        stream_name << "]\n";

    }

    template <typename E>
    void
    SDAL<E>::resizer() {
        if(arraySize >= start*2 && length() < arraySize*.5)
        {
            size_t currentSize = arraySize;
            E * temp = new E[currentSize];

            for(size_t i = 0; i < currentSize; i++ )
                temp[i] = list[i];

            size_t newsize = currentSize * 0.75;

            delete [] list;

            list = new E[newsize];

            for(size_t i = 0; i < newsize; i++)
                list[i] = 0;

            for(size_t i = 0; i < newsize; i++ )
                list[i] = temp[i];

            arraySize = newsize;
            delete []temp;
        }
    }

    template <typename E>           //return contents of list in an array
    E*
    SDAL<E>::contents(){

        E* returnable = new E[length()];

        for(size_t i = 0; i < tail; i++ )
            returnable[i] = list[i];
        return  returnable;
    }

    template <typename E>
    size_t
    SDAL<E>::atestsize() {
        return arraySize;
    }

}
#endif


