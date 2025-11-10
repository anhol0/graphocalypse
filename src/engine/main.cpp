#include <memory>
#include <rlImGui.h>
#include <raylib.h>
#include <string>
#include <imgui.h>
#include "scene.hpp"
#include "sceneManager.hpp"

#define WIDTH 800
#define HEIGHT 600

sceneManager sceneManager;

int main() {
  InitWindow(WIDTH, HEIGHT, "graphocalypse");
  rlImGuiSetup(true);
  SetTargetFPS(60);
  ImGui::GetIO().IniFilename = nullptr;

  sceneManager.addScene(0, std::make_shared<MainMenu>());
  sceneManager.addScene(1, std::make_shared<SecondaryMenu>());
  sceneManager.activateScene(0);

  while(!WindowShouldClose()) {
    BeginDrawing();
    rlImGuiBegin();
    ClearBackground(WHITE);

    sceneManager.updateCurrentScene();
    sceneManager.Draw();

    DrawText("graphocalypse 0.0.1", WIDTH-MeasureText("graphocalypse v0.0.1", 30), HEIGHT-30, 30, BLACK);
    DrawText(std::string(std::to_string(GetFPS())).c_str(), WIDTH-40, 0, 30, BLACK);
    rlImGuiEnd();
    EndDrawing();
  }
}