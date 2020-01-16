#version 330

in vec2 in_position;

out vec2 pass_textureCoords;

//position and scale of the quad. Position stored in transform.xy, and scale in transform.zw
uniform vec4 transform;

void main(void){
	
	//calc texture coords based on position
	pass_textureCoords = in_position;
	//apply position and scale to quad
	vec2 screenPosition = in_position * transform.zw + transform.xy;
	
	//convert to OpenGL coordinate system (with (0,0) in center of screen)
	
	gl_Position = vec4(screenPosition.x,screenPosition.y, 0.0, 1.0);

}