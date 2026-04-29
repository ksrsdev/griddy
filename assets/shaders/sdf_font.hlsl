Texture2D<float4> tex : register(t0, space2);
SamplerState samp : register(s0, space2);

static float4 input_color;
static float2 input_tex_coord;
static float4 _entryPointOutput_color;

struct SPIRV_Cross_Input
{
    float4 input_color : TEXCOORD0;
    float2 input_tex_coord : TEXCOORD1;
};

struct SPIRV_Cross_Output
{
    float4 _entryPointOutput_color : SV_Target0;
};

void frag_main()
{
    float4 _98 = tex.Sample(samp, input_tex_coord);
    float _99 = _98.w;
    float _101 = fwidth(_99);
    _entryPointOutput_color = float4(input_color.xyz, input_color.w * smoothstep(0.5f - _101, 0.5f + _101, _99));
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    input_color = stage_input.input_color;
    input_tex_coord = stage_input.input_tex_coord;
    frag_main();
    SPIRV_Cross_Output stage_output;
    stage_output._entryPointOutput_color = _entryPointOutput_color;
    return stage_output;
}
