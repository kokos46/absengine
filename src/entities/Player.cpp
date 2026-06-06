#include "include/Player.h"
#include "include/SceneManager.h"

Player::Player(Rectangle collision, float moveSpeed, float jumpForce, float gravity): Entity(collision), moveSpeed(moveSpeed), jumpForce(jumpForce), gravity(gravity) {}

void Player::ResetVelocity() {
    velocityY = 0.0f;
}

void Player::Movement(SceneManager& sceneManager, float dt) {
    // 1. Управление (X)
    // Используем тернарный оператор для чистоты: 1 если D, -1 если A, 0 если ничего
    float moveX = (IsKeyDown(KEY_D) - IsKeyDown(KEY_A)) * moveSpeed * dt;

    // 2. Гравитация (Y)
    velocityY += gravity * dt;

    // 3. Предварительное движение
    Move(moveX, velocityY * dt);

    // 4. Получаем ссылки на векторы сцены
    // Использование auto& ОБЯЗАТЕЛЬНО, чтобы работать с оригиналами, а не копиями
    auto& surfaces = sceneManager.GetCurrentScene().GetSurfaces();
    auto& triggers = sceneManager.GetCurrentScene().GetTriggers();

    Rectangle playerBox = GetCollisionBox();
    bool onGround = false;

    // 5. Обработка ФИЗИКИ (Твердые платформы)
    for (auto& entity : surfaces) {
        if (CheckCollisionRecs(playerBox, entity.GetCollisionBox())) {
            // Если игрок движется вниз, ставим его на платформу
            if (velocityY > 0) {
                Vector2 pos = GetPosition();
                pos.y = entity.GetCollisionBox().y - playerBox.height;
                SetPosition(pos);
                playerBox = GetCollisionBox();

                velocityY = 0;
                onGround = true;
            }
        }
    }

    // 6. Обработка ТРИГГЕРОВ (события)
    // Здесь мы проходим по оригиналам триггеров.
    // Любые изменения внутри trigger.Trigger() сохранятся в сцене.
    for (auto& trigger : triggers) {
        trigger.UpdateTriggerState(CheckCollisionRecs(playerBox, trigger.GetCollisionBox()));
    }

    // 7. Прыжок (только если стоим на твердом объекте)
    if (onGround && IsKeyPressed(KEY_SPACE)) {
        velocityY = -jumpForce;
    }

    sceneManager.ApplyPendingSceneChange(*this);
}
