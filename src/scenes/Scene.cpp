#include <utility>

#include "include/Scene.h"
void Scene::DrawScene() {
    for (const auto& plat : this->surfaces) {
        plat.Draw();
    }

    if (debugDrawTriggers) {
        for (const auto& trigger : this->triggers) {
            const Rectangle box = trigger.GetCollisionBox();
            DrawRectangleRec(box, Fade(ORANGE, 0.25f));
            DrawRectangleLinesEx(box, 2.0f, ORANGE);
        }
    }
}

Scene::Scene(float x, float y) {
    playerPos = Vector2(x, y);
}

void Scene::SetSurfaces(std::vector<Entity> &surfaces) {
    this->surfaces = surfaces;
}

void Scene::SetDebugDrawTriggers(bool enabled) {
    debugDrawTriggers = enabled;
}

Vector2 Scene::GetPlayerPos() const {
    return playerPos;
}

void Scene::AddEntity(Entity entity) {
    this->entities.push_back(entity);
}

void Scene::AddTrigger(Entity trigger) {
    this->triggers.push_back(trigger);
}
