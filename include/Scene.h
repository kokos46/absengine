#ifndef ABSENGINE_SCENE_H
#define ABSENGINE_SCENE_H
#include <raylib.h>
#include <vector>

#include "Entity.h"


class Scene {
    std::vector<Entity> surfaces;
    std::vector<Entity> entities;

    Vector2 playerPos{};

public:
    void DrawScene();
    void SetSurfaces(std::vector<Entity>& surfaces);
    std::vector<Entity> GetSurfaces();
    Vector2 GetPlayerPos();

    Scene(float x, float y);
};


#endif //ABSENGINE_SCENE_H
