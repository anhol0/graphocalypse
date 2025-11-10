#pragma once

#include "scene.hpp"
#include <memory>
#include <unordered_map>

class sceneManager {
public:
  void addScene(SceneId id, std::shared_ptr<Scene> scene) {
    scenes[id] = scene;
  }
  void activateScene(SceneId ID) {
    if(activeSceneID == ID) return;
    activeSceneID = ID;
    if(scenes.count(ID)) {
      scenes[ID]->Init();
    }
  }
  void updateCurrentScene() {
    if(!scenes.count(activeSceneID)) return;
    auto& scene = scenes[activeSceneID];
    scene->Update();
    if(scene->goToSceneID != -1) {
      activateScene(scene->goToSceneID);
      scene->goToSceneID = -1;
    }
  }
  void Draw() {
    if(scenes.count(activeSceneID)) {
      scenes[activeSceneID]->Render();
    }
  }
private:
  std::unordered_map<SceneId, std::shared_ptr<Scene>> scenes;
  SceneId activeSceneID = -1;
};