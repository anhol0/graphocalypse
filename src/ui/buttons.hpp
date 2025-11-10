#pragma once

#include "raylib.h"
#include <raylib.h>
#include <string>

struct Text {
  std::string text;
  Color textColor;
  int fontSize;
};

class Button {
  public:
    bool clickable = 1;
    Button(float posX, float posY, float width, float height) {
      button = { posX, posY, width, height };
    }

    Button() {
      button = { 0, 0, 0, 0 };
    }
    void Draw(Color color, Text text) {
      DrawRectangleRec(button, color);
      if(!text.text.empty()) {
        int textWidth = MeasureText(text.text.c_str(), text.fontSize);
        int textX = button.x + (button.width - textWidth) / 2;
        int textY = button.y + (button.height - text.fontSize) / 2;
        DrawText(text.text.c_str(), textX, textY, text.fontSize, text.textColor);        
      }
    }

    void Draw(Color color) {
      DrawRectangleRec(button, color);
    }

    void Draw(Color color, Text text, float cornerRadius) {
      DrawRectangleRounded(button, cornerRadius, 1, color);
      if(!text.text.empty()) {
        int textWidth = MeasureText(text.text.c_str(), text.fontSize);
        int textX = button.x + (button.width - textWidth) / 2;
        int textY = button.y + (button.height - text.fontSize) / 2;
        DrawText(text.text.c_str(), textX, textY, text.fontSize, text.textColor);        
      }
    }

    int Clicked() {
      if (CheckCollisionPointRec(GetMousePosition(), button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        return 1;
      else 
        return 0;
    }

    int Hover() {
      if (CheckCollisionPointRec(GetMousePosition(), button))
        return 1;
      else 
        return 0;
    }
  private:
    Rectangle button;
};