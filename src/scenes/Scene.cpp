#include "include/Scene.h"
void Scene::DrawScene() {
    for (const auto& plat : this->surfaces) {
        DrawRectangleRec(plat, DARKGRAY);
    }
}


void Scene::SetSurfaces(std::vector<Rectangle> &surfaces) {
    this->surfaces = surfaces;
}

std::vector<Rectangle> Scene::GetSurfaces() {
    return this->surfaces;
}
