#ifndef ABSENGINE_SCENE_H
#define ABSENGINE_SCENE_H
#include <raylib.h>
#include <vector>


class Scene {
    std::vector<Rectangle> surfaces;

public:
    void DrawScene();
    void SetSurfaces(std::vector<Rectangle>& surfaces);
    std::vector<Rectangle> GetSurfaces();
};


#endif //ABSENGINE_SCENE_H
