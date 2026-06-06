#include "../../include/SceneManager.h"
#include "include/Player.h"

SceneManager::SceneManager(const Scene& startScene) : currentScene(startScene) {

}

void SceneManager::changeScene(Scene newScene, Player& player) {
    this->currentScene = newScene;
    this->currentScene.SetDebugDrawTriggers(debugDrawTriggers);
    player.SetPosition(this->currentScene.GetPlayerPos());
    player.ResetVelocity();
}

void SceneManager::RequestSceneChange(Scene newScene) {
    pendingScene = newScene;
}

void SceneManager::ApplyPendingSceneChange(Player& player) {
    if (!pendingScene) {
        return;
    }

    changeScene(*pendingScene, player);
    pendingScene.reset();
}

void SceneManager::Draw() {
    currentScene.DrawScene();
}

void SceneManager::SetDebugDrawTriggers(bool enabled) {
    debugDrawTriggers = enabled;
    currentScene.SetDebugDrawTriggers(enabled);
}

Scene& SceneManager::GetCurrentScene() {
    return currentScene;
}

const Scene& SceneManager::GetCurrentScene() const {
    return currentScene;
}
