#include "test.h"

#include <iostream>

#include "ClassicAssert.h"
#include "Guis/GuiBase.h"
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
    run_test("Integer: value inside range", Clamp(5, 0, 10) == 5);
    run_test("Integer: value below minimum", Clamp(-5, 0, 10) == 0);
    run_test("Integer: value above maximum", Clamp(15, 0, 10) == 10);
    run_test("Integer: value equal to minimum", Clamp(0, 0, 10) == 0);
    run_test("Integer: value equal to maximum", Clamp(10, 0, 10) == 10);

    // --- Negative Range Tests ---
    run_test("Negative Range: value inside", Clamp(-5, -10, -1) == -5);
    run_test("Negative Range: value below", Clamp(-15, -10, -1) == -10);
    run_test("Negative Range: value above", Clamp(0, -10, -1) == -1);

    // --- Floating Point Tests (float) ---
    run_test("Float: value inside range", Clamp(5.5f, 0.0f, 10.0f) == 5.5f);
    run_test("Float: value below minimum", Clamp(-2.5f, 0.0f, 10.0f) == 0.0f);
    run_test("Float: value above maximum", Clamp(12.5f, 0.0f, 10.0f) == 10.0f);

    // --- Floating Point Tests (double) ---
    run_test("Double: value inside range", Clamp(5.5, 0.0, 10.0) == 5.5);
    run_test("Double: value below minimum", Clamp(-2.5, 0.0, 10.0) == 0.0);
    run_test("Double: value above maximum", Clamp(12.5, 0.0, 10.0) == 10.0);

    // --- Mixed Types Tests ---
    run_test("Mixed Types (int, float, double)", Clamp(15, 0.0f, 10.0) == 10.0);
    run_test("Mixed Types (float, int, int)", Clamp(-5.5f, 0, 10) == 0.0f);
    run_test("Mixed Types (double, int, int)", Clamp(7.8, 0, 10) == 7.8);

    // --- Unsigned int Tests ---
    run_test("(unsigned, unsigned, unsigned)", Clamp(6u, 0u, 100u) == 6u);
    run_test("(unsigned, unsigned, unsigned)", Clamp(6u, 9u, 100u) == 9u);
    run_test("(unsigned, unsigned, unsigned)", Clamp(100u, 9u, 100u) == 100u);
    run_test("(unsigned, unsigned, unsigned)", Clamp(1000u, 9u, 100u) == 100u);

    // run_test("Mixed Types (unsigned, int, int)",     Clamp(20u, -5, 10) == 10);  this test will trigger assert
    // run_test("Mixed Types (int, unsigned, unsigned)",Clamp(-10, 0u, 100u) == 0); this test will trigger assert

    // --- Edge Case ---
    run_test("Edge Case: min == max", Clamp(100, 5, 5) == 5);

    std::cout << "---------------------------------------\n";
    std::cout << "Tests Clamp finished.\n";
}

void run_entity_setters_getters()
{
    using namespace ClassicLauncher;

    GuiBase entity;

    std::cout << "Starting tests for Getter Setters Entity function...\n";
    std::cout << "---------------------------------------\n";

    entity.SetPosition(Vector2f{10, 10});
    run_test("(entity position Vector2f{10, 10} == other Vector2f{10, 10})", entity.GetPosition() == Vector2f{10, 10});
    run_test("(entity position Vector2f{10, 10} != other Vector2f{30, 10}))", entity.GetPosition() != Vector2f{30, 10});
    run_test("(entity position Vector2f{10, 10} != other Vector2f{10, 20})", entity.GetPosition() != Vector2f{10, 20});
    run_test("(entity position x == v)", entity.GetPosition().x == 10);
    run_test("(entity position y == v)", entity.GetPosition().y == 10);
    entity.GetPositionRef().x = 40;
    run_test("(entity position x modify getterRef)", entity.GetPosition().x == 40);

    entity.SetSize(300, 300);
    run_test("(entity size Sizef{300, 300} == other Sizef{300, 300})", entity.GetSize() == Sizef{300, 300});
    run_test("(entity size Sizef{300, 300} != other Sizef{300, 10}))", entity.GetSize() != Sizef{300, 10});
    run_test("(entity size Sizef{10, 10} != other Sizef{30, 300})", entity.GetSize() != Sizef{30, 300});
    run_test("(entity size x == v)", entity.GetSize().width == 300);
    run_test("(entity size x == v)", entity.GetSize().height == 300);
    entity.GetPositionRef().width = 800;
    run_test("(entity size x modify getterRef)", entity.GetSize().width == 800);
    run_test("(entity size x != v)", entity.GetSize().width != 300);

    std::cout << "---------------------------------------\n";
    std::cout << "Tests Getter Setters Entity finished.\n";
}

