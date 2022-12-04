#pragma once

class Jewel
{
private:
	glm::vec3 pos;
	glm::mat4 transformation;
	GLfloat size, angle;
	int respawn_time;
public:
	static objRead read_obj;
	static unsigned int vao;
	static unsigned int vbo[2];

	//true 면 미로임. false 면 벽
	GLboolean maze_state = true;

	//true 면 그려짐 false 면 안그려짐 (먹었는지 안먹었지는지)
	GLboolean status_draw = true;

	Jewel();
	~Jewel();
	glm::vec3 get_pos();
	GLfloat get_size();
	GLvoid set_pos(glm::vec3 m_pos, GLboolean m_state);

	GLvoid draw(unsigned int& modelLocation);
	GLfloat* get_bb();
	GLboolean collision(GLfloat* move_obj_bb);
	GLvoid respawn();
	GLvoid generate_big_jewel();
	GLvoid delete_big_jewel();
	GLvoid update();
};

objRead Jewel::read_obj; 
unsigned int Jewel::vao = 0;
unsigned int Jewel::vbo[2];

Jewel::Jewel()
{
	pos = glm::vec3{ 0.0f };
	//scale 한 크기의 절반
	size = 2.5f; angle = 0.0f;
	transformation = glm::mat4{ 1.0f };
	respawn_time = 0;

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


GLvoid Jewel::set_pos(glm::vec3 m_pos,GLboolean m_state)
{
	if (!m_state) {
		maze_state = false; status_draw = false;
		return;
	}

	pos = { m_pos.x, m_pos.y + 5.0f, m_pos.z };
	if (rand() % 50 == 0) {
		generate_big_jewel();
		return;
	}
	transformation = glm::mat4(1.0f);
	transformation = glm::translate(transformation, pos);
	//transformation = glm::scale(transformation, glm::vec3(5.0f, 5.0f, 5.0f));
}

GLvoid Jewel::update()
{
	angle += 1;
	transformation = glm::mat4(1.0f);
	transformation = glm::translate(transformation, pos);
	transformation = glm::rotate(transformation, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	if (size == 5.0f) {
		transformation = glm::scale(transformation, glm::vec3(2.0f, 2.0f, 2.0f));
	}
}


GLvoid Jewel::draw(unsigned int& modelLocation)
{
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformation));
	glBindVertexArray(Jewel::vao);
	glDrawArrays(GL_TRIANGLES, 0, Jewel::read_obj.outvertex.size());
}

GLfloat* Jewel::get_bb()
{
	GLfloat bb[4] = {
		pos.x - size, //left
		pos.x + size, //right
		pos.z - size, //bottom
		pos.z + size  //top
	};

	return bb;
}

GLboolean Jewel::collision(GLfloat* move_obj_bb)
{
	if (!maze_state || !status_draw) return false;

	GLfloat* Jewel_bb = get_bb();
	
	if (Jewel_bb[0] > move_obj_bb[1]) return false;
	if (Jewel_bb[1] < move_obj_bb[0]) return false;
	if (Jewel_bb[2] > move_obj_bb[3]) return false;
	if (Jewel_bb[3] < move_obj_bb[2]) return false;

	if (size == 5.0f) { // 거대 보석을 먹을때 몬스터 이벤트 처리 하면 됨.
		delete_big_jewel();
	}
	return true;
}

GLvoid Jewel::respawn()
{

	if (status_draw || !maze_state) return;

	respawn_time += 1;

	if (respawn_time == 2000) { //20초 가 지나면 생성
		status_draw = true;
	}
}

GLvoid Jewel::generate_big_jewel()
{
	size = 5.0f;
	pos.y += 5.0f;

	transformation = glm::mat4(1.0f);
	transformation = glm::translate(transformation, pos);
	transformation = glm::scale(transformation, glm::vec3(2.0f, 2.0f, 2.0f));
}

GLvoid Jewel::delete_big_jewel()
{
	size = 2.5f;
	pos.y -= 5.0f;

	transformation = glm::mat4(1.0f);
	transformation = glm::translate(transformation, pos);
}
