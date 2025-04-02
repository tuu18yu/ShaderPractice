#include "glad/glad.h"
#include <string>
#include <vector>

// Print information (e.g., compilation errors and warnings) about a give glsl
// shader.
//
// Inputs:
//   type_str  string identifying which kind of shader we're asking about (just
//   a prefix to print)
//   obj  id of object we're querying
//   paths  list of file paths containing corresponding shader source code
//     (assumings `#line ...` directive has been inserted between files).
// Returns true if printed anything.
bool print_shader_info_log(
  const std::string & type_str, 
  const GLuint obj,
  const std::vector<std::string> & paths);

// Implementation
#include "REDRUM.h"
#include "STR.h"

#include <iostream>

bool print_shader_info_log(
  const std::string & type_str, 
  const GLuint obj,
  const std::vector<std::string> & paths)
{
  GLint infologLength = 0;
  GLint charsWritten  = 0;
  
  glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
  
  if (infologLength > 0)
  {
    char * infoLog = new char[infologLength];
    std::cerr<<REDRUM("ERROR")<<": failed to compile "<<type_str<<std::endl;
    glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
    std::string log(infoLog);
    std::cerr<<log<<std::endl;
    delete[] infoLog;
    return true;
  }
  return false;
}