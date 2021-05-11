#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>

#include "mesh.h"
#include "camera.h"
#include "shader_utils.h"
#include "string.h"

#include <sys/time.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void setMesh(GLuint VBO, GLuint VAO, GLuint EBO, Mesh mesh);
//void buildShaderProgram(const char * vertexShaderSource, const char * fragmentShaderSource, GLuint shaderProgram);
GLuint createTexture(const char* textureFile);
void renderMesh(Mesh mesh, long int time, GLuint VAO, Camera camera);
void renderGround(Mesh mesh, GLuint VAO, Camera camera);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{   
 
    
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // Camera init

    Camera camera;
    camera.Init();

    // Mesh init
    Mesh mesh;
    //mesh.orient = 1;
    mesh.Init(10, 20, 0.2f, 1.0f);
    

    GLuint shaderProgram = glCreateProgram();
    mesh.shaderProgram = shaderProgram;
    mesh.linkShaders("shaders/grass.vert", "shaders/grass.frag");
    mesh.setMesh(mesh.VBO, mesh.VAO, mesh.EBO);
    mesh.loadTexture("data/grass_blade.png");
    
    std::cout << mesh.shaderProgram << std::endl;
    std::cout << mesh.textureID << std::endl;

    // Ground Init
    
    Mesh ground;
    ground.orient = 1;
    ground.Init(20,20, 2.0f, 2.0f);

    GLuint shaderGroundProgram = glCreateProgram();
    ground.shaderProgram = shaderGroundProgram;
    ground.linkShaders("shaders/ground.vert", "shaders/ground.frag");
    ground.setMesh(ground.VBO, ground.VAO, ground.EBO);
    ground.loadTexture("data/ground.png");

    std::cout << ground.shaderProgram << std::endl;
    std::cout << ground.textureID << std::endl;

    // time for shader
    struct timeval tp;

    gettimeofday(&tp, NULL);
    long int startTime = tp.tv_sec * 1000 + tp.tv_usec / 1000;;
    long int time = startTime;
    mesh.shaderTime = glGetUniformLocation(mesh.shaderProgram, "time");
    glUniform1f(mesh.shaderTime, time);

    mesh.PVID  = glGetUniformLocation(mesh.shaderProgram, "PV");
    ground.PVID  = glGetUniformLocation(ground.shaderProgram, "PV");
    

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        gettimeofday(&tp, NULL);
        time = tp.tv_sec * 1000 + tp.tv_usec / 1000;;
        time -= startTime;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        renderGround(ground, ground.VAO, camera);
        renderMesh(mesh, time, mesh.VAO, camera);

       
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    mesh.freeResources();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void renderMesh(Mesh mesh, long int time, GLuint VAO, Camera camera){
    glUseProgram(mesh.shaderProgram);
    //std::cout << "time " << time << std::endl;
    //time = time * 0.;
    glUniform1f(mesh.shaderTime, time*1e-3f);
    glUniformMatrix4fv(mesh.PVID, 1, GL_FALSE, &camera.PV[0][0]);
    // render
    // ------
    
    

    // draw our first triangle
    
    glBindTexture(GL_TEXTURE_2D, mesh.textureID);
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    //glDrawArrays(GL_TRIANGLES, 0, 6);




    glDrawElements(GL_TRIANGLES, mesh.getNumTriangles() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void renderGround(Mesh mesh, GLuint VAO, Camera camera){
    glUseProgram(mesh.shaderProgram);

    glUniformMatrix4fv(mesh.PVID, 1, GL_FALSE, &camera.PV[0][0]);
    
    glBindTexture(GL_TEXTURE_2D, mesh.textureID);
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDrawElements(GL_TRIANGLES, mesh.getNumTriangles() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

