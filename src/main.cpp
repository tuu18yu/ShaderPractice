#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Eigen/Core>
#include <Eigen/Geometry>

#include <iostream>
#include <algorithm>

#include "read_json.h"
#include "icosahedron.h"
#include "mesh_to_vao.h"
#include "create_shader_program_from_files.h"

#include "config.h"
#include "Input.h"

const std::string FILEPATH = "data/test-07.json";

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

bool isWireFrame = false;
bool isAnimating = true;
double lastTime = get_seconds();
double animationSeconds = 0;

GLuint prog_id=0;
Eigen::Affine3f view = Eigen::Affine3f::Identity() * Eigen::Translation3f(Eigen::Vector3f(0, 0, -10));
Eigen::Matrix4f proj = Eigen::Matrix4f::Identity();

GLuint VAO;
// Mesh data: RowMajor is important to directly use in OpenGL
Eigen::Matrix<float, Eigen::Dynamic, 3, Eigen::RowMajor> V; // vertices
Eigen::Matrix<GLuint, Eigen::Dynamic, 3, Eigen::RowMajor> F; // face

int main()
{
    std::vector<std::string> vertex_shader_paths;
    std::vector<std::string> tess_control_shader_paths;
    std::vector<std::string> tess_evaluation_shader_paths;
    std::vector<std::string> fragment_shader_paths;

    GLFWwindow* window;

    if (!glfwInit())
    {
        std::cerr << "GLFW could not start\n";
        return -1;
    }

    const auto & error = [] (int error, const char* description)
    {
        std::cerr << description << std::endl;
    };

    glfwSetErrorCallback(error);

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    window =  glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Shader Practice", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    //glfwSetWindowPos(window, 0, 0); // set upper-left corner to 0, 0
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // load icosahedron mesh verticies and face to V and F
    icosahedron(V, F);
    // take V and F(mesh) to GPU
    mesh_to_vao(V, F, VAO);

    const auto & reshape = [](
        GLFWwindow* window,
        int _width,
        int _height)
    {
        ::WINDOW_WIDTH = _width, ::WINDOW_HEIGHT = _height;

        // set up projection matrix
        float nearVal = 0.01;
        float farVal = 100;
        float top = tan(35./360.* M_PI)*nearVal;
        float right = top * (double)::WINDOW_WIDTH/(double)::WINDOW_HEIGHT;
        float left = -right;
        float bottom = -top;
        proj.setConstant(4,4,0.);
        proj(0,0) = (2.0 * nearVal) / (right - left);
        proj(1,1) = (2.0 * nearVal) / (top - bottom);
        proj(0,2) = (right + left) / (right - left);
        proj(1,2) = (top + bottom) / (top - bottom);
        proj(2,2) = -(farVal + nearVal) / (farVal - nearVal);
        proj(3,2) = -1.0;
        proj(2,3) = -(2.0 * farVal * nearVal) / (farVal - nearVal);
    };
    
    int width_window, height_window;
    glfwGetWindowSize(window, &width_window, &height_window);
    reshape(window, width_window, height_window);

    //Set up user inputs
    Input::Init(window);
    // Set up window resizing
    glfwSetWindowSizeCallback(window, reshape);

    glEnable(GL_DEPTH_TEST); // Ensures that fragments (pixels) closer to the camera are rendered in front 
    glEnable(GL_CULL_FACE); //  Skips rendering triangles that are facing away from the camera

    // Force compilation on first iteration through loop
    double time_of_last_shader_compilation = 0; // initialize to 0
    double time_of_last_json_load = 0;
    const auto any_changed = [] (const std::vector<std::string> &paths, const double time_of_last_shader_compilation) -> bool
    {
        for(const auto & path : paths)
        {
            double t = last_modification_time(path);
            if (t < 0.0) { std::cerr << "Error: Could not get file modification time.\n"; } 
            else if (t == 0.0) { std::cout << "Warning: File's mod time is the Unix epoch (probably not right).\n"; }

            if (t > time_of_last_shader_compilation)
            {
                std::cout<< path << " has changed since last compilation attempt."<< std::endl;
                return true;
            }
        }
        return false;
    };

    float start_time = get_seconds();

    std::cout << R"(Usage:
          [Click and drag]  to orbit view
          [Scroll]  to translate view in and out
          A,a  toggle animation
          L,l  toggle wireframe rending
          Z,z  reset view to look along z-axis
        )";


    // main loop
    while (!glfwWindowShouldClose(window))
    {
        double tic = get_seconds();

        // read json files from file path if there was change in file
        if(any_changed({ FILEPATH }, time_of_last_json_load))
        {
            std::cout<<"-----------------------------------------------"<<std::endl;
            time_of_last_json_load = get_seconds();
            if (!read_json(FILEPATH, vertex_shader_paths, tess_control_shader_paths, tess_evaluation_shader_paths, fragment_shader_paths))
            {
                std::cerr<<"Failed to read "<< FILEPATH << std::endl;
            }

            // force reload of shaders
            time_of_last_shader_compilation = 0;
        }

        // compile shader program if there was any change in shader file
        if (any_changed(vertex_shader_paths, time_of_last_shader_compilation) ||
            any_changed(tess_control_shader_paths, time_of_last_shader_compilation) ||
            any_changed(tess_evaluation_shader_paths, time_of_last_shader_compilation) ||
            any_changed(fragment_shader_paths, time_of_last_shader_compilation))
        {
            std::cout<<"-----------------------------------------------"<<std::endl;
            // remember the time we tried to compile
            time_of_last_shader_compilation = get_seconds();
            if (!create_shader_program_from_files( vertex_shader_paths, tess_control_shader_paths, tess_evaluation_shader_paths, fragment_shader_paths, prog_id))
            {
                // Force null shader to visually indicate failure
                glDeleteProgram(prog_id);
                prog_id = 0;
                std::cout<<"-----------------------------------------------"<<std::endl;
            }
        }
        
        glClearColor(0,0,0,0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear screen and depth
        glfwGetFramebufferSize(window, &::WINDOW_WIDTH, &::WINDOW_HEIGHT); 
        glViewport(0, 0, ::WINDOW_WIDTH, ::WINDOW_HEIGHT);

        glUseProgram(prog_id);

        // Attach uniforms
        if(isAnimating)
        {
          double now = get_seconds();
          animationSeconds += now - lastTime;
          lastTime = now;
        }

        // get memory location from shader in program and set the value to that location
        glUniform1f(glGetUniformLocation(prog_id,"animationSeconds"), animationSeconds);
        glUniformMatrix4fv( glGetUniformLocation(prog_id,"proj"),1,false,proj.data());
        glUniformMatrix4fv( glGetUniformLocation(prog_id,"view"),1,false,view.matrix().data());

        // Draw mesh as wireframe
        isWireFrame ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        for(int i = 0; i < 2; i++)
        {
            glUniform1i(glGetUniformLocation(prog_id, "isMoon"), i==1); // set true or false
            glBindVertexArray(VAO); // Bind your VAO (this enables your VBO + EBO + attributes)
            glDrawElements(GL_PATCHES, F.size(), GL_UNSIGNED_INT, 0); // draw patches
            glBindVertexArray(0); // Unbind VAO
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (Input::IsKeyPressed(GLFW_KEY_ESCAPE)) { glfwSetWindowShouldClose(window, true); }
        if (Input::IsKeyPressed(GLFW_KEY_A))
        {
            isAnimating ^= 1;
            if(isAnimating) { lastTime = get_seconds(); }
        }
        
        if (Input::IsKeyPressed(GLFW_KEY_L))
        {
            isWireFrame ^= 1;
            if (isWireFrame) { glDisable(GL_CULL_FACE); } 
            else { glEnable(GL_CULL_FACE); }
        }

        if (Input::IsKeyPressed(GLFW_KEY_Z))
        {
            view.matrix().block(0,0,3,3).setIdentity();
        }

        if (Input::IsMousePressed())
        {
            float factor = std::abs(view.matrix()(2,3));
            double dx = Input::GetMouseDeltaX();
            double dy = Input::GetMouseDeltaX();

            view.rotate(Eigen::AngleAxisf( dx*factor/float(width_window), Eigen::Vector3f(0,1,0)));
            view.rotate(Eigen::AngleAxisf( dy*factor/float(height_window), view.matrix().topLeftCorner(3,3).inverse()*Eigen::Vector3f(1,0,0)));
        }

        if (Input::IsMouseScrolled())
        {
            
            view.matrix()(2,3) = std::min(std::max(view.matrix()(2,3)+(float)Input::GetScrollY(), -100.0f), -2.0f);
        }

        Input::Update();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0; 
}