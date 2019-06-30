///----------------------------------------------------------------------------
///
///  Copyright 2015 Lockheed Martin Corporation 
///  Lockheed Martin Proprietary Information
///  
///  Author: Prepar3D Dev.
///  Description: Example shader file.
///       
///----------------------------------------------------------------------------

cbuffer cbViewData : register( b1 )
{
	float4   cbPositionWS;
   row_major float4x4 viewProjMatrix;
};

struct VsInput
{
	float4  Position     : POSITION;
	float4  Color        : COLOR0;
};

struct PsInput
{
	float4  Position     : SV_POSITION;
	float4  Color        : COLOR0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PsInput VS(VsInput Input)
{
	PsInput output;
   output.Position = mul(Input.Position + cbPositionWS, viewProjMatrix);
	output.Color = Input.Color;
   return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PsInput Input ) : SV_Target
{
    return Input.Color; 
}
