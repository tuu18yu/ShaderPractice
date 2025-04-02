// Add (hard code) an orbiting (point or directional) light to the scene. Light
// the scene using the Blinn-Phong Lighting Model.
//
// Uniforms:
uniform mat4 view;
uniform mat4 proj;
uniform float animationSeconds;
uniform bool isMoon;
// Inputs:
in vec3 sphere_fs_in;
in vec3 normal_fs_in;
in vec4 pos_fs_in; 
in vec4 view_pos_fs_in; 
// Outputs:
out vec3 color;
// expects: PI, blinn_phong
void main()
{
  vec3 objectColor;
  if (isMoon) { objectColor = vec3(119.0, 119.0, 128.0) / 255.0; }
  else { objectColor = vec3(0.0, 0.0, 205.0) / 255.0; }

  // Animate a directional light that orbits around the origin
  float angle = 2.0 * M_PI * (animationSeconds / 6.0);
  vec3 orbitXZ = normalize(vec3(cos(angle), 0.0, sin(angle)));
  vec3 orbitYZ = normalize(vec3(0.0, cos(angle), sin(angle)));

  vec3 lightDir = normalize(orbitXZ + orbitYZ);
  vec3 lightDirViewSpace = normalize((view * vec4(lightDir, 0.0)).xyz); // into view space

  // View direction: from point to camera
  vec3 viewDir = normalize(-view_pos_fs_in.xyz); // camera is at origin in view space 0 - view_pos_fs_in
  vec3 normal = normalize(normal_fs_in);
  // Call Blinn-Phong with ambient, diffuse, specular colors all = objectColor
  color = blinn_phong(objectColor, objectColor, objectColor, 1000.0, normal, viewDir, lightDirViewSpace);
}
