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

const float c_m0 = 1.0;
const float c_m1 = 0.25;
const float c_m2 = 0.25;

layout(location = 0) in vec3 va_Position_MS;
layout(location = 1) in vec2 va_UV;
layout(location = 2) in vec3 va_Normal_MS;
layout(location = 3) in vec3 va_Tangent_MS;
layout(location = 4) in vec3 va_Bitangent_MS;

out vec2 p_VertexUV;
out vec3 p_CameraToVertex_TS;
out vec3 p_VertexToPointLight_TS;
out vec3 p_DirLightDirection_TS;
out float p_PointLightAttenuation;
out vec4 p_ShadowMapCoord[4];
out vec3 p_VertexPosition_WS;
out vec3 p_VertexNormal_MS;
out vec3 p_VertexTangent_MS;
out vec3 p_VertexBitangent_MS;

uniform mat4 u_ViewMatrix;
uniform mat4 u_ModelViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ModelMatrix;
uniform mat4 u_DepthModelViewProjectionMatrix[4];
uniform int u_IsDirLightOn;
uniform vec3 u_DirLightDirection_WS;
uniform int u_IsPointLightOn;
uniform vec3 u_PointLightPosition_WS;

void main()
{
	// Compute vertex position in world space and camera space
	p_VertexPosition_WS = vec3(u_ModelMatrix * vec4(va_Position_MS, 1.0));
	vec4 vertexPosition_CS = u_ModelViewMatrix * vec4(va_Position_MS, 1.0);

	gl_Position = u_ProjectionMatrix * vertexPosition_CS;

	// Compute vertex normal, tangent and bitangent in camera space
	vec3 vertexNormal_CS = vec3(u_ModelViewMatrix * vec4(va_Normal_MS, 0.0));
	vec3 vertexTangent_CS = vec3(u_ModelViewMatrix * vec4(va_Tangent_MS, 0.0));
	vec3 vertexBitangent_CS = vec3(u_ModelViewMatrix * vec4(va_Bitangent_MS, 0.0));

	// Create the matrix for transformation from camera space to tangent space
	mat3 tBN = transpose(mat3(vertexTangent_CS, vertexBitangent_CS, vertexNormal_CS));

	// Compute the vector which goes from the camera to the vertex in tangent space
	vec3 cameraToVertex_CS = vec3(vertexPosition_CS);
	p_CameraToVertex_TS = tBN * cameraToVertex_CS;

	// Pass the vertex UV to the fragment shader for normal, diffuse and specular mapping
	p_VertexUV = va_UV;


	// Pass vertex normal, tangent and bitangent to the fragment shader for environment mapping
	p_VertexNormal_MS =  va_Normal_MS;
	p_VertexTangent_MS =  va_Tangent_MS;
	p_VertexBitangent_MS = va_Bitangent_MS;



	/******************* Directionl light *********************/
	if (u_IsDirLightOn == 1)
	{
		// Compute the vertex to light direction in tangent space
		p_DirLightDirection_TS = tBN * vec3(u_ViewMatrix * vec4(u_DirLightDirection_WS, 0.0));

		for (int i = 0; i < 4; i++)
		{
			p_ShadowMapCoord[i] = u_DepthModelViewProjectionMatrix[i] * vec4(va_Position_MS, 1.0);
		}
	}



	/******************* Point light *********************/
	if (u_IsPointLightOn == 1)
	{
		// Compute the vector which goes from the vertex to the light in tangent space
		vec4 lightPosition_CS = u_ViewMatrix * vec4(u_PointLightPosition_WS, 1.0);
		vec3 vertexToLight_CS = vec3(lightPosition_CS - vertexPosition_CS);
		p_VertexToPointLight_TS = tBN * vertexToLight_CS;

		// Compute the point light attenuation
		float l = length(vertexToLight_CS);
		float k = c_m2 * (l * l) + c_m1 * l + c_m0;
		p_PointLightAttenuation = min((1.0 / k), 1.0);
	}
}
