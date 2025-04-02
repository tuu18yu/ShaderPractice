// Inputs:
//   t  3D vector by which to translate
// Return a 4x4 matrix that translates and 3D point by the given 3D vector
mat4 translate(vec3 t)
{
  mat4 m = identity();
  m[3][0] = t.x;
  m[3][1] = t.y;
  m[3][2] = t.z;

  return m;
}

