#include "Square.hpp"
#include "stb_image.h"

GLuint instanceVBO;

Square::Square(std::string path)
{

	GLfloat translations[10];
	GLint index = 0;
	for (GLint x = 0; x < 10; x++)
	{
		translations[index++] = (GLfloat)x / 10.0f;
	}

	std::vector<float> vertices = {
		0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f};
	std::vector<unsigned int> indices = {
		0, 1, 3,
		1, 2, 3};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glGenBuffers(1, &instanceVBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(translations), translations, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_st), (void *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_st), (void *)offsetof(Vertex_st, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_st), (void *)offsetof(Vertex_st, texcoords));
	glEnableVertexAttribArray(3);
	// glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,sizeof(GLfloat), (GLvoid*)0);
	// glVertexAttribDivisor(2, 3);

	glBindVertexArray(0);

	textureId = textureFromfile(path.c_str());
}

Square::~Square()
{
}

void Square::draw(Shader shader)
{
	glActiveTexture(GL_TEXTURE0);
	shader.setInt("texture_diffuse1", 0);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glBindVertexArray(VAO);
	// glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 10);
	// glDrawArraysInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 10);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

unsigned int Square::textureFromfile(const char *path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
		{
			format = GL_RED;
		}
		else if (nrComponents == 3)
		{
			format = GL_RGB;
		}
		else if (nrComponents == 4)
		{
			format = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "TEXTURE FAILED TO LOAD PATH " << path << std::endl;
		stbi_image_free(data);
	}
	return (textureID);
}