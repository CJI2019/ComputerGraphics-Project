#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include "read_Obj.h"
#include "axes.h"
#include "cuboid.h"
#include "floor.h"
#include "mountain.h"
#include "move_obj.h"
#include "my_maze.h"
#include "state.h"
#include "Jewel.h"
#include "pac_man.h"
#include "chase_pac_man.h"
#include "wander_pac_man.h"
#include "find_path.h"

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid TimeEvent(int value);
GLvoid KeyEvent(unsigned char key, int x, int y);
GLvoid KeyUpEvent(unsigned char key, int x, int y);

GLvoid spKeyEvent(int key, int x, int y);
GLvoid spKeyUpEvent(int key, int x, int y);

GLvoid passiveMouseMotion(int x, int y);

void initBuffer();

const GLint window_w = 1000, window_h = 700;
GLfloat rColor = 0.0f, gColor = 0.0f, bColor = 0.0f;
GLfloat tv_rColor = 1.0f, tv_gColor = 1.0f, tv_bColor = 1.0f;

unsigned int modelLocation;
unsigned int viewLocation;
unsigned int projLocation;
unsigned int lightPosLocation;
unsigned int lightColorLocation;
unsigned int objColorLocation;
unsigned int viewPosLocation;
unsigned int ambientLocation;
unsigned int texture1Location;
unsigned int texture2Location;
unsigned int texture3Location;


glm::mat4 camera;
glm::vec3 camera_eye = glm::vec3(700.0f, 800.0f, 700.0f);
glm::vec3 camera_look = glm::vec3(0.0f, 0.0f, 0.0f);

GLfloat cameraAngle = 0.0f;

glm::mat4 topViewCamera;
glm::vec3 tVCamra_eye = glm::vec3(0.0f, 500.0f, 0.0f);

glm::mat4 projection;
glm::mat4 mini_projection;
glm::mat4 view;

GLuint vao;
GLuint vbo_axes[2];
axes_coordination axes;

map_floor* mapFloor;
GLuint vao_floor;
GLuint vbo_floor[2];

std::vector<std::vector<mountain>> mountain_list;

maze mountainMaze;

move_obj* mainObject;

//camera 1pov variable
GLboolean set_cusor = true, firstMouse = false;
GLfloat lastX, lastY, yaw = -90.0f, pitch = 0.0f;

Jewel** jewel;

chase_pac_man* test_chase_pac;
wander_pac_man* test_wander_pac;

