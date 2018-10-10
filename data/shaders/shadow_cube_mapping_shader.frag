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

in vec3 p_VertexPosition_WS;

layout(location = 0) out float FragmentDepth;

uniform vec3 u_LightPosition_WS;

void main()
{
	vec3 lightToVertex = p_VertexPosition_WS - u_LightPosition_WS;

	FragmentDepth = length(lightToVertex);
}
