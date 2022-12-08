#version 330
//out_Color: 버텍스 세이더에서 입력받는 색상 값
//FragColor: 출력할 색상의 값으로 프레임 버퍼로 전달 됨. 

//버텍스 세이더에게서 전달 받음
in vec3 out_Normal;
in vec3 FragPos;
in vec2 out_Texture;

//색상 출력
out vec4 FragColor;

uniform vec3 objectColor;                                                   //--- 응용 프로그램에서 설정한 객체의 색상 
uniform vec3 lightColor;													//--- 응용 프로그램에서 설정한 조명 색상
uniform vec3 lightPos;
uniform vec3 cameraEye;
uniform float ambientLight;
uniform sampler2D outTexture;

void main(void) 
{
	vec3 Normal = out_Normal;

	//앰비언트 (주변조명(간접조명))
	//float ambientLight = 0.5;                                             //--- 주변 조명 계수: 0.0 ≤ ambientLight ≤ 1.0 
	vec3 ambient = ambientLight * lightColor;							    //--- 주변 조명값

	vec3 distence = (lightPos - FragPos); // 거리
	float length = sqrt(distence.x*distence.x + distence.y*distence.y + distence.z*distence.z);

	vec3 length_color = lightColor / (length / 30);


	//디퓨즈 (산란 반사 조명)
	vec3 normalVector = normalize(Normal);							// 노말값 정규화

	vec3 lightDir = normalize(lightPos - FragPos);					// 표면과 조명 위치로 조명 방향 결정	
	
	float diffuseLight = max(dot(normalVector, lightDir), 0.0);		// N과 L의 내적 값으로 강도 조절 (음의 값을 가질 수 없게 한다.) 
	//vec3 diffuse = diffuseLight * lightColor;						// 산란반사조명값 = 산란반사값 * 조명색상값
	vec3 diffuse = diffuseLight * (length_color);						// 산란반사조명값 = 산란반사값 * 조명색상값
	
	//스팩큘러 (거울 반사 조명)
	int shininess = 64;										// 광택(매끄러움) 계수
	vec3 viewDir = normalize(cameraEye - FragPos);					// 관찰자 방향 설정
	vec3 reflectDir = reflect(-lightDir, normalVector);				// 반사 방향: reflect 함수 - 입사 벡터의 반사 방향 계산 
	float specularLight = max(dot(viewDir, reflectDir), 0.0);		// 관찰 방향과 반사 방향의 내적 값으로 강도 조절
	specularLight = pow(specularLight, shininess);					// shininess 승으로 하이라이트 표현
	//vec3 specular = specularLight * lightColor;						// 거울 반사 조명값
	vec3 specular = specularLight * (length_color);

	vec3 result = (ambient + diffuse + specular) * objectColor;		// 객체의 색과 주변조명값을 곱하여 최종 객체 색상 설정

	FragColor = vec4 (result , 1.0);

	if(out_Texture.x >= 0){
		FragColor = texture(outTexture, out_Texture)  * FragColor;
	}
}