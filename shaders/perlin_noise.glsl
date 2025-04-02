// Given a 3d position as a seed, compute a smooth procedural noise
// value: "Perlin Noise", also known as "Gradient noise".
//
// Inputs:
//   st  3D seed
// Returns a smooth value between (-1,1)
//
// expects: random_direction, smooth_step
float perlin_noise( vec3 st) 
{
  // Step 1: Get grid cell and local position
  vec3 i = floor(st);   // grid cell origin
  vec3 f = fract(st);   // local position within the cell (0..1)

  // Step 2: Get 8 random gradient vectors from corners of the cube
  vec3 c000 = random_direction(i + vec3(0.0, 0.0, 0.0));
  vec3 c100 = random_direction(i + vec3(1.0, 0.0, 0.0));
  vec3 c010 = random_direction(i + vec3(0.0, 1.0, 0.0));
  vec3 c110 = random_direction(i + vec3(1.0, 1.0, 0.0));
  vec3 c001 = random_direction(i + vec3(0.0, 0.0, 1.0));
  vec3 c101 = random_direction(i + vec3(1.0, 0.0, 1.0));
  vec3 c011 = random_direction(i + vec3(0.0, 1.0, 1.0));
  vec3 c111 = random_direction(i + vec3(1.0, 1.0, 1.0));

  // Step 3: Vectors from corners to point ()
  vec3 p000 = f - vec3(0.0, 0.0, 0.0);
  vec3 p100 = f - vec3(1.0, 0.0, 0.0);
  vec3 p010 = f - vec3(0.0, 1.0, 0.0);
  vec3 p110 = f - vec3(1.0, 1.0, 0.0);
  vec3 p001 = f - vec3(0.0, 0.0, 1.0);
  vec3 p101 = f - vec3(1.0, 0.0, 1.0);
  vec3 p011 = f - vec3(0.0, 1.0, 1.0);
  vec3 p111 = f - vec3(1.0, 1.0, 1.0);

  // Step 4: Dot products (influence)
  float n000 = dot(c000, p000);
  float n100 = dot(c100, p100);
  float n010 = dot(c010, p010);
  float n110 = dot(c110, p110);
  float n001 = dot(c001, p001);
  float n101 = dot(c101, p101);
  float n011 = dot(c011, p011);
  float n111 = dot(c111, p111);

  // Step 5: Smoothstep interpolation weights
  vec3 u = smooth_step(f); // vec3 smoothstep function applied to f

  // Step 6: Trilinear interpolation
  // value that blends n000 → n100 depending on how far along the X-axis
  float nx00 = mix(n000, n100, u.x);
  float nx10 = mix(n010, n110, u.x);
  float nx01 = mix(n001, n101, u.x);
  float nx11 = mix(n011, n111, u.x);

  float nxy0 = mix(nx00, nx10, u.y);
  float nxy1 = mix(nx01, nx11, u.y);

  float nxyz = mix(nxy0, nxy1, u.z);

  return nxyz; // final noise value ∈ (-1, 1)
}

