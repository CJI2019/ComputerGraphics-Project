#version 330 core
layout (location = 0) in vec3 vPos; //--- 응용 프로그램에서 받아온 도형 좌표값
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexture;


uniform mat4 modelTransform;		//--- 모델링 변환 행렬: uniform 변수로 선언
uniform mat4 viewTransform;
uniform mat4 projectionTransform;

out vec3 out_Normal;
out vec3 FragPos;
out vec2 out_Texture;

void main()
{
	gl_Position = projectionTransform * viewTransform * modelTransform * vec4(vPos, 1.0); //--- 좌표값에 modelTransform 변환을 적용한다.

	FragPos = vec3(modelTransform * vec4(vPos, 1.0));

	out_Normal = vec3(modelTransform * vec4(vNormal, 0.0));

	if(vTexture.x >= 0){
		out_Texture = vTexture;
	}
	else {
		out_Texture = vec2(-1.0f,-1.0f);
	}
}