#ifndef ABSENGINE_SCENEMANAGER_H
#define ABSENGINE_SCENEMANAGER_H
#include <memory>

#include "Scene.h"


class SceneManager {
    Scene currentScene;
public:
    SceneManager(Scene startScene);

    void changeScene(Scene newScene);
    void Draw();
    Scene GetCurrentScene();

    // ~SceneManager();
};


#endif //ABSENGINE_SCENEMANAGER_H