glm::vec3 light_pos = { 0.0f, 300.0f, 0.0f };
glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);
//glm::mat4 light_trans = glm::mat4(1.0f);

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(300, 50);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow("assignment");

	glewExperimental = GL_TRUE;
	glewInit();
	glutSetCursor(GLUT_CURSOR_NONE); // ���콺 Ŀ���� �Ⱥ��̰� �Ѵ�.


	mountain::rNum = 25;
	mountain::cNum = 25;
	mapFloor = new map_floor(mountain::rNum, mountain::cNum);

	//mapFloor.set_floor(mountain::rNum, mountain::cNum);

	mountainMaze.initialize((mountain::rNum + 1) / 2, (mountain::cNum + 1) / 2);
	while(!maze::completeGenerate)
		mountainMaze.generator();

	mountain::length = mapFloor->get_length();
	mountain::width = mapFloor->get_width();

	mountain_list = std::vector<std::vector<mountain>>(mountain::cNum);
	for (int i = 0; i < mountain::cNum; ++i)
	{		
		for (int j = 0; j < mountain::rNum; ++j)
		{
			mountain_list[i].push_back(mountain(j, i));
		}
	}

	test_chase_pac = new chase_pac_man();
	test_wander_pac = new wander_pac_man();
	mainObject = new move_obj();

	set_maze(mountainMaze, mountain_list);
	mainObject->reveal();

	jewel = new Jewel*[mountain::rNum];
	for (int i = 0; i < mountain::rNum; ++i) {
		jewel[i] = new Jewel[mountain::cNum];
	}
	for (int i = 0; i < mountain::rNum; ++i) {
		for (int j = 0; j < mountain::cNum; ++j) {
			jewel[i][j].set_pos(mountain_list[i][j].pos, mountain_list[i][j].maze_state);
		}
	}
	
	//���̴� �о�ͼ� ���̴� ���α׷� �����
	shaderID = make_shaderProgram();	//���̴� ���α׷� �����
	initBuffer();
	InitTexture();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutTimerFunc(10, TimeEvent, 0);
	glutKeyboardFunc(KeyEvent);
	glutSpecialFunc(spKeyEvent);
	glutKeyboardUpFunc(KeyUpEvent);
	glutSpecialUpFunc(spKeyUpEvent);
	glutPassiveMotionFunc(passiveMouseMotion);

	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_POINTS);

	modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	viewLocation = glGetUniformLocation(shaderID, "viewTransform");
	projLocation = glGetUniformLocation(shaderID, "projectionTransform");
	lightPosLocation = glGetUniformLocation(shaderID, "lightPos"); //���� ��ġ
	lightColorLocation = glGetUniformLocation(shaderID, "lightColor"); //���� �� ����
	objColorLocation = glGetUniformLocation(shaderID, "objectColor");  //��Ȳ�� �ݻ�
	viewPosLocation = glGetUniformLocation(shaderID, "cameraEye");     //--- viewPos �� ����: ī�޶� ��ġ 
	ambientLocation = glGetUniformLocation(shaderID, "ambientLight");
	texture1Location = glGetUniformLocation(shaderID, "outTexture1");
	texture2Location = glGetUniformLocation(shaderID, "outTexture2");
	texture3Location = glGetUniformLocation(shaderID, "outTexture3");


	camera = glm::lookAt(camera_eye, camera_look, glm::vec3(0.0f, 1.0f, 0.0f));
	
	projection = glm::mat4(1.0f);
	//������� �����̰� ������� ����X
	projection = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 500.0f);
	mini_projection = glm::mat4(1.0f);
	mini_projection = glm::perspective(glm::radians(60.0f), 1.0f, 800.0f, 3000.0f);

	glutMainLoop();
}

GLvoid drawScene()
{
	glUniform3f(lightPosLocation, light_pos.x, light_pos.y, light_pos.z);
	glUniform3f(lightColorLocation, light_color.x, light_color.y, light_color.z);
	glUniform1f(ambientLocation, 0.7f);

	glUniform1i(texture1Location, 0);
	glUniform1i(texture2Location, 1);
	glUniform1i(texture3Location, 2);

	glEnable(GL_DEPTH_TEST);
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//������ ���������ο� ���̴� �ҷ�����
	
	glUseProgram(shaderID);
	glViewport(0, 0, window_w, window_h);

	//ī�޶� ��ȯ ����
	if (STATE::quarter_view)
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera));
	else
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(mainObject->get_camera()));

	//���� ��ȯ ����
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projection));

	//�ٴ� �׸���
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glUniform3f(objColorLocation, 0.3f, 0.3f, 0.3f);
	mapFloor->draw(modelLocation);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture[4]);

	//glActiveTexture(GL_TEXTURE2);
	//glBindTexture(GL_TEXTURE_2D, texture[3]);

	glUniform3f(objColorLocation, 1.0f, 1.0f, 0.0f);
	test_wander_pac->draw(modelLocation);
	test_chase_pac->draw(modelLocation);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	for (int i = 0; i < mountain::cNum; ++i) {
		for (int j = 0; j < mountain::rNum; ++j) {
			glUniform3f(objColorLocation, 1.0f, 1.0f, 1.0f);
			mountain_list[i][j].drawMaze(modelLocation);
			glUniform3f(objColorLocation, 0.55f, 0.0f, 1.0f);
			if (jewel[i][j].status_draw) {
				jewel[i][j].draw(modelLocation);
			}
		}
	}
	
	//�̴ϸʿ����� �÷��̾� ��ü ����.
	//mainObject->draw(modelLocation);


	glViewport(window_w/8, window_h/8, 300, 300);

	glm::vec3 Player_location = mainObject->get_pos();
	glm::vec3 minimap_cameraUp =
		glm::rotate(glm::mat4(1.0f), glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
	tVCamra_eye = glm::vec3(Player_location.x, 1000.0f, Player_location.z);
	topViewCamera = glm::lookAt( tVCamra_eye,
		Player_location + glm::vec3(0.0f, -1.0f, 0.0f),	minimap_cameraUp);

	//glDisable(GL_DEPTH_TEST);

	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(mini_projection));

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(topViewCamera));

	if (STATE::minnimap_On) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glUniform3f(objColorLocation, 1.0f, 1.0f, 1.0f);
		mapFloor->draw(modelLocation);

		glUniform3f(objColorLocation, 0.3f, 0.5f, 0.8f);
		mainObject->draw(modelLocation);

		glUniform3f(objColorLocation, 1.0f, 1.0f, 0.0f);
		test_wander_pac->draw(modelLocation);
		test_chase_pac->draw(modelLocation);
		for (int i = 0; i < mountain::cNum; ++i) {
			for (int j = 0; j < mountain::rNum; ++j) {
				glUniform3f(objColorLocation, 0.0f, 0.0f, 0.0f);
				mountain_list[i][j].drawMaze(modelLocation);
				glUniform3f(objColorLocation, 0.55f, 0.0f, 1.0f);
				if (jewel[i][j].status_draw) {
					jewel[i][j].draw(modelLocation);
				}
			}
		}
	}
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}


