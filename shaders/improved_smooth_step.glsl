// Filter an input value to perform an even smoother step. This function should
// be a quintic polynomial with improved_smooth_step(0) = 0,
// improved_smooth_step(1) = 1, and zero first _and_ second derivatives at f=0
// and f=1. "Improving Noise" [Perlin 2002].
//
// Inputs:
//   f  input value
// Returns filtered output value
float improved_smooth_step( float f)
{
  return f * f * f * (f * (f * 6.0 - 15.0) + 10.0);
}
vec3 improved_smooth_step( vec3 f)
{
  return f * f * f * (f * (f * 6.0 - 15.0) + 10.0);
}
