#include "physics.hpp"
#include "raylib.h"

Physics::Physics(float gravity, Vector3 cameraPos, float inputSensitivity) {
  player.playerPos = cameraPos;
  player.radius = 1.0;
  player.inputSensitivity = inputSensitivity;
  ray = Ray{player.playerPos, Vector3{0.0, -1.0, 0.0f}};
  camera.position = player.playerPos;
  camera.fovy = 45.0f;
  camera.target = Vector3{0.0f, 2.0f, 0.0f};
  camera.up = {0.0f, 1.0f, 0.0f};
  camera.projection = CAMERA_PERSPECTIVE;
  g = gravity;
}

void Physics::Update() {
  player.velocity = (Vector3){
    (float)((IsKeyDown(KEY_W)) - (IsKeyDown(KEY_S))), 
    (float)((IsKeyDown(KEY_D)) - (IsKeyDown(KEY_A))),
    0
  };
  player.rotation = (Vector3){GetMouseDelta().x * player.inputSensitivity, GetMouseDelta().y * player.inputSensitivity, 0.0f};
  player.playerPos = camera.position;
  ray.position = player.playerPos;
  hit = GetRayCollisionMesh(ray, gameMap->meshes[0], gameMap->transform);
}

void Physics::SetMap(Model* map) {
  if(!map) {
    ClearMap();
    return;
  }
  gameMap = map;
}

void Physics::ClearMap() {
  gameMap = nullptr;
}

void Physics::PlayerUpdate() {
  if(hit.hit) {
    // player.velocity.z = 0.0f;
    camera.position.y += (player.radius - hit.distance);
    airBorne = false;
  }
  if(IsKeyDown(KEY_SPACE)) {
    player.velocity.z = 2.0;
    airBorne = true;
  } else if(IsKeyUp(KEY_SPACE) && airBorne) {
    player.velocity.z = -2.0;
  }
  UpdateCameraPro(&camera,player.velocity, player.rotation,0.0f);
}