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

layout(location = 0) in vec3 va_Position_MS;

uniform mat4 u_DepthModelViewProjectionMatrix;

void main()
{
	gl_Position = u_DepthModelViewProjectionMatrix * vec4(va_Position_MS, 1.0);
}
