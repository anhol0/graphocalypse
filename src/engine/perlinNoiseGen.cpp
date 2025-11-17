#include "raylib.h"
#include <rlights.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <random>
#include <vector>

class PerlinNoise {
public:
  PerlinNoise(int size, float scale);
  PerlinNoise(int size, float scaleLocal, std::uint32_t seed);
  Image genImage();
  std::vector<uint8_t> getPerlinNoiseData();

  private:
  int width, height;
  float scale;
  double perlin(double x, double y);
  std::random_device random_device;
  std::array<int, 512> genPpermutation(std::uint32_t seedGen);
  double fade(double t);
  double dotP(int hash, double x, double y);
  double lerp(double a, double b, double x);
  std::array<int, 512> p;
};

PerlinNoise::PerlinNoise(int size, float scaleLocal) {
  width = size;
  height = size;
  scale = scaleLocal;
  p = genPpermutation(random_device());
}

PerlinNoise::PerlinNoise(int size, float scaleLocal, std::uint32_t seed) {
  width = size;
  height = size;
  scale = scaleLocal;
  p = genPpermutation(seed);
}

Image PerlinNoise::genImage() {
  Image image = GenImageColor(width, height, BLACK);
  for(int y = 0; y < height; y++) {
    for(int x = 0; x < width; x++) {
      double temp = perlin(x*scale, y*scale);
      int color = (int)((temp+1) * 0.5 * 255);
      if(color < 0) color = 0;
      if(color > 255) color = 255;
      ImageDrawPixel(&image, x, y, (Color){(unsigned char)color, (unsigned char)color, (unsigned char)color, 255});
    }
  }
  return image;
}

std::vector<uint8_t> PerlinNoise::getPerlinNoiseData() {
  std::vector<uint8_t> noiseData;
  noiseData.reserve(width * height);
  for(int y = 0; y < height; y++) {
    for(int x = 0; x < width; x++) {
      double temp = perlin(x*scale, y*scale);
      uint8_t color = (uint8_t)((temp+1) * 0.5 * 255);
      noiseData.push_back(color);
    }
  }
  return noiseData;
}

double PerlinNoise::perlin(double x, double y) {
  int xi = int(x)&255;
  int yi = int(y)&255;

  double xf = x-int(x);
  double yf = y-int(y);

  double u = fade(xf);
  double v = fade(yf);

  int topLeft = p[p[xi]+yi];
  int topRight = p[p[xi+1]+yi];
  int bottomLeft = p[p[xi]+yi+1];
  int bottomRight = p[p[xi+1]+yi+1];

  double dotTopLeft = dotP(topLeft, xf, yf);
  double dotTopRight = dotP(topRight, xf-1, yf);
  double dotBottomLeft = dotP(bottomLeft, xf, yf-1);
  double dotBottomRight = dotP(bottomRight, xf-1, yf-1);

  double x1 = lerp(dotTopLeft, dotTopRight, u);
  double x2 = lerp(dotBottomLeft, dotBottomRight, u);

  double result = lerp(x1, x2, v);
  return result;
}

double PerlinNoise::dotP(int hash, double x, double y) {
  switch(hash & 3) {
    case 0: return x + y;
    case 1: return -x + y;
    case 2: return -x - y;
    case 3: return x - y;
    default: return 0;
  }
}

double PerlinNoise::lerp(double a, double b, double x) {
  return a + x*(b - a);
}

double PerlinNoise::fade(double t) {
  double result = 6 * std::pow(t, 5) - 
                  15 * std::pow(t,4) + 
                  10 * std::pow(t, 3);
  return result;
}

std::array<int, 512> PerlinNoise::genPpermutation(std::uint32_t seedGen) {
  std::array<int, 256> permutationInitial;
  std::mt19937 seed(seedGen);
  for(int i = 0; i < 256; i++) {
    permutationInitial[i] = i;
  }
  std::shuffle(permutationInitial.begin(), permutationInitial.end(), seed);
  std::array<int, 512> permutationFinal;
  for(int i = 0; i < 512; i++) {
    permutationFinal[i] = permutationInitial[i%256];
  }  
  return permutationFinal;
}


