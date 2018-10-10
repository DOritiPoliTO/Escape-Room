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

in vec2 p_UV;

out vec4 Color;

uniform sampler2D u_TextureSampler;
uniform vec4 u_ColorScale;

void main()
{
	Color = texture(u_TextureSampler, p_UV) * u_ColorScale;
} 
