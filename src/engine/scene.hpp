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
    camera.target = Vector3{0, 0, 0};
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
      if(ImGui::SliderFloat("Scale", &cameraPos, 200.f, 1000.0f)) {
        camera.position = Vector3{cameraPos, cameraPos, cameraPos};
      }
      if(ImGui::Button("Show FPS")) {
        showFps ^= 1;
      }
    });
  }

  void Update() override {
    if(redraw) {
      PerlinNoise noise = (seed != 0) ? PerlinNoise(size, scale, seed) : PerlinNoise(size, scale);
      image = noise.genImage();
      if(tex.id > 0) { UnloadMesh(map); UnloadTexture(tex); };
      tex = LoadTextureFromImage(image);
      mat.maps[MATERIAL_MAP_ALBEDO].texture = tex;
      map = GenMeshHeightmap(image, Vector3{size, 48, size});
      redraw = false;
    }
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 center(size * 0.5f, 0.0f, size * 0.5f);
    model = glm::rotate(model, (float)glm::radians(GetTime())*10, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, -center);

    BeginMode3D(camera);
    DrawMesh(map, mat, glMatToMatrix(model));
    EndMode3D();

    DrawTexture(tex, (WIDTH-size),0, WHITE);
  }

  void Render() override {
    window.draw();
    cameraControls.draw();
  }

  int seed = 0;
  bool redraw = false;
  float size = 256;
  float scale = 0.02;
  float cameraPos = 200.0f;
  Texture2D tex;
  Camera3D camera;
  Image image;
  Mesh map;
  Material mat = LoadMaterialDefault();
  Window window = Window("Options", ImGuiWindowFlags_NoCollapse);
  Window cameraControls = Window("Camera controls", ImGuiWindowFlags_NoCollapse);
};