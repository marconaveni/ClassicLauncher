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

    std::cout << "Teste Starting Clamp tests...\n";
    std::cout << "---------------------------------------\n";

    // --- Integer Tests ---
    run_test("Clamp<int>: 5 within [0, 10] returns 5", Clamp(5, 0, 10) == 5);
    run_test("Clamp<int>: -5 below [0, 10] returns 0", Clamp(-5, 0, 10) == 0);
    run_test("Clamp<int>: 15 above [0, 10] returns 10", Clamp(15, 0, 10) == 10);
    run_test("Clamp<int>: lower bound 0 is preserved", Clamp(0, 0, 10) == 0);
    run_test("Clamp<int>: upper bound 10 is preserved", Clamp(10, 0, 10) == 10);

    // --- Negative Range Tests ---
    run_test("Clamp<int>: -5 within [-10, -1] returns -5", Clamp(-5, -10, -1) == -5);
    run_test("Clamp<int>: -15 below [-10, -1] returns -10", Clamp(-15, -10, -1) == -10);
    run_test("Clamp<int>: 0 above [-10, -1] returns -1", Clamp(0, -10, -1) == -1);

    // --- Floating Point Tests (float) ---
    run_test("Clamp<float>: 5.5 within [0.0, 10.0] returns 5.5", Clamp(5.5f, 0.0f, 10.0f) == 5.5f);
    run_test("Clamp<float>: -2.5 below [0.0, 10.0] returns 0.0", Clamp(-2.5f, 0.0f, 10.0f) == 0.0f);
    run_test("Clamp<float>: 12.5 above [0.0, 10.0] returns 10.0", Clamp(12.5f, 0.0f, 10.0f) == 10.0f);

    // --- Floating Point Tests (double) ---
    run_test("Clamp<double>: 5.5 within [0.0, 10.0] returns 5.5", Clamp(5.5, 0.0, 10.0) == 5.5);
    run_test("Clamp<double>: -2.5 below [0.0, 10.0] returns 0.0", Clamp(-2.5, 0.0, 10.0) == 0.0);
    run_test("Clamp<double>: 12.5 above [0.0, 10.0] returns 10.0", Clamp(12.5, 0.0, 10.0) == 10.0);

    // --- Mixed Types Tests ---
    run_test("Clamp<mixed>: int 15 with float/double bounds returns 10.0", Clamp(15, 0.0f, 10.0) == 10.0);
    run_test("Clamp<mixed>: float -5.5 with int bounds returns 0.0", Clamp(-5.5f, 0, 10) == 0.0f);
    run_test("Clamp<mixed>: double 7.8 with int bounds returns 7.8", Clamp(7.8, 0, 10) == 7.8);

    // --- Unsigned int Tests ---
    run_test("Clamp<unsigned>: 6 within [0, 100] returns 6", Clamp(6u, 0u, 100u) == 6u);
    run_test("Clamp<unsigned>: 6 below [9, 100] returns 9", Clamp(6u, 9u, 100u) == 9u);
    run_test("Clamp<unsigned>: 100 at upper bound is preserved", Clamp(100u, 9u, 100u) == 100u);
    run_test("Clamp<unsigned>: 1000 above [9, 100] returns 100", Clamp(1000u, 9u, 100u) == 100u);

    // run_test("Mixed Types (unsigned, int, int)",     Clamp(20u, -5, 10) == 10);  this test will trigger assert
    // run_test("Mixed Types (int, unsigned, unsigned)",Clamp(-10, 0u, 100u) == 0); this test will trigger assert

    // --- Edge Case ---
    run_test("Clamp edge case: equal bounds [5, 5] always return 5", Clamp(100, 5, 5) == 5);

    std::cout << "---------------------------------------\n";
    std::cout << "Clamp tests finished.\n";
}

void run_entity_setters_getters()
{
    using namespace ClassicLauncher;

    EntityContext constext;
    GuiBase entity{constext};

    std::cout << "Starting GuiBase position and size tests...\n";
    std::cout << "---------------------------------------\n";

    entity.SetPosition(Vector2f{10, 10});
    run_test("GuiBase::SetPosition stores {10, 10}", entity.GetPosition() == Vector2f{10, 10});
    run_test("GuiBase position differs from {30, 10}", entity.GetPosition() != Vector2f{30, 10});
    run_test("GuiBase position differs from {10, 20}", entity.GetPosition() != Vector2f{10, 20});
    run_test("GuiBase position.x is 10 after SetPosition", entity.GetPosition().x == 10);
    run_test("GuiBase position.y is 10 after SetPosition", entity.GetPosition().y == 10);

    entity.SetSize(300, 300);
    run_test("GuiBase::SetSize stores {300, 300}", entity.GetSize() == Sizef{300, 300});
    run_test("GuiBase size differs from {300, 10}", entity.GetSize() != Sizef{300, 10});
    run_test("GuiBase size differs from {30, 300}", entity.GetSize() != Sizef{30, 300});
    run_test("GuiBase width is 300 after SetSize", entity.GetSize().width == 300);
    run_test("GuiBase height is 300 after SetSize", entity.GetSize().height == 300);
    run_test("GuiBase width is not 320 after SetSize", entity.GetSize().width != 320);

    std::cout << "---------------------------------------\n";
    std::cout << "GuiBase position and size tests finished.\n";
}

