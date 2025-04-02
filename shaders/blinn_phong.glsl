// Compute Blinn-Phong Shading given a material specification, a point on a
// surface and a light direction. Assume the light is white and has a low
// ambient intensity.
//
// Inputs:
//   ka  rgb ambient color
//   kd  rgb diffuse color
//   ks  rgb specular color
//   p  specular exponent (shininess)
//   n  unit surface normal direction
//   v  unit direction from point on object to eye
//   l  unit light direction
// Returns rgb color
vec3 blinn_phong(vec3 ka, vec3 kd, vec3 ks, float p, vec3 n, vec3 v, vec3 l)
{
  vec3 lightColor = vec3(1.0, 1.0, 1.0);

  // ambient light: a little bit of light that exists even when it's dark
  float ambientStrength = 0.00001;
  vec3 ambient = ambientStrength * lightColor * ka;

  // diffuse light: directional impact of light on object (more a part of an object faces the light source, the brighter it becomes)
  // dependent on angle between surface normal and light direction (closer to 1, surface facing towards light)
  float diff = max(dot(n, l), 0.0);
  vec3 diffuse = diff * lightColor * kd;

  // specular light: simulates the bright spot of a light that appears on shiny objects
  // dependent on angle between surface normal, light direction, and view direction from eye to object
  // closer the reflection of light is to view direction brigher

  // phong model
  // vec3 r = reflect(-l, n);                     // reflected light direction
  // float spec = pow(max(dot(r, v), 0.0), p);    // angle between reflected light and view direction

  // blinn_phong model
  vec3 h = normalize(l + v); // halfway vector
  float spec = pow(max(dot(n, h), 0.0), p);
  vec3 specular = spec * lightColor * ks;

  return ambient + diffuse + specular;
}


