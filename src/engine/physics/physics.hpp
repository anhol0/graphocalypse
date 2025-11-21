#include "raylib.h"

struct Player {
  Vector3 playerPos;
  Vector3 velocity;
  Vector3 rotation;
  float inputSensitivity;
  float radius;
};

class Physics {
  public:
    Physics(float gravity, Vector3 cameraPos, float inputSensitivity);
    void Update();
    void PlayerUpdate();
    void SetMap(Image* map);
    void ClearMap();
    ~Physics() = default;
    Camera3D camera;
    Player player;
    float g;
  private:
    Image* gameMap = nullptr;
    Color* pixels;
    bool airBorne;
    double prevTime = 0;
    double dt;
    int px, pz;
};