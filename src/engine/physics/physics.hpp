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
    void SetMap(Model* map);
    void ClearMap();
    ~Physics() = default;
    Camera3D camera;
    Player player;
    float g;
  private:
    float deltaTime;
    bool airBorne;
    float heightAt(Image map);
    Ray ray;
    RayCollision hit;
    Model* gameMap = nullptr;
};