/**
* Bugs Bugs everywhere. This is just a map editor I have hacked down in two days, don't expect something fancy. 
*/

/////////////////////////////////////////////////////////////////////////
//	Defines
//
#define SHADER_VERSION "#version 450\n"
#define CODE(x) #x
#define SHADER(x) SHADER_VERSION CODE(x)
#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#define I8(x) ((int)x)
#define I16(x) ((int)x)
#define TILE_SCALE_BITS   6
#define TILES_VIEW_WIDTH  I8(256u >> TILE_SCALE_BITS)
#define TILES_VIEW_HEIGHT I8(256u >> TILE_SCALE_BITS)
#define WORLD_WIDTH       8
#define WORLD_HEIGHT      8
#define WORLD_STRIDE      I8(TILES_VIEW_WIDTH * WORLD_WIDTH)
#define WORLD_EXTENT      I8(TILES_VIEW_HEIGHT * WORLD_HEIGHT)
#define TILE_WIDTH        I8(1 << TILE_SCALE_BITS)
#define TILE_HEIGHT       I8(1 << TILE_SCALE_BITS)
#define DEG2RAD(x) (x * 0.017453293)

#define SYMBOL_CLEF_G       "\U0001D11E"
#define SYMBOL_CLEF_F       "\U0001D122"
#define SYMBOL_FULL         "\U0001D15D"
#define SYMBOL_HALF         "\U0001D15E"
#define SYMBOL_QUARTER      "\U0001D15F"
#define SYMBOL_EIGHTH       "\U0001D160"
#define SYMBOL_SIXTEENTH    "\U0001D161"
#define SYMBOL_REPEAT_LEFT  "\U0001D106"
#define SYMBOL_REPEAT_RIGHT "\U0001D107"

#define IM_VEC2_CLASS_EXTRA \
    constexpr ImVec2(glm::vec2& f) : x(f.x), y(f.y) {} \
    operator glm::vec2() const { return glm::vec2(x, y); }
#define IM_VEC4_CLASS_EXTRA \
        constexpr ImVec4(const glm::vec4& f) : x(f.x), y(f.y), z(f.z), w(f.w) {} \
        operator glm::vec4() const { return glm::vec4(x,y,z,w); }

/////////////////////////////////////////////////////////////////////////
//	Includes
//
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#define GLAD_GL_IMPLEMENTATION
#include <gl.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <format>
#include <mesh.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <portable-file-dialogs.h>
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#undef min
#undef max

/////////////////////////////////////////////////////////////////////////
//	Types
//
typedef int   Error;
typedef float    f32;
typedef int idx_t;
typedef int16_t i16;
typedef int32_t i32;
typedef uint32_t u32;
using glm::vec2;
using glm::vec3;
struct frame_t { GLuint fbo = 0, tex = 0, rbo = 0; };
struct line_t
{
	glm::vec2 start;
	glm::vec2 end;

	line_t(glm::vec2 const& start, glm::vec2 const& end) : start(start), end(end) {}
};

struct alignas(16) uniforms_t // Align with 16 because of ubo
{
	// Camera
	glm::mat4 view = glm::mat4(1.f);
	glm::vec2 viewport = glm::vec2(1280, 720);
	// Cells
	int cellWidth = TILE_WIDTH;
	int cellHeight = TILE_HEIGHT;
	int gridWidth = WORLD_STRIDE;
	int gridHeight = WORLD_EXTENT;
};

struct object_t
{
	GLuint vao     = 0;
	GLuint vbo     = 0;
	GLuint ebo     = 0;
	GLuint tex     = 0;
	GLuint program = 0;
};

enum Tile_
{
	Tile_Air,
	Tile_Water,

	Tile_Top2,
	Tile_Top,
	Tile_TopLeft,
	Tile_TopRight,
	Tile_Bottom2,
	Tile_Bottom,
	Tile_BottomLeft,
	Tile_BottomRight,
	Tile_Left2,
	Tile_Left,
	Tile_Right2,
	Tile_Right,
	Tile_MiddleLeftTop,
	Tile_MiddleLeft,
	Tile_MiddleRight,
	Tile_Middle,
	Tile_MiddleBottom,
	Tile_MiddleRightTop,
	Tile_MiddleTop,
	Tile_Spikes,
	Tile_SpikedBall,
	Tile_Jumper,
	Tile_BarrierVertical,
	Tile_BarrierHorizontal,
	Tile_WaterTop,
	// Entities
	Tile_E0,
	Tile_E1,
	Tile_E2,
	Tile_E3,
	Tile_E4,
	Tile_E5,
	Tile_E6,
	Tile_E7,
	Tile_E8,
	Tile_E9,
	Tile_E10,
	Tile_E11,
	Tile_E12,
	Tile_E13,
	Tile_E14,
	Tile_E15,
	// Portal
	Tile_Portal0,
	Tile_Portal1,
	Tile_GravitasUp,
	Tile_SpikesDown,
	Tile_Warning,
	Tile_NA,
	Tile_GravitasDown,
	// <Add New Tiles Here>
	Tile_Max,
	// Wormhole
	// Tile_Wormhole0,
	// Tile_Wormhole1,
	// Tile_Wormhole2,
	// Tile_Wormhole3,
};
typedef i8 Tile;

const char* tileStrings[] =
{
	"Air",
	"Water",
	"Top2",
	"Top",
	"TopLeft",
	"TopRight",
	"Bottom2",
	"Bottom",
	"BottomLeft",
	"BottomRight",
	"Left2",
	"Left",
	"Right2",
	"Right",
	"MiddleLeftTop",
	"MiddleLeft",
	"MiddleRight",
	"Middle",
	"MiddleBottom",
	"MiddleRightTop",
	"MiddleTop",
	"Spikes",
	"SpikedBall",
	"Jumper",
	"BarrierVertical",
	"BarrierHorizontal",
	"WaterTop",
	"E0",
	"E1",
	"E2",
	"E3",
	"E4",
	"E5",
	"E6",
	"E7",
	"E8",
	"E9",
	"E10",
	"E11",
	"E12",
	"E13",
	"E14",
	"E15",
	"Portal0",
	"Portal1",
	"GravitasUp",
	"SpikesDown",
	"Warning",
	"NA",
	"GravitasDown",
	/*"Tile_Wormhole0",
	"Tile_Wormhole1",
	"Tile_Wormhole2",
	"Tile_Wormhole3",*/
};

struct txt_t
{
	std::string txt;
	object_t object;
	int tick;
};

typedef uint8_t u8;
enum Note_
{
	G2 = 0x00U, GS2 = 0x01U, AB2 = 0x01U,
	A2 = 0x02U, AS2 = 0x03U, BB2 = 0x03U,
	BH = 0x04U,
	C3 = 0x05U, CS3 = 0x06U, DB3 = 0x06U,
	D3 = 0x07U, DS3 = 0x08U, EB3 = 0x08U,
	E3 = 0x09U, ES3 = 0x0AU, FB3 = 0x0AU,
	F3 = 0x0AU, FS3 = 0x0BU, GB3 = 0x0BU,
	G3 = 0x0CU, GS3 = 0x0DU, AB3 = 0x0DU,
	A3 = 0x0EU, AS3 = 0x0FU, BB3 = 0x0FU,
	B3 = 0x10U, BS3 = 0x11U,
	C4 = 0x11U, CS4 = 0x12U, DB4 = 0x12U,
	D4 = 0x13U, DS4 = 0x14U, EB4 = 0x14U,
	E4 = 0x15U, ES4 = 0x16U,
	F4 = 0x16U, FS4 = 0x17U, GB4 = 0x17U,
	G4 = 0x18U, GS4 = 0x19U, AB4 = 0x19U,
	A4 = 0x1AU, AS4 = 0x1BU, BB4 = 0x1BU,
	B4 = 0x1CU, BS4 = 0x1DU,
	C5 = 0x1DU, CS5 = 0x1EU, DB5 = 0x1EU,
	D5 = 0x1FU, DS5 = 0x20U, EB5 = 0x20U,
	E5 = 0x21U,
	F5 = 0x22U, FS5 = 0x23U, GB5 = 0x23U,
	G5 = 0x24U, GS5 = 0x25U, AB5 = 0x25U,
	A5 = 0x26U, AS5 = 0x27U, BB5 = 0x27U,
	B5 = 0x28U, BS5 = 0x29U, CB6 = 0x28U,
	C6 = 0x29U, CS6 = 0x2AU, DB6 = 0x2AU,
	D6 = 0x2BU, DS6 = 0x2CU, EB6 = 0x2CU,
	E6 = 0x2DU, ES6 = 0x2EU,
	F6 = 0x2EU, FS6 = 0x2FU, GB6 = 0x2FU,
	G6 = 0x30U, GS6 = 0x31U, AB6 = 0x31U,
	A6 = 0x32U, AS6 = 0x33U, BB6 = 0x33U,
	B6 = 0x34U, BS6 = 0x35U,
	C7 = 0x35U, CS7 = 0x36U, DB7 = 0x36U,
	D7 = 0x37U, DS7 = 0x38U, EB7 = 0x38U,
	E7 = 0x39U,
	F7 = 0x3AU, FS7 = 0x3BU, GB7 = 0x3BU,
	G7 = 0x3CU, GS7 = 0x3DU, AB7 = 0x3DU,
	A7 = 0x3EU, AS7 = 0x3FU, BB7 = 0x3FU,
	Note_End = 0x80U, CH0 = 0x80U, CH1 = 0x80U, CHN = 0x80U
};
typedef u8 Note;

