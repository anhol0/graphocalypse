#include <memory>
#include <rlImGui.h>
#include <raylib.h>
#include <imgui.h>
#include <string>
#include "scene.hpp"
#include "sceneManager.hpp"

int WIDTH = 800;
int HEIGHT = 600;

sceneManager sceneManager;
bool showFps = false;
bool closeWindow = false;

int main() {
  InitWindow(WIDTH, HEIGHT, "graphocalypse");
  rlImGuiSetup(true);
  SetTargetFPS(60);
  ImGui::GetIO().IniFilename = nullptr;
  Font departureMono = LoadFont("/usr/share/fonts/NotoSansNerdFontPropo-CondensedBoldItalic.ttf");

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
    DrawTextEx(departureMono, "graphocalypse v0.0.1", Vector2{(float)(WIDTH-MeasureTextEx(departureMono, "graphocalypse v0.0.1", 30, 1.f).x), (float)(HEIGHT - MeasureTextEx(departureMono, "graphocalypse v0.0.1", 30, 1.f).y)}, 30, 1.f, BLACK);
    rlImGuiEnd();
    EndDrawing();
  }
}