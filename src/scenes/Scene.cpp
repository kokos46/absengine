#include "include/Scene.h"
void Scene::DrawScene() {
    for (const auto& plat : this->surfaces) {
        plat.Draw();
    }
}

Scene::Scene(float x, float y) {
    playerPos = Vector2(x, y);
}

void Scene::SetSurfaces(std::vector<Entity> &surfaces) {
    this->surfaces = surfaces;
}

std::vector<Entity> Scene::GetSurfaces() {
    return this->surfaces;
}

Vector2 Scene::GetPlayerPos() {
    return playerPos;
}

