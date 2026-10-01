#include <iostream>


inline bool Bitset(unsigned int bit,int number)
{
    int filter = 0 | (1 << bit);
    return ((filter & number) != 0);
}



int main()
{
    int t = 165;

    std::cout << std::boolalpha;

    std::cout << Bitset(0,t) << std::endl;
    std::cout << Bitset(1,t) << std::endl;
    std::cout << Bitset(2,t) << std::endl;
    std::cout << Bitset(3,t) << std::endl;
    std::cout << Bitset(4,t) << std::endl;
    std::cout << Bitset(5,t) << std::endl;
    std::cout << Bitset(6,t) << std::endl;
    std::cout << Bitset(7,t) << std::endl;
    std::cout << Bitset(8,t) << std::endl;
    std::cout << Bitset(9,t) << std::endl;
    std::cout << Bitset(10,t) << std::endl;

    
}