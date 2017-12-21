#ifndef _CDAL_H_
#define _CDAL_H_
#include "List.h"
#include <stdexcept>

namespace cop3530{

    template <typename E>
    class CDAL : public List <E>{
    public:
        CDAL(){
            head = nullptr;
            tail = 0;
            arraySize = 0;
        }                      //constructor and destructor
        ~CDAL() override {
            clear();
        }

        CDAL(const CDAL &src){          //copy constructor
            head = nullptr;
            arraySize = src.arraySize;
            tail = 0;

            node * curr = src.head;
            bool brake = false;

            while(curr){
                for(size_t i = 0; i < 50; i++)
                {
                    push_back(curr->data[i]);
                    if(tail == src.tail)
                    {
                        brake = true;
                        break;
                    }
                }
                if(brake)break;
                curr = curr->next;
            }

        }

        CDAL &operator=(const CDAL &src){       //copy-assignment operator
            if(this != &src){
                clear();

                head = nullptr;
                arraySize = src.arraySize;
                tail = 0;

                node * curr = src.head;
                bool brake = false;

                while(curr){
                    for(size_t i = 0; i < 50; i++)
                    {
                        push_back(curr->data[i]);
                        if(tail == src.tail)
                        {
                            brake = true;
                            break;
                        }
                    }
                    if(brake)break;
                    curr = curr->next;
                }
            }
            return *this;
        }

        CDAL(CDAL&& src){           //move constructor
            head = src.head;        //steal src's stuff
            tail = src.tail;
            arraySize = src.arraySize;

            src.head = nullptr;     //leave src with nothing
            src.tail = 0;
            src.arraySize = 0;
        }

