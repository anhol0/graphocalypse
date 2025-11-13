#include <memory>
#include <rlImGui.h>
#include <raylib.h>
#include <imgui.h>
#include <string>
#include "scene.hpp"
#include "sceneManager.hpp"

int WIDTH;
int HEIGHT;

sceneManager sceneManager;
bool showFps = false;
bool closeWindow = false;
Font chakraPetch;

int main() {
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(WIDTH, HEIGHT, "graphocalypse");
  WIDTH = GetScreenWidth();
  HEIGHT = GetScreenHeight();
  rlImGuiSetup(true);
  SetTargetFPS(60);
  ImGui::GetIO().IniFilename = nullptr;
  chakraPetch = LoadFontEx("src/assets/chakra_petch/ChakraPetch-Bold.ttf", 128, 0, 0);

  sceneManager.addScene(0, std::make_shared<MainMenu>());
  sceneManager.addScene(1, std::make_shared<SettingsMenu>());
  sceneManager.addScene(2, std::make_shared<PrepareForRunScene>());
  sceneManager.activateScene(0);

  while(!WindowShouldClose()) {
    BeginDrawing();
    rlImGuiBegin();
    ClearBackground(WHITE);

    sceneManager.updateCurrentScene();
    sceneManager.Draw();

    if(closeWindow) {
      rlImGuiEnd();
      EndDrawing();
      CloseWindow();
      break;
    }
    if(showFps) {
      DrawText(std::to_string(GetFPS()).c_str(), WIDTH - MeasureText(std::to_string(GetFPS()).c_str(), 30), 0, 30, BLACK);
    }
    DrawTextEx(chakraPetch, "graphocalypse v0.0.1", Vector2{(float)(WIDTH-MeasureTextEx(chakraPetch, "graphocalypse v0.0.1", 30, 1.f).x), (float)(HEIGHT - MeasureTextEx(chakraPetch, "graphocalypse v0.0.1", 30, 1.f).y)}, 30, 1.f, BLACK);
    rlImGuiEnd();
    EndDrawing();
  }
}