void run_vector2_size_tests()
{
    using namespace ClassicLauncher;

    std::cout << "Starting tests for Vector2 and Size...\n";
    std::cout << "---------------------------------------\n";

    // --- Vector2 basic tests ---
    Vector2i v1(10, 20);
    run_test("Vector2 constructor values", v1.x == 10 && v1.y == 20);

    Vector2i v2(5);
    run_test("Vector2 single value constructor", v2.x == 5 && v2.y == 5);

    run_test("Vector2 equality", v1 == Vector2i(10, 20));
    run_test("Vector2 inequality", v1 != Vector2i(0, 20));

    run_test("Vector2 addition", (v1 + Vector2i(5, 5)) == Vector2i(15, 25));
    run_test("Vector2 subtraction", (v1 - Vector2i(5, 5)) == Vector2i(5, 15));
    run_test("Vector2 scalar multiplication", (v1 * 2) == Vector2i(20, 40));
    run_test("Vector2 scalar division", (v1 / 2) == Vector2i(5, 10));

    // --- Size basic tests ---
    Sizei s1(30, 40);
    run_test("Size constructor values", s1.width == 30 && s1.height == 40);

    Sizei s2(7);
    run_test("Size single value constructor", s2.width == 7 && s2.height == 7);

    run_test("Size equality", s1 == Sizei(30, 40));
    run_test("Size inequality", s1 != Sizei(40, 40));

    run_test("Size addition", (s1 + Sizei(10, 10)) == Sizei(40, 50));
    run_test("Size subtraction", (s1 - Sizei(10, 10)) == Sizei(20, 30));
    run_test("Size scalar multiplication", (s1 * 2) == Sizei(60, 80));
    run_test("Size scalar division", (s1 / 2) == Sizei(15, 20));

    // --- Cross conversion tests ---
    Vector2i v_from_size(s1);
    run_test("Vector2 from Size constructor", v_from_size == Vector2i(30, 40));

    Sizei s_from_vector(v1);
    run_test("Size from Vector2 constructor", s_from_vector == Sizei(10, 20));

    std::cout << "---------------------------------------\n";
    std::cout << "Tests Vector2 and Size finished.\n";
}


void run_vector2_size_cast_tests()
{
    using namespace ClassicLauncher;

    std::cout << "Starting tests for Vector2/Size cast helpers...\n";
    std::cout << "---------------------------------------\n";

    // --- Vector2 casts ---
    Vector2f vf(10.7f, 20.9f);
    Vector2i vi = vf.ToInt();
    run_test("Vector2f -> ToInt()", vi == Vector2i(10, 20));

    Vector2i vi2(42, 99);
    Vector2f vf2 = vi2.ToFloat();
    run_test("Vector2i -> ToFloat()", vf2 == Vector2f(42.0f, 99.0f));

    Vector2i vi3(123, 456);
    Vector2d vd = vi3.ToDouble();
    run_test("Vector2i -> ToDouble()", vd == Vector2d(123.0, 456.0));

    // --- Size casts ---
    Sizef sf(7.9f, 15.1f);
    Sizei si = sf.ToInt();
    run_test("Sizef -> ToInt()", si == Sizei(7, 15));

    Sizei si2(640, 480);
    Sizef sf2 = si2.ToFloat();
    run_test("Sizei -> ToFloat()", sf2 == Sizef(640.0f, 480.0f));

    Sizei si3(800, 600);
    Sized sd = si3.ToDouble();
    run_test("Sizei -> ToDouble()", sd == Sized(800.0, 600.0));

    std::cout << "---------------------------------------\n";
    std::cout << "Tests Vector2/Size cast helpers finished.\n";
}


void run_entity_extra_setters_getters()
{
    using namespace ClassicLauncher;

    GuiBase entity;

    std::cout << "Starting extra tests for Getter Setters Entity... \n";
    std::cout << "---------------------------------------\n";

    // --- Offset ---
    entity.SetOffset(15, 25);
    run_test("(entity offset == Vector2f{15, 25})", entity.GetOffset() == Vector2f{15, 25});
    entity.GetOffsetRef().x = 50;
    run_test("(entity offset x modified by ref)", entity.GetOffset().x == 50);

    // --- Origin ---
    entity.SetOrigin(5, 7);
    run_test("(entity origin == Vector2f{5, 7})", entity.GetOrigin() == Vector2f{5, 7});
    entity.GetOriginRef().y = 99;
    run_test("(entity origin y modified by ref)", entity.GetOrigin().y == 99);

    // --- Scale ---
    entity.SetScale(2.0f, 3.0f);
    run_test("(entity scale == Vector2f{2.0, 3.0})", entity.GetScale() == Vector2f{2.0f, 3.0f});
    entity.GetScaleRef().x = 10.0f;
    run_test("(entity scale x modified by ref)", entity.GetScale().x == 10.0f);

    // --- Rotation ---
    entity.SetRotation(45.0f);
    run_test("(entity rotation == 45.0f)", entity.GetRotation() == 45.0f);
    entity.GetRotationRef() = 90.0f;
    run_test("(entity rotation modified by ref)", entity.GetRotation() == 90.0f);

    // --- Color ---
    entity.SetColor(100, 150, 200, 255);
    run_test("(entity color == Color{100,150,200,255})", entity.GetColor() == Color{100,150,200,255});
    entity.SetColorRed(10);
    run_test("(entity color red == 10)", entity.GetColor().r == 10);
    entity.SetColorGreen(20);
    run_test("(entity color green == 20)", entity.GetColor().g == 20);
    entity.SetColorBlue(30);
    run_test("(entity color blue == 30)", entity.GetColor().b == 30);
    entity.SetOpacity(128);
    run_test("(entity opacity == 128)", entity.GetColor().a == 128);

    entity.GetColorRef().r = 255;
    run_test("(entity color red modified by ref)", entity.GetColor().r == 255);

    std::cout << "---------------------------------------\n";
    std::cout << "Extra Getter/Setter tests finished.\n";
}


