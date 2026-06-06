#include "include/Player.h"
#include "include/SceneManager.h"

Player::Player(Rectangle collision, float moveSpeed, float jumpForce, float gravity): Entity(collision), moveSpeed(moveSpeed), jumpForce(jumpForce), gravity(gravity) {}

void Player::Movement(SceneManager& sceneManager, float dt) {
    // 1. Управление (X)
    float moveX = 0.0f;
    if (IsKeyDown(KEY_A)) moveX -= 1.0f;
    if (IsKeyDown(KEY_D)) moveX += 1.0f;

    // 2. Гравитация (Y)
    velocityY += gravity * dt;

    // Применяем движение к игроку
    Move(moveX * moveSpeed * dt, velocityY * dt);

    // 3. Обработка коллизий
    bool onGround = false;
    Rectangle playerBox = GetCollisionBox();

    for (const auto& entity : sceneManager.GetCurrentScene().GetSurfaces()) {
        if (CheckCollisionRecs(playerBox, entity.GetCollisionBox())) {
            if (velocityY > 0) { // Только если падаем вниз
                // Ставим игрока на платформу
                Vector2 pos = GetPosition();
                pos.y = entity.GetCollisionBox().y - playerBox.height;
                SetPosition(pos);

                velocityY = 0;
                onGround = true;
            }
        }
    }

    // Прыжок
    if (onGround && IsKeyPressed(KEY_SPACE)) {
        velocityY = -jumpForce;
    }
}