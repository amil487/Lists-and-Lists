#ifndef _PSLL_H_
#define _PSLL_H_
#include "List.h"
#include <stdexcept>

namespace cop3530{

    template <typename E>
    class PSLL : public List <E>{
    public:
        PSLL(){
            head = nullptr;
            tail = nullptr;
            freelist = nullptr;         //the pool starts out empty
            poolSize = 0;
        }
        ~PSLL() override {
            clear();
            node * curr = freelist;
            node * here;
            while (curr)
            {
                here = curr;
                curr = curr->next;
                delete here;
            }
        }

        PSLL(const PSLL &src){          //copy constructor
            head = nullptr;
            tail = nullptr;
            freelist = nullptr;
            poolSize = 0;
            node * there = src.head;

            while (there)
            {
                push_back(there->data);
                there = there->next;
            }
        }

        PSLL &operator=(const PSLL &src){       //copy-assignment operator
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

        PSLL(PSLL&& src){           //move constructor
            head = src.head;        //steal src's stuff
            tail = src.tail;
            freelist = src.freelist;
            poolSize = src.poolSize;

            src.head = nullptr;     //leave src with nothing
            src.tail = nullptr;
            src.freelist = nullptr;
            src.poolSize = 0;
        }

        PSLL &operator=(PSLL&& src){
            if(this != &src){
                clear();

                head = src.head;        //steal src's stuff
                tail = src.tail;
                freelist = src.freelist;
                poolSize = src.poolSize;

                src.head = nullptr;     //leave src with nothing
                src.tail = nullptr;
                src.freelist = nullptr;
                src.poolSize = 0;
            }
            return *this;
        }

        void insert(E element,size_t position) override;
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
        void printFreeList();
        void resizer();

    private:
        struct node{
            E data;
            node * next = nullptr;
        };
        node * head;
        node * tail;
        node * freelist;
        size_t poolSize;

//********************************Iterator class************************************************************//

    public:
        template <typename DataT>
        class PSLL_Iter {

        public:
            // type aliases required for C++ iterator compatibility
            using value_type = E;
            using reference = E&;
            using pointer = E*;
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::forward_iterator_tag;

            // type aliases for prettier code
            using self_type = PSLL_Iter;
            using self_reference = PSLL_Iter &;

        private:
            node *here;

        public:
            explicit PSLL_Iter(node *start = nullptr) : here(start) {}

            PSLL_Iter(const PSLL_Iter &src) : here(src.here) {}

            reference operator*() const {
                return here->data;
            }

            pointer operator->() const {
                return &(operator*());
            }

            self_reference operator=(PSLL_Iter<DataT> const &src) {
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

            bool operator==(PSLL_Iter<DataT> const &rhs) const {
                return (here == rhs.here);
            }

            bool operator!=(PSLL_Iter<DataT> const &rhs) const {
                return (here != rhs.here);
            }
        };

    public:
        //type aliases
        using value_type = E;
        using iterator = PSLL_Iter<E>;
        using const_iterator = PSLL_Iter<E const>;

        // iterators over a non-const List
        iterator begin(){
            return PSLL_Iter<E>(head);
        }
        iterator end(){
            return PSLL_Iter<E>(nullptr);
        }

        // iterators over a const List
        const_iterator begin() const {
            return PSLL_Iter<const E>(head);
        }
        const_iterator end() const {
            return PSLL_Iter<const E>(nullptr);
        }
    };

//***********************************Operations*********************************************************//

    template <typename E>
    void
    PSLL<E>::insert(E element, size_t position){
        size_t counter = 0;

        if (is_empty() && position != 0)
            throw std::runtime_error("PSLL<E>.insert(): cannot put element in position");

        if(position == 0)
        {
            push_front(element);
        }
        else if (position == length())
            push_back(element);
        else
        {
            if(freelist != nullptr)
            {
                node * curr = head;
                node * temp = curr;

                node * nodeptr = freelist;
                freelist = freelist->next;
                nodeptr->next = nullptr;
                nodeptr->data = element;

                while (curr)
                {
                    if (counter == position)
                    {
                        temp->next = nodeptr;
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
                    nodeptr->next = curr;
                }
                poolSize--;
            }
            else{
                node * nodeptr = new node;
                node * curr = head;
                node * temp = curr;

                while (curr)
                {
                    if (counter == position)
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
            }

            if (counter != position)
                throw std::runtime_error("PSLL<E>.insert(): position is invalid");

            resizer();
        }
    }

    template <typename E>
    void
    PSLL<E>::push_back(E element){

        if (is_empty() && freelist == nullptr)
        {
            node * nodeptr = new node;
            nodeptr->next = nullptr;
            nodeptr->data = element;
            head = tail = nodeptr;
        } else if(is_empty() && freelist != nullptr)
        {
            node * nodeptr = freelist;
            freelist = freelist->next;
            nodeptr->next = nullptr;
            nodeptr->data = element;
            head = tail = nodeptr;
            poolSize--;
        } else if(!is_empty() && freelist != nullptr){
            node * nodeptr = freelist;
            freelist = freelist->next;
            nodeptr->next = nullptr;
            nodeptr->data = element;
            tail = nodeptr;
            poolSize--;
        } else
        {
            node * nodeptr = new node;
            tail->next = nodeptr;
            nodeptr->data = element;
            nodeptr->next = nullptr;
            tail = nodeptr;
        }

        resizer();
    }

    template <typename E>
    void
    PSLL<E>::push_front(E element){

        if (is_empty() && freelist == nullptr)
        {
            node * nodeptr = new node;
            nodeptr->next = nullptr;
            nodeptr->data = element;
            head = tail = nodeptr;
        }else if(is_empty() && freelist != nullptr){
            node * nodeptr = freelist;
            freelist = freelist->next;
            nodeptr->next = nullptr;
            nodeptr->data = element;
            head = tail = nodeptr;
            poolSize--;
        }else if(!is_empty() && freelist != nullptr){
            node * nodeptr = freelist;
            freelist = freelist->next;
            nodeptr->next = nullptr;
            nodeptr->next = head;
            nodeptr->data = element;
            head = nodeptr;
            poolSize--;
        }else
        {
            node * nodeptr = new node;
            nodeptr->next = head;
            nodeptr->data = element;
            head = nodeptr;
        }

        resizer();
    }

    template <typename E>
    E
    PSLL<E>::replace(E element, size_t position){
        size_t counter = 0;
        node * curr = head;
        E returnable;

        if (head == nullptr)
            throw std::runtime_error("PSLL<E>.replace(): list is empty");

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
            throw std::runtime_error("PSLL<E>.replace(): position is invalid");
        else
            return returnable;
    }

    template <typename E>
    E
    PSLL<E>::remove(size_t position){
        size_t counter = 0;
        node * curr = head;
        node * prev = curr;
        E returnable;

        if (head == nullptr)
            throw std::runtime_error("PSLL<E>.remove(): list is empty");

        while (curr)
        {
            if (counter == position)
            {
                returnable = curr->data;
                prev->next = prev->next->next;

                curr->next = freelist;
                freelist = curr;
                poolSize++;
                break;
            }
            prev = curr;
            curr = curr->next;
            counter++;
        }

        if (counter != position)
            throw std::runtime_error("PSLL<E>.remove(): position is invalid");
        else
        {
            resizer();
            return returnable;
        }
    }

    template <typename E>
    E
    PSLL<E>::pop_back(void){
        node * curr = head;
        node * prev = curr;
        E returnable = tail->data;

        if (head == nullptr)
            throw std::runtime_error("PSLL<E>.pop_back(): list is empty");

        while (curr)
        {
            if (curr->next == nullptr)
            {
                prev->next = nullptr;
                tail = prev;

                curr->next = freelist;
                freelist = curr;
                poolSize++;
                break;
            }
            prev = curr;
            curr = curr->next;
        }

        resizer();
        return returnable;
    }

    template <typename E>
    E
    PSLL<E>::pop_front(void){
        E returnable = head->data;
        node * curr = head;

        if (head == nullptr)
            throw std::runtime_error("PSLL<E>.pop_back(): list is empty");

        head = head->next;

        curr->next = freelist;
        freelist = curr;
        poolSize++;

        resizer();
        return returnable;
    }

    template <typename E>
    E&
    PSLL<E>::item_at(size_t position){
        size_t counter = 0;
        node * curr = head;

        if (head == nullptr)
            throw std::runtime_error("PSLL<E>.remove(): list is empty");

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
            throw std::runtime_error("PSLL<E>.remove(): position is invalid");
    }

    template <typename E>
    E&
    PSLL<E>::peek_back(void){
        return tail->data;
    }

    template <typename E>
    E&
    PSLL<E>::peek_front(void){
        return head->data;
    }

    template <typename E>
    bool
    PSLL<E>::is_empty(void){
        if (!head)
            return true;
        else
            return false;
    }

    template <typename E>
    bool
    PSLL<E>::is_full(void){
        return false;		//linked list can't be full
    }

    template <typename E>
    size_t
    PSLL<E>::length(void){
        size_t len = 0;
        node * curr = head;
        while (curr){
            ++len;
            curr = curr->next;
        }
        return len;
    }

    template <typename E>
    void
    PSLL<E>::resizer(){
        size_t len = length();
        size_t flen = poolSize;
        node * curr = freelist;

        if(len > 99 && flen > len *0.5 )
        {
            curr = freelist;
            node * prev = curr;

            while (flen > len *0.5)
            {
                prev = curr;
                curr = curr->next;
                delete prev;
                flen--;
            }

            freelist = curr;
        }
    }

    template <typename E>
    void
    PSLL<E>::clear(void){
        node * curr = head;
        node * prev = curr;

        while (curr){
            prev = curr;
            curr = curr->next;
            prev->next = freelist;
            freelist = prev;
        }
        head = tail = nullptr;
    }

    template <typename E>
    bool
    PSLL<E>::contains(E element, bool(*equals_function)(const E&,const E&)){
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

    template<typename E>
    void
    PSLL<E>::print(std::ostream& stream_name){
        node * curr = head;
        stream_name << "[";
        while (curr){
            stream_name << curr->data;
            if(curr->next != nullptr) stream_name<< ", ";
            curr = curr->next;
        }
        stream_name << "]\n";

    }

    template<typename E>
    void
    PSLL<E>::printFreeList() {
        node * curr = freelist;
        std::cout << "[";
        while (curr){
            std::cout << curr->data;
            if(curr->next != nullptr) std::cout<< ", ";
            curr = curr->next;
        }
        std::cout << "]\n";

    }

    template <typename E>
    E*
    PSLL<E>::contents(){
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
    };

}
#endif
