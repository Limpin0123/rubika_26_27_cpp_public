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

#include "Entity.h"
#include "Scene.h"
#include "SceneMgr.h"

#include "TransformComponent.h"
#include "RenderComponent.h"
#include "StaticDrawable.h"
#include "AnimatedDrawable.h"

int main()
{
    gData.Init();

    sf::RenderWindow window(sf::VideoMode({ 1080, 720 }), "SFML works!", sf::State::Windowed);

    ImGui::SFML::Init(window);


    sf::Clock clock;
    clock.restart();

    /*### DEBUG AND TESTING ###*/
    gData.SceneManager->AddAndLoadScene("Debug_Scene");
    gData.TextureManager->LoadTexture("C:\\Github\\rubika_26_27_cpp_public\\MyEngine\\Ressources\\IsaacSprite.png");


    Player* player = new Player();
    gData.SceneManager->activeScene->AddEntity(player);
    RenderComponent* rc = player->AddComponent<RenderComponent>();
    AnimatedDrawable* ad = rc->AddDrawable<AnimatedDrawable>();
    ad->SetTextureAndAnimation("IsaacSprite.png", "Body_Left");

    //Input Action Register
    InputAction* iaUp = new InputAction("Up");
    InputAction* iaDown = new InputAction("Down");
    InputAction* iaLeft = new InputAction("Left");
    InputAction* iaRight = new InputAction("Right");

    gData.InputManager->AddInputAction(iaUp);
    gData.InputManager->AddInputAction(iaDown);
    gData.InputManager->AddInputAction(iaLeft);
    gData.InputManager->AddInputAction(iaRight);

    gData.InputManager->LoadBindings();

    /*gData.InputManager->BindIAToKey(sf::Keyboard::Key::Up, iaUp->GetName());
    gData.InputManager->BindIAToKey(sf::Keyboard::Key::Down, iaDown->GetName());
    gData.InputManager->BindIAToKey(sf::Keyboard::Key::Left, iaLeft->GetName());
    gData.InputManager->BindIAToKey(sf::Keyboard::Key::Right, iaRight->GetName());*/

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
    /*### ###*/

    while (window.isOpen())
    {
        gData.IncrementFrameCount();

        PROFILER_EVENT_BEGIN(PROFILER_COLOR_BLACK, "Frame %llu", gData.FrameCount);

        int deltaTimeMicroS = clock.getElapsedTime().asMicroseconds();
        float fDeltaTimeMS = (float)deltaTimeMicroS / 1000.f;
        float fDeltaTimeS = fDeltaTimeMS / 1000.f;
        sf::Time imGuiTime = clock.restart();

        PROFILER_EVENT_BEGIN(PROFILER_COLOR_BLUE, "Event & Input");

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

        PROFILER_EVENT_END();

        PROFILER_EVENT_BEGIN(PROFILER_COLOR_RED, "Update");
#ifdef _USE_IMGUI
        ImGui::SFML::Update(window, imGuiTime);
#endif

        // sample
        //ImGui::ShowDemoWindow();
        //Logger::DrawLogger();
        gData.DebugMgr->Draw();

        PROFILER_EVENT_END();

        PROFILER_EVENT_BEGIN(PROFILER_COLOR_GREEN, "Draw");

        gData.InputManager->Update(fDeltaTimeS);
        gData.SceneManager->UpdateActiveScene(fDeltaTimeS);

        window.clear();
        gData.SceneManager->DrawActiveScene(window);
#ifdef _USE_IMGUI
        ImGui::SFML::Render(window);
#endif
        window.display();

        PROFILER_EVENT_END();

        PROFILER_EVENT_END();
    }

    ImGui::SFML::Shutdown();

    gData.Shut();
    gData.Destroy();

    return 0;
}