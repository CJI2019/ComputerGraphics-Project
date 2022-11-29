#pragma once

class Jewel
{
private:
	glm::vec3 pos;
	glm::mat4 transformation;
	GLfloat size;
public:

	GLboolean cover_maze = false;

	Jewel();
	~Jewel();
	glm::vec3 get_pos();
	GLfloat get_size();
	GLvoid set_pos(GLfloat x,GLfloat y,GLfloat z);
	GLvoid draw(unsigned int& modelLocation, objRead obj);
};

Jewel::Jewel()
{
	pos = glm::vec3{ 0.0f };
	size = 1.0f;
	transformation = glm::mat4{ 1.0f };
}

Jewel::~Jewel()
{

}

glm::vec3 Jewel::get_pos() { return pos; }

GLfloat Jewel::get_size() { return size; }


GLvoid Jewel::set_pos(GLfloat x, GLfloat y, GLfloat z)
{
	pos = { x, y, z };
	transformation = glm::mat4(1.0f);
	transformation = glm::translate(transformation, pos);
	transformation = glm::scale(transformation, glm::vec3(5.0f, 5.0f, 5.0f));
}

GLvoid Jewel::draw(unsigned int& modelLocation, objRead obj)
{
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformation));
	glBindVertexArray(obj.vao);
	glDrawArrays(GL_TRIANGLES, 0, obj.outvertex.size());
}

