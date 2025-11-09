#include <functional>
#ifndef WINDOWS_HPP

#include "imgui.h"

class Window {
public:
  Window(const char *name) : name(name) {};
  Window(const char *name, ImGuiWindowFlags flags) {
    this->name = name;
    this->flags = flags;
  };
  void setContents(std::function<void()> f) { content = f; };
  void draw() {
    if(flags)
      ImGui::Begin(name, nullptr, flags);
    else
      ImGui::Begin(name);
    if(content) content();
    ImGui::End();
  }
private:
  std::function<void()> content;
  const char *name;
  ImGuiWindowFlags flags = 0;
};

#endif