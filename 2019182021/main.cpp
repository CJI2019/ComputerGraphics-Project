#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include "axes.h"
#include "read_Obj.h"
#include "cuboid.h"
#include "floor.h"
#include "mountain.h"
#include "move_obj.h"
#include "my_maze.h"
#include "state.h"


GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid TimeEvent(int value);
GLvoid KeyEvent(unsigned char key, int x, int y);
GLvoid KeyUpEvent(unsigned char key, int x, int y);

GLvoid spKeyEvent(int key, int x, int y);
GLvoid spKeyUpEvent(int key, int x, int y);

GLvoid passiveMouseMotion(int x, int y);

void initBuffer();

GLvoid convert_OpenglXY_WindowXY(int& x, int& y, const float& ox, const float& oy);
GLvoid convert_WindowXY_OpenglXY(const int& x, const int& y, float& ox, float& oy);

const GLint window_w = 1000, window_h = 700;
GLfloat rColor = 0.0f, gColor = 0.0f, bColor = 0.0f;
GLfloat tv_rColor = 1.0f, tv_gColor = 1.0f, tv_bColor = 1.0f;



unsigned int modelLocation;
unsigned int viewLocation;
unsigned int projLocation;

glm::mat4 camera;
glm::vec3 camera_eye = glm::vec3(700.0f, 800.0f, 700.0f);
glm::vec3 camera_look = glm::vec3(0.0f, 0.0f, 0.0f);

GLfloat cameraAngle = 0.0f;

glm::mat4 topViewCamera;
glm::vec3 tVCamra_eye = glm::vec3(0.0f, 1000.0f, 0.0f);

glm::mat4 projection;
glm::mat4 mini_projection;
glm::mat4 view;

GLuint vao;
GLuint vbo_axes[2];
axes_coordination axes;

map_floor mapFloor;
GLuint vao_floor;
GLuint vbo_floor[2];

std::vector<std::vector<mountain>> mountain_list;

maze mountainMaze;

move_obj* mainObject;

//camera 1pov variable
GLboolean set_cusor = true, firstMouse = false;
GLfloat lastX, lastY, yaw = -90.0f, pitch = 0.0f;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(300, 50);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow("assignment");

	glewExperimental = GL_TRUE;
	glewInit();

	mountain::rNum = 25;
	mountain::cNum = 25;
	mapFloor.set_floor(mountain::rNum, mountain::cNum);
	//std::cout << "hello";
	mountainMaze.initialize((mountain::rNum + 1) / 2, (mountain::cNum + 1) / 2);
	while(!maze::completeGenerate)
		mountainMaze.generator();

	mountain::length = mapFloor.get_length();
	mountain::width = mapFloor.get_width();

	mountain_list = std::vector<std::vector<mountain>>(mountain::cNum);
	for (int i = 0; i < mountain::cNum; ++i)
	{		
		for (int j = 0; j < mountain::rNum; ++j)
		{
			mountain_list[i].push_back(mountain(j, i));
		}
	}

	mainObject = new move_obj();

	set_maze(mountainMaze, mountain_list);
	mainObject->reveal();

	//세이더 읽어와서 세이더 프로그램 만들기
	shaderID = make_shaderProgram();	//세이더 프로그램 만들기
	initBuffer();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutTimerFunc(100, TimeEvent, 0);
	glutKeyboardFunc(KeyEvent);
	glutSpecialFunc(spKeyEvent);
	glutKeyboardUpFunc(KeyUpEvent);
	glutSpecialUpFunc(spKeyUpEvent);
	glutPassiveMotionFunc(passiveMouseMotion);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	viewLocation = glGetUniformLocation(shaderID, "viewTransform");
	projLocation = glGetUniformLocation(shaderID, "projectionTransform");


	camera = glm::lookAt(camera_eye, camera_look, glm::vec3(0.0f, 1.0f, 0.0f));
	topViewCamera = glm::lookAt(tVCamra_eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	
	projection = glm::mat4(1.0f);
	//근평면은 포함이고 원평면은 포함X
	projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 3000.0f);
	mini_projection = glm::mat4(1.0f);
	mini_projection = glm::ortho(-1000.0f, 1000.0f, -1000.0f, 1000.0f, 50.0f, 3000.0f);

	glutMainLoop();
}

GLvoid drawScene()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//랜더링 파이프라인에 세이더 불러오기
	
	glUseProgram(shaderID);
	glViewport(0, 0, window_w, window_h);

	//카메라 변환 적용
	if (STATE::quarter_view)
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera));
	else
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(mainObject->get_camera()));

	//투영 변환 적용
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projection));
	
	//좌표축 그리기
	modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(axes.transformation));
	glBindVertexArray(vao);
	glDrawArrays(GL_LINES, 0, 6);

	//바닥 그리기
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, mapFloor.get_ptr_transformation());
	glBindVertexArray(vao_floor);
	glDrawArrays(GL_TRIANGLES, 0, mapFloor.get_vertex().size() / 3);
		

	for (int i = 0; i < mountain::cNum; ++i)
	{
		for (int j = 0; j < mountain::rNum; ++j)
			mountain_list[i][j].drawMaze(modelLocation);
	}
	

	mainObject->draw(modelLocation);


	glViewport(800, 500, 200, 200);
	glDisable(GL_DEPTH_TEST);

	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projection));
	if(STATE::minimap_perspective)
		glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(mini_projection));

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(topViewCamera));

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, mapFloor.get_ptr_transformation());
	glBindVertexArray(vao_floor);
	glDrawArrays(GL_TRIANGLES, 0, mapFloor.get_vertex().size() / 3);


	mainObject->draw(modelLocation);



	for (int i = 0; i < mountain::cNum; ++i)
		for (int j = 0; j < mountain::rNum; ++j)
			mountain_list[i][j].drawMaze(modelLocation);

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}


GLvoid TimeEvent(int value)
{
	//camera = glm::mat4(1.0f);
	//camera = glm::lookAt(camera_eye, camera_look, glm::vec3(0.0f, 1.0f, 0.0f));
	//camera = glm::rotate(camera, glm::radians(cameraAngle), glm::vec3(0.0f, 1.0f, 0.0f));

	if (!mountain::initAni)
	{
		for (int i = 0; i < mountain::cNum; ++i)
		{
			for (int j = 0; j < mountain::rNum; ++j)
				mountain_list[i][j].init_animation();
		}
	}

	mainObject->move(mountain_list);

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

	yaw -= xoffset;
	pitch += yoffset;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
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

	glGenVertexArrays(1, &vao_floor);
	glGenBuffers(2, vbo_floor);

	glBindVertexArray(vao_floor);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_floor[1]);
	glBufferData(GL_ARRAY_BUFFER, mapFloor.get_color().size() * sizeof(GLfloat), mapFloor.get_color().data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_floor[0]);
	glBufferData(GL_ARRAY_BUFFER, mapFloor.get_vertex().size() * sizeof(GLfloat), mapFloor.get_vertex().data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

}
