// Create a bumpy surface by using procedural noise to generate a height (
// displacement in normal direction).
//
// Inputs:
//   is_moon  whether we're looking at the moon or centre planet
//   s  3D position of seed for noise generation
// Returns elevation adjust along normal values between -0.1 and 0.1 are
//   reasonable.
float bump_height( bool isMoon, vec3 s)
{
 if (isMoon) {
    // Keep original for moon — rough and cratered
    float noise = 0.0;
    float amplitude = 1.0;
    float totalAmplitude = 0.0;

    for (int i = 0; i < 5; ++i) {
      float freq = pow(2.0, float(i));
      float amp = amplitude * pow(0.5, float(i));
      noise += improved_perlin_noise(s * freq) * amp;
      totalAmplitude += amp;
    }

    noise /= totalAmplitude;
    noise = clamp(noise * 0.2, -0.1, 0.1);
    return noise * 1.8;
  }

  // --- Earth: continent-style bump ---

  // Low-frequency base defines land vs ocean shape
  float base = improved_perlin_noise(s * 2.0); // wide, slow continents
  base = smoothstep(0.05, 0.2, base); // sharpen land/ocean boundary

  // Add mid-frequency noise for terrain variation
  float detail1 = improved_perlin_noise(s * 8.0) * 0.05;
  float detail2 = improved_perlin_noise(s * 16.0) * 0.02;

  float height = base * (0.05 + detail1 + detail2); // bump only land

  return clamp(height, 0.0, 0.12); // ocean stays at 0
}
