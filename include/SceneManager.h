#ifndef ABSENGINE_SCENEMANAGER_H
#define ABSENGINE_SCENEMANAGER_H
#include <optional>

#include "Scene.h"

class Player;

class SceneManager {
    Scene currentScene;
    std::optional<Scene> pendingScene;
    bool debugDrawTriggers = false;
public:
    SceneManager(const Scene& startScene);

    void changeScene(Scene newScene, Player& player);
    void RequestSceneChange(Scene newScene);
    void ApplyPendingSceneChange(Player& player);
    void Draw();
    void SetDebugDrawTriggers(bool enabled);
    Scene& GetCurrentScene();
    const Scene& GetCurrentScene() const;

    // ~SceneManager();
};


#endif //ABSENGINE_SCENEMANAGER_H
