#version 330 core

in vec4 in_Position;
in vec2 texCoord;
in vec3 normals;

uniform mat4 Matrix;
uniform	mat4 ViewMatrix;
uniform	mat4 ProjectionMatrix;
uniform float time;
uniform vec4 lightPosition;
uniform float cameraValue;

out vec2 v_TexCoord;
out vec3 v_Normals;
out vec4 v_FragPos;
out vec4 v_lightPosition;
out vec4 v_CameraPos;

void main(void)
{
	gl_Position =  ProjectionMatrix*ViewMatrix * Matrix * in_Position;
	v_TexCoord = texCoord;
    v_Normals = mat3(transpose(inverse(ViewMatrix * Matrix))) * normals;
    v_FragPos = ViewMatrix * Matrix * in_Position;
    v_lightPosition = ViewMatrix * lightPosition;
    v_CameraPos = vec4(0.0f,0.0f,-cameraValue,1.0f);

}