        CDAL &operator=(CDAL&& src){
            if(this != &src){
                clear();

                head = src.head;        //steal src's stuff
                tail = src.tail;
                arraySize = src.arraySize;

                src.head = nullptr;     //leave src with nothing
                src.tail = 0;
                src.arraySize = 0;
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
        E*  cont() const;
        E* sender();
        void resizer();

    private:
        struct node{                    //node structure
            E * data = new E[50];
            node * next = nullptr;
        };
        node * head;        //"The CDAL will, of course, have a data member referring to the first node in its chain"
        size_t tail;
        size_t arraySize;

//********************************Iterator class************************************************************//

    public:
        template <typename DataT>
        class CDAL_Iter {

        public:
            // type aliases required for C++ iterator compatibility
            using value_type = E;
            using reference = E&;
            using pointer = E*;
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::forward_iterator_tag;

            // type aliases for prettier code
            using self_type = CDAL_Iter;
            using self_reference = CDAL_Iter &;

        private:
            E * here;
            size_t take;

        public:
            explicit CDAL_Iter(pointer start = nullptr, size_t position = 0) : here(start), take(position){}

            CDAL_Iter(const CDAL_Iter &src) : here(src.here),take(src.take) {}

            reference operator*() const {
                return here[take];
            }

            pointer operator->() const {
                return &(operator*());
            }

            self_reference operator=(CDAL_Iter<DataT> const &src) {
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

            bool operator==(CDAL_Iter<DataT> const &rhs) const {
                return (take == rhs.take);
            }

            bool operator!=(CDAL_Iter<DataT> const &rhs) const {
                return (take != rhs.take);
            }
        };

    public:
        //type aliases
        using value_type = E;
        using iterator = CDAL_Iter<E>;
        using const_iterator = CDAL_Iter<E const>;

        // iterators over a non-const List
        iterator begin(){
            return CDAL_Iter<E>(contents(),0);
        }
        iterator end(){
            return CDAL_Iter<E>(contents(), tail);
        }

        // iterators over a const List
        const_iterator begin() const {
            return CDAL_Iter<const E>(cont(), 0);
        }
        const_iterator end() const {
            return CDAL_Iter<const E>(cont(), tail);
        }
    };

//***********************************Operations*********************************************************//

    template <typename E>
    void
    CDAL<E>::insert(E element, size_t position){        //insert a node at a valid location

        if (is_empty() && position != 0)       //if the list is empty
            throw std::runtime_error("CDAL<E>.insert(): cannot put element in position");
        else if (is_full())       //if the list is full
            throw std::runtime_error("CDAL<E>.insert(): list is full");
        else if (length() < position || position < 0)
            throw std::runtime_error("CDAL<E>.insert(): position is invalid");

        if(position == 0)       //if the node is wanted at the first position
        {
            push_front(element);
        }
        else if (position == length())      //if the node is wanted at the end
            push_back(element);
        else
        {
            E * filling = sender();
            size_t currentSize = length();

            for(size_t i = tail-1; i >= position; i--)
            {
                filling[i + 1] = filling[i];
            }
            filling[position] = element;

            size_t temptail = tail;

            clear();

            tail = temptail;

            node *nodeptr = new node;
            nodeptr->next = nullptr;
            head = nodeptr;
            node * curr = head;

            size_t tracer = 0;
            arraySize = 50;
            for(size_t i = 0; i < currentSize + 1; i++, tracer++ )
            {
                if(tracer != 0 && tracer % 50 == 0)
                {
                    arraySize += 50;
                    curr->next = new node;
                    curr = curr->next;
                    tracer = 0;
                }
                curr->data[tracer] = filling[i];
            }
            tail++;
            delete [] filling;
        }
    }

    template <typename E>               //function to push node at the end of the list
    void
    CDAL<E>::push_back(E element){

        if(head == nullptr)
        {
            arraySize += 50;
            node *nodeptr = new node;
            nodeptr->next = nullptr;
            nodeptr->data[tail] = element;
            head = nodeptr;
            tail++;
        }
        else if(tail % 50 != 0)
        {
            node * temp = head;

            while(temp->next != nullptr)
                temp = temp->next;

            temp->data[tail % 50] = element;

            tail++;
        }
        else
        {
            node *nodeptr = new node;
            node * temp = head;

            while(temp->next != nullptr)
                temp = temp->next;

            temp->next = nodeptr;
            nodeptr->next = nullptr;
            nodeptr->data[0] = element;
            tail++;
            arraySize += 50;
        }
    }

    template <typename E>                   //function that pushes the node to the front of list
    void
    CDAL<E>::push_front(E element){

        if( tail == 0)
        {
            arraySize += 50;
            node *nodeptr = new node;
            nodeptr->next = nullptr;
            nodeptr->data[0] = element;
            head = nodeptr;
            tail++;
        }
        else
        {
            E * filling = sender();
            size_t currentSize = length();

            for(size_t i = currentSize; i > 0; i--)
                filling[i] = filling[i - 1];

            filling[0] = element;

            size_t temptail = tail;

            clear();

            tail = temptail;

            node *nodeptr = new node;
            nodeptr->next = nullptr;
            head = nodeptr;
            node * curr = head;
            arraySize = 50;

            size_t tracer = 0;
            for(size_t i = 0; i < currentSize + 1; i++, tracer++ )
            {
                if(tracer != 0 && tracer % 50 == 0)
                {
                    arraySize += 50;
                    curr->next = new node;
                    curr = curr->next;
                    tracer = 0;
                }
                curr->data[tracer] = filling[i];
            }
            tail++;
            delete filling;
        }
    }

    template <typename E>                   //function to replace the data of an existing node
    E
    CDAL<E>::replace(E element, size_t position){

        if (is_empty())
            throw std::runtime_error("CDAL<E>.replace(): list is empty");
        else if(position > tail - 1)
            throw std::runtime_error("CDAL<E>.replace(): invalid position");
        else
        {
            E returnable;
            E * filling = contents();
            size_t currentSize = length();

            returnable = filling[position];
            filling[position] = element;

            size_t temptail = tail;

            clear();

            tail = temptail;

            node *nodeptr = new node;
            nodeptr->next = nullptr;
            head = nodeptr;
            node * curr = head;
            arraySize = 50;

            size_t tracer = 0;
            for(size_t i = 0; i < currentSize; i++, tracer++ )
            {
                if(tracer != 0 && tracer % 50 == 0)
                {
                    curr->next = new node;
                    curr = curr->next;
                    tracer = 0;
                    arraySize += 50;
                }
                curr->data[tracer] = filling[i];
            }
            delete [] filling;
            return returnable;
        }
    }

    template <typename E>           //function to remove existing nodes
    E
    CDAL<E>::remove(size_t position){
        E returnable;

        if (is_empty())
            throw std::runtime_error("CDAL<E>.remove(): list is empty");
        else if(position > length() - 1 || position < 0)
            throw std::runtime_error("CDAL<E>.remove(): position is invalid");
        else{
            E * filling = contents();
            size_t currentSize = length();
            returnable = filling[position];

            for(size_t i = position + 1; i < currentSize; i++)
                filling[i - 1] = filling[i];

            size_t temptail = tail;

            clear();

            tail = temptail;

            node *nodeptr = new node;
            nodeptr->next = nullptr;
            head = nodeptr;
            node * curr = head;

            size_t tracer = 0;
            arraySize = 50;
            for(size_t i = 0; i < currentSize + 1; i++, tracer++ )
            {
                if(tracer != 0 && tracer % 50 == 0)
                {
                    arraySize += 50;
                    curr->next = new node;
                    curr = curr->next;
                    tracer = 0;
                }
                curr->data[tracer] = filling[i];
            }
            delete filling;
        }

        if(tail > 0)
            tail--;

        resizer();
        return returnable;
    }

    template <typename E>           //remove last element
    E
    CDAL<E>::pop_back(void){
        if (is_empty())
            throw std::runtime_error("CDAL<E>.pop_back(): list is empty");

        E * filling = contents();
        E returnable = filling[tail -1];
        delete [] filling;

        if(tail > 0)
            tail--;

        resizer();

        return returnable;
    }

    template <typename E>           //remove front element
    E
    CDAL<E>::pop_front(void){
        if (is_empty())
            throw std::runtime_error("CDAL<E>.pop_front(): list is empty");

        E * filling = contents();
        size_t currentSize = length();

        E returnable = filling[0];

        for(size_t i = 0; i < currentSize; i++)
            filling[i] = filling[i + 1];

        size_t temptail = tail;

        clear();

        tail = temptail;

        node *nodeptr = new node;
        nodeptr->next = nullptr;
        head = nodeptr;
        node * curr = head;

        size_t tracer = 0;
        arraySize = 50;
        for(size_t i = 0; i < currentSize + 1; i++, tracer++ )
        {
            if(tracer != 0 && tracer % 50 == 0)
            {
                arraySize += 50;
                curr->next = new node;
                curr = curr->next;
                tracer = 0;
            }
            curr->data[tracer] = filling[i];
        }
        delete [] filling;

        if(tail > 0)
            tail--;

        resizer();

        return returnable;
    }

    template <typename E>           //returns data without messing with list
    E&
    CDAL<E>::item_at(size_t position){
        if (is_empty())
            throw std::runtime_error("CDAL<E>.remove(): list is empty");
        else if(position > length() - 1 || position < 0)
            throw std::runtime_error("CDAL<E>.remove(): invalid postion");
        else
        {
            node * curr = head;
            size_t counter = 0;
            size_t inc = 0;

            if(head->next != nullptr){
                while(curr)
                {
                    while (counter < position )
                    {
                        if(counter == position)
                        {
                            return curr->data[position % 50];
                        }
                        counter++;
                        inc++;

                        if(inc == 50)
                        {
                            inc = 0;
                            break;
                        }
                    }
                    curr = curr->next;
                }
            }
            return curr->data[position % 50];
        }
    }

    template <typename E>       //check the last node
    E&
    CDAL<E>::peek_back(void){
        if (is_empty())
            throw std::runtime_error("CDAL<E>.peek_back(): list is empty");
        else
        {
            node * curr = head;
            if(head->next != nullptr){
                while(curr){

                    curr = curr->next;
                }
            }

            return curr->data[tail-1 % 50];
        }
    }

    template <typename E>       //check the first node
    E&
    CDAL<E>::peek_front(void){
        if (is_empty())
            throw std::runtime_error("CDAL<E>.peek_front(): list is empty");
        else
        {
            return head->data[0];
        }
    }

    template <typename E>       //check if list is empty
    bool
    CDAL<E>::is_empty(void){
        if (head == nullptr)
            return true;
        else
            return false;
    }

    template <typename E>       //check if list is full
    bool
    CDAL<E>::is_full(void){
        if (arraySize == length() - 1)
            return true;
        else
            return false;
    }

    template <typename E>       //return length of list
    size_t
    CDAL<E>::length(void){
        return tail;
    }

    template <typename E>       //clear the list
    void
    CDAL<E>::clear(void){

        if(!is_empty())
        {
            node * curr = head;
            node * temp;

            while(curr)
            {
                temp = curr;
                curr = curr->next;
                delete [] temp->data;
                delete temp;
            }
            delete curr;

            head = nullptr;
            tail = 0;
            arraySize = 0;
        }
        else
            throw std::runtime_error("CDAL<E>.clear(): list is already empty");
    }

    template <typename E>           //check if the list contains an element
    bool
    CDAL<E>::contains(E element, bool(*equals_function)(const E&,const E&)){
        E * list = contents();

        for(size_t i = 0; i < length(); i++)
        {
            if(equals_function(element, list[i]))
                return true;
        }

        delete [] list;
        return false;
    }

    template<typename E>            //print the list
    void
    CDAL<E>::print(std::ostream& stream_name){

        E * list = contents();

        stream_name << "[";
        for(size_t i = 0; i < length(); i++ )
        {
            stream_name << list[i];
            if(i + 1 < length())
                stream_name << ", ";
        }
        stream_name << "]\n";

        delete [] list;

    }

    template <typename E>
    void
    CDAL<E>::resizer() {
        if(length() < arraySize*.5)
        {
            size_t nodeNum = 0;
            node * curr = head;

            while (curr)
            {
                nodeNum++;
                curr = curr->next;
            }

            E * list = contents();
            size_t currentSize = length();
            size_t modSize = arraySize * 0.75;

            size_t temptail = tail;

            clear();

            tail = temptail;

            node *nodeptr = new node;
            nodeptr->next = nullptr;
            head = nodeptr;
            curr = head;

            size_t tracer = 0;
            arraySize = 50;
            for(size_t i = 0; i < modSize; i++, tracer++ )
            {
                if(tracer != 0 && tracer % 50 == 0)
                {
                    arraySize += 50;
                    curr->next = new node;
                    curr = curr->next;
                    tracer = 0;
                }
                if (i < currentSize)
                    curr->data[tracer] = list[i];
                else
                    curr->data[tracer] = 0;
            }
            delete [] list;
        }
    }

    template <typename E>           //return contents of list in an array of size + 1
    E*
    CDAL<E>::sender(){

        if(head == nullptr)
            throw std::runtime_error("CDAL<E>.contents(): list is empty");

        E* returnable = new E[length() + 1];
        node * curr = head;
        size_t pos = 0;

        for(size_t i = 0; i < length(); i++, pos++)
        {
            if(i != 0 && i % 50 == 0)
            {
                pos = 0;
                curr = curr->next;
            }
            returnable[i] = curr->data[pos];
        }

        return  returnable;
    }

    template <typename E>           //return contents of list in an array
    E*
    CDAL<E>::contents(){

        if(head == nullptr)
            throw std::runtime_error("CDAL<E>.contents(): list is empty");

        E* returnable = new E[length()];
        node * curr = head;
        size_t pos = 0;

        for(size_t i = 0; i < length(); i++, pos++)
        {
            if(i != 0 && i % 50 == 0)
            {
                pos = 0;
                curr = curr->next;
            }
            returnable[i] = curr->data[pos];
        }
        return  returnable;
    }

    template <typename E>           //return contents of list in an array
    E*
    CDAL<E>::cont() const {

        if(head == nullptr)
            throw std::runtime_error("CDAL<E>.contents(): list is empty");

        E* returnable = new E[tail];
        node * curr = head;
        size_t pos = 0;

        for(size_t i = 0; i < tail; i++, pos++)
        {
            if(i != 0 && i % 50 == 0)
            {
                pos = 0;
                curr = curr->next;
            }
            returnable[i] = curr->data[pos];
        }
        return  returnable;
    }
}
#endif
