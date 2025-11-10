#pragma  once
#include "../ui/windows.hpp"
#include "../ui/buttons.hpp"
#include "raylib.h"

using SceneId = int;

class Scene {
public:
  virtual ~Scene() = default;
  virtual void Init() = 0;
  virtual void Update() = 0;
  virtual void Render()= 0;
  SceneId goToSceneID = -1;
};

class MainMenu : public Scene {
public:
  
  void Init() override {
    button = Button((float)(800 - 170)/2, (float)(600 - 50) / 2, 170, 50);
  }

  void Update() override {
    if(button.Clicked()) {
      goToSceneID = 1;
    }
  }

  void Render() override {
    button.Draw(BLACK, Text{"Next Screen", WHITE, 24});
  }

  Button button;

};

class SecondaryMenu : public Scene {
  public:

  void Init() override {
    button = Button((float)(800 - 210)/2, (float)(600 - 50) / 2, 210, 50);
    window.setContents([&]() {
      ImGui::SetWindowSize(ImVec2(100, 100));
      ImGui::Text("Test Text");
    });
  }

  void Update() override {
    if(button.Clicked()) {
      goToSceneID = 0;
    }
  }

  void Render() override {
    button.Draw(BLACK, Text{"Previous Screen", WHITE, 24});
    window.draw();
  }
  Button button;
  Window window = Window("some text");
};