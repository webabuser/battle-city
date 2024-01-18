#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"
#include "Renderer/Texture2D.h"

GLfloat points[] = {
    0.0f, 0.5f, 0.0f,
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
	{


		// Initialize the library
        if (!glfwInit())
        {
            std::cout << "glfwInit() failed" << std::endl;
        	return -1;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


		// Create a windowed mode window and its OpenGL context
		GLFWwindow* pWindow = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "Battle City", nullptr, nullptr);
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
		
		glClearColor(0.5, 1, 0, 1); //Устанавливаем цвет буфера
		
        /********************************* Установка шадеров*****************
            // Устанавливаем шейдеры
            GLuint vs = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vs, 1, &vertex_shader, nullptr);
            glCompileShader(vs);

             GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fs, 1, &fragment_shader, nullptr);
            glCompileShader(fs);

            // Теперь надо слинковать шейдеры

            GLuint shader_program = glCreateProgram();
            glAttachShader(shader_program, vs);
            glAttachShader(shader_program, fs);
            glLinkProgram(shader_program);

            // После линковки удаляем шейдеры

            glDeleteShader(vs);
            glDeleteShader(fs);

            */
            //Заменили код выше объектным



            {

                ResourceManager resourceManager(argv[0]);

                auto pDefaultShaderProgram = resourceManager.loadShaders("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
                if (!pDefaultShaderProgram)
                {
                    std::cerr << "Can't create shader program: " << "DefaultShader" << std::endl;
                    return -1;
                }

                auto tex = resourceManager.loadTexture("DefaultTexture", "res/textures/map_16x16.png");

                // Генерируем буфер для информации от шейдеров
                GLuint points_vbo = 0;
                glGenBuffers(1, &points_vbo);
                glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
                glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

                // Генерируем буфер для информации от шейдеров цвета
                GLuint colors_vbo = 0;
                glGenBuffers(1, &colors_vbo);
                glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
                glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

                // Генерируем буфер для текстуры
                GLuint texCoord_vbo = 0;
                glGenBuffers(1, &texCoord_vbo);
                glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
                glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW);

                //Сейчас пока карта не знает что делать с данной информацией буферов

                GLuint vao = 0;
                glGenVertexArrays(1, &vao);
                glBindVertexArray(vao);

                glEnableVertexAttribArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

                glEnableVertexAttribArray(1);
                glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

                // Для координат текстуры
                glEnableVertexAttribArray(2);
                glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

            /***************************************************************************/
                pDefaultShaderProgram->use();
                pDefaultShaderProgram->setInt("tex", 0);

		    // Loop until the user closes the window
		    while (!glfwWindowShouldClose(pWindow))
		    {
			    // Render here
			    glClear(GL_COLOR_BUFFER_BIT);

                //glUseProgram(shader_program);
                // вместо функции выше используем объетный метод
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
	}
	
	std::cout << "Hello World" << std::endl;
	
	std::cin.get();
    return 0;
}