enum Unit_
{
	Unit_Full,
	Unit_Half,
	Unit_Quarter,
	Unit_Eighth,
	Unit_Sixteenth,

	Unit_DottedHalf,
	Unit_DottedQuarter,
	Unit_DottedEighth,
	Unit_DottedSixteenth,

	Unit_Max, // End of enum
};
typedef int Unit;

struct grand_note_t
{
	Unit clefG;
	Unit clefF;
};

struct measure_t
{
	std::vector<grand_note_t> grandStaff;
};

struct section_t
{
	std::vector<measure_t> measures{4};
	int  beats = 4;
	Unit unit  = Unit_Quarter;
};

struct music_sheet_t
{
	char trackName[50]{};
	std::vector<section_t> sections{1};
};

typedef struct nibble_ampoff_table_t
{
	u8 amplitudes[16];
} nibble_ampoff_table_t, * nibble_ampoff_table;

typedef struct twang_table_t
{
	u8 frequencies[8];
} twang_table_t, * twang_table;

typedef struct track_t
{
	nibble_ampoff_table_t const* amplitudes;
	twang_table_t const* frequencies;
	Note notes[];
} track_t, * track;

struct editor_t
{
	std::vector<line_t>     lines;
	std::vector<glm::ivec2> sprite;
	char spriteName[50]{};
	char stageName[50]{};
	int selectedPoint = -1;
	ImFont* font;
	music_sheet_t musicSheet;
	// UI
	f32 splitterWidth = 6.f;
	f32 stageSplitRatio = 0.75;
	f32 spriteSplitRatio = 0.75;
	f32 trackSplitRatio = 0.75;
	bool spriteFirstIsMov = true;
	f32 spriteRotation = 0.0f;
	int draggedSpritePoint = -1;
	int beats   = 4; // Beats per measure
	int unitBit = 2; // Unit of a beat in measure
	// Camera
	f32       zoom = 7.5f;
	glm::vec2 position = glm::vec2(0.5);
	bool      isPanning = false;
	glm::vec2 lastMousePos;
	glm::ivec2 tile = { 0,0 };
	glm::ivec2 selectionStart{ 0, 0 }, selectionEnd{ 1, 1 };
	uniforms_t uniforms;
	// GL stuff
	GLuint   ubo = 0;
	object_t lineList;
	object_t grid;
	frame_t  stageTarget;
	object_t atlas;
	glm::ivec2 atlasDim = {1000, 1000};
	std::vector<stbtt_packedchar> atlasChars;
	std::map<idx_t, txt_t> txts;
	double last = 0.0;

	GLuint noteTextures[Unit_Max]{};
};
editor_t e;

struct world_t
{
	// "Emulation"
	bool sceneIsTarget = true;
	glm::ivec2 beam = { 0, 0 };
	glm::ivec2 selectedTile;
	int ticks = 0;
	bool freq2;
	bool freq16;
	std::vector<Tile> stage;
	int tileIdx;
};
world_t WORLD;

/////////////////////////////////////////////////////////////////////////
//	Forward Declerations
//
static GLFWwindow* window_create(const char* title, int w, int h);
void target_texture_recreate(int width, int height, frame_t& frame);
Error file_read(std::string file, std::string& content);
int main();
GLuint program_create_fv(const char* const fragmentVertexShader[2]);
bool centered_button(const char* label, const ImVec2& size = ImVec2(0, 0));
void save_stage();
void open_stage();
void save_sprite();
void open_sprite();
void centered_text(const char* label);
void beam_set_position(i32 y, i32 x);
void Draw_Line_d(int a, int b);
void Draw_VLc(void const* x);
void Intensity_5F();
void world_render_tile(Tile tile, i16 tileLeft, i16 tileRight, i16 tileTop, i16 tileBottom);
void world_progress(void);
void copy_to_clipboard(std::string const& text);
void load_font();
void print_txt(float x, float y, idx_t id, std::string txt, float scaleX = 0.6, float scaleY = 0.75);
void window_size_callback(GLFWwindow* window, int width, int height);
void window_draw();
GLuint texture_read(const char* file);

/////////////////////////////////////////////////////////////////////////
//	Functions
//
int main()
{
	if (!pfd::settings::available())
	{
		std::cout << "Portable File Dialogs are not available on this platform.\n";
		return 1;
	}
	pfd::settings::verbose(true);

	std::string linesFrag, linesVert, gridFrag, gridVert;
	if (file_read(SHADERS_PATH "/lines.frag", linesFrag) || file_read(SHADERS_PATH "/lines.vert", linesVert) || file_read(SHADERS_PATH "/grid.frag", gridFrag) || file_read(SHADERS_PATH "/grid.vert", gridVert))
	{
		printf("Could not read file.\n");
		return 0;
	}
	const char* shaders[4] =
	{
		linesFrag.c_str(),
		linesVert.c_str(),
		gridFrag.c_str(),
		gridVert.c_str(),
	};

	GLFWwindow* win = window_create("Stage Editor", 1280, 720);
	if (!win)
	{
		glfwTerminate();
		return 0;
	}

	// OGL stuff
	e.lines.reserve(10000);
	{
		// Load Font
		load_font();

		// Read notes
		e.noteTextures[Unit_Full]          = texture_read(IMGS_PATH "/full.png");
		e.noteTextures[Unit_Half]          = texture_read(IMGS_PATH "/half.png");
		e.noteTextures[Unit_DottedHalf]    = texture_read(IMGS_PATH "/dotted_half.png");
		e.noteTextures[Unit_Quarter]       = texture_read(IMGS_PATH "/quarter.png");
		e.noteTextures[Unit_DottedQuarter] = texture_read(IMGS_PATH "/dotted_quarter.png");
		e.noteTextures[Unit_Eighth]        = texture_read(IMGS_PATH "/eighth.png");
		e.noteTextures[Unit_Sixteenth]     = texture_read(IMGS_PATH "/sixteenth.png");

		// Create ubo buffer for camera
		{
			glGenBuffers(1, &e.ubo);
			glBindBuffer(GL_UNIFORM_BUFFER, e.ubo);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(uniforms_t), NULL, GL_STATIC_DRAW);
			glBindBufferRange(GL_UNIFORM_BUFFER, 0, e.ubo, 0, sizeof(uniforms_t));
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}
		// Create texture to render to 
		target_texture_recreate(1280, 720, e.stageTarget);

		// Create line list buffers
		{
			// Compile lines program
			if (e.lineList.program = program_create_fv(shaders), !e.lineList.program)
			{
				glfwTerminate();
				return 0;
			}
			glGenBuffers(1, &e.lineList.vbo);
			glBindBuffer(GL_ARRAY_BUFFER, e.lineList.vbo);
			glBufferData(GL_ARRAY_BUFFER, e.lines.capacity() * sizeof(e.lines[0]), nullptr, GL_STATIC_DRAW);

			// Create vao
			glGenVertexArrays(1, &e.lineList.vao);
			glBindVertexArray(e.lineList.vao);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(float), nullptr);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		// Create quad for grid, as well as a texture for selection
		{
			// Compile lines program
			if (e.grid.program = program_create_fv(shaders + 2), !e.grid.program)
			{
				glfwTerminate();
				return 0;
			}
			glUseProgram(e.grid.program);
			GLint location = glGetUniformLocation(e.grid.program, "grid");
			glUniform1i(location, 0);
			glUseProgram(0);
			// Create Texture
			std::vector<float> intensities(e.uniforms.gridWidth * e.uniforms.gridHeight, 0.2f);
			intensities[(WORLD_EXTENT - 1) * WORLD_STRIDE] = 1.f;
			glGenTextures(1, &e.grid.tex);
			glBindTexture(GL_TEXTURE_2D, e.grid.tex);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_RED, e.uniforms.gridWidth, e.uniforms.gridHeight);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, e.uniforms.gridWidth, e.uniforms.gridHeight, 0, GL_RED, GL_FLOAT, intensities.data());
			glBindTexture(GL_TEXTURE_2D, 0);

			// Create quad
			const GLfloat quadVertices[] = 
			{ 
				0.0f, 0.0f, 1.0f,
				1.0f, 0.0f, 1.0f,
				0.0f,  1.0f, 0.0f,
				0.0f,  1.0f, 1.0f,
				1.0f, 0.0f, 0.0f,
				1.0f,  1.0f, 1.0f,
			};
			glGenBuffers(1, &e.grid.vbo);
			glBindBuffer(GL_ARRAY_BUFFER, e.grid.vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
			// Create vao
			glGenVertexArrays(1, &e.grid.vao);
			glBindVertexArray(e.grid.vao);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), nullptr);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			glDisable(GL_CULL_FACE);
			glDepthMask(GL_TRUE);
			//glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
	}

	double last = glfwGetTime();
	WORLD.stage.resize(e.uniforms.gridWidth * e.uniforms.gridHeight, Tile_Air);
	
	glfwSetWindowSizeCallback(win, window_size_callback);
	/**
	* Main Loop
	*/
	while (!glfwWindowShouldClose(win))
	{
		glfwPollEvents();
		if (glfwGetWindowAttrib(win, GLFW_ICONIFIED) != 0)
		{
			ImGui_ImplGlfw_Sleep(10);
			continue;
		}
		
		window_draw();
	}
	// Im too lazy to clean up opengl stuff
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(win);
	glfwTerminate();
	return 0;
}

