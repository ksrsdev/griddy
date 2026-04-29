Texture2D<float4> tex : register(t0, space2);
SamplerState samp : register(s0, space2);

struct PSInput {
	float4 pos : SV_Position;
	float4 color : COLOR0;
	float2 tex_coord : TEXCOORD0;
};

struct PSOutput {
	float4 color : SV_Target;
};

PSOutput main(PSInput input) {
	PSOutput output;
	float distance = tex.Sample(samp, input.tex_coord).a;
    float delta = fwidth(distance);
	float alpha = smoothstep(0.50 - delta, 0.50 + delta, distance);
	output.color = float4(input.color.rgb, input.color.a * alpha);
	return output;
};
