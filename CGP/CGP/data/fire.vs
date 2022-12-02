////////////////////////////////////////////////////////////////////////////////
// Filename: fire.vs
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};
cbuffer NoiseBuffer
{
    float frameTime;
    float3 scrollSpeeds;
    float3 scales;
    float padding;
};


//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float2 texCoords1 : TEXCOORD1;
    float2 texCoords2 : TEXCOORD2;
    float2 texCoords3 : TEXCOORD3;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType FireVertexShader(VertexInputType input)
{
    PixelInputType output;

    // �ùٸ� ��� ������ ���� ��ġ ������ ������ 4���� �ǵ��� �մϴ�
    input.position.w = 1.0f;

    // ������ ��ġ�� ����, ��, ���� ��Ŀ� ���� ���� ����մϴ�.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

    // ù��° ������ �ؽ����� ��ǥ�� ù��° ũ�� �� ������ ��ũ�� �ӵ� ���� �̿��Ͽ� ����մϴ�.
    output.texCoords1 = (input.tex * scales.x);
    output.texCoords1.y = output.texCoords1.y + (frameTime * scrollSpeeds.x);

    // �ι�° ������ �ؽ����� ��ǥ�� �ι�° ũ�� �� ������ ��ũ�� �ӵ� ���� �̿��Ͽ� ����մϴ�.
    output.texCoords2 = (input.tex * scales.y);
    output.texCoords2.y = output.texCoords2.y + (frameTime * scrollSpeeds.y);

    // ����° ������ �ؽ����� ��ǥ�� ����° ũ�� �� ������ ��ũ�� �ӵ� ���� �̿��Ͽ� ����մϴ�.
    output.texCoords3 = (input.tex * scales.z);
    output.texCoords3.y = output.texCoords3.y + (frameTime * scrollSpeeds.z);
    
    return output;
}
