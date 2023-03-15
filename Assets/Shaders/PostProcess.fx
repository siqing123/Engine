// Description: Shader for post processing
cbuffer PostProcessBuffer : register(b0)
{
    int mode;
	int textureDimensions;
	int useBlurEffect;
	int useEmbossEffect;
	float embossamount;
}
Texture2D textureMap : register(t0);
Texture2D bloomeMap : register(t1);
SamplerState textureSampler : register(s0);

struct VS_INPUT
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float2 texCoord : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
	output.position = float4(input.position, 1.0f);
	output.texCoord = input.texCoord;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 color = textureMap.Sample(textureSampler, input.texCoord);

    if (mode == 2) // Bloom
    {
        float4 bloom = bloomeMap.Sample(textureSampler, input.texCoord);
        color = color + bloom;
    }
    else
    {
        if (useEmbossEffect)
        {
            color = float4(0.5, 0.5, 0.5, 1.0);
            color -= textureMap.Sample(textureSampler, input.texCoord - 0.003) * embossamount;
            color += textureMap.Sample(textureSampler, input.texCoord + 0.003) * embossamount;
            color.xyz = (color.x + color.y + color.z) / 3.0f;
		
            float4 bloom = bloomeMap.Sample(textureSampler, input.texCoord);
            color = color + bloom;
        }
        if (useBlurEffect)
        {
		
	
            float KernelOffsets[3] = { 0.0f, 1.3846153846f, 3.2307692308f };
            float BlurWeights[3] = { 0.2270270270f, 0.3162162162f, 0.0702702703f };
            float3 textureColor = float3(1.0f, 0.0f, 0.0f);
            float2 uv = input.texCoord;

            textureColor = color.xyz * BlurWeights[0];
            for (int i = 1; i < 3; i++)
            {
                float2 normalizedOffset = float2(0.0f, KernelOffsets[i]) / textureDimensions;
                textureColor += textureMap.Sample(textureSampler, uv + normalizedOffset).xyz * BlurWeights[i];
                textureColor += textureMap.Sample(textureSampler, uv - normalizedOffset).xyz * BlurWeights[i];
            }

            for (int j = 1; j < 3; j++)
            {
                float2 normalizedOffset = float2(KernelOffsets[j], 0.0f) / textureDimensions;
                textureColor += textureMap.Sample(textureSampler, uv + normalizedOffset).xyz * BlurWeights[j];
                textureColor += textureMap.Sample(textureSampler, uv - normalizedOffset).xyz * BlurWeights[j];
            }
	
            textureColor = textureColor / 2;
            color = float4(textureColor, 1.0);
            float4 bloom = bloomeMap.Sample(textureSampler, input.texCoord);
            color = color + bloom;
        }
    }
    return color;
}
