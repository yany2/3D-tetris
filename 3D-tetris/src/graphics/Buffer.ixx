#include <glad/gl.h>

#include <iostream>

export module graphics:Buffer;

namespace graphics {
	class Buffer
	{
	private:
		unsigned int vao;
		unsigned int vbo;
		unsigned int ebo;
		unsigned int size;
		unsigned int indexc;
	public:
		Buffer
		(
			float* vertex,
			unsigned int vertexc,
			unsigned char* attrib,
			unsigned int attrc,
			unsigned int* index,
			unsigned int indexc
		);
		~Buffer();
		void draw(unsigned int drawMode = GL_TRIANGLES) const;
	};
}

graphics::Buffer::Buffer
(
	float* vertex,
	unsigned int vertexc,
	unsigned char* attrib,
	unsigned int attrc,
	unsigned int* index,
	unsigned int indexc
) : size(0), indexc(indexc)
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	for (int i = 0; i < attrc; i++)
		size += attrib[i];
	glBufferData(GL_ARRAY_BUFFER, vertexc * sizeof(float), vertex, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexc * sizeof(unsigned int), index, GL_STATIC_DRAW);

	int sum = 0;
	for (int i = 0; i < attrc; i++)
	{
		glVertexAttribPointer(i, attrib[i], GL_FLOAT, GL_FALSE, size * sizeof(float), (void*)(sum * sizeof(float)));
		sum += attrib[i];
		glEnableVertexAttribArray(i);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

graphics::Buffer::~Buffer()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

void graphics::Buffer::draw(unsigned int drawMode) const
{
	glBindVertexArray(vao);
	glDrawElements(drawMode, indexc, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}