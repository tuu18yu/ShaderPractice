// Generate a procedural planet and orbiting moon. Use layers of (improved)
// Perlin noise to generate planetary features such as vegetation, gaseous
// clouds, mountains, valleys, ice caps, rivers, oceans. Don't forget about the
// moon. Use `animation_seconds` in your noise input to create (periodic)
// temporal effects.
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


  float frequency = 7.0;         // Frequency of Perlin noise
  float waveScale = 30.0;        // Frequency of sine wave (how many stripes)
  float distortion = 15.0;        // Strength of noise warping

  float noise = improved_perlin_noise(sphere_fs_in * frequency);
  float pattern = sin(waveScale * sphere_fs_in.y + distortion * noise);
  float marbleMoon = pattern * 0.5 + 0.5;

  frequency = 10.0;         // Frequency of Perlin noise
  waveScale = 8.0;        // Frequency of sine wave (how many stripes)
  distortion = 3.5;        // Strength of noise warping

  noise = improved_perlin_noise(sphere_fs_in * frequency);
  pattern = sin(waveScale * sphere_fs_in.y + distortion * noise);
  float marbleEarth = pattern * 0.5 + 0.5;
  marbleEarth = clamp(marbleEarth + 0.3, 0.0, 1.0);

  // Use it as mix factor between two colors
  vec3 baseColor;
  if (isMoon) 
{
    // Moon colors and texture
    vec3 color1 = vec3(58.0, 61.0, 66.0) / 255.0;
    vec3 color2 = vec3(119.0, 119.0, 128.0) / 255.0;
    baseColor = mix(color1, color2, marbleMoon); // bump or noise-based blend
}
else 
{
    vec3 shallowOceanColor = vec3(0.0, 0.5, 1.0);
    vec3 deepOceanColor    = vec3(0.0, 0.1, 0.4);
    vec3 landColor1 = vec3(173.0, 255.0, 47.0) / 255.0;
    vec3 landColor2 = vec3(34.0, 139.0, 34.0) / 255.0;

    float height = bump_height(isMoon, sphere_fs_in);
    float landThreshold = 0.02;

    // Mix land color by marbleEarth
    vec3 landColor = mix(landColor1, landColor2, marbleEarth);

    // Ocean depth factor: deeper = dark
    float depthFactor = smoothstep(-0.05, 0.1, height);
    vec3 oceanColor = mix(deepOceanColor, shallowOceanColor, depthFactor);

    // Final baseColor
    if (height < landThreshold)
        baseColor = oceanColor;
    else
        baseColor = landColor;
}

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
