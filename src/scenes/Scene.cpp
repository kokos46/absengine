#include "include/Scene.h"
void Scene::DrawScene() {
    for (const auto& plat : this->surfaces) {
        plat.Draw();
    }
}


void Scene::SetSurfaces(std::vector<Entity> &surfaces) {
    this->surfaces = surfaces;
}

std::vector<Entity> Scene::GetSurfaces() {
    return this->surfaces;
}
