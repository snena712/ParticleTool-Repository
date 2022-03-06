//--------------------------------------------------------------------------------------
// Particle Rendering
//--------------------------------------------------------------------------------------
#include	"ComputeShaderHeader.hlsl"


// 初め
struct VSParticleOut
{
	float3 position : POSITION;
	float4 color : COLOR;
	float3 scale : SCALE;
};

struct GSParticleOut
{
    float4 position : SV_Position;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
    float4 WPos		: TEXCOORD1;
};


//--------------------------------------------------------------------------------------
// Visualization Helper
//--------------------------------------------------------------------------------------

static const float4 Rainbow[5] = {
    float4(1, 0, 0, 1), // red
    float4(1, 1, 0, 1), // orange
    float4(0, 1, 0, 1), // green
    float4(0, 1, 1, 1), // teal
    float4(0, 0, 1, 1), // blue
};

float4 VisualizeNumber(float n)
{
    return lerp(Rainbow[floor(n * 4.0f)], Rainbow[ceil(n * 4.0f)], frac(n * 4.0f));
}

float4 VisualizeNumber(float n, float lower, float upper)
{
    return VisualizeNumber(saturate((n - lower) / (upper - lower)));
}


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------

VSParticleOut ParticleVS(uint ID : SV_VertexID)
{
	VSParticleOut Out = (VSParticleOut)0;

	Out.position = ParticlesRO[ID].pos;
	Out.scale = ParticlesRO[ID].scale;

	//Base _base = base[ParticlesRO[ID].parentId];
	
	Out.color = ParticlesRO[ID].color;


	return Out;
}


//--------------------------------------------------------------------------------------
// ジオメトリシェーダー
//--------------------------------------------------------------------------------------

//static const float2 g_positions[4] = { float2(-1, 1), float2(1, 1), float2(-1, -1), float2(1, -1) };
static const float2 g_positions[4] = { float2(-0.5, 0.5), float2(0.5, 0.5), float2(-0.5, -0.5), float2(0.5, -0.5) };
static const float2 g_texcoords[4] = { float2(0, 1), float2(1, 1), float2(0, 0), float2(1, 0) };

[maxvertexcount(4)]
void ParticleGS(point VSParticleOut In[1], inout TriangleStream<GSParticleOut> SpriteStream)
{
	[unroll]
	for (int i = 0; i < 4; i++)
	{
		GSParticleOut Out = (GSParticleOut)0;
		//float4 position = float4(In[0].position, 0, 1) + 1 * float4(g_positions[i], 0, 0);
		float4 position = float4(In[0].position.x, In[0].position.y, 0, 1) + 1 * float4(g_positions[i].x, g_positions[i].y, 0, 0);

		//
		Out.position = mul(position, World);
		Out.WPos = Out.position;
		Out.position = mul(Out.position, View);
		Out.position = mul(Out.position, Projection);


		// Out.color = In[0].color;
		Out.color = In[0].color;
		Out.texcoord = g_texcoords[i];
		SpriteStream.Append(Out);
	}
	SpriteStream.RestartStrip();
}


[maxvertexcount(4)]
void ParticleBillbordGS(point VSParticleOut In[1], inout TriangleStream<GSParticleOut> SpriteStream)
{
	float3 _center = float3(In[0].position.x, In[0].position.y, In[0].position.z);


	float3 campos = 0;
	campos.x = EyePos.x;
	campos.y = EyePos.y;
	campos.z = EyePos.z;

	// 姿勢作成
	float3 up = float3(0.0f, 1.0f, 0.0f);
	float3 look = campos - _center;
	look = normalize(look);
	float3 right = cross(up, look);

	// サイズ作成　　// ていぎなし
	float halfWidth = 0.5f * 1 * In[0].scale.x;
	float halfHeight = 0.5f * 1 * In[0].scale.y;

	float4 v[4];
	v[0] = float4(_center + halfWidth * right - halfHeight * up, 1.0f);
	v[1] = float4(_center + halfWidth * right + halfHeight * up, 1.0f);
	v[2] = float4(_center - halfWidth * right - halfHeight * up, 1.0f);
	v[3] = float4(_center - halfWidth * right + halfHeight * up, 1.0f);


	GSParticleOut Out = (GSParticleOut)0;
	[unroll]
	for (int i = 0; i < 4; ++i)
	{
		Out.position = mul(v[i], View);
		Out.WPos = v[i];
		Out.position = mul(Out.position, Projection);

		//gout.NormalW = look;
		Out.texcoord = g_texcoords[i];
		//gout.PrimID = primID;
		Out.color = In[0].color;

		SpriteStream.Append(Out);
	}

	SpriteStream.RestartStrip();
}

