#include "init.h"

#include <stdio.h>
#include <stdlib.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <SDL3_shadercross/SDL_shadercross.h>

#include "sync_input.h"

static bool Init_SDFFontState(GameEngine *eng);

Context InitContext(void)
{
	GameEngine eng = {0};
	GameInput input = {0};
	GameData data = {0};

	//set variables that need non-junk data
	data.isRunning = true;

	//Create global struct
	Context ctx = {
		.eng = eng,
		.input = input,
		.data = data,
	};

	return ctx;
}

bool Init_SDLSubsystems(void)
{
	//Init SDL Video
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("SDL_Init Error: %s", SDL_GetError());
		return false;
	}

	//Init SDL TTF
	if (!TTF_Init()) {
		SDL_Log("SDL_Init Error: %s", SDL_GetError());
		return false;
	}

	//Init SDL_shadercross
	if (!SDL_ShaderCross_Init()) {
		printf("ERROR: failed to init shadercross\n");
		SDL_Log("Failed to initialize: %s", SDL_GetError());
		TTF_Quit();
		return false;
	}
	
	return true;
}

bool Init_GameEngine(GameEngine *eng)
{
	SDL_SetHint(SDL_HINT_VIDEO_WAYLAND_ALLOW_LIBDECOR, "0"); 
	eng->window = SDL_CreateWindow("Griddy", 960, 540, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
	if (eng->window == nullptr) {
		printf("ERROR FATAL: SDL Window not created!\n");
		return false;
	}
	//gpu specifices use gpu (allows Render State switch)
	eng->renderer = SDL_CreateRenderer(eng->window, "gpu");
	if (!eng->renderer) {
		const char *errormsg = SDL_GetError();
		printf("ERROR FATAL: SDL Renderer not created!\n%s\n", errormsg);
		return false;
	}

	//TextEngine
	eng->textEngine = TTF_CreateRendererTextEngine(eng->renderer);
	if (!eng->textEngine) {
		printf("ERROR FATAL: text Engine not created!\n");
		return false;
	}

	//Font
	eng->font = TTF_OpenFont("assets/fonts/Press_Start_2P/PressStart2P-Regular.ttf", 64);
	if (eng->font == nullptr) {
		printf("ERROR: fontTitle not loaded\n");
		return false;
	}
	TTF_SetFontSDF(eng->font, true);
	TTF_SetFontWrapAlignment(eng->font, TTF_HORIZONTAL_ALIGN_CENTER);
	
	//Setup sdfRenderState
	if (!Init_SDFFontState(eng)){
		printf("ERROR FATAL: Could not create fontSDFState\n");
		return false;
	}

	//throttle cpu
	SDL_SetRenderVSync(eng->renderer, 1);
	return true;
}

static bool Init_SDFFontState(GameEngine *eng)
{
	const char *hlsl_source = 
	"	Texture2D<float4> tex : register(t0, space2);\n"
	"	SamplerState samp : register(s0, space2);\n"
	"	struct PSInput {\n"
//	"		float4 color : TEXCOORD0;\n"
	"		float4 pos : SV_Position;\n"
	"		float4 color : COLOR0;\n"
	"		float2 tex_coord : TEXCOORD0;\n"
	"	};\n"
	"\n"
	"	struct PSOutput {\n"
	"		float4 color : SV_Target;\n"
	"	};\n"
	"\n"
	"	PSOutput main(PSInput input) {\n"
	"		PSOutput output;\n"
	"		float distance = tex.Sample(samp, input.tex_coord).a;\n"
	"       float delta = fwidth(distance);\n"
	"		float alpha = smoothstep(0.50 - delta, 0.50 + delta, distance);\n"
	"		output.color = float4(input.color.rgb, input.color.a * alpha);\n"
	"		return output;\n"
	"	}\n";
	
	SDL_PropertiesID props = SDL_GetRendererProperties(eng->renderer);

	SDL_GPUDevice *gpu_device = (SDL_GPUDevice *)SDL_GetPointerProperty(
		props, 
		SDL_PROP_RENDERER_GPU_DEVICE_POINTER, 
		NULL
	);

	if (!gpu_device) {
		SDL_Log("Could not get GPU device from renderer! Is the 'gpu' backend active?");
		return false;
	}

	//Compile hlsl shader into spir-v 
	size_t spirv_size;
	SDL_ShaderCross_HLSL_Info hlsl_info = {
		.source = hlsl_source,
		.entrypoint = "main",
		.shader_stage = SDL_SHADERCROSS_SHADERSTAGE_FRAGMENT
	};

	void* spirv_bytecode = SDL_ShaderCross_CompileSPIRVFromHLSL(&hlsl_info, &spirv_size);

	//Create GPU_Shader object from SPIR-V shader 
	
	// Describe the resources (1 texture, 1 sampler, no uniform buffers)
	SDL_ShaderCross_GraphicsShaderResourceInfo res_info = {
		.num_samplers = 1,
		.num_storage_textures = 0,
		.num_storage_buffers = 0,
		.num_uniform_buffers = 0 
	};

	// Wrap the bytecode
	SDL_ShaderCross_SPIRV_Info spirv_info = {
		.bytecode = (const Uint8 *)spirv_bytecode,
		.bytecode_size = spirv_size,
		.entrypoint = "main",
		.shader_stage = SDL_SHADERCROSS_SHADERSTAGE_FRAGMENT
	};

	// Create the actual GPU shader
	// (Remember: get gpu_device from SDL_GetRendererProperties as shown before)
	SDL_GPUShader *test_shader = SDL_ShaderCross_CompileGraphicsShaderFromSPIRV(
		gpu_device, 
		&spirv_info, 
		&res_info, 
		0
	);

	// Clean up the temporary buffer immediately
	SDL_free(spirv_bytecode);

	SDL_GPURenderStateCreateInfo info = {.fragment_shader =  test_shader};

	eng->sdfRenderState = SDL_CreateGPURenderState(eng->renderer, &info);
	
	return true;
}

void Init_GameData(GameEngine *eng, GameData *data)
{
	//Window Size
	int winW, winH;
	SDL_GetWindowSize(eng->window, &winW, &winH);
	data->window.size.x = winW;
	data->window.size.y = winH;

	data->padding = GetPaddingForWindow(data->window.size.x);

	//Mouse
	float mX, mY;
	SDL_GetMouseState(&mX, &mY);
	data->mouse.pos.x = mX;
	data->mouse.pos.y = mY;
}

void Deinit_GameEngine(GameEngine *eng)
{
	printf("Deinit_GameEngine\n");
	
	SDL_ShaderCross_Quit();

	if (eng->textEngine != nullptr) {
		TTF_DestroyRendererTextEngine(eng->textEngine);
	}

	if (eng->font != nullptr) {
		TTF_CloseFont(eng->font);
	}

	if (eng->sdfRenderState != nullptr) {
		SDL_DestroyGPURenderState(eng->sdfRenderState);
	}

	if (eng->renderer != nullptr) {
		SDL_DestroyRenderer(eng->renderer);
	}

	if (eng->window != nullptr) {
		SDL_DestroyWindow(eng->window);
	}
}

void Deinit_SDLSubsystems(void)
{
	TTF_Quit();
	SDL_Quit();
}

void Deinit_StateData(void **data)
{
	if (data != nullptr && *data != nullptr) {
		free(*data);
		*data = nullptr;
	}
}
