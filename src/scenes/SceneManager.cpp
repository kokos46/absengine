#include <utility>

#include "../../include/SceneManager.h"

SceneManager::SceneManager(Scene startScene) {
    this->currentScene = std::move(startScene);
}

void SceneManager::changeScene(Scene newScene) {
    this->currentScene = newScene;
    newScene.DrawScene();
}

void SceneManager::Draw() {
    currentScene.DrawScene();
}

Scene SceneManager::GetCurrentScene() {
    return currentScene;
}