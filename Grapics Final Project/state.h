#pragma once

namespace STATE
{
	GLboolean perspective = true;
	GLboolean dir[4] = { false, false, false, false };
	GLboolean quarter_view = true;
	GLboolean minimap_perspective = false;
	GLboolean minnimap_On = true;
}

class SNOW {
private:
	glm::vec3 location;
	glm::vec3 oldlocation;
	GLfloat speed;
public:
	SNOW* next, * perv;

	GLvoid insert_list(SNOW* head) {
		SNOW* newnode = new SNOW;
		newnode->next = head->next;
		newnode->perv = nullptr;
		if (newnode->next != nullptr) {
			newnode->next->perv = newnode;
		}
		head->next = newnode;
	}

	GLvoid delete_list(SNOW* head, SNOW* snow) {
		if (head->next == snow) { // head의 next가 자신임과 동시에 perv가 nullptr인 경우임
			if (snow->next != nullptr) {
				snow->next->perv = nullptr;
			}
			head->next = snow->next;

			return;
		}

		snow->perv->next = snow->next;
		if (snow->next != nullptr) {
			snow->next->perv = snow->perv;
		}
		return;
	}

	glm::vec3 get_location() { return location; }

	GLboolean update(SNOW* head,glm::vec3 pos) {
		location.x = oldlocation.x + pos.x;
		location.z = oldlocation.z + pos.z;

		location.y -= speed;
		if (location.y < 0.0f) {
			delete_list(head, this);
			insert_list(head);
			return true;
		}
		return false;
	}

	GLvoid draw(unsigned int modelLocation, objRead& Circle) {
		glm::mat4 lRT(1.0f);
		lRT = glm::translate(lRT, this->location);
		lRT = glm::scale(lRT, glm::vec3(0.1f, 0.1f, 0.1f));
		lRT = glm::translate(lRT, glm::vec3(0.0f, -0.5f, 0.0f));
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(lRT));
		//glDrawArrays(GL_TRIANGLES, 0, Hexahedron.vtindexct);
		glDrawArrays(GL_TRIANGLES, 0, Circle.outvertex.size());
	}

	SNOW() {
		location = {
			(GLfloat(rand() % 30) - 15) / 1,
			(GLfloat(rand() % 2) + 50.0f),
			(GLfloat(rand() % 30) - 15) / 1
		};
		oldlocation = location;

		speed = (GLfloat(rand() % 3) + 1.0f) / 1;
		next = nullptr;
		perv = nullptr;
	}
	~SNOW() {
		this->next = nullptr;
		this->perv = nullptr;
	}

};