void run_vector2_size_tests()
{
    using namespace ClassicLauncher;

    std::cout << "Starting Vector2 and Size tests...\n";
    std::cout << "---------------------------------------\n";

    // --- Vector2 basic tests ---
    Vector2i v1(10, 20);
    run_test("Vector2i(10, 20) stores x=10 and y=20", v1.x == 10 && v1.y == 20);

    Vector2i v2(5);
    run_test("Vector2i(5) duplicates the value to both components", v2.x == 5 && v2.y == 5);

    run_test("Vector2i equality compares both components", v1 == Vector2i(10, 20));
    run_test("Vector2i inequality detects different x/y values", v1 != Vector2i(0, 20));

    run_test("Vector2i addition sums both components", (v1 + Vector2i(5, 5)) == Vector2i(15, 25));
    run_test("Vector2i subtraction subtracts both components", (v1 - Vector2i(5, 5)) == Vector2i(5, 15));
    run_test("Vector2i scalar multiplication scales both components", (v1 * 2) == Vector2i(20, 40));
    run_test("Vector2i scalar division divides both components", (v1 / 2) == Vector2i(5, 10));

    // --- Size basic tests ---
    Sizei s1(30, 40);
    run_test("Sizei(30, 40) stores width=30 and height=40", s1.width == 30 && s1.height == 40);

    Sizei s2(7);
    run_test("Sizei(7) duplicates the value to both dimensions", s2.width == 7 && s2.height == 7);

    run_test("Sizei equality compares width and height", s1 == Sizei(30, 40));
    run_test("Sizei inequality detects different dimensions", s1 != Sizei(40, 40));

    run_test("Sizei addition sums width and height", (s1 + Sizei(10, 10)) == Sizei(40, 50));
    run_test("Sizei subtraction subtracts width and height", (s1 - Sizei(10, 10)) == Sizei(20, 30));
    run_test("Sizei scalar multiplication scales both dimensions", (s1 * 2) == Sizei(60, 80));
    run_test("Sizei scalar division divides both dimensions", (s1 / 2) == Sizei(15, 20));

    // --- Cross conversion tests ---
    Vector2i v_from_size(s1);
    run_test("Vector2i can be constructed from Sizei", v_from_size == Vector2i(30, 40));

    Sizei s_from_vector(v1);
    run_test("Sizei can be constructed from Vector2i", s_from_vector == Sizei(10, 20));

    std::cout << "---------------------------------------\n";
    std::cout << "Vector2 and Size tests finished.\n";
}


void run_vector2_size_cast_tests()
{
    using namespace ClassicLauncher;

    std::cout << "Starting Vector2/Size cast helper tests...\n";
    std::cout << "---------------------------------------\n";

    // --- Vector2 casts ---
    Vector2f vf(10.7f, 20.9f);
    Vector2i vi = vf.ToInt();
    run_test("Vector2f::ToInt truncates {10.7, 20.9} to {10, 20}", vi == Vector2i(10, 20));

    Vector2i vi2(42, 99);
    Vector2f vf2 = vi2.ToFloat();
    run_test("Vector2i::ToFloat converts integers to float components", vf2 == Vector2f(42.0f, 99.0f));

    Vector2i vi3(123, 456);
    Vector2d vd = vi3.ToDouble();
    run_test("Vector2i::ToDouble converts integers to double components", vd == Vector2d(123.0, 456.0));

    // --- Size casts ---
    Sizef sf(7.9f, 15.1f);
    Sizei si = sf.ToInt();
    run_test("Sizef::ToInt truncates {7.9, 15.1} to {7, 15}", si == Sizei(7, 15));

    Sizei si2(640, 480);
    Sizef sf2 = si2.ToFloat();
    run_test("Sizei::ToFloat converts integers to float dimensions", sf2 == Sizef(640.0f, 480.0f));

    Sizei si3(800, 600);
    Sized sd = si3.ToDouble();
    run_test("Sizei::ToDouble converts integers to double dimensions", sd == Sized(800.0, 600.0));

    std::cout << "---------------------------------------\n";
    std::cout << "Vector2/Size cast helper tests finished.\n";
}


void run_entity_extra_setters_getters()
{
    using namespace ClassicLauncher;

    EntityContext constext;
    GuiBase entity{constext};

    std::cout << "Starting GuiBase transform and color tests...\n";
    std::cout << "---------------------------------------\n";

    // --- Offset ---
    entity.SetOffset(15, 25);
    run_test("GuiBase::SetOffset stores {15, 25}", entity.GetOffset() == Vector2f{15, 25});

    // --- Origin ---
    entity.SetOrigin(5, 7);
    run_test("GuiBase::SetOrigin stores {5, 7}", entity.GetOrigin() == Vector2f{5, 7});

    // --- Scale ---
    entity.SetScale(2.0f, 3.0f);
    run_test("GuiBase::SetScale stores {2.0, 3.0}", entity.GetScale() == Vector2f{2.0f, 3.0f});

    // --- Rotation ---
    entity.SetRotation(45.0f);
    run_test("GuiBase::SetRotation stores 45.0f", entity.GetRotation() == 45.0f);

    // --- Color ---
    entity.SetColor(100, 150, 200, 255);
    run_test("GuiBase::SetColor stores RGBA {100, 150, 200, 255}", entity.GetColor() == Color{100, 150, 200, 255});
    entity.SetColorRed(10);
    run_test("GuiBase::SetColorRed updates the red channel to 10", entity.GetColor().r == 10);
    entity.SetColorGreen(20);
    run_test("GuiBase::SetColorGreen updates the green channel to 20", entity.GetColor().g == 20);
    entity.SetColorBlue(30);
    run_test("GuiBase::SetColorBlue updates the blue channel to 30", entity.GetColor().b == 30);
    entity.SetOpacity(128);
    run_test("GuiBase::SetOpacity updates alpha to 128", entity.GetColor().a == 128);


    std::cout << "---------------------------------------\n";
    std::cout << "GuiBase transform and color tests finished.\n";
}
