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
      ImGui::SetWindowSize(ImVec2{400, 400});
      ImGui::SetWindowPos(ImVec2(0, 0));
      if (ImGui::Button("Regenerate")) {
        redraw = true;
      }

      if (ImGui::SliderFloat("Resolution", &size, 100, 700)) {
        redraw = true; 
      }

      if (ImGui::SliderFloat("Frqeuqncy", &scale, 0.001f, 0.1f)) {
        redraw = true; 
      }

      ImGui::SliderFloat("Max height", &height, 16, 100);
      if(ImGui::InputInt("Enter seed", &seed)) {
        redraw = true;
      }

      if(ImGui::Button("Clear seed")) {
        seed = 0;
        redraw = true;
      }
      if(ImGui::Button("Animated Rotation")) {
        rotate ^= true;
      }
      if(ImGui::Button("Show 2D noise")) {
        showNoise2D ^= true;
      }
      ImGui::Text("Camera Options");
      if(ImGui::SliderFloat("Distance", &cameraPos, 200.f, 1000.0f)) {
        camera.position.x = std::sin(cameraAngle) * cameraPos;
        camera.position.y = cameraElevation;
        camera.position.z = std::cos(cameraAngle) * cameraPos;
      }
      if(ImGui::SliderFloat("Camera rotation", &cameraAngle, 0, 2*PI)) {
        camera.position.x = std::sin(cameraAngle) * cameraPos;
        camera.position.y = cameraElevation;
        camera.position.z = std::cos(cameraAngle) * cameraPos;
      }
      if(ImGui::SliderFloat("Elevation", &cameraElevation, 0, 800)) {
        camera.position.y = cameraElevation;
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
      map = GenMeshHeightmap(image, Vector3{size, height, size});
      currentheight = height;
      redraw = false;
    }
    if(currentheight != height) {
      if(tex.id > 0) { UnloadMesh(map); UnloadTexture(tex); };
      tex = LoadTextureFromImage(image);
      mat.maps[MATERIAL_MAP_ALBEDO].texture = tex;
      map = GenMeshHeightmap(image, Vector3{size, height, size});
      currentheight = height;
    }
    model = glm::mat4(1.0f);
    glm::vec3 center(size * 0.5f, 0.0f, size * 0.5f);
    if(rotate) {
      rotationAngle = rotationAngle + 0.01;
    }
    model = glm::rotate(model, rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, -center);
  }

  void Render() override {
    window.draw();
    BeginMode3D(camera);
    DrawMesh(map, mat, glMatToMatrix(model));
    EndMode3D();
    if(showNoise2D) {
      DrawTexture(tex, (WIDTH-size),0, WHITE);
    }
  }

  int seed = 0;
  bool rotate = true;
  bool redraw = false;
  bool showNoise2D = false;
  float rotationAngle = 0;
  float cameraAngle = 0;
  float cameraPos = 200.0f;
  float cameraElevation = cameraPos;
  float size = 256;
  float scale = 0.02;
  float height = 16;
  float currentheight = height;
  Texture2D tex;
  Camera3D camera;
  Image image;
  Mesh map;
  glm::mat4 model;
  Material mat = LoadMaterialDefault();
  Window window = Window("Options", ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
};