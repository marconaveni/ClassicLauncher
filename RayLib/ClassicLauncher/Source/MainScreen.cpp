#include "MainScreen.h"

#include "ObjectManager.h"
#include "TextureManager.h"

MainScreen::MainScreen()
{
    grid = std::make_shared<Grid>(TextureManager::GetInstance()->GetSprite("ref"), Vector2{ 0, 0 }, Rectangle{ 0, 0, 1280, 720 });
    miniCovers = std::make_shared<MiniCovers>();
}

MainScreen::~MainScreen() = default;

void MainScreen::BeginPlay()
{
    Object::BeginPlay();
}

void MainScreen::Tick()
{
    //Object::Tick();
}

void MainScreen::Draw()
{
    //Object::Draw();
}

void MainScreen::EndDraw()
{
    //Object::EndDraw();
}

void MainScreen::EndPlay()
{
    //Object::EndPlay();
}

void MainScreen::Collision()
{
    //Object::Collision();
}
