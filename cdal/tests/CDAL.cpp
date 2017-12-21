#include "CDAL.h"
using namespace cop3530;

int main()
{
    CDAL<int> list;

    std::cout << "Pushing at back\n";

    list.push_back(0);
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);
    list.push_back(5);
    list.push_back(6);
    list.push_back(7);
    list.push_back(8);
    list.push_back(9);
    list.push_back(10);
    list.insert(11,11);
    list.print(std::cout);
    std::cout << "Peek front value: " <<list.peek_front() << "\n";
    std::cout << "Peek back value: "<<list.peek_back() << "\n";

    list.clear();
    if(list.is_empty())
        std::cout << "list is empty\n\n";

    std::cout << "Pushing at front\n";


    list.push_front(0);
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);
    list.push_front(4);
    list.push_front(5);
    list.push_front(6);
    list.push_front(7);
    list.push_front(8);
    list.push_front(9);
    list.push_front(10);
    list.print(std::cout);
    std::cout << "Peek front value: " <<list.peek_front() << "\n";
    std::cout << "Peek back value: "<<list.peek_back() << "\n\n";

    std::cout << "Reversing order via replace \n";

    list.replace(0,0);
    list.replace(1,1);
    list.replace(2,2);
    list.replace(3,3);
    list.replace(4,4);
    list.replace(5,5);
    list.replace(6,6);
    list.replace(7,7);
    list.replace(8,8);
    list.replace(9,9);
    list.replace(10,10);
    list.print(std::cout);
    std::cout << "Peek front value: " <<list.peek_front() << "\n";
    std::cout << "Peek back value: "<<list.peek_back() << "\n\n";

    std::cout << "insertions\n";
    std::cout << "inserting 100 at 5\n";
    list.insert(100,5);
    list.print(std::cout);
    std::cout << "inserting 110 at 5\n";
    list.insert(110,5);
    list.print(std::cout);
    std::cout << "inserting 111 at 5\n";
    list.insert(111,5);
    list.print(std::cout);
    std::cout << "inserting 121 at 5\n";
    list.insert(121,5);
    list.print(std::cout);

    std::cout <<std::endl;

    std::cout << "removals\n";
    std::cout << "removing: " << list.remove(5)  << std::endl;
    list.print(std::cout);
    std::cout << "removing: " << list.remove(5)  << std::endl;
    list.print(std::cout);
    std::cout << "removing: " << list.remove(5)  << std::endl;
    list.print(std::cout);
    std::cout << "removing: " << list.remove(5)  << std::endl;
    list.print(std::cout);

    std::cout <<std::endl;

    std::cout << "Popping begins\n";

    std::cout << "List before pops:";
    list.print(std::cout);

    std::cout << "Popping back: "<< list.pop_back() << "\n";
    std::cout << "Popping front: " << list.pop_front()<< "\n";
    std::cout << "List after pops: ";
    list.print(std::cout);
    std::cout << "Popping back: "<< list.pop_back() << "\n";
    std::cout << "Popping front: " << list.pop_front()<< "\n";
    std::cout << "List after pops: ";
    list.print(std::cout);
    std::cout << "Popping back: "<< list.pop_back() << "\n";
    std::cout << "Popping front: " << list.pop_front()<< "\n";
    std::cout << "List after pops: ";
    list.print(std::cout);
    std::cout << "Popping back: "<< list.pop_back() << "\n";
    std::cout << "Popping front: " << list.pop_front()<< "\n";
    std::cout << "List after pops: ";
    list.print(std::cout);
    std::cout << "Peek back: "<< list.peek_back() << "\n";
    std::cout << "Peek front: " << list.peek_front()<< "\n";
    std::cout << "List after pops: ";
    list.print(std::cout);

    std::cout <<std::endl;

    std::cout << "Dynamic testing\n";

    list.clear();

    for(int i = 0; i < 300; i++)
    {
        list.push_back(i);
    }

    std::cout<< "after push: ";

    list.print(std::cout);

    for(int i = 0; i < 290; i++)
    {
        list.pop_front();
    }

    std::cout<< "\nafter pop: ";

    list.print(std::cout);

    std::cout << "\nPrint using item at\n";

    for(int i = 0; i < list.length(); i++)
    {
        std::cout << list.item_at(i) << "\n";
    }

    std::cout <<std::endl;

    std::cout << "Print using contents\n";

    int *please = list.contents();
    std::cout << "[";
    for(int i = 0; i < list.length(); i++)
    {
        std::cout << please[i];
        if(i < list.length() - 1)
            std::cout << ", ";
    }
    std::cout <<"]" << std::endl;

    std::cout <<std::endl;

	delete [] please;

    std::cout << "Begin Iterating\n";

    CDAL<int>::iterator citer = list.begin();
    CDAL<int>::iterator cend = list.end();

    for ( ; citer != cend ; ++citer ) {
        std::cout << citer.operator*() << std::endl;
    }

    std::cout <<std::endl;

    std::cout << "Begin Const Iter\n";

    const CDAL <int> newlist = list;

    CDAL<int>::const_iterator  citer2 = newlist.begin();
    CDAL<int>::const_iterator cend2 = newlist.end();

    for ( ; citer2 != cend2 ; ++citer2 ) {
        std::cout << citer2.operator*() << std::endl;
    }

    return 0;
}