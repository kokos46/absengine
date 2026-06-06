#include "../../include/SceneManager.h"
#include "include/Player.h"

SceneManager::SceneManager(const Scene& startScene) : currentScene(startScene) {

}

void SceneManager::changeScene(Scene newScene, Player& player) {
    this->currentScene = newScene;
    player.SetPosition(newScene.GetPlayerPos());
    newScene.DrawScene();
}

void SceneManager::Draw() {
    currentScene.DrawScene();
}

Scene SceneManager::GetCurrentScene() {
    return currentScene;
}