void window_draw()
{
	// Update target texture on window size change
	{
		int width, height;
		glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);
		if (width ^ ((int)e.uniforms.viewport.x) || height ^ ((int)e.uniforms.viewport.y))
		{
			target_texture_recreate(((int)e.uniforms.viewport.x), ((int)e.uniforms.viewport.y), e.stageTarget);
			e.uniforms.viewport = glm::vec2(width, height);
		}
	}

	// Update lines and camera. This is super inefficient, but who cares with 4.2GHz and PCIe 5.0.
	e.uniforms.view = glm::mat4(1.f);
	e.uniforms.view = glm::translate(e.uniforms.view, glm::vec3(-e.position, 0.f));
	e.uniforms.view = glm::scale(e.uniforms.view, glm::vec3(e.zoom / e.uniforms.cellWidth, e.zoom / e.uniforms.cellHeight, 1.0f));

	double current = glfwGetTime();
	if ((current - e.last) >= 0.02f)
	{
		e.lines.clear();
		//e.lines.push_back({ glm::vec2(0,0), glm::vec2(100,100) });
		world_progress();
		e.last = glfwGetTime();
	}

	glBindBuffer(GL_ARRAY_BUFFER, e.lineList.vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, e.lines.size() * sizeof(e.lines[0]), e.lines.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_UNIFORM_BUFFER, e.ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(e.uniforms), &e.uniforms);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);


	// Start Frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// UI
	if (ImGui::IsKeyPressed(ImGuiKey_Q))
		glfwSetWindowShouldClose(glfwGetCurrentContext(), true);

	ImGui::DockSpaceOverViewport(1, nullptr, ImGuiDockNodeFlags_NoUndocking);

	ImGui::SetNextWindowDockID(1);
	if (ImGui::Begin("Sprite"))
	{
		glm::vec2 size = ImGui::GetWindowSize();
		if (ImGui::BeginTable("Table", 3, ImGuiTableFlags_SizingFixedFit))
		{
			glm::vec2 dummySize = size;
			dummySize.x *= e.spriteSplitRatio;
			ImGui::TableNextColumn();
			//ImGui::SetColumnWidth(0, size.x* e.splitRatio);
			//ImGui::Image((ImTextureID)e.target.tex, imgSize, ImVec2(0, 1), ImVec2(1, 0));
			ImGui::InvisibleButton("Sprite Grid", dummySize);
			glm::vec2 p = ImGui::GetItemRectMin();
			float cellSize = dummySize.y / TILE_HEIGHT;
			float gridWidth = cellSize * TILE_WIDTH;
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			drawList->AddRectFilled(p, ImGui::GetItemRectMax(), ImColor(0.21f, 0.25f, 0.27f));
			float x = 0, y = 0;
			for (size_t yi = 0; yi <= TILE_HEIGHT; yi++)
			{
				drawList->AddLine({ p.x, p.y + y }, { p.x + std::min(gridWidth, dummySize.x), p.y + y }, yi == TILE_HEIGHT / 2 ? 0xFF00FF00 : 0xFFFFFFFF, yi == TILE_HEIGHT / 2 ? 2.0 : 0.125);
				y += cellSize;
			}
			for (size_t xi = 0; xi <= TILE_WIDTH; xi++)
			{
				drawList->AddLine({ p.x + std::min(x, dummySize.x), p.y }, { p.x + std::min(x, dummySize.x), p.y + dummySize.y }, xi == TILE_WIDTH / 2 ? 0xFFFF0000 : 0xFFFFFFFF, xi == TILE_WIDTH / 2 ? 2.0 : 0.125);
				x += cellSize;
			}

			const auto cellMidByTile = [=](glm::vec2 point) -> glm::vec2 { return p + glm::vec2(point) * cellSize; };

			if (e.sprite.size())
			{
				glm::vec2 origin = cellMidByTile(e.sprite.front()); 
				glm::vec2 lastMid = origin;
				for (glm::ivec2 const& point : e.sprite)
				{
					glm::vec2 mid = cellMidByTile(point);
					drawList->AddCircleFilled(mid, cellSize / 2, 0xA0FF0000);
					drawList->AddLine(lastMid, mid, 0xFFFFFFFF, 3.f);
					lastMid = mid;
				}
			}

			if (ImGui::IsItemHovered())
			{
				glm::vec2 mousePos = ImGui::GetMousePos();
				glm::vec2 local = mousePos - p;
				glm::vec2 tile = glm::floor(local / cellSize);
				glm::vec2 tileMid = cellMidByTile(tile);
				if (tile.x >= 0 && tile.x < TILE_WIDTH && tile.y >= 0 && tile.y < TILE_HEIGHT)
				{
					drawList->AddCircleFilled(tileMid, cellSize / 2, 0xFF000080);

					auto contains = [&](glm::ivec2 const& t) { return t.x == tile.x && t.y == tile.y; };
					if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
					{
						e.sprite.emplace_back(glm::ivec2(tile.x, tile.y));
					}
					else if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
					{
						auto it = std::find_if(e.sprite.rbegin(), e.sprite.rend(), contains);
						if (it != e.sprite.rend())
							e.sprite.erase(std::next(it).base());
					}
					else if (ImGui::IsMouseClicked(ImGuiMouseButton_Middle))
					{
						auto it = std::find_if(e.sprite.rbegin(), e.sprite.rend(), contains);
						if (it != e.sprite.rend())
						{
							e.draggedSpritePoint = std::distance(e.sprite.begin(), std::next(it).base());
						}
					}
				}
			}

			if (ImGui::IsMouseReleased(ImGuiMouseButton_Middle))
			{
				e.draggedSpritePoint = -1;
			}
			else if (e.draggedSpritePoint >= 0)
			{
				glm::vec2 mousePos = ImGui::GetMousePos();
				glm::vec2 local = mousePos - p;
				glm::vec2 tile = glm::floor(local / cellSize);
				e.sprite[e.draggedSpritePoint] = tile;
			}

			ImGui::TableNextColumn();
			ImGui::Button("Sprite Splitter", ImVec2(e.splitterWidth, size.y));
			if (ImGui::IsItemActive())
			{
				int x, y;
				glfwGetWindowPos(glfwGetCurrentContext(), &x, &y);
				e.spriteSplitRatio = (ImGui::GetMousePos().x - x) / size.x;
				e.spriteSplitRatio = std::min(std::max(e.spriteSplitRatio, 0.01f), 0.99f);
			}

			ImGui::TableNextColumn();
			if (centered_button("Flip H."))
			{
				for (size_t i = 0; i < e.sprite.size(); ++i)
				{
					e.sprite[i].x = TILE_WIDTH / 2 - (e.sprite[i].x - TILE_WIDTH / 2);
				}
			}
			ImGui::SameLine();
			if (centered_button("Flip V."))
			{
				for (size_t i = 0; i < e.sprite.size(); ++i)
				{
					e.sprite[i].y = TILE_HEIGHT / 2 - (e.sprite[i].y - TILE_HEIGHT / 2);
				}
			}
			ImGui::NewLine();

			f32 spriteRotation = e.spriteRotation;

			if (ImGui::ArrowButton("RotateLeft", ImGuiDir_Left))
				spriteRotation -= 5.f;
			ImGui::SameLine();
			if (ImGui::ArrowButton("RotateRight", ImGuiDir_Right))
				spriteRotation += 5.f;

			f32 scaling;
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			if (ImGui::Button("+"))
			{
				scaling = 1;
				goto scale;
			}
			ImGui::SameLine();
			if (ImGui::Button("-"))
			{
				scaling = -1;
			scale:
				if (!e.sprite.empty())
				{
					glm::vec2 centroid(0.f);
					for (glm::ivec2& pos : e.sprite)
						centroid += glm::vec2(pos);
					centroid /= static_cast<float>(e.sprite.size());
					for (glm::ivec2& pos : e.sprite)
					{
						glm::vec2 posF = pos;
						glm::vec2 centered = (posF - centroid);
						pos = glm::round(centered + scaling * glm::sign(centered) + centroid);
					}
				}
			}

			if (spriteRotation != e.spriteRotation)
			{
				f32 dphi = spriteRotation - e.spriteRotation;
				e.spriteRotation = spriteRotation;
				if (!e.sprite.empty())
				{
					f32 deg = DEG2RAD(dphi);
					glm::mat2 rot = glm::mat2(
						glm::vec2(cos(deg), sin(deg)),
						glm::vec2(-sin(deg), cos(deg))
					);

					glm::vec2 centroid(0.f);
					for (const auto& p : e.sprite)
						centroid += glm::vec2(p);
					centroid /= static_cast<float>(e.sprite.size());

					for (auto& pos : e.sprite)
					{
						glm::vec2 posF = pos;
						glm::vec2 rotated = rot * (posF - centroid) + centroid;
						pos = glm::floor(rotated);
					}
				}	
			}
			ImGui::NewLine();
			if (ImGui::Button("Clear"))
			{
				e.sprite.clear();
				e.selectedPoint = -1;
			}
			ImGui::SameLine();
			if (ImGui::Button("Export") && e.sprite.size() > 0)
			{
				size_t points = e.sprite.size() - e.spriteFirstIsMov;
				std::string stage = "const i8 " + std::string(e.spriteName) + "[" + std::to_string(points * 2 + 1) + "] = \n{\n\tLENGTH(" + std::to_string(points) + "),\n";
				glm::ivec2 prev = e.spriteFirstIsMov ? e.sprite.front() : glm::ivec2{ TILE_WIDTH / 2, TILE_HEIGHT / 2 };
				for (size_t i = e.spriteFirstIsMov; i < e.sprite.size(); i++)
				{
					glm::ivec2 point = e.sprite[i];
					stage += std::format("\t{}, {},\n", prev.y - point.y, point.x - prev.x);
					prev = point;
				}
				stage += "};\n";
				copy_to_clipboard(stage);
			}
			ImGui::SameLine();
			if (ImGui::Button("Save"))
			{
				save_sprite();
			}
			if (ImGui::Button("Open..."))
			{
				open_sprite();
			}
			ImGui::Checkbox("Move On First Point", &e.spriteFirstIsMov);

			ImGui::InputText("Name", e.spriteName, sizeof(e.spriteName));


			if (ImGui::BeginTable("points", 2, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_SizingStretchSame, ImVec2{ size.x - dummySize.x - e.splitterWidth, 0 }))
			{
				ImGui::TableSetupColumn("Position");
				ImGui::TableSetupColumn("Change");

				ImGui::TableHeadersRow();
				if (e.sprite.size())
				{
					glm::ivec2 prevPoint = e.spriteFirstIsMov ? e.sprite.front() : glm::ivec2{ TILE_WIDTH / 2, TILE_HEIGHT / 2 };
					for (i32 i = e.spriteFirstIsMov; i < e.sprite.size(); ++i)
					{
						ImGui::TableNextRow();
						ImGui::TableNextColumn();

						glm::ivec2 point = e.sprite[i];
						std::string txt = std::format("({}, {})##{}", point.x - TILE_WIDTH / 2, TILE_HEIGHT / 2 - point.y, i);
						bool selected = e.selectedPoint == i;
						ImVec2 selectableSize = { ImGui::GetContentRegionAvail().x, ImGui::GetTextLineHeightWithSpacing() };
						if (ImGui::Selectable(txt.c_str(), &selected, 0, selectableSize))
						{
							e.selectedPoint = i;
						}
						if (ImGui::BeginDragDropSource())
						{
							ImGui::SetDragDropPayload("POINT_INDEX", &i, sizeof(int));
							ImGui::EndDragDropSource();
						}

						if (ImGui::BeginDragDropTarget())
						{
							if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("POINT_INDEX"))
							{
								if (idx_t idx = *(idx_t*)payload->Data; idx != i)
									std::swap(e.sprite[idx], e.sprite[i]);
							}
							ImGui::EndDragDropTarget();
						}

						ImGui::TableNextColumn();
						ImGui::Text("(%d, %d)", point.x - prevPoint.x, prevPoint.y - point.y);
						prevPoint = point;
					}
				}
				ImGui::EndTable();
			}
			ImGui::EndTable();
		}
	}
	ImGui::End();

	ImGui::SetNextWindowDockID(1);
	if (ImGui::Begin("Stage", nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDecoration))
	{
		// Draw scene
		glBindFramebuffer(GL_FRAMEBUFFER, e.stageTarget.fbo);
		{
			glViewport(0, 0, e.uniforms.viewport.x, e.uniforms.viewport.y);
			glClearColor(0.21, 0.25, 0.27, 1.00);
			glClear(GL_COLOR_BUFFER_BIT);
			// Draw grid
			glUseProgram(e.grid.program);
			glBindVertexArray(e.grid.vao);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, e.grid.tex);
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, e.uniforms.gridWidth * e.uniforms.gridHeight);
			glBindTexture(GL_TEXTURE_1D, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glUseProgram(0);
			glBindVertexArray(0);
			// Draw line list
			glUseProgram(e.lineList.program);
			glBindVertexArray(e.lineList.vao);
			glDrawArrays(GL_LINES, 0, e.lines.size() * 2);
			glUseProgram(0);
			glBindVertexArray(0);
			// Draw txts
			glUseProgram(e.atlas.program);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, e.atlas.tex);
			for (auto const& kv : e.txts)
			{
				if (kv.second.object.vao)
				{
					glBindVertexArray(kv.second.object.vao);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, kv.second.object.ebo);
					glDrawElements(GL_TRIANGLES, kv.second.txt.size() * 6, GL_UNSIGNED_INT, nullptr);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
					glBindVertexArray(0);
				}
			}
			glBindTexture(GL_TEXTURE_2D, 0);
			glUseProgram(0);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// ImGui side begins here
		glm::vec2 size = ImGui::GetWindowSize();
		if (ImGui::BeginTable("Table", 3, ImGuiTableFlags_SizingFixedFit))
		{
			ImGui::TableNextColumn();
			//ImGui::SetColumnWidth(0, size.x* e.splitRatio);
			glm::vec2 imgSize = size;
			imgSize.x = size.x * e.stageSplitRatio;
			ImGui::Image((ImTextureID)e.stageTarget.tex, imgSize, ImVec2(0, 1), ImVec2(1, 0));
			if (ImGui::IsItemHovered())
			{
				if (ImGui::IsMouseDragging(ImGuiMouseButton_Right))
					e.isPanning = true;

				if (f32 wheel = ImGui::GetIO().MouseWheel; ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && wheel)
					e.zoom += wheel;

				glm::vec2 imagePosition = ImGui::GetItemRectMin();
				glm::vec2 mouse = ImGui::GetMousePos();
				glm::vec2 screenPosition = mouse - imagePosition;

				glm::vec2 screenSpace = glm::vec2((screenPosition.x / imgSize.x - 0.5) * 2.f, (0.5 - screenPosition.y / imgSize.y) * 2.f);
				glm::vec2 worldSpace = glm::inverse(e.uniforms.view) * glm::vec4(screenSpace, -1.f, 1.f);
				glBindTexture(GL_TEXTURE_2D, e.grid.tex);
				auto outsideSelection = [&]() { return !(glm::all(glm::greaterThanEqual(e.tile, e.selectionStart)) && glm::all(glm::lessThan(e.tile, e.selectionEnd)));};
				if (outsideSelection())
				{
					float opacity = 0.2f;
					glTexSubImage2D(GL_TEXTURE_2D, 0, e.tile.x, WORLD_EXTENT - e.tile.y - 1, 1, 1, GL_RED, GL_FLOAT, &opacity);
				}
				e.tile.x = worldSpace.x;
				e.tile.y = WORLD_EXTENT - worldSpace.y;
				bool insideGrid = e.tile.x >= 0 && e.tile.x < e.uniforms.gridWidth && e.tile.y >= 0 && e.tile.y < e.uniforms.gridHeight;
				if (insideGrid)
				{
					if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
					{
						float opacity = 0.2f;
						glTexSubImage2D(GL_TEXTURE_2D, 0, e.selectionStart.x, WORLD_EXTENT - e.selectionEnd.y, e.selectionEnd.x - e.selectionStart.x, e.selectionEnd.y - e.selectionStart.y, GL_RED, GL_FLOAT, &opacity);
						if (!ImGui::IsKeyDown(ImGuiKey_LeftShift))
						{
							e.selectionStart = e.tile;
							e.selectionEnd = e.tile;
						}
						else
						{
							e.selectionEnd = e.tile;
						}

						glm::ivec2 selMin = glm::min(e.selectionStart, e.selectionEnd);
						glm::ivec2 selMax = glm::max(e.selectionStart, e.selectionEnd);
						e.selectionStart = selMin;
						e.selectionEnd = selMax;
						e.selectionEnd += glm::ivec2(1, 1);
						opacity = 1.f;
						glTexSubImage2D(GL_TEXTURE_2D, 0, e.selectionStart.x, WORLD_EXTENT - e.selectionEnd.y, e.selectionEnd.x - e.selectionStart.x, e.selectionEnd.y - e.selectionStart.y, GL_RED, GL_FLOAT, &opacity);
					}
					//else // Hover
					//{
					//	opacity = 0.5f;
					//	glTexSubImage2D(GL_TEXTURE_2D, 0, e.tile.x, WORLD_EXTENT - e.tile.y - 1, 1, 1, GL_RED, GL_FLOAT, &opacity);
					//}
				}
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			if (e.isPanning)
			{
				glm::vec2 delta = glm::vec2(ImGui::GetMousePos()) - e.lastMousePos;
				delta.y *= -1;
				e.position += (e.zoom * delta) / e.uniforms.viewport;
			}

			if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
			{
				e.isPanning = false;
			}
			e.lastMousePos = ImGui::GetMousePos();

			ImGui::TableNextColumn();
			ImGui::Button("Stage Splitter", ImVec2(e.splitterWidth, size.y));
			if (ImGui::IsItemActive())
			{
				int x, y;
				glfwGetWindowPos(glfwGetCurrentContext(), &x, &y);
				e.stageSplitRatio = (ImGui::GetMousePos().x - x) / size.x;
				e.stageSplitRatio = std::min(std::max(e.stageSplitRatio, 0.01f), 0.99f);
			}
			//ImGui::SameLine();
			ImGui::TableNextColumn();
			ImGui::SetNextWindowSize({ size.x - imgSize.x - e.splitterWidth, size.y });
			if (ImGui::BeginChild("Buttons and stuff..."))
			{
				ImGui::Text("Pos:      (%.3f, %.3f), Zoom: %.3f", e.position.x * e.uniforms.cellWidth, e.position.y * e.uniforms.cellHeight, e.zoom);
				ImGui::Text("Tile:     (%d, %d)", e.tile.x, e.tile.y);
				if (e.selectionStart == e.selectionEnd)
				{
					ImGui::Text("Selected: (%d, %d)", e.selectionStart.x, e.selectionStart.y);
				}
				else
				{
					ImGui::Text("Selected: (%d, %d) To (%d, %d)", e.selectionStart.x, e.selectionStart.y, e.selectionEnd.x, e.selectionEnd.y);
				}
				ImGui::NewLine();
				ImGui::InputText("Stage", e.stageName, IM_ARRAYSIZE(e.stageName));
				if (ImGui::Button("Fill"))
				{
					ImGui::OpenPopup("Material");
				}

				if (ImGui::BeginPopup("Material"))
				{
					if (ImGui::Selectable("Air"))
					{
						std::fill(WORLD.stage.begin(), WORLD.stage.end(), Tile_Air);
					}
					if (ImGui::Selectable("Water"))
					{
						std::fill(WORLD.stage.begin(), WORLD.stage.end(), Tile_Water);
					}
					if (ImGui::Selectable("N/A"))
					{
						std::fill(WORLD.stage.begin(), WORLD.stage.end(), Tile_NA);
					}
					ImGui::EndPopup();
				}

				if (ImGui::Button("Export"))
				{
					std::string stage = "";
					stage += "const Tile " + std::string(e.stageName) + "[" + std::to_string(WORLD_EXTENT * WORLD_STRIDE) + "] = \n{\n";
					for (size_t y = 0, idx = 0; y < WORLD_EXTENT; ++y)
					{
						stage += "\t";
						for (size_t x = 0; x < WORLD_STRIDE; ++x, ++idx)
						{
							stage += std::format("Tile_{}, ", tileStrings[WORLD.stage[idx]]);
						}
						stage += "\n";
					}
					stage += "};\n";
					copy_to_clipboard(stage);
				}
				ImGui::SameLine();
				if (ImGui::Button("Save"))
				{
					save_stage();
				}
				ImGui::SameLine();
				if (ImGui::Button("Open..."))
				{
					open_stage();
				}

				ImGui::NewLine();
				Tile& tile = WORLD.stage[e.selectionStart.y * WORLD_STRIDE + e.selectionStart.x];
				glm::vec2 uiTileSize = { TILE_WIDTH, TILE_HEIGHT };
				int tilesInRow = std::floor((size.x - imgSize.x - e.splitterWidth) / uiTileSize.x);
				WORLD.sceneIsTarget = false;
				if (tilesInRow > 0)
				{
					for (int i = 0; i < Tile_Max; i++)
					{
						std::string id = std::format("##TILE_{}", i);
						if (ImGui::Selectable(id.c_str(), tile == i, 0, uiTileSize))
						{
							for (int y = e.selectionStart.y; y < e.selectionEnd.y; ++y)
								for (int x = e.selectionStart.x; x < e.selectionEnd.x; ++x)
									WORLD.stage[y * WORLD_STRIDE + x] = i;
						}

						ImDrawList* draw_list = ImGui::GetWindowDrawList();
						glm::vec2 p = ImGui::GetItemRectMin();
						glm::vec2 q = ImGui::GetItemRectMax();
						glm::vec2 txtSize = ImGui::CalcTextSize(tileStrings[i]);
						glm::vec2 mid = (p + q - txtSize) / 2.f;
						draw_list->AddRect(p, q, IM_COL32(255, 255, 255, 255));
						draw_list->AddText(ImVec2(mid.x, p.y), IM_COL32(255, 255, 255, 255), tileStrings[i]);
						world_render_tile(i, 0, TILE_WIDTH, 0, -TILE_HEIGHT);

						if ((i + 1) % tilesInRow != 0)
							ImGui::SameLine();
					}
				}
				WORLD.sceneIsTarget = true;

			}
			ImGui::EndChild();

			ImGui::EndTable();
		}
	}
	ImGui::End();

	ImGui::SetNextWindowDockID(1);
	if (ImGui::Begin("Music Sheet", nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDecoration))
	{
		glm::vec2 size = ImGui::GetWindowSize();
		if (ImGui::BeginTable("Table", 3, ImGuiTableFlags_SizingFixedFit))
		{
			ImGui::TableNextColumn();
			//ImGui::SetColumnWidth(0, size.x* e.splitRatio);
			glm::vec2 trackSize = size;
			trackSize.x = size.x * e.trackSplitRatio;
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			glm::vec2 pos = ImGui::GetCursorScreenPos();
			glm::vec2 scroll = { ImGui::GetScrollX(), ImGui::GetScrollY() };
			ImGui::InvisibleButton("Sheet", trackSize);
			glm::vec2 min = ImGui::GetItemRectMin();
			glm::vec2 max = ImGui::GetItemRectMax();
			//if (ImGui::IsItemFocused())

			ImGui::PushFont(e.font);
			{
				glm::vec2 clefFSize = ImGui::CalcTextSize(SYMBOL_CLEF_F);
				auto rmin = min + 0.5f;
				auto rmax = max - 0.5f;
				auto color = IM_COL32(255, 255, 255, 255);
				glm::vec2 mousePos = ImGui::GetMousePos();
				glm::vec2 local = mousePos - min;
				for (section_t& s : e.musicSheet.sections)
				{
					for (size_t c = 0; c < 2; c++) // Clef F and G
					{
						rmin.y += 50;
						auto next = rmin + glm::vec2(rmax.x - rmin.x, 100);
						vec2 textPos = { rmin.x + 0.5f, (next.y + rmin.y) * 0.5f - clefFSize.y };
						drawList->AddText(textPos, color, SYMBOL_EIGHTH);
						drawList->AddRect(rmin, next, color);
						drawList->AddLine({ rmin.x + 50, rmin.y }, { rmin.x + 50, next.y }, color);
						for (size_t l = 0; l < 4; ++l)
						{
							f32 nextY = rmin.y + 25;
							if (local.y >= rmin.y && local.y < nextY)
							{
								vec2 min = rmin;
								vec2 max = rmin + vec2{ 35, 50 };
								drawList->AddImage(e.noteTextures[Unit_Sixteenth], min, max, { 0, 0 }, { 1, 1 }, IM_COL32(248, 248, 255, 255));
							}
							rmin.y = nextY;
							drawList->AddLine(rmin, ImVec2{ next.x, rmin.y}, color);
						}
					
						for (measure_t& m : s.measures)
						{

						}
						rmin.y += 50;
					}
				}
			}
			ImGui::PopFont();
			
			
			ImGui::TableNextColumn();
			ImGui::Button("Track Splitter", ImVec2(e.splitterWidth, size.y));
			if (ImGui::IsItemActive())
			{
				int x, y;
				glfwGetWindowPos(glfwGetCurrentContext(), &x, &y);
				e.trackSplitRatio = (ImGui::GetMousePos().x - x) / size.x;
				e.trackSplitRatio = std::min(std::max(e.trackSplitRatio, 0.01f), 0.99f);
			}
			//ImGui::SameLine();
			ImGui::TableNextColumn();
			ImGui::SetNextWindowSize({ size.x - trackSize.x - e.splitterWidth, size.y });
			if (ImGui::BeginChild("Buttons and stuff..."))
			{
				static const char* beats[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16" };
				if (ImGui::BeginCombo("Beat", beats[e.beats - 1]))
				{
					for (int i = 0; i < IM_ARRAYSIZE(beats); ++i)
					{
						bool isSelected = i == (e.beats - 1);
						if (ImGui::Selectable(beats[i]))
							e.beats = i + 1;
						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				static const char* units[] =
				{
					SYMBOL_FULL,
					SYMBOL_HALF,
					SYMBOL_QUARTER, // Quarter
					SYMBOL_EIGHTH, 
					SYMBOL_SIXTEENTH,
				};
				if (ImGui::BeginCombo("Unit", units[e.unitBit]))
				{
					for (int i = 0; i < 5; ++i)
					{
						bool isSelected = i == e.unitBit;
						if (ImGui::Selectable(units[i]))
							e.unitBit = i;
						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
				ImGui::NewLine();
				ImGui::InputText("Name", e.musicSheet.trackName, IM_ARRAYSIZE(e.musicSheet.trackName));
				
				if (ImGui::Button("Export"))
				{
					std::string track = "const track_t ";
					track += std::string(e.musicSheet.trackName) + "[] = \n{\n";
					for (size_t y = 0, idx = 0; y < WORLD_EXTENT; ++y)
					{
						track += "\t";
						for (size_t x = 0; x < WORLD_STRIDE; ++x, ++idx)
						{
							//stage += std::format("Tile_{}, ", tileStrings[WORLD.stage[idx]]);
						}
						track += "\n";
					}
					track += "};\n";
					copy_to_clipboard(track);
				}
				ImGui::SameLine();
				if (ImGui::Button("Save"))
				{
					
				}
				ImGui::SameLine();
				if (ImGui::Button("Open..."))
				{
					
				}

				ImGui::NewLine();

				for (int s = 0; s < e.musicSheet.sections.size(); s++)
				{
					ImGui::Text("Section %d:\t", s);
					ImGui::SameLine();
					ImGui::Button("+");
					ImGui::SameLine();
					ImGui::Button("-");
					ImGui::NewLine();
				}
			}
			ImGui::EndChild();

			ImGui::EndTable();
		}
	}
	ImGui::End();

	// ImGui
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// End Frame
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(context);
	}
	glfwSwapBuffers(glfwGetCurrentContext());
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
	glfwPollEvents();
	window_draw();
}

GLuint program_create_fv(const char* const fragmentVertexShader[2])
{
	GLint   logLength = 0xFF, compiled;
	GLchar* log = (GLchar*)malloc(logLength);
	assert(log != NULL);
	memset(log, 0, logLength);
	GLuint  shader[2]{};
	GLuint  program = glCreateProgram();
	for (GLuint s = 0; s < (sizeof(shader) / sizeof(GLuint)); ++s)
	{
		shader[s] = glCreateShader(GL_FRAGMENT_SHADER + s);
		glShaderSource(shader[s], 1, fragmentVertexShader + s, NULL);
		glCompileShader(shader[s]);
		glGetShaderiv(shader[s], GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			int size;
			glGetShaderiv(shader[s], GL_INFO_LOG_LENGTH, &size);
			if (size > logLength)
			{
				logLength = size;
				free(log);
				log = (GLchar*)malloc(logLength + 1);
				assert(log != NULL);
				memset(log, 0, logLength);
			}
			glGetShaderInfoLog(shader[s], logLength, &logLength, log);
			printf("Failed compilation:\n%s\n", log);
			glDeleteProgram(program);
			program = 0;
			goto clean;
		}
		glAttachShader(program, shader[s]);
	}
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &compiled);
	if (!compiled)
	{
		int size;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &size);
		if (size > logLength)
		{
			logLength = size;
			free(log);
			log = (GLchar*)malloc(logLength);
			assert(log != NULL);
			memset(log, 0, logLength);
		}
		glGetProgramInfoLog(program, logLength, &logLength, log);
		printf("Failed linkage:\n%s\n", log);
		glDeleteProgram(program);
		program = 0;
	}
clean:
	for (GLuint s = 0; s < (sizeof(shader) / sizeof(GLuint)); ++s)
		glDeleteShader(shader[s]);
	free(log);
	glUseProgram(0);

	return program;
}


Error file_read(std::string file, std::string& content)
{
	FILE* stream = fopen(file.data(), "rb");
	if (!stream)
		return true;

	fseek(stream, 0, SEEK_END);
	long size = ftell(stream);
	rewind(stream);

	content.resize(size);
	size_t read = fread((void*)content.data(), sizeof(char), size, stream);
	fclose(stream);
	if (read != size)
	{
		printf("[Reader:] Failed to fully read the file.\n");
		return true;
	}
	return false;
}

static GLFWwindow* window_create(const char* title, int w, int h)
{
	// GLFW
	if (!glfwInit())
	{
		assert(false && "Could not load glfw.");
		return nullptr;
	}
	GLFWwindow* window = glfwCreateWindow(w, h, title, nullptr, nullptr);
	if (!window)
	{
		assert(false && "Could not create glfw window.");
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoaderLoadGL())
	{
		assert(false && "Could not load glad.");
		return nullptr;
	}
	glfwSwapInterval(0); // Enable vsync

	// ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard
		| ImGuiConfigFlags_DockingEnable
		| ImGuiConfigFlags_ViewportsEnable;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(SHADER_VERSION);
	ImGuiStyle& style = ImGui::GetStyle();
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.TabRounding = 5.f;
		style.FrameRounding = 5.f;
		style.FramePadding.x = 15.f;
		style.FramePadding.y = 9.5f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	style.PopupRounding = 5.f;
	style.ItemSpacing.x = 0.f;
	style.ItemSpacing.y = 9.f;

	// Corners
	style.WindowRounding = 8.0f;
	style.ChildRounding = 8.0f;
	style.FrameRounding = 6.0f;
	style.PopupRounding = 6.0f;
	style.ScrollbarRounding = 6.0f;
	style.GrabRounding = 6.0f;
	style.TabRounding = 6.0f;

	ImFont* defaultFont = io.Fonts->AddFontDefault();
	ImFontConfig config;
	config.MergeMode = false;
	config.GlyphMinAdvanceX = 13.0f; 
	static const ImWchar musical_symbol_ranges[] = { 0x1D100U, 0x1D1FFU, '\0', };
	
	e.font = io.Fonts->AddFontFromFileTTF(ASSETS_PATH "/fonts/bravura.ttf", 32.f, &config, musical_symbol_ranges);
	
	// Colors
	ImVec4* colors                         = style.Colors;
	colors[ImGuiCol_Text]                  = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
	colors[ImGuiCol_TextDisabled]          = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
	colors[ImGuiCol_WindowBg]              = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_ChildBg]               = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
	colors[ImGuiCol_PopupBg]               = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border]                = ImVec4(0.43f, 0.50f, 0.56f, 0.50f);
	colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg]               = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
	colors[ImGuiCol_FrameBgActive]         = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
	colors[ImGuiCol_TitleBg]               = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
	colors[ImGuiCol_TitleBgActive]         = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg]             = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
	colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark]             = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrab]            = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
	colors[ImGuiCol_Button]                = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_ButtonHovered]         = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colors[ImGuiCol_ButtonActive]          = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
	colors[ImGuiCol_Header]                = ImVec4(0.20f, 0.25f, 0.29f, 0.55f);
	colors[ImGuiCol_HeaderHovered]         = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_HeaderActive]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Separator]             = ImVec4(0.43f, 0.50f, 0.56f, 0.50f);
	colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
	colors[ImGuiCol_SeparatorActive]       = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_ResizeGrip]            = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_Tab]                   = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_TabHovered]            = ImVec4(0.28f, 0.56f, 1.00f, 0.80f);
	colors[ImGuiCol_TabActive]             = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_TabUnfocused]          = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.14f, 0.22f, 0.36f, 1.00f);
	colors[ImGuiCol_DockingPreview]        = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
	colors[ImGuiCol_DockingEmptyBg]        = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_PlotLines]             = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget]        = ImVec4(1.00f, 0.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);


	return window;
}

