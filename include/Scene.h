#ifndef ABSENGINE_SCENE_H
#define ABSENGINE_SCENE_H
#include <functional>
#include <raylib.h>
#include <vector>

#include "Entity.h"


class Scene {
    std::vector<Entity> surfaces;
    std::vector<Entity> entities;
    std::vector<Entity> triggers;
    bool debugDrawTriggers = false;

    Vector2 playerPos{};

public:
    void DrawScene();
    void SetDebugDrawTriggers(bool enabled);
    void SetSurfaces(std::vector<Entity>& surfaces);
    std::vector<Entity>& GetSurfaces() { return surfaces; }
    std::vector<Entity>& GetTriggers() { return triggers; }
    Vector2 GetPlayerPos() const;

    void AddEntity(Entity entity);
    void RemoveEntity(Entity entity);
    void AddTrigger(Entity entity);
    void RemoveTrigger(Entity entity);
    Scene(float x, float y);


};


#endif //ABSENGINE_SCENE_H
