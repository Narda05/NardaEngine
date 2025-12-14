// Force Field Shader using cel Shadering effect 
cbuffer TransformBuffer : register(b0)
{
    matrix wvp;
    matrix world;
    matrix lwvp;
    float3 viewPosition;
}

cbuffer LightBuffer : register(b1)
{
    float4 lightAmbient;
    float4 lightDiffuse;
    float4 lightSpecular;
    float4 lightDirection;
}

cbuffer MaterialBuffer : register(b2)
{
    float4 materialEmissive;
    float4 materialAmbient;
    float4 materialDiffuse;
    float4 materialSpecular;
    float4 materialShininess;
}

cbuffer ForceFieldSettingsBuffer : register(b4)
{
    int mode;
    float param0; // time for animating border
    float param1; // noise scale (if set from CPU)
    float param2; // noise amplitude (if set from CPU)
    float param3; // rim widthfactor (if set fromCPU)
}

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 worldNormal : NORMAL;
    float3 worldTangent : TANGENT;
    float2 texCoord : TEXCOORD;
    float3 dirToLight : TEXCOORD1;
    float3 dirToView : TEXCOORD2;
    float4 lightNDCPosition : TEXCOORD3;
    float3 worldPos : TEXCOORD4;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = mul(float4(input.position, 1.0f), wvp);
    output.worldNormal = mul(input.normal, (float3x3) world);
    output.worldTangent = mul(input.tangent, (float3x3) world);
    output.texCoord = input.texCoord;
    output.dirToLight = -lightDirection;
    
    float4 worldPosition = mul(float4(input.position, 1.0f), world);
    output.worldPos = worldPosition.xyz;
    output.dirToView = normalize(viewPosition - worldPosition.xyz);
    
    return output;
}

// Hash noise based on world position
float HashNoise(float3 p)
{
    p = p * float3(12.9898f, 78.233f, 37.719f);
    return frac(sin(dot(p, float3(1.0f, 1.0f, 1.0f))) * 43758.5453f);
}

float4 PS(VS_OUTPUT input) : SV_Target
{
   // normalize
    float3 N = normalize(input.worldNormal);
    float3 L = normalize(input.dirToLight);
    float3 V = normalize(input.dirToView);

    // Fresnel rim base
    float fresnel = 1.0f - saturate(dot(V, N));
    fresnel = pow(fresnel, 1.5f);

    // lighting (ambient + diffuse)
    float4 ambientLight = lightAmbient;
    float d = saturate(dot(L, N));
    float4 diffuseLight = d * lightDiffuse;
    float4 lighting = ambientLight + diffuseLight;

    // specular
    float3 R = reflect(-L, N);
    float specBase = saturate(dot(R, V));
    float shin = materialShininess.x;
    float spec = pow(specBase, max(1.0f, shin));
    float4 specular = spec * lightSpecular * materialSpecular;

    // colors
    float4 rimColor = materialEmissive; // rim
    float4 centerColor = materialAmbient; // center

    // params
    float time = param0;
    float baseNoiseScale = (param1 > 0.0f) ? param1 : materialDiffuse.r;
    float noiseAmp = (param2 > 0.0f) ? param2 : materialDiffuse.g;
    float rimWidthFactor = saturate((param3 >= 0.0f) ? param3 : materialDiffuse.b);


    // animated world-space noise
    float noiseScaleSpeed = 0.8f; // larger = faster oscillation
    float noiseScaleAmp = 0.30f; // fractional amplitude (0.0..1.0 typical)
    float animatedNoiseScale = baseNoiseScale * (1.0f + sin(time * noiseScaleSpeed) * noiseScaleAmp);

    
    // animated offset (movement)
    float moveAmp = 0.6f;
    float3 offset = float3(sin(time * 0.9f) * moveAmp, cos(time * 1.1f) * moveAmp, sin(time * 1.37f) * moveAmp);
    
    // sample point & hash
    float3 wp = input.worldPos * animatedNoiseScale + offset;
    float nVal = HashNoise(wp + float3(time * 0.35f, time * 0.57f, time * 0.79f));
    float noise = nVal - 0.5f;
    
    // rim intensity
    float rim = saturate(fresnel + noise * noiseAmp);

    // thresholds
    float baseInner = 0.30f;
    float baseOuter = 0.70f;
    float inner = lerp(baseInner, baseInner * 0.6f, rimWidthFactor);
    float outer = lerp(baseOuter, baseOuter * 1.1f, rimWidthFactor);
    inner += 0.02f * sin(time * 2.2f);
    outer += 0.02f * sin(time * 2.2f);

    // masks
    float rimMask = smoothstep(inner, outer, rim); // rim band
    float bandWidth = 0.12f;
    float bandMask = smoothstep(inner - bandWidth, inner + bandWidth * 0.25f, rim) * (1.0f - rimMask);

    // center mask: 1 inside center region, fades near rim
    float centerMask = 1.0f - smoothstep(inner - bandWidth*1.0f, inner - bandWidth*0.2f, rim);
    centerMask = saturate(centerMask * (1.0f - rimMask));

    // inner noise influence
    float innerNoiseStrength = 0.75f;
    float innerNoise = saturate(noise * innerNoiseStrength) * bandMask;

    // center alpha purple
    float centerAlpha = 0.5f;

    // outer halo
    float haloInner = outer + 0.02f;
    float haloOuter = outer + 0.30f;
    float haloMask = smoothstep(haloInner, haloOuter, rim) * (1.0f - rimMask);

    // lit colors
    float4 litRim = rimColor * (lighting * 0.9f + materialEmissive * 2.5f);
    float4 litCenter = centerColor * (lighting * 0.6f + centerColor * 0.2f);

    float4 finalColor = float4(0,0,0,0);

     // priority: rim -> center -> halo
    if (rimMask > 0.001f)
    {
        finalColor = lerp(float4(0, 0, 0, 0), litRim, rimMask) + specular;
        finalColor.a = saturate(0.95f * rimMask);
        finalColor.rgb += rimColor.rgb * 0.6f * haloMask;
    }
    else if (centerMask > 0.001f)
    {
        // center uses centerColor but alpha forced to 0.5
        float4 centerGlow = rimColor * (innerNoise * 0.45f); // subtle rim-colored detail near edge
        finalColor = lerp(litCenter, centerGlow + litCenter, innerNoise);
        finalColor.a = centerAlpha;
    }
    else if (haloMask > 0.001f)
    {
        finalColor = float4(rimColor.rgb * 0.6f * haloMask, saturate(0.35f * haloMask));
    }
    else
    {
        finalColor = float4(0, 0, 0, 0);
    }

    finalColor.a = saturate(finalColor.a);
    return finalColor;
}