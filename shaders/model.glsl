// Construct the model transformation matrix. The moon should orbit around the
// origin. The other object should stay still.
//
// Inputs:
//   is_moon  whether we're considering the moon
//   time  seconds on animation clock
// Returns affine model transformation as 4x4 matrix
//
// expects: identity, rotate_about_y, translate, PI
mat4 model(bool isMoon, float time)
{
  if (isMoon)
  {
    mat4 scaleMat = uniform_scale(0.3);
    mat4 translateMat = translate(vec3(0.0, 0.0, 2.0));

    float angle = 2.0 * M_PI * (time / 4.0);  
    mat4 rotation = rotate_about_y(angle);

    return rotation * translateMat * scaleMat;
  }

  return identity();
}
