#include <raylib.h>
#include <vector>
#include "include/SceneManager.h"


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

const int FPS = 30;

int main() {
    const int screenWidth = 1366;
    const int screenHeight = 768;
    InitWindow(screenWidth, screenHeight, "Raylib Platformer - Fixed");

    // Инициализация сцен
    Scene testScene1; testScene1.SetSurfaces(testScene1Platforms);
    Scene testScene;  testScene.SetSurfaces(testScene2Platforms);
    SceneManager sceneManager(testScene1);

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
        for (const auto& entity : sceneManager.GetCurrentScene().GetSurfaces()) {
            // ВАЖНО: вызываем GetCollisionBox()
            if (CheckCollisionRecs(player, entity.GetCollisionBox())) {
                if (velocityY > 0) {
                    player.y = entity.GetCollisionBox().y - player.height;
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
            DrawText("A/D - move, Space - jump", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}