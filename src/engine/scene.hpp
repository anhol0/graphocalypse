#pragma  once
#include "../ui/windows.hpp"
#include "../ui/buttons.hpp"
#include "glMatToMatrix.hpp"
#include "imgui.h"
#include "raylib.h"
#include "perlinNoiseGen.cpp"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "physics/physics.hpp"

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
    Start = Button(
      (float)(WIDTH - buttonWidth) / 2, 
      (float)(HEIGHT - buttonHeight) / 2 - (buttonHeight + offset), 
     buttonWidth, buttonHeight
    );
  }
  void Update() override {
    if(Start.Clicked()) {
      goToSceneID = 1;
    }
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
    PerlinNoise noise = (seed != 0) ? PerlinNoise(size, scale, seed) : PerlinNoise(size, scale);
    image = noise.genImage();
    tex = LoadTextureFromImage(image);
    mat.maps[MATERIAL_MAP_ALBEDO].texture = tex;
    map = LoadModelFromMesh(GenMeshHeightmap(image, Vector3{size, height, size}));
    physics.SetMap(&map);
    
    window.setContents([&]() {
      ImGui::SetWindowSize(ImVec2{400, 400});
      ImGui::SetWindowPos(ImVec2(0, 0));
      if(ImGui::Button("Show 2D noise")) {
        showNoise2D ^= true;
      }
      if(ImGui::Button("Show FPS")) {
        showFps ^= 1;
      }
    });
  }

  void Update() override {
    if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && IsCursorHidden()) {
      EnableCursor();
    }
    else if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && 
            GetMousePosition().x > 400 && 
            GetMousePosition().x > 400 && !IsCursorHidden()) 
    {
      DisableCursor();
    }
    physics.Update();
    if(IsCursorHidden()) {
      physics.PlayerUpdate();
    }
  }

  void Render() override {
    window.draw();
    BeginMode3D(physics.camera);
    DrawModel(map, Vector3{0, 0, 0}, 1.0f, WHITE);
    DrawModelWires(map, Vector3{0, 0, 0}, 1.0f, BLUE);
    EndMode3D();
    if(showNoise2D) {
      DrawTexture(tex, (WIDTH-size),0, WHITE);
    }
  }

  // ~Generator() override {
  //   physics.ClearMap(); 
  //   UnloadModel(map); 
  //   UnloadTexture(tex); 
  // }

  int seed = 0;
  bool showNoise2D = false;
  float size = 1024; // map size
  float scale = 0.02; // frequency of changes in map
  float height = 16; // max height when color = 255
  // Physics settings
  float sensitivity = 0.2f;
  Vector3 initialCameraPos{200, 200, 200};
  // textures and model handling
  Texture2D tex;
  Image image;
  Model map;
  Physics physics = Physics(9.8, initialCameraPos, sensitivity);
  Material mat = LoadMaterialDefault();
  Window window = Window("Options", ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
};