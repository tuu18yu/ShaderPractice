// Set the pixel color using Blinn-Phong shading (e.g., with constant blue and
// gray material color) with a bumpy texture.
// 
// Uniforms:
uniform mat4 view;
uniform mat4 proj;
uniform float animationSeconds;
uniform bool isMoon;
// Inputs:
//                     linearly interpolated from tessellation evaluation shader
//                     output
in vec3 sphere_fs_in;
in vec3 normal_fs_in;
in vec4 pos_fs_in; 
in vec4 view_pos_fs_in; 
// Outputs:
//               rgb color of this pixel
out vec3 color;
// expects: model, blinn_phong, bump_height, bump_position,
// improved_perlin_noise, tangent
void main()
{
  // Tangent and bitangent vectors (orthonormal to the normal)
  vec3 T, B;
  tangent(sphere_fs_in, T, B); 

  // Finite difference offset for bump derivatives
  float eps = 0.0001;

  // Compute bump height derivatives in tangent directions
  vec3 h = bump_position(isMoon, sphere_fs_in);
  vec3 hT = bump_position(isMoon, sphere_fs_in + eps * T);
  vec3 hB = bump_position(isMoon, sphere_fs_in + eps * B);

  // Approximate bumped normal using gradient
  vec3 dpdu = (hT - h) / eps;
  vec3 dpdv = (hB - h) / eps;
  
  vec3 n = normalize(cross(dpdu, dpdv));
  // if (dot(n, sphere_fs_in) < 0.0) n = -n;
  // n = normalize(mix(n, sphere_fs_in, 0.2));

  // Transform normal to view space
  vec3 normal = normalize((view * vec4(n, 0.0)).xyz);


  float frequency = 8.0;         // Frequency of Perlin noise
  float waveScale = 30.0;        // Frequency of sine wave (how many stripes)
  float distortion = 5.0;        // Strength of noise warping

  float noise = improved_perlin_noise(sphere_fs_in * frequency);
  float pattern = sin(waveScale * sphere_fs_in.y + distortion * noise);

  // Convert [-1, 1] to [0, 1]
  float marble = pattern * 0.5 + 0.5;

  // Use it as mix factor between two colors
  vec3 color1, color2, baseColor;

  if (isMoon) { color1 =  vec3(58.0, 61.0, 66.0) / 255.0; color2 = vec3(119.0, 119.0, 128.0) / 255.0;}
  else { color1 = vec3(27.0, 27.0, 224.0) / 255.0; color2 = vec3(0.0, 0.0, 205.0) / 255.0;}

  baseColor = mix(color1, color2, marble);

  // Light direction (orbiting in two axes)
  float angle = 2.0 * M_PI * (animationSeconds / 6.0);
  vec3 orbitXZ = normalize(vec3(cos(angle), 0.0, sin(angle)));
  vec3 orbitYZ = normalize(vec3(0.0, cos(angle), sin(angle)));
  vec3 lightDir = normalize(orbitXZ + orbitYZ);
  vec3 lightDirViewSpace = normalize((view * vec4(lightDir, 0.0)).xyz);

  // View direction
  vec3 viewDir = normalize(-view_pos_fs_in.xyz);

  // Apply lighting to procedural base color
  color = blinn_phong(baseColor, baseColor, baseColor, 1000.0, normal, viewDir, lightDirViewSpace);
}
