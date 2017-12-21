#ifndef _SSLL_H_
#define _SSLL_H_
#include "List.h"
#include <stdexcept>

namespace cop3530{

    template <typename E>
    class SSLL : public List <E>{
    public:
        SSLL(){                         //constructor
            head = nullptr;
            tail = nullptr;
        }
        ~SSLL() override {              //destructor
            clear();
        }
        SSLL(const SSLL &src){          //copy constructor
            head = nullptr;
            tail = nullptr;
            node * there = src.head;

            while (there)
            {
                push_back(there->data);
                there = there->next;
            }
        }

        SSLL &operator=(const SSLL &src){       //copy-assignment operator
            if(this != &src){
                clear();

                node * there = src.head;

                while (there)
                {
                    push_back(there->data);
                    there = there->next;
                }
            }
            return *this;
        }

        SSLL(SSLL&& src){           //move constructor
            head = src.head;        //steal src's stuff
            tail = src.tail;

            src.head = nullptr;     //leave src with nothing
            src.tail = nullptr;
        }

        SSLL &operator=(SSLL&& src){
            if(this != &src){
                clear();

                head = src.head;        //steal src's stuff
                tail = src.tail;

                src.head = nullptr;     //leave src with nothing
                src.tail = nullptr;
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

    private:
        struct node{                    //node structure
            E data;
            node * next = nullptr;
        };
        node * head;
        node * tail;

//********************************Iterator class************************************************************//

    public:
        template <typename DataT>
        class SSLL_Iter {

        public:
            // type aliases required for C++ iterator compatibility
            using value_type = E;
            using reference = E&;
            using pointer = E*;
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::forward_iterator_tag;

            // type aliases for prettier code
            using self_type = SSLL_Iter;
            using self_reference = SSLL_Iter &;

        private:
            node *here;

        public:
            explicit SSLL_Iter(node *start = nullptr) : here(start) {}

            SSLL_Iter(const SSLL_Iter &src) : here(src.here) {}

            reference operator*() const {
                return here->data;
            }

            pointer operator->() const {
                return &(operator*());
            }

            self_reference operator=(SSLL_Iter<DataT> const &src) {
                if (this == &src)
                    return (*this);
                here = src.here;
                return *this;
            }

            self_reference operator++() { // preincrement
                here = here->next;
                return *this;
            }

            self_type operator++(int) { // postincrement
                self_type tmp(*this);
                ++(*this);
                return tmp;
            }

            bool operator==(SSLL_Iter<DataT> const &rhs) const {
                return (here == rhs.here);
            }

            bool operator!=(SSLL_Iter<DataT> const &rhs) const {
                return (here != rhs.here);
            }
        };

    public:
        //type aliases
        using value_type = E;
        using iterator = SSLL_Iter<E>;
        using const_iterator = SSLL_Iter<E const>;

        // iterators over a non-const List
        iterator begin(){
            return SSLL_Iter<E>(head);
        }
        iterator end(){
            return SSLL_Iter<E>(nullptr);
        }

        // iterators over a const List
        const_iterator begin() const {
            return SSLL_Iter<const E>(head);
        }
        const_iterator end() const {
            return SSLL_Iter<const E>(nullptr);
        }
    };

//***********************************Operations*********************************************************//

    template <typename E>
    void
    SSLL<E>::insert(E element, size_t position){        //insert a node at a valid location
        size_t counter = 0;
        node * curr = head;
        node * temp = curr;

        if (is_empty() && position != 0)       //if the list is empty
            throw std::runtime_error("SSLL<E>.insert(): cannot put element in position");

        if(position == 0)       //if the node is wanted at the first position
        {
            push_front(element);
        }
        else if (position == length())      //if the node is wanted at the end
            push_back(element);
        else
        {
		node * nodeptr = new node;
            while (curr)
            {
                if (counter == position)            //if the node is to be located anywhere else
                {
                    temp->next = nodeptr;
                    nodeptr->data = element;
                    nodeptr->next = curr;
                    break;
                }
                temp = curr;
                curr = curr->next;
                counter++;
            }

            if (counter == position)
            {
                temp->next = nodeptr;
                nodeptr->data = element;
                nodeptr->next = curr;
            }

            if (counter < position || position < 0)
                throw std::runtime_error("SSLL<E>.insert(): position is invalid");
        }
    }

    template <typename E>               //function to push node at the end of the list
    void
    SSLL<E>::push_back(E element){
        node * nodeptr = new node;

        if (is_empty())            //if the list is empty
        {
            nodeptr->next = nullptr;
            nodeptr->data = element;
            head = tail = nodeptr;
        }
        else
        {
            tail->next = nodeptr;
            nodeptr->data = element;
            nodeptr->next = nullptr;			//not sure if nodeptr->next = NULL is necessary before
            tail = nodeptr;
        }
    }

    template <typename E>                   //function that pushes the node to the front of list
    void
    SSLL<E>::push_front(E element){

        node * nodeptr = new node;

        if (is_empty())                //if the list is empty
        {
            nodeptr->next = nullptr;
            nodeptr->data = element;
            head = tail = nodeptr;
        }
        else
        {
            nodeptr->next = head;			//not sure if nodeptr->next = NULL is necessary before
            nodeptr->data = element;
            head = nodeptr;
        }
    }

    template <typename E>                   //function to replace the data of an existing node
    E
    SSLL<E>::replace(E element, size_t position){
        size_t counter = 0;
        node * curr = head;
        E returnable;

        if (is_empty())
            throw std::runtime_error("SSLL<E>.replace(): list is empty");

        while (curr)
        {
            if (counter == position)
            {
                returnable = curr->data;
                curr->data = element;
                break;
            }
            curr = curr->next;
            counter++;
        }

        if (counter != position)
            throw std::runtime_error("SSLL<E>.replace(): position is invalid");
        else
            return returnable;
    }

    template <typename E>           //function to remove existing nodes
    E
    SSLL<E>::remove(size_t position){
        size_t counter = 0;
        node * curr = head;
        node * prev = curr;
        E returnable;

        if (is_empty())
            throw std::runtime_error("SSLL<E>.remove(): list is empty");

        while (curr)
        {
            if (counter == position)
            {
                returnable = curr->data;
                prev->next = prev->next->next;
                delete curr;
                break;
            }
            prev = curr;
            curr = curr->next;
            counter++;
        }

        if (counter != position)
            throw std::runtime_error("SSLL<E>.remove(): position is invalid");
        else
            return returnable;
    }

    template <typename E>           //remove last element
    E
    SSLL<E>::pop_back(void){
        node * curr = head;
        node * prev = curr;
        E returnable = tail->data;

        if (is_empty())
            throw std::runtime_error("SSLL<E>.pop_back(): list is empty");

        while (curr)
        {
            if (curr->next == nullptr)
            {
                prev->next = nullptr;
                tail = prev;
                delete curr;
                break;
            }
            prev = curr;
            curr = curr->next;
        }

        return returnable;
    }

    template <typename E>           //remove front element
    E
    SSLL<E>::pop_front(void){
        E returnable = head->data;
        node * curr = head;

        if (is_empty())
            throw std::runtime_error("SSLL<E>.pop_back(): list is empty");

        head = head->next;
        delete curr;

        return returnable;
    }

    template <typename E>           //returns reference to data
    E&
    SSLL<E>::item_at(size_t position){
        size_t counter = 0;
        node * curr = head;

        if (is_empty())
            throw std::runtime_error("SSLL<E>.remove(): list is empty");

        while (curr)
        {
            if (counter == position)
            {
                return curr->data;
            }
            curr = curr->next;
            counter++;
        }

        if (counter != position)
            throw std::runtime_error("SSLL<E>.remove(): position is invalid");
    }

    template <typename E>       //check the last node
    E&
    SSLL<E>::peek_back(void){
        return tail->data;
    }

    template <typename E>       //check the first node
    E&
    SSLL<E>::peek_front(void){
        return head->data;
    }

    template <typename E>       //check if list is empty
    bool
    SSLL<E>::is_empty(void){
        if (!head)
            return true;
        else
            return false;
    }

    template <typename E>       //check if list is full
    bool
    SSLL<E>::is_full(void){
        return false;		//linked list can't be full
    }

    template <typename E>       //return length of list
    size_t
    SSLL<E>::length(void){
        size_t len = 0;
        node * curr = head;
        while (curr){
            ++len;
            curr = curr->next;
        }
        return len;
    }

    template <typename E>       //clear the list
    void
    SSLL<E>::clear(void){
        node * curr = head;
        node * prev = curr;

        while (curr){
            prev = curr;
            curr = curr->next;
            delete prev;
        }
        delete curr;
        head = tail = nullptr;
    }

    template <typename E>           //check if the list contains an element
    bool
    SSLL<E>::contains(E element, bool(*equals_function)(const E&,const E&)){
        node * curr = head;
        while(curr)
        {
            if(equals_function(element, curr->data))
                return true;
            else
                curr = curr->next;
        }
        return false;
    }

    template<typename E>            //print the list
    void
    SSLL<E>::print(std::ostream& stream_name){
        node * curr = head;
        stream_name << "[";
        while (curr){
            stream_name << curr->data;
            if(curr->next != nullptr) stream_name<< ", ";
            curr = curr->next;
        }
        stream_name << "]\n";

    }

    template <typename E>           //return contents of list in an array
    E*
    SSLL<E>::contents(){
        size_t size = length();
        E* list_array = 0;
        list_array = new E[size];
        node * curr = head;

        for(size_t i = 0; curr; i++)
        {
            list_array[i] = curr->data;
            curr = curr->next;
        }
        return  list_array;
    }

}
#endif
