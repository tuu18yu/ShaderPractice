#include "glad/glad.h"
#include <Eigen/Core>

// Send a triangle mesh to the GPU using a vertex array object.
//
// Inputs:
//   V  #V by 3 list of 3D mesh vertex positions
//   F  #F by 3 list of triangle indices into V
// Outputs:
//   VAO  identifier of compiled vertex array object.
inline void mesh_to_vao(
  const Eigen::Matrix< float,Eigen::Dynamic,3,Eigen::RowMajor> & V,
  const Eigen::Matrix<GLuint,Eigen::Dynamic,3,Eigen::RowMajor> & F,
  GLuint & VAO);

// Implementation

inline void mesh_to_vao(
  const Eigen::Matrix< float,Eigen::Dynamic,3,Eigen::RowMajor> & V,
  const Eigen::Matrix<GLuint,Eigen::Dynamic,3,Eigen::RowMajor> & F,
  GLuint & VAO)
{
  // Generate and attach buffers to vertex array
  // Vertex Array Object - Stores how vertex data is used
  // What vertex attributes exist, Which buffers they use, Their formats, The bound element array buffer (EBO)
  glGenVertexArrays(1, &VAO);

  GLuint VBO, EBO;
  glGenBuffers(1, &VBO); // Vertex Buffer Object - Stores actual vertex data (positions, etc.)
  glGenBuffers(1, &EBO); // Element Buffer Object - Stores indices (which vertices make up each triangle)

  // Any buffer you bind or attribute layout you define will be stored inside VAO
  glBindVertexArray(VAO);

  // OpenGL stores: "Attribute 0 will use VBO bound to GL_ARRAY_BUFFER."
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float)*V.size(), V.data(), GL_STATIC_DRAW);

  // EBO is stored in the VAO binding, even though it's not part of a vertex attribute.
  // So when you bind this VAO later, OpenGL will also automatically re-bind this EBO.
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*F.size(), F.data(), GL_STATIC_DRAW);

  // "The data for attribute 0 is stored in the currently bound VBO (via GL_ARRAY_BUFFER). Each vertex takes 3 floats"
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  // Enables the attribute at index 0
  glEnableVertexAttribArray(0);
  
  // Unbinds the VBO from GL_ARRAY_BUFFER
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  //Unbinds the VAO 
  glBindVertexArray(0);
}