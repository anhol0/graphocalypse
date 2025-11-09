#include <rlImGui.h>
#include <raylib.h>
#include <string>
#include "imgui.h"
#include "../ui/windows.hpp"

int main() {
  InitWindow(800, 600, "graphocalypse");
  rlImGuiSetup(true);
  SetTargetFPS(60);
  ImGui::GetIO().IniFilename = nullptr;

  Window testWindow("testWindow");
  int clicked = 0;
  testWindow.setContents([&]() {
    ImGui::Text("example window");
    ImGui::SetWindowSize(ImVec2(150,80), ImGuiCond_Always);
    if(ImGui::Button("click", ImVec2(60, 30))) {
      clicked++;
    }
    DrawText(std::string("Button was clicked: " + std::to_string(clicked) + " times" ).c_str(), 400, 300, 24, BLACK);
  });

  Window testWindow2("testWindow2", ImGuiWindowFlags_NoMove);
  testWindow2.setContents([&]() {
    ImGui::Text("example window");
    ImGui::SetWindowSize(ImVec2(150,80), ImGuiCond_Always);
    ImGui::SetWindowPos(ImVec2(100, 200));
    if(ImGui::Button("click", ImVec2(60, 30))) {
      clicked++;
    }
    DrawText(std::string("Button was clicked: " + std::to_string(clicked) + " times" ).c_str(), 400, 300, 24, BLACK);
  });

  while(!WindowShouldClose()) {
    BeginDrawing();
    rlImGuiBegin();
    ClearBackground(WHITE);
    
    testWindow.draw();
    testWindow2.draw();

    DrawText("graphocalypse 0.0.1", 0, 0, 30, BLACK);
    rlImGuiEnd();
    EndDrawing();
  }
}