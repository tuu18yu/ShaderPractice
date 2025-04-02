// Set the pixel color to blue or gray depending on is_moon.
//
// Uniforms:
uniform bool isMoon;
// Outputs:
out vec3 color;
void main()
{
  if (isMoon) { color = vec3(119.0, 119.0, 128.0) / 255.0; }
  else { color = vec3(24.0, 27.0, 201.0) / 255.0; }
}
