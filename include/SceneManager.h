#ifndef ABSENGINE_SCENEMANAGER_H
#define ABSENGINE_SCENEMANAGER_H
#include "Scene.h"

class Player;

class SceneManager {
    Scene currentScene;
public:
    SceneManager(const Scene& startScene);

    void changeScene(Scene newScene, Player& player);
    void Draw();
    Scene GetCurrentScene();

    // ~SceneManager();
};


#endif //ABSENGINE_SCENEMANAGER_H
