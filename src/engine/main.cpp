#include <memory>
#include <rlImGui.h>
#include <raylib.h>
#include <imgui.h>
#include <string>
#include "scene.hpp"
#include "sceneManager.hpp"

int WIDTH, HEIGHT;
float aspectRatio;
sceneManager sceneManager;
bool showFps = false;
bool closeWindow = false;
Font chakraPetch;
ImFont* imguiFont; 

int main() {
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  SetConfigFlags(FLAG_FULLSCREEN_MODE);
  InitWindow(WIDTH, HEIGHT, "Perlin Noise Gen");
  WIDTH = GetScreenWidth();
  HEIGHT = GetScreenHeight();
  aspectRatio = (float)(WIDTH)/HEIGHT;
  rlImGuiSetup(true);
  SetTargetFPS(60);
  ImGuiIO& io = ImGui::GetIO();
  io.IniFilename = nullptr;
  chakraPetch = LoadFontEx("src/assets/chakra_petch/ChakraPetch-Bold.ttf", 128, 0, 0);
  io.Fonts->AddFontDefault();
  imguiFont = io.Fonts->AddFontFromFileTTF("src/assets/chakra_petch/ChakraPetch-Bold.ttf", 30.f); 
  io.Fonts->Build();

  sceneManager.addScene(0, std::make_shared<MainMenu>());
  sceneManager.addScene(1, std::make_shared<Generator>());
  sceneManager.activateScene(0);

  while(!WindowShouldClose()) {
    BeginDrawing();
    rlImGuiBegin();
    ClearBackground(BLUE);

    sceneManager.updateCurrentScene();
    sceneManager.Draw();

    if(showFps) {
      DrawText(std::to_string(GetFPS()).c_str(), WIDTH - MeasureText(std::to_string(GetFPS()).c_str(), 30), 0, 30, BLACK);
    }

    if(closeWindow) {
      rlImGuiEnd();
      EndDrawing();
      CloseWindow();
      break;
    }
    DrawTextEx(chakraPetch, "Perlin noise generator v0.1.1", Vector2{(float)(WIDTH-MeasureTextEx(chakraPetch, "Perlin noise generator v0.0.1", 30, 1.f).x), (float)(HEIGHT - MeasureTextEx(chakraPetch, "graphocalypse v0.0.1", 30, 1.f).y)}, 30, 1.f, BLACK);
    rlImGuiEnd();
    EndDrawing();
  }
}