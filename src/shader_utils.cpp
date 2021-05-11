#include "shader_utils.h"
#include <tuple>
#include <iostream>
#include <fstream>
#include <sstream>



std::string shaderUtils::add_shader(std::string fileName)
{
    std::string src; int32_t size;

    //Читаем файл с кодом шейдера
    std::ifstream t;
    std::string fileContent;

    t.open(fileName);
    if(t.is_open())
    {
        std::stringstream buffer;
        buffer << t.rdbuf();
        fileContent = buffer.str();
    }
    else std::cout<<"File "<<fileName<<" opening failed"<<std::endl;
    t.close();
    size = fileContent.length();  //Set the Size

    std::string result = fileContent;
   
    src = result;
    
    
    //std::cout << src << std::endl;
    return src;
}

void shaderUtils::buildShaderProgram(const char * vertexShaderSource, const char * fragmentShaderSource, GLuint shaderProgram)
{
        // build and compile our shader program
    // ------------------------------------
    // vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // link shaders
    //GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return;
}

GLuint shaderUtils::createTexture(const char* textureFile)
{
	GLuint textureID = 0;
	
	int width, height, numComponents;
    unsigned char* image = SOIL_load_image(textureFile, &width, &height, 0, SOIL_LOAD_RGBA);
    if (image == NULL){
        std::cout << "FIASCO" << image << std::endl;
    }

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);


    
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0); 

	return textureID;
}