#include <raylib.h>
#include <vector>
#include "include/SceneManager.h"

std::vector<Rectangle> platforms1 = {
    { 0, 400, 800, 50 },    // Пол
    { 200, 300, 200, 20 },  // Платформа 1
    { 500, 200, 200, 20 },  // Платформа 2
    { 100, 150, 150, 20 }   // Платформа 3
};

std::vector<Rectangle> platforms2 = {
    { 0, 400, 800, 50 },
    { 200, 300, 200, 20 },
};

const int FPS = 30;

int main() {
    const int screenWidth = 1366;
    const int screenHeight = 768;
    InitWindow(screenWidth, screenHeight, "Raylib Platformer - Fixed");

    // Инициализация сцен
    Scene testScene1; testScene1.SetSurfaces(platforms2);
    Scene testScene;  testScene.SetSurfaces(platforms1);
    SceneManager sceneManager(testScene);

    Rectangle player = { 400, 300, 40, 40 };
    float velocityY = 0.0f;

    // НАСТРОЙКИ ФИЗИКИ (пиксели в секунду)
    const float moveSpeed = 400.0f;   // Было 5, стало 400
    const float jumpForce = 600.0f;   // Сила прыжка
    const float gravity = 1800.0f;    // Гравитация

    Camera2D camera = { 0 };
    camera.target = { player.x, player.y };
    camera.offset = { screenWidth / 2.0f, screenHeight / 2.0f };
    camera.zoom = 1.0f;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // 1. Управление (X)
        float moveX = 0.0f;
        if (IsKeyDown(KEY_A)) moveX -= 1.0f;
        if (IsKeyDown(KEY_D)) moveX += 1.0f;
        player.x += moveX * moveSpeed * dt;

        // 2. Гравитация и Прыжок (Y)
        velocityY += gravity * dt;
        player.y += velocityY * dt;

        // 3. Обработка коллизий (теперь корректно с deltaTime)
        bool onGround = false;
        for (const auto& plat : sceneManager.GetCurrentScene().GetSurfaces()) {
            if (CheckCollisionRecs(player, plat)) {
                if (velocityY > 0) { // Падаем сверху
                    player.y = plat.y - player.height;
                    velocityY = 0;
                    onGround = true;
                }
            }
        }

        if (onGround && IsKeyDown(KEY_SPACE)) {
            velocityY = -jumpForce;
        }

        // Переключение сцен
        if (IsKeyPressed(KEY_ONE)) sceneManager.changeScene(testScene1);
        if (IsKeyPressed(KEY_TWO)) sceneManager.changeScene(testScene);

        // Плавная камера
        camera.target.x += (player.x - camera.target.x) * 0.1f; // target.x -> player.x
        camera.target.y += (player.y - camera.target.y) * 0.1f;

        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode2D(camera);
                sceneManager.Draw();
                DrawRectangleRec(player, RED);
            EndMode2D();
            DrawText("A/D - движение, Пробел - прыжок", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}