#include <iostream>

#include "test.h"

int main()
{

    std::cout << "Starting tests function...\n";
    std::cout << "---------------------------------------\n";

    run_clamp();
    run_entity_setters_getters();
    run_vector2_size_tests();
    run_vector2_size_cast_tests();
    run_entity_extra_setters_getters();

    std::cout << "---------------------------------------\n";
    std::cout << "Tests finished.\n";
    return 0;
}

//  g++ -o ./Test/main.exe -std=c++20 ./Test/main.cpp ./Test/test.cpp ./Source/Guis/GuiBase.cpp ./Source/Entity/Entity.cpp  -I./Source

//  g++ -o ./Test/main.out -std=c++20 ./Test/main.cpp ./Test/test.cpp ./Source/Guis/GuiBase.cpp ./Source/Entity/Entity.cpp  -I./Source

//  ./Test/main.out