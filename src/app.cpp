#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <stdlib.h>


struct ShaderProgramSource
{
    std::string VertextSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);
    
    enum class ShaderType
    {
        NONE = -1, VERTEXT = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if(line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEXT;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
        
    }
 
    return { ss[0].str(), ss[1].str() };  
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id  = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *) alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Faild to compile "
                  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                  << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertextShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertextShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    int result;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        //char *message = (char *) alloca(length * sizeof(char));  // original code
        char* message = (char *) malloc(length * sizeof(char));
        glGetProgramInfoLog(program, length, &length, message);
        std::cout << "Failed to validate program" << std::endl;
        std::cout << message << std::endl;
        free(message);
        glDeleteProgram(program);
        return 0;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // /* Extra code to run 3.3 */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
    #endif

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // /* Initialise GLEW */
    if(glewInit() != GLEW_OK)
        std::cout << "Error: GLEW Initialisation" << std::endl;

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    float positions[] = {
            -0.5f, -0.5f,
             0.5f, -0.5f,
             0.5f, 0.5f,

             0.5f, 0.5f,
             -0.5f, 0.5f,
             -0.5f, -0.5f
    };

    GLuint vertexArrayID;
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    ShaderProgramSource source = ParseShader("resources/shaders/basic.shader");
    std::cout << "VERTEXT SOURE" << std::endl;
    std::cout << source.VertextSource << std::endl;

    std::cout << "FRAGMENT SOURE" << std::endl;
    std::cout << source.FragmentSource << std::endl;


    unsigned int shader = CreateShader(source.VertextSource, source.FragmentSource);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    { 
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        // My CODE 
        // glBegin(GL_TRIANGLES);
        // glVertex2f(-0.5f, -0.5f);
        // glVertex2f(0.0f, 0.5f);
        // glVertex2f(0.5f, -0.5f);
        // glEnd();
        // glFlush();

        //


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}
