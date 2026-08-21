#ifndef TEST_H
#define TEST_H


#include <string>

#define _DEBUG


void run_test(const std::string& test_name, bool result);


void run_clamp();
void run_entity_setters_getters();
void run_vector2_size_tests();
void run_vector2_size_cast_tests();
void run_entity_extra_setters_getters();

#endif