GLvoid TimeEvent(int value)
{
	mainObject->move(mountain_list, jewel);

	
	test_chase_pac->set_path(mountain_list, *mainObject);
	test_chase_pac->move();

	
	if (!test_wander_pac->set_path(mountain_list, *mainObject))
	{
		if (test_wander_pac->get_miss_time() == 0)
			test_wander_pac->set_path(mountain_list);
		else
		{
			test_wander_pac->miss_time_gone();
		}
	}
	else
	{
		test_wander_pac->set_miss_time(100);
	}
	//test_wander_pac->print_time();
	test_wander_pac->move();



	glutPostRedisplay();
	glutTimerFunc(10, TimeEvent, 0);
}

GLvoid KeyEvent(unsigned char key, int x, int y)
{
	mainObject->setDirection(key, true);

	if (key == 'q')
	{
		delete mainObject;
		glutExit();
	}
	else if (key == 'm') {
		STATE::minnimap_On = (STATE::minnimap_On + 1) % 2;
	}
}

GLvoid KeyUpEvent(unsigned char key, int x, int y)
{
	mainObject->setDirection(key, false);
}

GLvoid spKeyEvent(int key, int x, int y)
{
	
}

GLvoid spKeyUpEvent(int key, int x, int y)
{
	
}

GLvoid GLTranspose(GLclampf* x, GLclampf* y)
{
	if (*x < 1) *x = -(1 - *x);
	else *x -= 1;

	if (*y < 1) *y = (1 - *y);
	else *y = -(*y - 1);
}

GLvoid passiveMouseMotion(int x, int y)
{
	
	GLfloat xPos = (GLfloat)x / ((GLfloat)window_w / 2), yPos = (GLfloat)y / ((GLfloat)window_h / 2);
	GLTranspose(&xPos, &yPos);

	if (set_cusor) {
		glutWarpPointer(window_w / 2, window_h / 2);
		set_cusor = false;
		firstMouse = true;
	}
	else {
		set_cusor = true;
	}

	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
		return;
	}
	
	GLfloat xoffset = xPos - lastX;
	GLfloat yoffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	GLfloat sensitivity = 60.0f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;
	
	if (yaw < 0.0f) yaw += 360.0f;
	if (yaw > 360.0f) yaw -= 360.0f;


	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	glm::vec3 front;
	front =
		glm::rotate(glm::mat4(1.0f), glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
	mainObject->change_camera_look(glm::normalize(front));

}


void initBuffer()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(2, vbo_axes);

	glBindVertexArray(vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo_axes[1]);
	glBufferData(GL_ARRAY_BUFFER, axes.axes_color.size() * sizeof(GLfloat), axes.axes_color.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_axes[0]);
	glBufferData(GL_ARRAY_BUFFER, axes.axes_vertex.size() * sizeof(GLfloat), axes.axes_vertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}
