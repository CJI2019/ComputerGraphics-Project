#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include "read_Obj.h"
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
unsigned int textureLocation;

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

objRead hexahedron;

glm::mat4 face[4];
glm::mat4 modelceiling(1.0f);
void wall_face_init();

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(300, 50);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow("assignment");

	glewExperimental = GL_TRUE;
	glewInit();
	glutSetCursor(GLUT_CURSOR_NONE); // 마우스 커서를 안보이게 한다.

	mountain::rNum = 25;
	mountain::cNum = 25;
	mapFloor = new map_floor(mountain::rNum, mountain::cNum);

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

	jewel = new Jewel*[mountain::rNum];
	for (int i = 0; i < mountain::rNum; ++i) {
		jewel[i] = new Jewel[mountain::cNum];
	}
	for (int i = 0; i < mountain::rNum; ++i) {
		for (int j = 0; j < mountain::cNum; ++j) {
			jewel[i][j].set_pos(mountain_list[i][j].pos, mountain_list[i][j].maze_state);
		}
	}
	//벽 경계면 초기화
	wall_face_init();
	modelceiling = glm::translate(modelceiling, glm::vec3(0.0f, 75.0f, 0.0f));
	modelceiling = glm::scale(modelceiling, glm::vec3(500.0f, 1.0f, 500.0f));
	modelceiling = glm::scale(modelceiling, glm::vec3(0.05f, 1.0f, 0.05f));
	modelceiling = glm::translate(modelceiling, glm::vec3(0.0f, -25.0f, 0.0f)); // 천장 모델변환

	//세이더 읽어와서 세이더 프로그램 만들기
	shaderID = make_shaderProgram();	//세이더 프로그램 만들기
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
	lightPosLocation = glGetUniformLocation(shaderID, "lightPos"); //조명 위치
	lightColorLocation = glGetUniformLocation(shaderID, "lightColor"); //조명 색 설정
	objColorLocation = glGetUniformLocation(shaderID, "objectColor");  //주황색 반사
	viewPosLocation = glGetUniformLocation(shaderID, "cameraEye");     //--- viewPos 값 전달: 카메라 위치 
	ambientLocation = glGetUniformLocation(shaderID, "ambientLight");
	//texture1Location = glGetUniformLocation(shaderID, "outTexture1");



	camera = glm::lookAt(camera_eye, camera_look, glm::vec3(0.0f, 1.0f, 0.0f));
	
	projection = glm::mat4(1.0f);
	//근평면은 포함이고 원평면은 포함X
	projection = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 5000.0f);
	mini_projection = glm::mat4(1.0f);
	mini_projection = glm::perspective(glm::radians(60.0f), 1.0f, 800.0f, 3000.0f);

	glutMainLoop();
}

