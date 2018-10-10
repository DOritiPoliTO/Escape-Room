#version 330 core

// Naming convention
// x_TS = x in Tangent Space
// x_CS = x in Camera Space
// x_MS = x in Model Space
// x_WS = x in World Space
// va_x = vertex attribute
// u_x = uniform
// p_x = variable passed from vertex shader to fragment shader
// c_x = const

layout(location = 0) in vec3 va_Position;
layout(location = 1) in vec2 va_UV;

out vec2 p_UV;

uniform mat4 u_ModelViewProjectionMatrix;

void main()
{
	gl_Position = u_ModelViewProjectionMatrix * vec4(va_Position, 1.0);

	p_UV = va_UV;
} 
