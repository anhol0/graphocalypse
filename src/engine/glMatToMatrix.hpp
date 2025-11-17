#include <raylib.h>
#include <glm/matrix.hpp>

inline Matrix glMatToMatrix(glm::mat4 matrix) {
  Matrix finalMat;
  finalMat.m0 = matrix[0][0];
  finalMat.m4 = matrix[1][0];
  finalMat.m8 = matrix[2][0];
  finalMat.m12 = matrix[3][0];

  finalMat.m1 = matrix[0][1];
  finalMat.m5 = matrix[1][1];
  finalMat.m9 = matrix[2][1];
  finalMat.m13 = matrix[3][1];

  finalMat.m2 = matrix[0][2];
  finalMat.m6 = matrix[1][2];
  finalMat.m10 = matrix[2][2];
  finalMat.m14 = matrix[3][2];

  finalMat.m3 = matrix[0][3];
  finalMat.m7 = matrix[1][3];
  finalMat.m11 = matrix[2][3];
  finalMat.m15 = matrix[3][3];
  
  return finalMat;
}