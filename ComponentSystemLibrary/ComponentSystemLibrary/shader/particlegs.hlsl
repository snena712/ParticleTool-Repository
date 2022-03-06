#include	"psvscommon.hlsl"

[maxvertexcount(6)]
void main(
	point GS_OUTPUT input[1] : SV_POSITION, 
	inout TriangleStream< GS_OUTPUT > TriStream)
{
	GS_OUTPUT output = (GS_OUTPUT)0;

	output.Position = input[0].Position;
	output.Position.x -= 1.0;
	output.Position.y -= 1.0;
	output.Color = input[0].Color;
	output.Tex = input[0].Tex;
	TriStream.Append(output);

	output.Position = input[0].Position;
	output.Position.x += 1.0;
	output.Position.y -= 1.0;
	output.Color = input[0].Color;
	output.Tex = input[0].Tex;
	TriStream.Append(output);

	output.Position = input[0].Position;
	output.Position.x -= 1.0;
	output.Position.y += 1.0;
	output.Color = input[0].Color;
	output.Tex = input[0].Tex;
	TriStream.Append(output);

	TriStream.RestartStrip();

	output.Position = input[0].Position;
	output.Position.x += 1.0;
	output.Position.y += 1.0;
	output.Color = input[0].Color;
	output.Tex = input[0].Tex;
	TriStream.Append(output);

	output.Position = input[0].Position;
	output.Position.x -= 1.0;
	output.Position.y += 1.0;
	output.Color = input[0].Color;
	output.Tex = input[0].Tex;
	TriStream.Append(output);

	output.Position = input[0].Position;
	output.Position.x += 1.0;
	output.Position.y -= 1.0;
	output.Color = input[0].Color;
	output.Tex = input[0].Tex;
	TriStream.Append(output);

	TriStream.RestartStrip();
}