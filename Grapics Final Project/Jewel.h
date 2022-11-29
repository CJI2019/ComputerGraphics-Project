#pragma once

class Jewel
{
private:
	glm::vec3 pos;
	glm::mat4 transformation;
	GLfloat size;
public:
	static objRead read_obj;
	static unsigned int vao;
	static unsigned int vbo[2];

	GLboolean cover_maze = false;

	Jewel();
	~Jewel();
	glm::vec3 get_pos();
	GLfloat get_size();
	GLvoid set_pos(GLfloat x,GLfloat y,GLfloat z);

	GLvoid draw(unsigned int& modelLocation);
};

objRead Jewel::read_obj; 
unsigned int Jewel::vao = 0;
unsigned int Jewel::vbo[2];

Jewel::Jewel()
{
	pos = glm::vec3{ 0.0f };
	size = 1.0f;
	transformation = glm::mat4{ 1.0f };

	if (Jewel::vao == 0) {
		Jewel::read_obj.loadObj_normalize_center("jewel.obj");

		glGenVertexArrays(1, &Jewel::vao);
		glGenBuffers(3, Jewel::vbo);

		glBindVertexArray(Jewel::vao);

		glBindBuffer(GL_ARRAY_BUFFER, Jewel::vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, Jewel::read_obj.color.size() * 3 * 4, Jewel::read_obj.color.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, Jewel::vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, Jewel::read_obj.outvertex.size() * 3 * 4, Jewel::read_obj.outvertex.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
	}
}

Jewel::~Jewel()
{

}

glm::vec3 Jewel::get_pos() { return pos; }

GLfloat Jewel::get_size() { return size; }


GLvoid Jewel::set_pos(GLfloat x, GLfloat y, GLfloat z)
{
	pos = { x, y + 5.0f * 1.0f, z };
	transformation = glm::mat4(1.0f);
	transformation = glm::translate(transformation, pos);
	transformation = glm::scale(transformation, glm::vec3(5.0f, 5.0f, 5.0f));
}

GLvoid Jewel::draw(unsigned int& modelLocation)
{
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformation));
	glBindVertexArray(Jewel::vao);
	glDrawArrays(GL_TRIANGLES, 0, Jewel::read_obj.outvertex.size());
}

