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

layout(location = 0) in vec2 va_Position;
layout(location = 1) in vec2 va_UV;

out vec2 p_UV;

uniform vec2 u_Position;
uniform float u_Scale;
uniform float u_AspectRatio;

void main()
{
	gl_Position = vec4(
		va_Position.x * u_Scale * u_AspectRatio + u_Position.x,
		va_Position.y * u_Scale + u_Position.y,
		0.0,
		1.0
	);

	p_UV = va_UV;
} 
