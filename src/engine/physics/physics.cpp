#include "physics.hpp"
#include "raylib.h"

Physics::Physics(float gravity, Vector3 cameraPos, float inputSensitivity) {
  player.playerPos = cameraPos;
  player.radius = 1.0;
  player.inputSensitivity = inputSensitivity;
  camera.position = player.playerPos;
  camera.fovy = 45.0f;
  camera.target = Vector3{0.0f, 2.0f, 0.0f};
  camera.up = {0.0f, 1.0f, 0.0f};
  camera.projection = CAMERA_PERSPECTIVE;
  g = gravity;
}

void Physics::Update() {
  double time = GetTime();
  dt = time - prevTime;
  prevTime = time;
  player.velocity = (Vector3){
    (float)((IsKeyDown(KEY_W)) - (IsKeyDown(KEY_S))) * 0.6f, 
    (float)((IsKeyDown(KEY_D)) - (IsKeyDown(KEY_A))) * 0.6f,
    0
  };
  player.rotation = (Vector3){GetMouseDelta().x * player.inputSensitivity, GetMouseDelta().y * player.inputSensitivity, 0.0f};
  player.playerPos = camera.position;
  px = (int)player.playerPos.x;
  pz = (int)player.playerPos.z;
}

void Physics::SetMap(Image* map) {
  if(!map) {
    ClearMap();
    return;
  }
  gameMap = map;
  pixels = LoadImageColors(*gameMap);
}

void Physics::ClearMap() {
  gameMap = nullptr;
}

void Physics::PlayerUpdate() {
  // Checking boundaries
  if(px < 0) 
    camera.position.x = 0;
  if(pz < 0) 
    camera.position.z = 0;
  if(px > gameMap->height) 
    camera.position.x = gameMap->height;
  if(pz > gameMap->width) 
    camera.position.z = gameMap->width;
  // Collision with the ground
  float mapHeight = pixels[pz * gameMap->width + px].r / 16.f;
  if(camera.position.y < mapHeight + player.radius) {
    camera.position.y = mapHeight + player.radius;
    airBorne = false;
    player.velocity.z = 0; // Velocity is Z because 
                           // for second argument UpdateCameraPro takes 
                           // vector3 with {Forward, Up, Right} directions
  }
  else if(camera.position.y > mapHeight + player.radius) airBorne = true;
  if(IsKeyDown(KEY_SPACE) && !airBorne) {
    player.velocity.z = 6.0f * dt;
    airBorne = true;
  } 
  else if(airBorne) {
    player.velocity.z -= g * dt;
  }
  camera.position.y += player.velocity.z * dt;
  UpdateCameraPro(&camera,player.velocity, player.rotation,0.0f);
}