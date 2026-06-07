//
// Created by Попехин Константин on 06.06.2026.
//

#ifndef ABSENGINE_PLAYER_H
#define ABSENGINE_PLAYER_H
#include "Animator.h"
#include "Entity.h"

class SceneManager;


class Player : public Entity {
    enum class AnimationState {
        Idle,
        Run,
        Jump,
        Fall
    };

    float moveSpeed;
    float jumpForce;
    float gravity;

    float velocityY = 0.0f;

    Animator runAnimator;
    Animator idleAnimator;
    Animator* currentAnimator;
    AnimationState currentAnimationState = AnimationState::Idle;
    bool facingLeft = false;

    void SetAnimation(AnimationState state);

public:
    Player(Rectangle collision, float moveSpeed, float jumpForce, float gravity);
    void Movement(SceneManager& sceneManager, float dt);
    void ResetVelocity();
    void UnloadAnimations();

    void Draw();
};


#endif //ABSENGINE_PLAYER_H
