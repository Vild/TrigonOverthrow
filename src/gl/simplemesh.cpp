#include "simplemesh.hpp"

SimpleMesh::SimpleMesh()
{
	drawMode = GL_POINTS;
	maxInstances = 0;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);
}

SimpleMesh::~SimpleMesh()
{
}

SimpleMesh & SimpleMesh::setDrawMode(GLenum mode)
{
	drawMode = mode;
	return *this;
}

SimpleMesh & SimpleMesh::addVertex(const glm::vec3 & vertex)
{
	vertices.push_back(vertex);
	return *this;
}


void SimpleMesh::finalize(int maxInstances)
{
	this->maxInstances = maxInstances;

	glBindVertexArray(VAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, IBO);
		{
			glBufferData(GL_ARRAY_BUFFER, maxInstances * sizeof(glm::mat4), NULL, GL_STREAM_DRAW);

			int location = 1;
			for (int i = 0; i < 4; i++)
			{
				glEnableVertexAttribArray(location + i);
				glVertexAttribPointer(location + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(i * sizeof(glm::vec4)));
				glVertexAttribDivisor(location + i, 1);
			}
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	glBindVertexArray(0);
}

void SimpleMesh::draw()
{
	glBindVertexArray(VAO);
	{
		glDrawArrays(drawMode, 0, vertices.size());
	}
	glBindVertexArray(0);
}

void SimpleMesh::draw(std::vector<glm::mat4>& instances)
{
	glBindBuffer(GL_ARRAY_BUFFER, IBO);
	{
		if (instances.size() > maxInstances)
		{
			maxInstances = instances.size();
			glBufferData(GL_ARRAY_BUFFER, maxInstances * sizeof(glm::mat4), &instances.front(), GL_STREAM_DRAW);
		}
		else
		{
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * instances.size(), &instances.front());
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(VAO);
	{
		glDrawArraysInstanced(drawMode, 0, vertices.size(), instances.size());
	}
	glBindVertexArray(0);
}
