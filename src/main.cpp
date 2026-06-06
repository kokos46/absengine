#include <raylib.h>
#include <vector>
#include "include/SceneManager.h"
#include "include/Player.h"

// Данные сцен
std::vector<Entity> testScene1Platforms = {
    Entity({ 0, 400, 800, 50 }),
    Entity({ 200, 300, 200, 20 }),
    Entity({ 500, 200, 200, 20 }),
    Entity({ 100, 150, 150, 20 })
};

std::vector<Entity> testScene2Platforms = {
    Entity({ 0, 400, 800, 50 }),
    Entity({ 200, 300, 200, 20 })
};

int main() {
    const int screenWidth = 1366;
    const int screenHeight = 768;
    InitWindow(screenWidth, screenHeight, "Raylib Platformer - Finalized");

    // Инициализация сцен
    Scene testScene1(400, 300); testScene1.SetSurfaces(testScene1Platforms);
    Scene testScene(600, 300);  testScene.SetSurfaces(testScene2Platforms);
    SceneManager sceneManager(testScene);

    // Создаем игрока (используем Rectangle для начальной позиции)


    const float moveSpeed = 400.0f;
    const float jumpForce = 600.0f;
    const float gravity = 1800.0f;

    Player testPlayer(Rectangle{ sceneManager.GetCurrentScene().GetPlayerPos().x,
                                 sceneManager.GetCurrentScene().GetPlayerPos().y, 40, 40 }, moveSpeed, jumpForce, gravity);
    float velocityY = 0.0f;

    Camera2D camera = { 0 };
    camera.target = { 400, 300 };
    camera.offset = { screenWidth / 2.0f, screenHeight / 2.0f };
    camera.zoom = 1.0f;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        testPlayer.Movement(sceneManager, GetFrameTime());
        // Переключение сцен
        if (IsKeyPressed(KEY_ONE)) sceneManager.changeScene(testScene1, testPlayer);
        if (IsKeyPressed(KEY_TWO)) sceneManager.changeScene(testScene, testPlayer);
        // testPlayer.SetPosition(sceneManager.GetCurrentScene().GetPlayerPos());

        // Плавная камера (следим за игроком)
        camera.target.x += (testPlayer.GetPosition().x - camera.target.x) * 0.1f;
        camera.target.y += (testPlayer.GetPosition().y - camera.target.y) * 0.1f;

        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode2D(camera);
                sceneManager.Draw(); // отрисовка сцен
                testPlayer.Draw(); // отрисовка игрока
            EndMode2D();

            DrawText("A/D - move, Space - jump | 1,2 - switch scene", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}