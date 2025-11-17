#pragma  once
#include "../ui/windows.hpp"
#include "../ui/buttons.hpp"
#include "imgui.h"
#include "raylib.h"
#include "perlinNoiseGen.cpp"

using SceneId = int;

extern int WIDTH, HEIGHT;
extern bool showFps;
extern bool closeWindow;
extern Font chakraPetch;
extern ImFont* imguiFont;
extern float aspectRatio;

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
    if(Start.Clicked()) {
      goToSceneID = 1;
    }
    Start = Button(
      (float)(WIDTH - buttonWidth) / 2, 
      (float)(HEIGHT - buttonHeight) / 2 - (buttonHeight + offset), 
     buttonWidth, buttonHeight
    );
  }
  void Render() override {
    DrawTextEx(chakraPetch, "Perlin noise generator",
               Vector2{(WIDTH - gameLogoSize.x)/2,((float)(HEIGHT - buttonHeight) / 2 - (buttonHeight + offset)-gameLogoSize.y)/2},
               90, 1, BLACK);
    Start.Draw(BLACK, Text{"Generate", WHITE, 24}, 30);
  }
  int buttonWidth = 200, buttonHeight = 50;
  int offset = 15;
  Vector2 gameLogoSize = MeasureTextEx(chakraPetch, "Perlin noise generator", 90, 1);
  Button Start = Button((float)(WIDTH - buttonWidth) / 2, (float)(HEIGHT - buttonHeight) / 2 - (buttonHeight + offset), buttonWidth, buttonHeight);
};

class Generator : public Scene {
  void Init() override {
    camera.position = Vector3{cameraPos, cameraPos, cameraPos};
    camera.fovy = 45.0f;
    camera.target = cubePosition;
    camera.up = {0.0f, 1.0f, 0.0f};
    camera.projection = CAMERA_PERSPECTIVE;
    window.setContents([&]() {
      if (ImGui::Button("Regenerate")) {
        redraw = true;
      }

      if (ImGui::SliderFloat("Resolution", &size, 100, 700)) {
        redraw = true; 
      }

      if (ImGui::SliderFloat("Scale", &scale, 0.001f, 0.1f)) {
        redraw = true; 
      }

      if(ImGui::InputInt("Enter seed", &seed)) {
        redraw = true;
      }

      if(ImGui::Button("Clear seed")) {
        seed = 0;
        redraw = true;
      }
    });
    cameraControls.setContents([&]() {
      if(ImGui::SliderFloat("Scale", &cameraPos, 3.0, 100.0f)) {
        camera.position = Vector3{cameraPos, cameraPos, cameraPos};
      }
    });
  }

  void Update() override {
    if(redraw) {
      PerlinNoise noise = (seed != 0) ? PerlinNoise(size, scale, seed) : PerlinNoise(size, scale);
      Image image = noise.genImage();
      tex = LoadTextureFromImage(image);
      redraw = false;
    }
    BeginMode3D(camera);
    DrawCube(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, GRAY);
    DrawCubeWires(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, DARKGRAY);
    EndMode3D();
    DrawTexture(tex, (WIDTH-size)/2,( HEIGHT-size)/2, WHITE);
  }

  void Render() override {
    window.draw();
    cameraControls.draw();
  }

  int seed = 0;
  bool redraw = false;
  float size = 256;
  float scale = 0.02;
  float cameraPos = 3.0f;
  Texture2D tex;
  Vector3 cubePosition = Vector3{0.0f};
  Vector3 cubeSize = Vector3{0.1f, 0.1f, 0.1f};
  Camera3D camera;
  Window window = Window("Options", ImGuiWindowFlags_NoCollapse);
  Window cameraControls = Window("Camera controld", ImGuiWindowFlags_NoCollapse);
};