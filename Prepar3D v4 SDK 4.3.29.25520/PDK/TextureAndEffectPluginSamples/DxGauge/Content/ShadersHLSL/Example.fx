///----------------------------------------------------------------------------
///
///  Copyright 2010 Lockheed Martin Corporation 
///  Lockheed Martin Proprietary Information
///  
///  Author: Prepar3D Dev.
///  Description: Example shader file.
///               **Borrowed and Refactored from Microsoft DX11 Example
///                
///----------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
float4 VS( float4 Pos : POSITION ) : SV_POSITION
{
    return Pos;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( float4 Pos : SV_POSITION ) : SV_Target
{
    return float4( 1.0f, 1.0f, 0.0f, 1.0f );    // Yellow, with Alpha = 1
}
