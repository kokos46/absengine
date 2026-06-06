//
// Created by Попехин Константин on 06.06.2026.
//

#ifndef ABSENGINE_PLAYER_H
#define ABSENGINE_PLAYER_H
#include "Entity.h"

class SceneManager;


class Player : public Entity {
    float moveSpeed;
    float jumpForce;
    float gravity;

    float velocityY = 0.0f;
public:
    Player(Rectangle collision, float moveSpeed, float jumpForce, float gravity);
    void Movement(SceneManager& sceneManager, float dt);
};


#endif //ABSENGINE_PLAYER_H
