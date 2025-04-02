// Given a 3d position as a seed, compute an even smoother procedural noise
// value. "Improving Noise" [Perlin 2002].
//
// Inputs:
//   st  3D seed
// Values between  -½ and ½ ?
//
// expects: random_direction, improved_smooth_step
float improved_perlin_noise( vec3 st) 
{
 // 1. Grid corner and local position within cell
  vec3 i = floor(st);
  vec3 f = fract(st);

  // 2. Gradient directions at 8 cube corners
  vec3 c000 = random_direction(i + vec3(0.0, 0.0, 0.0));
  vec3 c100 = random_direction(i + vec3(1.0, 0.0, 0.0));
  vec3 c010 = random_direction(i + vec3(0.0, 1.0, 0.0));
  vec3 c110 = random_direction(i + vec3(1.0, 1.0, 0.0));
  vec3 c001 = random_direction(i + vec3(0.0, 0.0, 1.0));
  vec3 c101 = random_direction(i + vec3(1.0, 0.0, 1.0));
  vec3 c011 = random_direction(i + vec3(0.0, 1.0, 1.0));
  vec3 c111 = random_direction(i + vec3(1.0, 1.0, 1.0));

  // 3. Vectors from each corner to the point
  vec3 p000 = f - vec3(0.0, 0.0, 0.0);
  vec3 p100 = f - vec3(1.0, 0.0, 0.0);
  vec3 p010 = f - vec3(0.0, 1.0, 0.0);
  vec3 p110 = f - vec3(1.0, 1.0, 0.0);
  vec3 p001 = f - vec3(0.0, 0.0, 1.0);
  vec3 p101 = f - vec3(1.0, 0.0, 1.0);
  vec3 p011 = f - vec3(0.0, 1.0, 1.0);
  vec3 p111 = f - vec3(1.0, 1.0, 1.0);

  // 4. Dot products between gradient and offset vectors
  float n000 = dot(c000, p000);
  float n100 = dot(c100, p100);
  float n010 = dot(c010, p010);
  float n110 = dot(c110, p110);
  float n001 = dot(c001, p001);
  float n101 = dot(c101, p101);
  float n011 = dot(c011, p011);
  float n111 = dot(c111, p111);

  // 5. Apply improved_smooth_step to f
  vec3 u = improved_smooth_step(f);

  // 6. Trilinear interpolation with smoothstep weights
  float nx00 = mix(n000, n100, u.x);
  float nx10 = mix(n010, n110, u.x);
  float nx01 = mix(n001, n101, u.x);
  float nx11 = mix(n011, n111, u.x);

  float nxy0 = mix(nx00, nx10, u.y);
  float nxy1 = mix(nx01, nx11, u.y);

  float nxyz = mix(nxy0, nxy1, u.z);

  return nxyz; // Value typically in [-0.5, 0.5]
}

