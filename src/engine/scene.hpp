#pragma  once
#include "../ui/windows.hpp"
#include "../ui/buttons.hpp"
#include "imgui.h"
#include "raylib.h"

using SceneId = int;

extern int WIDTH, HEIGHT;
extern bool showFps;
extern bool closeWindow;
extern Font chakraPetch;
extern ImFont* imguiFont;

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

  }
  void Update() override {
    if(Settings.Clicked()) {
      goToSceneID = 1;
    }
    if(Start.Clicked()) {
      goToSceneID = 2;
    }
    if(Exit.Clicked()) {
      closeWindow = true;
    }
    Start = Button(
      (float)(WIDTH - buttonWidth) / 2, 
      (float)(HEIGHT - buttonHeight) / 2 - (buttonHeight + offset), 
     buttonWidth, buttonHeight);
    Settings = Button((float)(WIDTH - buttonWidth) / 2, (float)(HEIGHT - buttonHeight) / 2, buttonWidth, buttonHeight);
    Exit = Button((float)(WIDTH - buttonWidth) / 2, (float)(HEIGHT - buttonHeight) / 2 + (buttonHeight + offset), buttonWidth, buttonHeight);
  }
  void Render() override {
    DrawTextEx(chakraPetch, "GRAPHOCALYPSE", Vector2{(WIDTH - gameLogoSize.x)/2, ((float)(HEIGHT - buttonHeight) / 2 - (buttonHeight + offset)-gameLogoSize.y)/2}, 90, 1, BLACK);
    Start.Draw(BLACK, Text{"Start run", WHITE, 24}, 30);
    Settings.Draw(BLACK, Text{"Settings", WHITE, 24}, 30);
    Exit.Draw(BLACK, Text{"Exit", WHITE, 24}, 30);

  }
  int buttonWidth = 200, buttonHeight = 50;
  int offset = 15;
  Vector2 gameLogoSize = MeasureTextEx(chakraPetch, "GRAPHOCALYPSE", 90, 1);
  Button Start = Button((float)(WIDTH - buttonWidth) / 2, (float)(HEIGHT - buttonHeight) / 2 - (buttonHeight + offset), buttonWidth, buttonHeight);
  Button Settings = Button((float)(WIDTH - buttonWidth) / 2, (float)(HEIGHT - buttonHeight) / 2, buttonWidth, buttonHeight);
  Button Exit = Button((float)(WIDTH - buttonWidth) / 2, (float)(HEIGHT - buttonHeight) / 2 + (buttonHeight + offset), buttonWidth, buttonHeight);

};

// Scenes to go to from the Main Scene
class SettingsMenu : public Scene {
  public:
  
  void Init() override {
    button = Button((float)(WIDTH - 210)/2, (float)(HEIGHT - 50) / 2, 210, 50);
    window.setContents([&]() {
      float windowHeight = (float)(HEIGHT)/4;
      float windowWidth = (float)(WIDTH)/4;
      ImGui::SetWindowSize(ImVec2(windowWidth, windowHeight));
      ImGui::SetWindowPos(ImVec2((WIDTH-windowWidth)/2,(HEIGHT-windowWidth)/2));
      ImGui::PushFont(imguiFont);
      ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 15.f);
      ImGui::Text("Settings");
      if(ImGui::Button("Show FPS", ImVec2(windowWidth, windowHeight/8))) {
        showFps ^= true;
      }
      ImGui::PopStyleVar();
      ImGui::PopFont();
    });
  }
    
    void Update() override {
      if(button.Clicked()) {
        goToSceneID = 0;
      }
    if(button.Hover()) {
      color = RED;
    } else {
      color = BLACK;
    }
    button = Button((float)(WIDTH - 210)/2, (float)(HEIGHT - 50) / 2, 210, 50);
  }
  
  void Render() override {
    button.Draw(color, Text{"Previous Screen", WHITE, 24}, 30);
    window.draw();
  }
  Button button;
  Window window = Window("some text", ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
  Color color = BLACK;
};

class PrepareForRunScene : public Scene {
  void Init() override {

  }

  void Update() override {

  }

  void Render() override 
  {

  }
};

class WeaponEditor : public Scene {
public:
  
  void Init() override {
    button = Button((float)(WIDTH - 170)/2, (float)(HEIGHT - 50) / 2, 170, 50);
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

// Scenes to go to 