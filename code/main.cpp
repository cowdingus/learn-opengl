#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"
#include "stb_image.h"

#include <iostream>
#include <stdexcept>

void framebuffer_size_callback([[maybe_unused]] GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec3 aColor;\n"
	"out vec3 ourColor;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"	ourColor = aColor;\n"
	"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
	"in vec3 ourColor;\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(ourColor.rgb, 1.0f);\n"
	"}\0";

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Trianglo", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	/******************** Texture ********************/
	stbi_set_flip_vertically_on_load(true);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("assets/images/container.jpg", &width, &height, &nrChannels, 0);

	if (!data)
	{
		stbi_image_free(data);
		throw std::runtime_error("Failed to load container.jpg");
	}

	unsigned int texture;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);

	data = stbi_load("assets/images/awesomeface.png", &width, &height, &nrChannels, 0);

	if (!data)
	{
		stbi_image_free(data);
		throw std::runtime_error("Failed to load awesomeface.png");
	}

	unsigned int texture1;
	glGenTextures(1, &texture1);

	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);
	/******************** Texture ********************/

	/******************** Triangle ********************/
	float vertices[] = {
		// positions         // colors
		0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f    // top
	};

	// Make a VBO object
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// Make a VAO object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	/******************** Triangle ********************/

	/******************** Rectangle ********************/
	float rectangleVertices[] = {
		// positions         // colors        // texcoords
		0.5f,  0.5f, 0.0f,   1.f, 1.f, 1.f,   1.0f, 1.0f,  // top right
		0.5f, -0.5f, 0.0f,   1.f, 1.f, 1.f,   1.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  1.f, 1.f, 1.f,   0.0f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f,  1.f, 1.f, 1.f,   0.0f, 1.0f,  // top left
	};
	unsigned int rectangleIndices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	// Make a VBO object
	unsigned int rectangleVBO;
	glGenBuffers(1, &rectangleVBO);

	// Make a EBO object
	unsigned int rectangleEBO;
	glGenBuffers(1, &rectangleEBO);

	// Make a VAO object
	unsigned int rectangleVAO;
	glGenVertexArrays(1, &rectangleVAO);

	glBindVertexArray(rectangleVAO);

	glBindBuffer(GL_ARRAY_BUFFER, rectangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectangleEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleIndices), rectangleIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	/******************** Rectangle ********************/

	#define USE_SHADER_CLASS 1
	#if(!USE_SHADER_CLASS)
	/******************** SHADER ********************/
	// Make a shader object for vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// TODO: CHECK SHADER COMPILATION STATUS NOTE: IT'S DONE
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << infoLog << std::endl;
		throw std::runtime_error("Failed to compile vertex shader");
	}

	// Make a shader object for fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cerr << infoLog << std::endl;
		throw std::runtime_error("Failed to compile fragment shader");
	}

	// Use both shader (bind it)
	// Create shader program

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << infoLog << std::endl;
		throw std::runtime_error("Failed to link shader program");
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	/******************** SHADER ********************/
	#else
	Shader shader("assets/shaders/vertexShader.glsl", "assets/shaders/fragmentShader.glsl");
	#endif

	#if(!USE_SHADER_CLASS)
	glUseProgram(shaderProgram);
	#else
	shader.use();
	#endif

	#define DRAW_RECTANGLE 1
	#if(DRAW_RECTANGLE)

	#if(USE_SHADER_CLASS)
	shader.setInt("ourTexture1", 0);
	shader.setInt("ourTexture2", 1);
	#endif

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glBindVertexArray(rectangleVAO);
	#elif(DRAW_TRIANGLE)
	glBindVertexArray(VAO);
	#endif

	/*
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
	*/

	unsigned int transformLoc = glGetUniformLocation(shader.getId(), "transform");

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		#if(DRAW_RECTANGLE)
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		#elif(DRAW_TRIANGLE)
		glDrawArrays(GL_TRIANGLES, 0, 3);
		#endif

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}
