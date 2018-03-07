
struct DirectionalLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
};

struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;		//for the normal
	float2 uv			: TEXCOORD;		//for the UV
};

cbuffer externalData : register(b0)
{
	DirectionalLight light;
	DirectionalLight light2;
};

// Texture-related resources
Texture2D MetalText		: register(t0);
Texture2D BambooText	: register(t1);
Texture2D BrickText		: register(t2);
SamplerState Samp		: register(s0);

float4 main(VertexToPixel input) : SV_TARGET
{
		// Just return the input color
		// - This color (like most values passing through the rasterizer) is 
		//   interpolated for each pixel between the corresponding vertices 
		//   of the triangle we're rendering

		// Renormalize any interpolated normals
		input.normal = normalize(input.normal);

	float4 surfaceColor = MetalText.Sample(Samp, input.uv);
	//float4 surfaceColor2 = BrickText.Sample(Samp, input.uv);

	//Calculate the normalized direction to the light
	//Negate the light’s direction, normalize that and store in a float3 variable
	//for light 1
	float3 dir = -normalize(light.Direction);
	float dirNdotL = dot(input.normal, dir);
	dirNdotL = saturate(dirNdotL); //clamps it between 0 and 1

								   //final calculation
								   //return light.DiffuseColor * dirNdotL + light.AmbientColor;// Dir light calc


								   //for light 2
	float3 dir2 = -normalize(light2.Direction);
	float dirNdotL2 = dot(input.normal, dir2);
	dirNdotL2 = saturate(dirNdotL2);
	//return light2.DiffuseColor * dirNdotL2 + light.AmbientColor;

	float4 lightOne = surfaceColor * (light.DiffuseColor * dirNdotL + light.AmbientColor);
	float4 lightTwo = surfaceColor * (light2.DiffuseColor * dirNdotL2 + light2.AmbientColor);

	return lightOne + lightTwo;
}