//[maxvertexcount(4)]
//void ParticleBillbordGS(point VSParticleOut In[1], inout TriangleStream<GSParticleOut> SpriteStream)
//{
//	// The point passed in is in the horizontal center of the billboard, and at the bottom vertically. Because of this,
//		// we will take the trees width and divide it by two when finding the x axis for the quads vertices.
//	float halfWidth = In[0].scale.x / 2.0f;
//
//	float3 campos = 0;
//	campos.x = ViewProjection._14;
//	campos.y = ViewProjection._24;
//	campos.z = ViewProjection._34;
//
//	float3 planeNormal = In[0].position - campos;
//	planeNormal.y = 0.0f;
//	planeNormal = normalize(planeNormal);
//
//	float3 upVector = float3(0.0f, 1.0f, 0.0f);
//
//	// Now we need to find the billboards right vector, so we can easily find the billboards vertices from the input point
//	float3 rightVector = normalize(cross(planeNormal, upVector)); // Cross planes normal with the up vector (+y) to get billboards right vector
//
//	rightVector = rightVector * halfWidth; // change the length of the right vector to be half the width of the billboard
//
//	// Get the billboards "height" vector which will be used to find the top two vertices in the billboard quad
//	upVector = float3(0, In[0].scale.y, 0);
//
//	// Create the billboards quad
//	float3 vert[4];
//
//	// We get the points by using the billboards right vector and the billboards height
//	vert[0] = In[0].position - rightVector; // Get bottom left vertex
//	vert[1] = In[0].position + rightVector; // Get bottom right vertex
//	vert[2] = In[0].position - rightVector + upVector; // Get top left vertex
//	vert[3] = In[0].position + rightVector + upVector; // Get top right vertex
//
//	// Get billboards texture coordinates
//	float2 texCoord[4];
//	texCoord[0] = float2(0, 1);
//	texCoord[1] = float2(1, 1);
//	texCoord[2] = float2(0, 0);
//	texCoord[3] = float2(1, 0);
//
//	// Now we "append" or add the vertices to the outgoing stream list
//	GSParticleOut outputVert;
//	for (int i = 0; i < 4; i++)
//	{
//		outputVert.position = mul(float4(vert[i], 1.0f), ViewProjection);
//		outputVert.position = mul(outputVert.position, Projection);
//
//
//		outputVert.WPos = float4(vert[i], 0.0f);
//		outputVert.texcoord = texCoord[i];
//		outputVert.color = float4(1, 1, 1, 1);
//
//		SpriteStream.Append(outputVert);
//	}
//}








//--------------------------------------------------------------------------------------
// ピクセル
//--------------------------------------------------------------------------------------

float4 ParticlePS(GSParticleOut In) : SV_Target
{
    //return In.color;
    return float4(1,1,1,1);
}

float4 ParticleBillbord(GSParticleOut In) : SV_Target
{

	float4 texcol = g_Tex.Sample(g_SamplerLinear, In.texcoord);
	float4 col;
	col = texcol * In.color;
	//return col;
	return col;
}

// todo 仮で体力ないとき描画しないようにした
