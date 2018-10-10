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

const float c_Bias = 0.005;
const float c_zNear = 0.01;
const float c_zFar = 8.01;
const int c_NumberOfSamples = 4;
const float c_VisibilityReduction = 1.0 / c_NumberOfSamples;
const vec2 c_PoissonDisk[16] = vec2[](
	vec2( -0.94201624, -0.39906216 ),
	vec2( 0.94558609, -0.76890725 ),
	vec2( -0.094184101, -0.92938870 ),
	vec2( 0.34495938, 0.29387760 ),
	vec2( -0.91588581, 0.45771432 ),
	vec2( -0.81544232, -0.87912464 ),
	vec2( -0.38277543, 0.27676845 ),
	vec2( 0.97484398, 0.75648379 ),
	vec2( 0.44323325, -0.97511554 ),
	vec2( 0.53742981, -0.47373420 ),
	vec2( -0.26496911, -0.41893023 ),
	vec2( 0.79197514, 0.19090188 ),
	vec2( -0.24188840, 0.99706507 ),
	vec2( -0.81409955, 0.91437590 ),
	vec2( 0.19984126, 0.78641367 ),
	vec2( 0.14383161, -0.14100790 )
);

in vec2 p_VertexUV;
in vec3 p_CameraToVertex_TS;
in vec3 p_VertexToPointLight_TS;
in vec3 p_DirLightDirection_TS;
in float p_PointLightAttenuation;
in vec4 p_ShadowMapCoord[4];
in vec3 p_VertexPosition_WS;
in vec3 p_VertexNormal_MS;
in vec3 p_VertexTangent_MS;
in vec3 p_VertexBitangent_MS;

layout(location = 0) out vec3 Color;

uniform sampler2D u_NormalTextureSampler;
uniform sampler2D u_DiffuseColorTextureSampler;
uniform sampler2D u_SpecularTextureSampler;
uniform samplerCubeShadow u_ShadowMapCubeSampler;
uniform sampler2DShadow u_ShadowMapSampler[4];
uniform int u_IsDirLightOn;
uniform vec3 u_DirLightAmbient;
uniform vec3 u_DirLightDiffuse;
uniform vec3 u_DirLightSpecular;
uniform int u_IsPointLightOn;
uniform vec3 u_PointLightDiffuse;
uniform vec3 u_PointLightSpecular;
uniform vec3 u_PointLightPosition_WS;
uniform samplerCube u_EnvironmentMapCubeSampler;
uniform vec3 u_CameraPosition_WS;
uniform float u_MaterialReflectivity;
uniform mat4 u_ModelMatrix;

float vectorToDepth(vec3 Vec)
{
    vec3 AbsVec = abs(Vec);
    float LocalZcomp = max(AbsVec.x, max(AbsVec.y, AbsVec.z));

    float normZComp = (c_zFar + c_zNear) / (c_zFar - c_zNear) - (2 * c_zFar * c_zNear) 
		/ (c_zFar - c_zNear) / LocalZcomp;
    return (normZComp + 1.0) * 0.5;
}

