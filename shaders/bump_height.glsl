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

 // Earth: continents and ocean depth

  // Low-frequency noise defines land/ocean regions
  float base = improved_perlin_noise(s * 2.0); // shape of continents
  float landMask = smoothstep(0.05, 0.2, base); // 0 = ocean, 1 = land

  // Mid/high-frequency noise for bumpy terrain
  float detail1 = improved_perlin_noise(s * 8.0) * 0.05;
  float detail2 = improved_perlin_noise(s * 16.0) * 0.02;

  // Land: positive bump
  float landHeight = landMask * (0.05 + detail1 + detail2);

  // Ocean: dip below 0 using inverted noise
  float oceanDepth = (1.0 - landMask) * (-0.05 - 0.02 * improved_perlin_noise(s * 6.0));

  // Combine
  float height = landHeight + oceanDepth;

  return clamp(height, -0.2, 0.2);
}
