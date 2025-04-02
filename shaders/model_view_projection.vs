// Determine the perspective projection (do not conduct division) in homogenous
// coordinates. If is_moon is true, then shrink the model by 70%, shift away
// from the origin by 2 units and rotate around the origin at a frequency of 1
// revolution per 4 seconds.
//
// Uniforms:
//                  4x4 view transformation matrix: transforms "world
//                  coordinates" into camera coordinates.
uniform mat4 view; // 4x4 perspective projection matrix: transforms
uniform mat4 proj; // number of seconds animation has been running
uniform float animationSeconds; // whether we're rendering the moon or the other object
uniform bool isMoon;

// Inputs:
//                  3D position of mesh vertex
in vec3 pos_vs_in; // vertex shader
// Ouputs:
//                   transformed and projected position in homogeneous
//                   coordinates
out vec4 pos_cs_in; // control shader
// expects: PI, model
void main()
{
  // Compute the model matrix (all the affine transformations in world space)
  mat4 m = model(isMoon, animationSeconds);

  // Apply transformations (model → view → projection)
  pos_cs_in = proj * view * m * vec4(pos_vs_in, 1.0);
}