void main()
{
	vec3 ambientColor = vec3(0.0, 0.0, 0.0);
	vec3 diffuseColor = vec3(0.0, 0.0, 0.0);
	vec3 specularColor = vec3(0.0, 0.0, 0.0);



	// Extract the normal, diffuse color, and specular color and intensity from the textures
	vec3 fragmentNormal_TS = normalize(texture(u_NormalTextureSampler, p_VertexUV).rgb * 2.0 - 1.0);
	vec3 materialDiffuseColor = texture(u_DiffuseColorTextureSampler, p_VertexUV).rgb;
	vec4 specular = texture(u_SpecularTextureSampler, p_VertexUV).rgba;
	vec3 materialSpecularColor = specular.rgb;
	float materialSpecularity = specular.a;



	// Normalize the camera to vertex vector
	vec3 cameraToVertexDirection_TS = normalize(p_CameraToVertex_TS);



	/******************* Directional light *********************/
	if (u_IsDirLightOn == 1)
	{
		/********************* Ambient component **********************/
		ambientColor = u_DirLightAmbient * materialDiffuseColor;

		float visibility = 1.0;
		for (int i = 0; i < 4; i++)
		{
			float b = c_Bias / (i + 1);
			float s = 2400.0 * (i + 1);

			for (int j = 0; j < c_NumberOfSamples; j++)
			{
				visibility -= c_VisibilityReduction * (1.0 - texture(u_ShadowMapSampler[i], 
					vec3(p_ShadowMapCoord[i].xy + c_PoissonDisk[j] / s, (p_ShadowMapCoord[i].z - b) 
					/ p_ShadowMapCoord[i].w)));
			}
		}

		if (visibility < 0.025)
		{
			visibility = 0.025;
		}
		
		/****************** Diffuse reflection *******************/	
		vec3 vertexToLightDirection_TS = normalize(p_DirLightDirection_TS);

		// Compute cosine of angle between normal and light direction
		float nDotL = dot(fragmentNormal_TS, vertexToLightDirection_TS);

		diffuseColor = visibility * u_DirLightDiffuse * materialDiffuseColor * max(0.0, nDotL);

		/***************** Specular reflection *******************/
		// Compute direction of reflected ray and normalize it
		vec3 reflectedLightDirection_TS = normalize(reflect(vertexToLightDirection_TS, fragmentNormal_TS));

		if (nDotL > 0.0)
		{
			float rDotL = dot(reflectedLightDirection_TS, cameraToVertexDirection_TS);
			specularColor = materialSpecularity * visibility * u_DirLightSpecular * materialSpecularColor 
				* pow(max(0.0, rDotL), 100.0 * (1.0 + materialSpecularity));
		}
	}



	/******************* Point light *********************/
	if (u_IsPointLightOn == 1)
	{
		vec3 lightToFragment = p_VertexPosition_WS - u_PointLightPosition_WS;
		float depth = vectorToDepth(lightToFragment);
		float s;
		float bias;
		float l = length(lightToFragment);
		if (l < 0.5)
		{
			s = 0.001;
			bias = c_Bias;
		}
		else
		{
			s = 0.001 + (l - 1.0) * (0.03 - 0.001) / 6.0;
			bias = c_Bias * 0.02;
		}
		float visibility = 1.0;
		for (int i = 0; i < c_NumberOfSamples; i++)
		{
			float d1 = c_PoissonDisk[i].x * s;
			float d2 = c_PoissonDisk[i].y * s;
			vec3 v = lightToFragment;
			v.x += d1;
			v.y += d2;
			v.z += d2;
			visibility -= c_VisibilityReduction * (1.0 - texture(u_ShadowMapCubeSampler, 
				vec4(v, depth - bias)));
		}

		if (visibility < 0.025)
		{
			visibility = 0.025;
		}

		/****************** Diffuse reflection *******************/		
		vec3 fragmentToLightDirection_TS = normalize(p_VertexToPointLight_TS);

		float nDotL = dot(fragmentNormal_TS, fragmentToLightDirection_TS);

		diffuseColor += visibility * p_PointLightAttenuation * u_PointLightDiffuse * materialDiffuseColor 
			* max(0.0, nDotL);

		/***************** Specular reflection *******************/
		vec3 reflectedLightDirection_TS = normalize(reflect(fragmentToLightDirection_TS, fragmentNormal_TS));

		if (nDotL > 0.0)
		{
			float rDotL = dot(reflectedLightDirection_TS, cameraToVertexDirection_TS);
			specularColor += materialSpecularity * visibility * p_PointLightAttenuation * u_PointLightSpecular 
				* materialSpecularColor * pow(max(0.0, rDotL), 100.0 * (1.0 + materialSpecularity));
		}
	}
	


	// Get the environment map reflection color
	mat3 TM = mat3(normalize(p_VertexTangent_MS), normalize(p_VertexBitangent_MS), normalize(p_VertexNormal_MS));
	mat3 RM = mat3(u_ModelMatrix[0].xyz, u_ModelMatrix[1].xyz, u_ModelMatrix[2].xyz);
	vec3 fragmentNormal_WS = normalize(RM * TM * fragmentNormal_TS);
	vec3 r = reflect(normalize(p_VertexPosition_WS - u_CameraPosition_WS), fragmentNormal_WS);
	vec3 environmentMapReflectionColor = texture(u_EnvironmentMapCubeSampler, r).rgb;



	Color = mix(ambientColor + diffuseColor + specularColor, environmentMapReflectionColor, u_MaterialReflectivity);
}
