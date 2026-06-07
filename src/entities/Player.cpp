#include "include/Player.h"

#include "include/Animator.h"
#include "include/SceneManager.h"

Player::Player(Rectangle collision, float moveSpeed, float jumpForce, float gravity): Entity(collision), moveSpeed(moveSpeed),
    jumpForce(jumpForce), gravity(gravity), runAnimator("npc01_spritesheet.png", 8, 6, 0, 8, 0.1f),
    idleAnimator("npc01_spritesheet.png", 8, 6, 2, 8, 0.0f)
{
    currentAnimator = &idleAnimator;
}

void Player::ResetVelocity() {
    velocityY = 0.0f;
}

void Player::UnloadAnimations() {
    runAnimator.Unload();
    idleAnimator.Unload();
}

void Player::SetAnimation(AnimationState state) {
    if (currentAnimationState == state) {
        return;
    }

    currentAnimationState = state;
    currentAnimator = state == AnimationState::Run ? &runAnimator : &idleAnimator;
    currentAnimator->Reset();
}

void Player::Movement(SceneManager& sceneManager, float dt) {
    // 1. Управление (X)
    // Используем тернарный оператор для чистоты: 1 если D, -1 если A, 0 если ничего
    int inputAxis = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
    float moveX = inputAxis * moveSpeed * dt;

    if (inputAxis < 0) {
        facingLeft = true;
    } else if (inputAxis > 0) {
        facingLeft = false;
    }

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
        onGround = false;
    }

    if (!onGround && velocityY < 0.0f) {
        SetAnimation(AnimationState::Jump);
    } else if (!onGround && velocityY > 0.0f) {
        SetAnimation(AnimationState::Fall);
    } else if (inputAxis != 0) {
        SetAnimation(AnimationState::Run);
    } else {
        SetAnimation(AnimationState::Idle);
    }

    currentAnimator->Update(dt);

    sceneManager.ApplyPendingSceneChange(*this);
}

void Player::Draw() {
    currentAnimator->Draw(GetCollisionBox(), facingLeft);
}
