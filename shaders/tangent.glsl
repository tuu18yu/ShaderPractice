// Input:
//   N  3D unit normal vector
// Outputs:
//   T  3D unit tangent vector
//   B  3D unit bitangent vector
void tangent(in vec3 N, out vec3 T, out vec3 B)
{
  // Pick a helper vector that's not parallel to N
  vec3 helper = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(0.0, 1.0, 0.0);

  // T is perpendicular to N
  T = normalize(cross(helper, N));

  // B is perpendicular to both N and T
  B = normalize(cross(N, T));
}
