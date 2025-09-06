#include "test.h"
#include <iostream>
#include "ClassicAssert.h"
#include "Utils/Math.h"

// Função auxiliar para imprimir os resultados do teste
void run_test(const std::string& test_name, bool result)
{
    std::cout << "[ " << (result ? "\x1B[32mPASSED\x1B[0m" : "\x1B[31mFAIL\x1B[0m") << " ] " << test_name << "\n";
}

void run_clamp()
{

    using namespace ClassicLauncher::Math;

    std::cout << "Starting tests for Clamp function...\n";
    std::cout << "---------------------------------------\n";

    // --- Integer Tests ---
    run_test("Integer: value inside range",           Clamp(5, 0, 10) == 5);
    run_test("Integer: value below minimum",          Clamp(-5, 0, 10) == 0);
    run_test("Integer: value above maximum",          Clamp(15, 0, 10) == 10);
    run_test("Integer: value equal to minimum",       Clamp(0, 0, 10) == 0);
    run_test("Integer: value equal to maximum",       Clamp(10, 0, 10) == 10);

    // --- Negative Range Tests ---
    run_test("Negative Range: value inside",          Clamp(-5, -10, -1) == -5);
    run_test("Negative Range: value below",           Clamp(-15, -10, -1) == -10);
    run_test("Negative Range: value above",           Clamp(0, -10, -1) == -1);

    // --- Floating Point Tests (float) ---
    run_test("Float: value inside range",             Clamp(5.5f, 0.0f, 10.0f) == 5.5f);
    run_test("Float: value below minimum",            Clamp(-2.5f, 0.0f, 10.0f) == 0.0f);
    run_test("Float: value above maximum",            Clamp(12.5f, 0.0f, 10.0f) == 10.0f);

    // --- Floating Point Tests (double) ---
    run_test("Double: value inside range",            Clamp(5.5, 0.0, 10.0) == 5.5);
    run_test("Double: value below minimum",           Clamp(-2.5, 0.0, 10.0) == 0.0);
    run_test("Double: value above maximum",           Clamp(12.5, 0.0, 10.0) == 10.0);

    // --- Mixed Types Tests ---
    run_test("Mixed Types (int, float, double)",      Clamp(15, 0.0f, 10.0) == 10.0);
    run_test("Mixed Types (float, int, int)",         Clamp(-5.5f, 0, 10) == 0.0f);
    run_test("Mixed Types (double, int, int)",        Clamp(7.8, 0, 10) == 7.8);
    
    // --- Unsigned int Tests ---
    run_test("(unsigned, unsigned, unsigned)",        Clamp(6u, 0u, 100u) == 6u);
    run_test("(unsigned, unsigned, unsigned)",        Clamp(6u, 9u, 100u) == 9u);
    run_test("(unsigned, unsigned, unsigned)",        Clamp(100u, 9u, 100u) == 100u);
    run_test("(unsigned, unsigned, unsigned)",        Clamp(1000u, 9u, 100u) == 100u);

    // run_test("Mixed Types (unsigned, int, int)",     Clamp(20u, -5, 10) == 10);  this test will trigger assert
    // run_test("Mixed Types (int, unsigned, unsigned)",Clamp(-10, 0u, 100u) == 0); this test will trigger assert

    // --- Edge Case ---
    run_test("Edge Case: min == max",                 Clamp(100, 5, 5) == 5);

    std::cout << "---------------------------------------\n";
    std::cout << "Tests finished.\n";
}

