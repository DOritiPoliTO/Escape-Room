#version 330 core

#define NONE 0
#define OUTLINE 1
#define GLOW 2
#define DROP_SHADOWS 3

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

out vec4 FragmentColor;

uniform sampler2D u_TextureSampler;
uniform vec4 u_Color;
uniform int u_EffectType;
uniform float u_EffectScale;
uniform vec3 u_EffectColor;

const float c_Width = 0.5;
const float c_Edge = 0.18;
const float c_DeltaWidth = 0.2;
const float c_DeltaEdge = 0.3;
const vec2 c_Offset = vec2(-0.008, 0.008);

void main()
{
	float distance = 1.0 - texture(u_TextureSampler, p_UV).a;
	
	float alpha = 1.0 - smoothstep(c_Width, c_Width + c_Edge, distance);

	if (u_EffectType == OUTLINE)
	{
		float w = c_Width + c_DeltaWidth * u_EffectScale;

		float outlineAlpha = 1.0 - smoothstep(w, w + c_Edge, distance);

		float a = alpha + (1.0 - alpha) * outlineAlpha;

		vec3 color = mix(u_EffectColor, u_Color.rgb, alpha / a);

		FragmentColor = vec4(color, u_Color.a * a);
	}
	else if (u_EffectType == GLOW)
	{
		float e = c_Edge + c_DeltaEdge * u_EffectScale;

		float outlineAlpha = 1.0 - smoothstep(c_Width, c_Width + e, distance);

		float a = alpha + (1.0 - alpha) * outlineAlpha;

		vec3 color = mix(u_EffectColor, u_Color.rgb, alpha / a);

		FragmentColor = vec4(color, u_Color.a * a);
	}
	else if (u_EffectType == DROP_SHADOWS)
	{
		float distance2 = 1.0 - texture(u_TextureSampler, p_UV + c_Offset).a;

		float w = c_Width + c_DeltaWidth * u_EffectScale * 0.2;

		float outlineAlpha = 1.0 - smoothstep(w, w + c_Edge, distance2);

		float a = alpha + (1.0 - alpha) * outlineAlpha;

		vec3 color = mix(u_EffectColor, u_Color.rgb, alpha / a);

		FragmentColor = vec4(color, u_Color.a * a);
	}
	else
	{
		FragmentColor = vec4(u_Color.rgb, u_Color.a * alpha);
	}
} 
