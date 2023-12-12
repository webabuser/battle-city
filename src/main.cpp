#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"
#include "Renderer/Texture2D.h"


#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
   x;\
   ASSERT(GLLogCall(#x, __FILE__, __LINE__));


static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL_Error] (" << error << "): " << function <<
        " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}





GLfloat point[] = {
    0.0f,  0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
   -0.5f, -0.5f, 0.0f
};

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};


GLfloat texCoord[] = {
    0.5f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f
}; 


int g_windowSizeX = 640;
int g_windowSizeY = 480;

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    g_windowSizeX = width;
    g_windowSizeY = height;

    glViewport(0, 0, g_windowSizeX, g_windowSizeY);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }

}


 

int main(int argc, char** argv)
{

		// Initialize the library
        if (!glfwInit())
        {
            std::cout << "glfwInit() failed" << std::endl;
        	return -1;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


		// Create a windowed mode window and its OpenGL context
		GLFWwindow* pWindow = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "Hello World", nullptr, nullptr);
		if (!pWindow)
		{
            std::cout << "glfwCreateWindow failed" << std::endl;
			glfwTerminate();
			return -1;
		}

        glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
		glfwSetKeyCallback(pWindow, glfwKeyCallback);
        
        // Make the window's context current
		glfwMakeContextCurrent(pWindow);

		if(!gladLoadGL())
		{
			std::cout << "Can't load Glad" << std::endl;
			return -1;
		}
		
        std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
        std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
		std::cout << "OpenGL" << GLVersion.major << "." << GLVersion.minor << std::endl;
		
		GLCall(glClearColor(0, 1, 0, 1)); //Устанавливаем цвет буфера
		
        {
            ResourceManager resourceManager(argv[0]);
            auto pDefaultShaderProgram = resourceManager.loadShaders("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");

            if (!pDefaultShaderProgram)
            {
                std::cerr << "Can't create shader program: " << "DefaultShader" << std::endl;
                return -1;
            }

            auto tex = resourceManager.loadTexture("DefaultTexture", "res/textures/map_16x16.png");

            GLuint points_vbo = 0;
            GLCall(glGenBuffers(1, &points_vbo));
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, points_vbo));
            GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW));

            GLuint colors_vbo = 0;
            GLCall(glGenBuffers(1, &colors_vbo));
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, colors_vbo));
            GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW));

            GLuint texCoord_vbo = 0;
            GLCall(glGenBuffers(1, &texCoord_vbo));
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo));
            GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW));

            GLuint vao = 0;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            GLCall(glEnableVertexAttribArray(0));
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, points_vbo));
            GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr));

            GLCall(glEnableVertexAttribArray(1));
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, colors_vbo));
            GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr));

            //Буфер для координат текстур
            GLCall(glEnableVertexAttribArray(2));
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo));
            GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr)); //2 координаты

            GLCall(pDefaultShaderProgram->use());
            GLCall(pDefaultShaderProgram->setInt("tex", 0)); //Устанавливаем текстуру в нулевой слот


		    // Loop until the user closes the window
		    while (!glfwWindowShouldClose(pWindow))
		    {
			    // Render here
			    glClear(GL_COLOR_BUFFER_BIT);


                pDefaultShaderProgram->use();
                glBindVertexArray(vao);
                tex->bind();
                glDrawArrays(GL_TRIANGLES, 0, 3);


			    // Swap front and back buffers
			    glfwSwapBuffers(pWindow);

			    // Poll for and process events
			    glfwPollEvents();
		    }
        }
		glfwTerminate();
	
	
	std::cout << "Hello World" <<"-"<<sizeof(colors) << std::endl;
	
	//std::cin.get();
    return 0;
}