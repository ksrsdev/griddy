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
	printf("HOLA\n");

	//throttle cpu
	SDL_SetRenderVSync(eng->renderer, 1);
	return true;
}

static bool Init_SDFFontState(GameEngine *eng)
{
	SDL_PropertiesID props = SDL_GetRendererProperties(eng->renderer);

	SDL_GPUDevice *gpu_device = (SDL_GPUDevice *)SDL_GetPointerProperty(
		props, 
		SDL_PROP_RENDERER_GPU_DEVICE_POINTER, 
		NULL
	);

	if (!gpu_device) {
		SDL_Log("Could not get GPU device from renderer! Is the 'gpu' backend active?");
		printf("Could not get GPU device from renderer! Is the 'gpu' backend active?\n");
		return false;
	}

	//Determine correct shader format
	SDL_GPUShaderFormat supported_formats = SDL_GetGPUShaderFormats(gpu_device);

	const char *shader_file = NULL;
    SDL_GPUShaderFormat selected_format = SDL_GPU_SHADERFORMAT_INVALID;

	//Load correct pre-compiled shader based on gpu backend
	if (supported_formats & SDL_GPU_SHADERFORMAT_SPIRV) {
        shader_file = "assets/shaders/sdf_font.spv";
        selected_format = SDL_GPU_SHADERFORMAT_SPIRV;
    } else if (supported_formats & SDL_GPU_SHADERFORMAT_MSL) {
        shader_file = "assets/shaders/sdf_font.msl";
        selected_format = SDL_GPU_SHADERFORMAT_MSL;
    } else if (supported_formats & SDL_GPU_SHADERFORMAT_DXIL) {
        shader_file = "assets/shaders/sdf_font.dlil";
        selected_format = SDL_GPU_SHADERFORMAT_DXIL;
    }

	if (!shader_file) {
        SDL_Log("GPU does not support any of our shader formats!");
        printf("GPU does not support any of our shader formats!\n");
        return false;
    }


	//Load shader
	size_t code_size;
    void* code = SDL_LoadFile(shader_file, &code_size);
    if (!code) {
        SDL_Log("Failed to load shader file: %s", shader_file);
        return false;
    }

	//Create Shader
	SDL_GPUShaderCreateInfo shader_info = {
        .code = (const Uint8 *)code,
        .code_size = code_size,
        .entrypoint = "main",
        .format = selected_format,
        .stage = SDL_GPU_SHADERSTAGE_FRAGMENT,
        .num_samplers = 1,
        .num_storage_textures = 0,
        .num_storage_buffers = 0,
        .num_uniform_buffers = 0
    };

	SDL_GPUShader *final_shader = SDL_CreateGPUShader(gpu_device, &shader_info);

	// Clean up the temporary buffer 
	SDL_free(code);

	if (!final_shader) {
        SDL_Log("Failed to create GPU shader!");
        printf("Failed to create GPU shader!\n");
        return false;
    }

	SDL_GPURenderStateCreateInfo info = {.fragment_shader =  final_shader};
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
