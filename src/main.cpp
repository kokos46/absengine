#include <iostream>
#include <raylib.h>
#include <vector>
#include "include/SceneManager.h"
#include "include/Player.h"

bool isFullscreen = false; // Состояние полноэкранного режима

void ToggleFullscreenMode() {
    isFullscreen = !isFullscreen;

    if (isFullscreen) {
        // 1. Убираем рамки
        SetWindowState(FLAG_WINDOW_UNDECORATED);
        // 2. Растягиваем на весь экран
        int monitor = GetCurrentMonitor();
        SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
        SetWindowPosition(0, 0);
    } else {
        // 1. Возвращаем рамки
        ClearWindowState(FLAG_WINDOW_UNDECORATED);
        // 2. Устанавливаем исходный размер (например, 1366x768)
        SetWindowSize(1366, 768);
        // 3. Можно отцентрировать окно
        SetWindowPosition(100, 100);
    }
}

// Данные сцен
std::vector testScene1Platforms = {
    Entity({ 0, 400, 800, 50 }),
    Entity({ 200, 300, 200, 20 }),
    Entity({ 500, 200, 200, 20 }),
    Entity({ 100, 150, 150, 20 })
};

std::vector testScene2Platforms = {
    Entity({ 0, 400, 800, 50 }),
    Entity({ 200, 300, 200, 20 })
};

constexpr float moveSpeed = 300.0f;
constexpr float jumpForce = 600.0f;
constexpr float gravity = 1800.0f;

int main() {
    const int screenWidth = 1366;
    const int screenHeight = 768;
    InitWindow(screenWidth, screenHeight, "Raylib Platformer - Finalized");

    // Инициализация сцен
    Scene testScene1(100, 300); testScene1.SetSurfaces(testScene1Platforms);
    Scene testScene(600, 300);  testScene.SetSurfaces(testScene2Platforms);

    SceneManager sceneManager(testScene);

    Player testPlayer(Rectangle{ sceneManager.GetCurrentScene().GetPlayerPos().x,
                                 sceneManager.GetCurrentScene().GetPlayerPos().y, 40, 40 }, moveSpeed, jumpForce, gravity);

    Entity doorTrigger({ 400, 300, 40, 40 }, true);
    doorTrigger.SetOnTriggerEnter([&]() {
        sceneManager.RequestSceneChange(testScene1);
    });

    Entity trigger2({300, 300, 40, 40}, true);
    trigger2.SetOnTriggerEnter([&]() {
        sceneManager.RequestSceneChange(testScene);
    });

    testScene.AddTrigger(doorTrigger);
    sceneManager.changeScene(testScene, testPlayer);

    testScene1.AddTrigger(trigger2);
    sceneManager.changeScene(testScene1, testPlayer);

    bool debugDrawTriggers = false;
    Camera2D camera = {};
    camera.target = { 400, 300 };
    camera.offset = { screenWidth / 2.0f, screenHeight / 2.0f };
    camera.zoom = 2.0f;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        testPlayer.Movement(sceneManager, GetFrameTime());
        // Переключение сцен
        if (IsKeyPressed(KEY_ONE)) sceneManager.changeScene(testScene1, testPlayer);
        if (IsKeyPressed(KEY_TWO)) sceneManager.changeScene(testScene, testPlayer);
        if (IsKeyPressed(KEY_F3)) {
            debugDrawTriggers = !debugDrawTriggers;
            sceneManager.SetDebugDrawTriggers(debugDrawTriggers);
        }
        if (IsKeyPressed(KEY_F4)) {
            ToggleFullscreen();
        }

        // Плавная камера (следим за игроком)
        camera.target.x += (testPlayer.GetPosition().x - camera.target.x) * 0.1f;
        camera.target.y += (testPlayer.GetPosition().y - camera.target.y) * 0.1f;

        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode2D(camera);
                sceneManager.Draw(); // отрисовка сцен
                testPlayer.Draw(); // отрисовка игрока
            EndMode2D();

            DrawText("A/D - move, Space - jump | 1,2 - switch scene | F3 - debug triggers", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    testPlayer.UnloadAnimations();
    CloseWindow();
    return 0;
}
