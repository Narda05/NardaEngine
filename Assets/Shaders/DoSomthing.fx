// simple shader that takes a position and can add color

// defines the expected vertex data per vertex 

struct VS_INPUT
{
    float3 position : POSITION;
};

//output that gets interpolated in the rasterizer
struct VS_OUTPUT
{
    float4 position : SV_POSITION; // position of the vertex in screen space
};

// gets applied to every vertex
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = float4(input.position, 1.0f);
    return output;
}

// lerped vertex data that is converted to a pixel on the screen
float4 PS(VS_OUTPUT input) : SV_TARGET
{
    // output color
    return float4(1, 1, 0, 1); // red color
}