void target_texture_recreate(int width, int height, frame_t& frame)
{
	// In case of resize
	glDeleteFramebuffers(1, &frame.fbo);
	glDeleteTextures(1, &frame.tex);
	glDeleteRenderbuffers(1, &frame.rbo);
	// Gen framebuffer
	glGenFramebuffers(1, &frame.fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, frame.fbo);
	// Create texture 
	glGenTextures(1, &frame.tex);
	glBindTexture(GL_TEXTURE_2D, frame.tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frame.tex, 0);
	// Create render buffer
	glGenRenderbuffers(1, &frame.rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, frame.rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, frame.rbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		assert(false); // Oopsie...
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool centered_button(const char* label, const ImVec2& size)
{
	ImGuiStyle& style = ImGui::GetStyle();
	float off = (ImGui::GetContentRegionAvail().x - (ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f)) / 2;
	if (off > 0.f)
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
	return ImGui::Button(label, size);
}

void centered_text(const char* label)
{
	ImGuiStyle& style = ImGui::GetStyle();
	float off = (ImGui::GetContentRegionAvail().x - (ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f)) / 2;
	if (off > 0.f)
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
	return ImGui::Text(label);
}


void open_sprite()
{
	pfd::open_file file = pfd::open_file("Choose files to read", pfd::path::home(), { "All Files", "*" }, pfd::opt::none);
	std::vector<std::string> result = file.result();
	if (result.size())
	{
		std::ifstream stream(result.front());
		nlohmann::json json = nlohmann::json::parse(stream);
		std::string name = json["name"];
		int i;
		for (i = 0; name[i] != '\0'; i++)
			e.spriteName[i] = name[i];
		e.spriteName[i] = '\0';
		e.sprite.clear();
		std::vector<int> x = json["sprite_x"];
		std::vector<int> y = json["sprite_y"];

		for (size_t i = 0; i < x.size(); ++i)
		{
			e.sprite.emplace_back(x[i], y[i]);
		}
	}
}

void save_sprite()
{
	std::string def = pfd::path::home() + pfd::path::separator() + std::string(e.spriteName) + ".json";
	pfd::save_file f = pfd::save_file("Choose file to save", def, { "JSON Files (.json)", "*.json" });
	std::string result = f.result();
	if (result.size())
	{
		std::vector<int> x, y;
		for (size_t i = 0; i < e.sprite.size(); i++)
		{
			glm::ivec2 const& point = e.sprite[i];
			x.emplace_back(point.x);
			y.emplace_back(point.y);
		}

		nlohmann::json json;
		json["name"] = std::string(e.spriteName);
		json["sprite_x"] = x;
		json["sprite_y"] = y;

		std::ofstream stream(result);
		stream << json.dump(4);
	}
}

void open_stage()
{
	pfd::open_file file =  pfd::open_file("Choose files to read", pfd::path::home(), { "All Files", "*" }, pfd::opt::none);
	std::vector<std::string> result = file.result();
	if (result.size())
	{
		std::ifstream stream(result.front());
		nlohmann::json json = nlohmann::json::parse(stream);
		std::string name = json["name"];
		int i;
		for (i = 0; name[i] != '\0'; i++)
			e.stageName[i] = name[i];
		e.stageName[i] = '\0';
		WORLD.stage = json["stage"].get<std::vector<Tile>>();
	}
}

void save_stage()
{
	std::string def = pfd::path::home() + pfd::path::separator() + std::string(e.stageName) + ".json";
	pfd::save_file f = pfd::save_file("Choose file to save", def, { "JSON Files (.json)", "*.json" });
	std::string result = f.result();
	if (result.size())
	{
		nlohmann::json json;
		json["name"]  = std::string(e.stageName);
		json["stage"] = WORLD.stage;
		std::ofstream stream(result);
		stream << json.dump(4);
	}
}

void world_render_tile(Tile tile, i16 tileLeft, i16 tileRight, i16 tileTop, i16 tileBottom)
{
	switch (tile)
	{
	case Tile_Air:
		if (WORLD.sceneIsTarget)
		{
			idx_t idx = tile - Tile_E0;
			print_txt(tileLeft + (tileRight - tileLeft) / 4, tileBottom + (tileTop - tileBottom) / 4, WORLD.tileIdx, "Air", 0.35, 0.50);
		}
		break;
	case Tile_Water:
		if (WORLD.sceneIsTarget)
		{
			idx_t idx = tile - Tile_E0;
			print_txt(tileLeft + (tileRight - tileLeft) / 16, tileBottom + (tileTop - tileBottom) / 4, WORLD.tileIdx, "Water", 0.35, 0.50);
		}
		break;
	case Tile_Top2:
	case Tile_Top:
	{
		int startX, deltaX;
		{
			startX = tileLeft;
			deltaX = TILE_WIDTH;
		}
		beam_set_position(tileTop, startX);
		Draw_Line_d(0, deltaX);
	}
	break;
	case Tile_TopLeft:
		{
			int startX, deltaX;
			{
				startX = tileLeft;
				deltaX = TILE_WIDTH;
			}
			beam_set_position(I8(tileTop), startX);
			Draw_Line_d(0, deltaX);
		}
		{
			int startY, deltaY;
			{
				startY = tileTop;
				deltaY = -TILE_HEIGHT;
			}
			beam_set_position(startY, (tileLeft));
			Draw_Line_d(deltaY, 0);
		}
		break;
	case Tile_TopRight:
		{
			int startX, deltaX;
			{
				startX = tileLeft;
				deltaX = TILE_WIDTH;
			}
			beam_set_position((tileTop), startX);
			Draw_Line_d(0, deltaX);
		}

		{
			int startY, deltaY;
			{
				startY = (tileTop);
				deltaY = -TILE_HEIGHT;
			}
			beam_set_position(startY, (tileRight));
			Draw_Line_d(deltaY, 0);
		}
		break;
	case Tile_Bottom2:
	case Tile_Bottom:
		{
			int startX, deltaX;
			{
				startX = (tileLeft);
				deltaX = TILE_WIDTH;
			}
			beam_set_position((tileBottom), startX);
			Draw_Line_d(0, deltaX);
		}
		break;
	case Tile_BottomLeft:
	{
		int startX, deltaX;
		{
			startX = (tileLeft);
			deltaX = TILE_WIDTH;
		}
		beam_set_position((tileBottom), startX);
		Draw_Line_d(0, deltaX);
	}
	{
		int startY, deltaY;
		{
			startY = (tileTop);
			deltaY = -TILE_HEIGHT;
		}
		beam_set_position(startY, (tileLeft));
		Draw_Line_d(deltaY, 0);
	}
	break;
	case Tile_BottomRight:
	{
		int startX, deltaX;
		{
			startX = (tileLeft);
			deltaX = TILE_WIDTH;
		}
		beam_set_position((tileBottom), startX);
		Draw_Line_d(0, deltaX);
	}
	{
		int startY, deltaY;
		{
			startY = (tileTop);
			deltaY = -TILE_HEIGHT;
		}
		beam_set_position(startY, (tileRight));
		Draw_Line_d(deltaY, 0);
	}
	break;
	case Tile_Left2:
	case Tile_Left:
	{
		int startY, deltaY;
		{
			startY = (tileTop);
			deltaY = -TILE_HEIGHT;
		}
		beam_set_position(startY, (tileLeft));
		Draw_Line_d(deltaY, 0);
	}
	break;
	case Tile_Right2:
	case Tile_Right:
	{
		int startY, deltaY;
		{
			startY = (tileTop);
			deltaY = -TILE_HEIGHT;
		}
		beam_set_position(startY, (tileRight));
		Draw_Line_d(deltaY, 0);
	}
	break;
	case Tile_MiddleLeftTop:
	{
		int startX, deltaX;
		{
			startX = (tileLeft);
			deltaX = (TILE_WIDTH >> 1);
		}
		beam_set_position((tileTop), startX);
		Draw_Line_d(0, deltaX);
	}
	break;
	case Tile_MiddleLeft:
	{
		int startX, deltaX;
		{
			startX = (tileLeft);
			deltaX = (TILE_WIDTH >> 1);
		}
		beam_set_position((tileTop - (TILE_HEIGHT >> 1)), startX);
		Draw_Line_d(0, deltaX);
	}
	break;
	case Tile_MiddleRight:
	{
		int startX, deltaX;
		{
			startX = (tileLeft + (TILE_WIDTH >> 1));
			deltaX = (TILE_WIDTH >> 1);
		}
		beam_set_position((tileTop - (TILE_HEIGHT >> 1)), startX);
		Draw_Line_d(0, deltaX);
	}
	break;
	case Tile_Middle:
	{
		int startX, deltaX;
		{
			startX = (tileLeft + (TILE_WIDTH >> 2));
			deltaX = (TILE_WIDTH >> 1);
		}
		beam_set_position((tileTop - (TILE_HEIGHT >> 1)), startX);
		Draw_Line_d(0, deltaX);
	}
	break;
	case Tile_MiddleBottom:
	{
		int startX, deltaX;
		{
			startX = (tileLeft + (TILE_WIDTH >> 2));
			deltaX = (TILE_WIDTH >> 1);
		}
		beam_set_position((int)tileBottom, startX);
		Draw_Line_d(0, deltaX);
	}
	break;
	case Tile_MiddleRightTop:
	{
		int startX, deltaX;
		{
			startX = (tileLeft + (TILE_WIDTH >> 1));
			deltaX = (TILE_WIDTH >> 1);
		}
		beam_set_position((int)tileTop, startX);
		Draw_Line_d(0, deltaX);
	}
	break;
	case Tile_MiddleTop:
	{
		int startX, deltaX;
		{
			startX = (tileLeft + (TILE_WIDTH >> 2));
			deltaX = (TILE_WIDTH >> 1);
		}
		beam_set_position((int)tileTop, startX);
		Draw_Line_d(0, deltaX);
	}
	break;
	case Tile_Spikes:
	{
		beam_set_position(tileBottom, tileLeft);
		Draw_VLc((void* const)spikes);
	}

	break;
	case Tile_SpikedBall:
	{
		beam_set_position((tileTop - (TILE_HEIGHT >> 1)), (tileLeft + (TILE_HEIGHT >> 2)));
		Draw_VLc((void* const)spikedBall);
	}
	break;
	case Tile_Jumper:
	{
		beam_set_position((tileBottom), (tileLeft + (TILE_WIDTH >> 2)));
		Draw_VLc((void* const)jumper);
	}
	break;
	case Tile_BarrierVertical:
	{
		beam_set_position((tileTop), (tileRight));
		Draw_VLc((void* const)barrierVertical);
	}
	break;
	case Tile_BarrierHorizontal:
	{
		beam_set_position((tileBottom), (tileRight));
		Draw_VLc((void* const)barrierHorizontal);
	}
	break;
	case Tile_WaterTop:
	{
		beam_set_position((tileTop), (tileLeft));
		Draw_VLc((void* const)watertop[(WORLD.ticks >> 3) & 0x7]);
	}
	break;
	case Tile_E0:
	case Tile_E1:
	case Tile_E2:
	case Tile_E3:
	case Tile_E4:
	case Tile_E5:
	case Tile_E6:
	case Tile_E7:
	case Tile_E8:
	case Tile_E9:
	case Tile_E10:
	case Tile_E11:
	case Tile_E12:
	case Tile_E13:
	case Tile_E14:
	case Tile_E15:
	{
		if (WORLD.sceneIsTarget)
		{
			idx_t idx = tile - Tile_E0;
			std::string txt = std::format("E{}", idx);
			print_txt(tileLeft + (tileRight - tileLeft) / 4, tileBottom + (tileTop - tileBottom) / 4, WORLD.tileIdx, txt, 0.35, 0.50);
		}
	}
	break;
	case Tile_Portal0:
	case Tile_Portal1:
	{
		int tick = (WORLD.ticks >> 3) & 0x7;
		const i8* p = portal[tick];
		int tileCenterY = tileTop - (TILE_HEIGHT >> 1) + p[1];
		int tileCenterX = tileLeft + (TILE_WIDTH >> 1) + p[2];
		
		{
			beam_set_position((tileCenterY), (tileCenterX));
			Draw_VLc((void* const)p);
		}
	}
	break;
	case Tile_GravitasUp:
		beam_set_position(tileBottom, tileLeft + (TILE_WIDTH >> 1));
		Draw_VLc((void* const)gravitasUp);
		break;
	case Tile_SpikesDown:
		beam_set_position(tileTop, tileLeft);
		Draw_VLc((void* const)spikesDown);
		break;
	case Tile_Warning:
		beam_set_position(tileTop, tileLeft);
		Draw_VLc((void* const)warning);
		break;
	case Tile_NA:
		if (WORLD.sceneIsTarget)
		{
			print_txt(tileLeft + (tileRight - tileLeft) / 4, tileBottom + (tileTop - tileBottom) / 4, WORLD.tileIdx, "N/A", 0.35, 0.50);
		}
		break;
	case Tile_GravitasDown:
		beam_set_position(tileTop, tileLeft + (TILE_WIDTH >> 1));
		Draw_VLc((void* const)gravitasDown);
		break;
	default:
		break;
	}
}

void print_txt(float x, float y, idx_t id, std::string txt, float scaleX, float scaleY)
{
	auto& kv = e.txts[id];
	kv.tick = WORLD.ticks;
	if (kv.txt != txt)
	{
		kv.txt = txt;
		glDeleteVertexArrays(1, &kv.object.vao);
		glDeleteBuffers(2, &kv.object.vbo);

		struct vertex_t
		{
			glm::vec3 pos;
			glm::vec2 uv;
		};
		struct quad_t
		{
			vertex_t v0;
			vertex_t v1;
			vertex_t v2;
			vertex_t v3;
		};

		struct quad_elements_t
		{
			GLuint elements[6];
		};
		
		std::vector<quad_t> quads(txt.size());
		std::vector<quad_elements_t> elements(txt.size());

		uint32_t qi = 0, ii = 0;
		f32 xOff = 0, yOff = 0;
		for (char const& c : txt)
		{
			stbtt_aligned_quad quad;

			stbtt_GetPackedQuad(e.atlasChars.data(), e.atlasDim.x, e.atlasDim.y, c - ' ', &xOff, &yOff, &quad, 1);
			auto xmin = quad.x0 * scaleX;
			auto xmax = quad.x1 * scaleX;
			auto ymin = -quad.y1 * scaleY;
			auto ymax = -quad.y0 * scaleY;

			quads[qi] =
			{
				.v0 = { glm::vec3{xmin + x, ymin + y, 0}, glm::vec2{ quad.s0, quad.t1 } },
				.v1 = { glm::vec3{xmin + x, ymax + y, 0}, glm::vec2{ quad.s0, quad.t0 } },
				.v2 = { glm::vec3{xmax + x, ymax + y, 0}, glm::vec2{ quad.s1, quad.t0 } },
				.v3 = { glm::vec3{xmax + x, ymin + y, 0}, glm::vec2{ quad.s1, quad.t1 } },
			};
			elements[qi] =
			{
				{ii, ii + 1, ii + 2, ii, ii + 2, ii + 3},
			};
			++qi;
			ii += 4;
		}
		// Create quad
		const GLfloat quadVertices[] =
		{
			0.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f,
			0.0f,  1.0f, 0.0f,
			0.0f,  1.0f, 1.0f,
			1.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 1.0f,
		};
		glGenBuffers(1, &kv.object.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, kv.object.vbo);
		glBufferData(GL_ARRAY_BUFFER, quads.size() * sizeof(quad_t), quads.data(), GL_STATIC_DRAW);
		glGenBuffers(1, &kv.object.ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, kv.object.ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(quad_elements_t), elements.data(), GL_STATIC_DRAW);
		// Create vao
		glGenVertexArrays(1, &kv.object.vao);
		glBindVertexArray(kv.object.vao);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, 5 * sizeof(float), nullptr);
		glEnableVertexAttribArray(1);
		size_t off = 3 * sizeof(float);
		glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(float), (const void*)off);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}

void clean_up_unused_txts()
{
	for (auto& kv : e.txts)
	{
		if (kv.second.tick != WORLD.ticks)
		{
			glDeleteVertexArrays(1, &kv.second.object.vao);
			glDeleteBuffers(2, &kv.second.object.vbo);
			kv.second = {};
		}
	}
}

void world_progress(void)
{
	++WORLD.ticks;
	WORLD.freq2  = WORLD.ticks & 1;
	WORLD.freq16 = (WORLD.ticks >> 4) & 1;

	// int y = I8(CAMERA.position.y >> TILE_SCALE_BITS);
	// int x = I8(CAMERA.position.x >> TILE_SCALE_BITS);

	// i16 right  = CAMERA.position.x + 127;
	// i16 top    = CAMERA.position.y + 127;


	WORLD.selectedTile.x = 0;
	WORLD.selectedTile.y = 0;

	int xMax = WORLD_STRIDE;
	int yMax = WORLD_EXTENT;

	int idxStride = xMax - WORLD.selectedTile.x;
	int idxDelta = WORLD_STRIDE - idxStride + 1;
	i16 tileIdx = WORLD.selectedTile.y * I16(WORLD_STRIDE) + WORLD.selectedTile.x;
	// i16 tileIdxMax = yMax * WORLD_WIDTH + xMax;
	Intensity_5F();
	while (true)
	{
		i16 tileLeft  = (I16(WORLD.selectedTile.x) << TILE_SCALE_BITS);
		i16 tileRight = tileLeft + TILE_WIDTH;
		i16 tileTop   = WORLD_EXTENT * TILE_HEIGHT - (I16(WORLD.selectedTile.y) << TILE_SCALE_BITS); // This one inverts the y axis
		i16 tileBottom = tileTop - TILE_HEIGHT;

		// monitor("TILE IDX", tileIdx);
		// if (tileLeft >= I16(-128) && tileRight <= I16(127) && tileTop <= I16(127) && tileBottom >= I16(-128))
		// bool clip       = !(tileRight <= left || tileLeft >= right || tileBottom <= top || tileTop >= bottom);
		// if (tileLeft >= -128 && tileLeft <= 127 && tileTop <= 127 && tileTop >= -128)
		{
			// i16 offsetX = tileLeft - left;
			// i16 offsetY = tileTop  - top;
			const Tile tile = WORLD.stage[tileIdx];
			WORLD.tileIdx = tileIdx;
			world_render_tile(tile, tileLeft, tileRight, tileTop, tileBottom);
		}

		if (++WORLD.selectedTile.x < xMax)
		{
			++tileIdx;
		}
		else
		{
			if (++WORLD.selectedTile.y < yMax)
			{
				tileIdx += idxDelta;
				WORLD.selectedTile.x -= idxStride; // Reset to start of row
			}
			else // End of row
			{
				break;
			}
		}
	}
	clean_up_unused_txts();
}


void beam_set_position(i32 y, i32 x)
{
	WORLD.beam.x = x;
	WORLD.beam.y = y;
}
void Draw_Line_d(int y, int x)
{
	glm::vec2 begin = glm::vec2(WORLD.beam);
	glm::ivec2 endI = { WORLD.beam.x + x, WORLD.beam.y + y };
	glm::vec2 end = glm::vec2(endI);
	if (WORLD.sceneIsTarget)
	{
		e.lines.emplace_back(line_t{ begin / glm::vec2(TILE_WIDTH, TILE_HEIGHT), end / glm::vec2(TILE_WIDTH, TILE_HEIGHT) });
	}
	else
	{
		glm::vec2 p = ImGui::GetItemRectMin();
		glm::vec2 q = ImGui::GetItemRectMax();
		//q.x -= 5.f;
		glm::vec2 itemSize = (glm::vec2(q) - glm::vec2(p));

		glm::vec2 scale = 0.5f * glm::vec2(itemSize.y, -itemSize.y) / glm::vec2(TILE_WIDTH, TILE_HEIGHT);

		glm::vec2 anchor = p + glm::vec2(itemSize.x - 96.0f * scale.x, itemSize.y + 80.0f * scale.y);

		glm::vec2 begin_ = anchor + begin * scale;
		glm::vec2 end_ = anchor + end * scale;

		ImGui::GetWindowDrawList()->AddLine(begin_, end_, 0xFFFFFFFF, 1.0f);
	}
	WORLD.beam = endI;
}
void Draw_VLc(void const* x)
{
	struct list_t
	{
		i8 length;
		glm::i8vec2 data[];
	};
	list_t const* list = (list_t const*)x;
	for (size_t i = 0; i <= list->length; i++)
	{
		glm::i8vec2 v = list->data[i];
		Draw_Line_d(v.x, v.y);
	}
}
void Intensity_5F()
{
}

void copy_to_clipboard(std::string const& text)
{
	HGLOBAL mem = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
	memcpy(GlobalLock(mem), text.c_str(), text.size() + 1);
	GlobalUnlock(mem);
	OpenClipboard(0);
	EmptyClipboard();
	SetClipboardData(CF_TEXT, mem);
	CloseClipboard();
}

void load_font()
{
	//std::ifstream(font, std::ios::binary | std::ios::ate).tellg();
	
	std::string fontBytes;
	assert(!file_read(ASSETS_PATH "/fonts/AncizarSans-Regular.ttf", fontBytes));

	std::vector<uint8_t> atlasBytes;
	atlasBytes.resize(e.atlasDim.x * e.atlasDim.y, 0);
	
	stbtt_pack_context context;
	assert(stbtt_PackBegin(&context, atlasBytes.data(), e.atlasDim.x, e.atlasDim.y, 0, 1, nullptr));
		
	int chars = '~' - ' ';
	e.atlasChars.resize(chars);
	stbtt_PackSetOversampling(&context, 2, 2);
	assert(stbtt_PackFontRange(&context, (const unsigned char*)fontBytes.data(), 0, 80, ' ', chars, e.atlasChars.data()));

	stbtt_PackEnd(&context);

	glGenTextures(1, &e.atlas.tex);
	glBindTexture(GL_TEXTURE_2D, e.atlas.tex);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, e.atlasDim.x, e.atlasDim.y, 0, GL_RED, GL_UNSIGNED_BYTE, atlasBytes.data());
	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
	glGenerateMipmap(GL_TEXTURE_2D);

	std::string fontFrag, fontVert;
	assert(!file_read(SHADERS_PATH "/font.frag", fontFrag) & !file_read(SHADERS_PATH "/font.vert", fontVert));
	const char* shaders[2] =
	{
		fontFrag.c_str(),
		fontVert.c_str(),
	};
	e.atlas.program =  program_create_fv(shaders);
	assert(e.atlas.program != 0);
	glUseProgram(e.atlas.program);
	int loc = glGetUniformLocation(e.atlas.program, "atlas");
	assert(loc >= 0);
	glUniform1i(loc, 0);
	glUseProgram(0);
}

GLuint texture_read(const char* file)
{
	FILE* f = fopen(file, "rb");
	if (f == NULL)
		return false;
	fseek(f, 0, SEEK_END);
	size_t file_size = (size_t)ftell(f);
	if (file_size == -1)
		return false;
	fseek(f, 0, SEEK_SET);
	void* file_data = IM_ALLOC(file_size);
	fread(file_data, 1, file_size, f);
	fclose(f);

	int x, y;
	stbi_uc* img = stbi_load_from_memory((const unsigned char*)file_data, file_size, &x, &y, NULL, 4);
	assert(img != nullptr);

	GLuint tex = 0;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(img);
	IM_FREE(file_data);
	return tex;
}