GLvoid drawScene()
{
	glUniform3f(lightPosLocation, mainObject->get_pos().x, mainObject->get_pos().y, mainObject->get_pos().z);
	glUniform3f(lightColorLocation, light_color.x, light_color.y, light_color.z);
	glUniform1f(ambientLocation, 0.7f);

	glEnable(GL_DEPTH_TEST);
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//랜더링 파이프라인에 세이더 불러오기
	
	glUseProgram(shaderID);
	glViewport(0, 0, window_w, window_h);

	//카메라 변환 적용
	if (STATE::quarter_view) {
		camera_eye = glm::rotate(glm::mat4(1.0f), glm::radians(0.5f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(camera_eye, 1.0f);
		camera = glm::lookAt(camera_eye, camera_look, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera));
	}
	else
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(mainObject->get_camera()));

	//투영 변환 적용
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projection));

	//바닥 그리기
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glUniform3f(objColorLocation, 0.3f, 0.3f, 0.3f);
	mapFloor->draw(modelLocation);

	//경계 벽
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBindVertexArray(hexahedron.vao);
	if (!STATE::quarter_view) {
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelceiling));
		glDrawArrays(GL_TRIANGLES, 0, hexahedron.outvertex.size());
		for (int i = 0; i < 4; ++i) {
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(face[i]));
			glDrawArrays(GL_TRIANGLES, 0, hexahedron.outvertex.size());
		}
	}

	glUniform3f(objColorLocation, 1.0f, 1.0f, 1.0f);
	test_wander_pac->draw(modelLocation);
	test_chase_pac->draw(modelLocation);

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
	
	//미니맵에서만 플레이어 객체 보임.

	if (STATE::quarter_view) {
		glutSwapBuffers();
		return;
	}

	glViewport(window_w/8, window_h/8, 300, 300);
	glUniform1f(ambientLocation, 5.0f);

	glm::vec3 Player_location = mainObject->get_pos();
	glm::vec3 minimap_cameraUp =
		glm::rotate(glm::mat4(1.0f), glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
	tVCamra_eye = glm::vec3(Player_location.x, 1000.0f, Player_location.z);
	topViewCamera = glm::lookAt( tVCamra_eye,
		Player_location + glm::vec3(0.0f, -1.0f, 0.0f),	minimap_cameraUp);

	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(mini_projection));

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(topViewCamera));

	if (STATE::minnimap_On) {
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

	if (STATE::quarter_view) {
		glutPostRedisplay();
		glutTimerFunc(10, TimeEvent, 0);
		return;
	}

	mainObject->move(mountain_list, jewel);

	if (test_chase_pac->stun())
	{
		test_chase_pac->set_path(mountain_list, *mainObject);
		test_chase_pac->move();
	}
	
	if (test_wander_pac->stun())
	{
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
		test_wander_pac->move();
	}

	if (test_chase_pac->colide(mainObject->get_bb()) || test_wander_pac->colide(mainObject->get_bb()))
	{
		//std::cout << "end";
		//char a;
		//std::cin >> a;
		mountainMaze.ResetMaze();
		mountain_list.clear();
		mapFloor->reset();

		mapFloor->set_floor(mountain::rNum, mountain::cNum);
		mountainMaze.initialize((mountain::rNum + 1) / 2, (mountain::cNum + 1) / 2);
		while (!maze::completeGenerate)
			mountainMaze.generator();

		mountain_list = std::vector<std::vector<mountain>>(mountain::cNum);
		for (int i = 0; i < mountain::cNum; ++i)
		{
			for (int j = 0; j < mountain::rNum; ++j)
			{
				mountain_list[i].push_back(mountain(j, i));
			}
		}

		set_maze(mountainMaze, mountain_list);
		
		mainObject->reset();
		test_wander_pac->reset();
		test_chase_pac->reset();


		STATE::quarter_view = true;
	}


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
	else if (key == 's') {
		STATE::quarter_view = false;
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

void wall_face_init()
{
	face[0] = glm::mat4(1.0f);
	face[0] = glm::translate(face[0], glm::vec3(0.0f, 0.0f, -500.0f - 25.0f));
	face[0] = glm::scale(face[0], glm::vec3(1.0f, 0.1f, 1.0f));
	face[0] = glm::rotate(face[0], glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	face[0] = glm::scale(face[0], glm::vec3(500.0f, 1.0f, 500.0f));
	face[0] = glm::scale(face[0], glm::vec3(0.05f, 1.0f, 0.05f));
	face[0] = glm::translate(face[0], glm::vec3(0.0f, -25.0f, 0.0f));

	face[1] = glm::mat4(1.0f);
	face[1] = glm::translate(face[1], glm::vec3(0.0f, 200.0f, +500.0f + 25.0f));
	face[1] = glm::rotate(face[1], glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	face[1] = glm::scale(face[1], glm::vec3(500.0f, 1.0f, 500.0f));
	face[1] = glm::scale(face[1], glm::vec3(0.05f, 1.0f, 0.05f));
	face[1] = glm::translate(face[1], glm::vec3(0.0f, -25.0f, 0.0f));

	face[2] = glm::mat4(1.0f);
	face[2] = glm::translate(face[2], glm::vec3(500.0f + 25.0f, 200.0f, 0.0f));
	face[2] = glm::rotate(face[2], glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	face[2] = glm::scale(face[2], glm::vec3(500.0f, 1.0f, 500.0f));
	face[2] = glm::scale(face[2], glm::vec3(0.05f, 1.0f, 0.05f));
	face[2] = glm::translate(face[2], glm::vec3(0.0f, -25.0f, 0.0f));

	face[3] = glm::mat4(1.0f);
	face[3] = glm::translate(face[3], glm::vec3(-500.0f - 25.0f, 200.0f, 0.0f));
	face[3] = glm::rotate(face[3], glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	face[3] = glm::scale(face[3], glm::vec3(500.0f, 1.0f, 500.0f));
	face[3] = glm::scale(face[3], glm::vec3(0.05f, 1.0f, 0.05f));
	face[3] = glm::translate(face[3], glm::vec3(0.0f, -25.0f, 0.0f));
}

void initBuffer()
{

	hexahedron.loadObj_normalize_center("cuboid.obj");
	glGenVertexArrays(1, &hexahedron.vao);
	glGenBuffers(3, hexahedron.vbo);

	glBindVertexArray(hexahedron.vao);
	glBindBuffer(GL_ARRAY_BUFFER, hexahedron.vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, hexahedron.outvertex.size() * sizeof(GLfloat), hexahedron.outvertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, hexahedron.vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, hexahedron.outnormal.size() * sizeof(GLfloat), hexahedron.outnormal.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, hexahedron.vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, hexahedron.outuv.size() * sizeof(GLfloat), hexahedron.outuv.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

}
