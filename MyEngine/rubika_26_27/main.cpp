#include <SFML/Graphics.hpp>
#include <Imgui/imgui.h>
#include <Imgui/imgui-SFML.h>

#include <Globals.h>
#include <Profiler.h>
#include <Logger.h>
#include <DebugMgr.h>
#include <TextureMgr.h>

#include <inputMgr.h>
#include "InputAction.h"
#include "Player.h"
#include "Obstacle.h"

#include "Entity.h"
#include "Scene.h"
#include "SceneMgr.h"

#include "PhysicMgr.h"

int main()
{
#pragma region INITIALIZATION

    gData.Init();

    sf::RenderWindow window(sf::VideoMode({ 1080, 720 }), "SFML works!", sf::State::Windowed);

    ImGui::SFML::Init(window);


    sf::Clock clock;
    clock.restart();

#pragma region TEXTURE INITIALIZATION
    gData.TextureManager->LoadTexture("C:\\Github\\rubika_26_27_cpp_public\\MyEngine\\Ressources\\IsaacSprite.png");
    gData.TextureManager->LoadTexture("C:\\Github\\rubika_26_27_cpp_public\\MyEngine\\Ressources\\Basement.png");
#pragma endregion
#pragma region SCENE INITIALIZATION
    gData.SceneManager->AddAndLoadScene("Debug_Scene");

    Player* player = new Player();
    gData.SceneManager->activeScene->AddEntity(player);
    for (int i = 0; i < 5000; i++)
    {
        gData.SceneManager->activeScene->AddEntity(new Obstacle(sf::Vector2f(100, 100), "Obstacle" + std::to_string(i)));
    }
#pragma endregion
#pragma region INPUT INITIALIZATION

    InputAction* iaUp = new InputAction("Up");
    InputAction* iaDown = new InputAction("Down");
    InputAction* iaLeft = new InputAction("Left");
    InputAction* iaRight = new InputAction("Right");

    gData.InputManager->AddInputAction(iaUp);
    gData.InputManager->AddInputAction(iaDown);
    gData.InputManager->AddInputAction(iaLeft);
    gData.InputManager->AddInputAction(iaRight);

    gData.InputManager->LoadBindings();

    std::function<void(float)> UpLambda = [&player](float value) -> void
        {
            player->MoveUp(value);
        };
    std::function<void(float)> DownLambda = [&player](float value) -> void
        {
            player->MoveDown(value);
        };
    std::function<void(float)> LeftLambda = [&player](float value) -> void
        {
            player->MoveLeft(value);
        };
    std::function<void(float)> RightLambda = [&player](float value) -> void
        {
            player->MoveRight(value);
        };

    //New way of creating the lambda
    //gData.InputManager->RegisterToIA(iaUp->GetName(), "Move Up", std::bind(&Player::MoveUp, &player, std::placeholders::_1), &player);

    gData.InputManager->RegisterToIA(iaUp->GetName(), "Move Up", UpLambda, &player);
    gData.InputManager->RegisterToIA(iaDown->GetName(), "Move Down", DownLambda, &player);
    gData.InputManager->RegisterToIA(iaLeft->GetName(), "Move Left", LeftLambda, &player);
    gData.InputManager->RegisterToIA(iaRight->GetName(), "Move Right", RightLambda, &player);
#pragma endregion
#pragma endregion

    while (window.isOpen())
    {
        gData.IncrementFrameCount();

        PROFILER_EVENT_BEGIN(PROFILER_COLOR_BLACK, "Frame %llu", gData.FrameCount);

        int deltaTimeMicroS = clock.getElapsedTime().asMicroseconds();
        float fDeltaTimeMS = (float)deltaTimeMicroS / 1000.f;
        float fDeltaTimeS = fDeltaTimeMS / 1000.f;
        sf::Time imGuiTime = clock.restart();

#pragma region INPUTS
        PROFILER_EVENT_BEGIN(PROFILER_COLOR_RED, "Event & Input");
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

#ifdef _USE_IMGUI
            ImGui::SFML::ProcessEvent(window, event.value());
#endif
        }
        PROFILER_EVENT_BEGIN(PROFILER_COLOR_ORANGE, "Input Update");
        gData.InputManager->Update(fDeltaTimeS);
        PROFILER_EVENT_END();

        PROFILER_EVENT_END();
#pragma endregion

#pragma region UPDATE
        PROFILER_EVENT_BEGIN(PROFILER_COLOR_DARK_BLUE, "Update");
#ifdef _USE_IMGUI
        ImGui::SFML::Update(window, imGuiTime);
#endif
        PROFILER_EVENT_BEGIN(PROFILER_COLOR_BLUE, "Scene Update");
        gData.SceneManager->UpdateActiveScene(fDeltaTimeS);
        PROFILER_EVENT_END();

        PROFILER_EVENT_BEGIN(PROFILER_COLOR_BLUE, "Physic Update");
        gData.PhysicManager->Update(fDeltaTimeS);
        PROFILER_EVENT_END();

        PROFILER_EVENT_END();
#pragma endregion

#pragma region DRAW
        PROFILER_EVENT_BEGIN(PROFILER_COLOR_DARK_GREEN, "Draw");
        window.clear();

        PROFILER_EVENT_BEGIN(PROFILER_COLOR_GREEN, "ImGUI Drawing");
        gData.DebugMgr->Draw();
        PROFILER_EVENT_END();
        PROFILER_EVENT_BEGIN(PROFILER_COLOR_GREEN, "Scene Drawing");
        gData.SceneManager->DrawActiveScene(window);
        PROFILER_EVENT_END();
#ifdef _USE_IMGUI
        ImGui::SFML::Render(window);
#endif
        window.display();

        PROFILER_EVENT_END();
#pragma endregion

        PROFILER_EVENT_END();
    }

    ImGui::SFML::Shutdown();

    gData.Shut();
    gData.Destroy();

    return 0;
}