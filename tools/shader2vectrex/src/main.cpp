/**
* This was an attempt to turn fragment shaders from https://www.shadertoy.com/ in to few lines or points 
* to save me the annoyance of typing in animations into an array. Unfortunately, this is a very stupid idea.
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

/////////////////////////////////////////////////////////////////////////
//	Includes
//
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#define GLAD_GL_IMPLEMENTATION
#include <gl.h>
#include <GLFW/glfw3.h>

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <cassert>
#include <string>
#include <chrono>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <unordered_set>


/////////////////////////////////////////////////////////////////////////
//	Types
//
typedef int Error;
struct frame_t
{
    GLuint fbo = 0, tex = 0, rbo = 0;
};

enum Edge_ { Edge_Top, Edge_Right, Edge_Bottom, Edge_Left };
typedef int Edge;

struct v2i
{
    int x;
    int y;
};

struct v2f
{
    float x;
    float y;

    bool operator==(const v2f& other) const {
        return std::abs(x - other.x) < 1e-1f && std::abs(y - other.y) < 1e-1f;
    }
};


typedef struct vertex_list_t
{
    int y, x;
    int vector;
    v2i data[];
} vertex_list_t, *vertex_list;

struct line_t
{
    v2f start;
    v2f stop;
};
struct rgb_t
{
    uint8_t r, g, b;
};

struct object_t
{
    GLuint vbo, ebo, vao;
};

struct alignas(16) uniforms_t // Align with 16 because of ubo
{
    float iResolution[3]; // viewport resolution (in pixels)
    float iTime;          // shader playback time (in seconds)
    float iTimeDelta;     // render time (in seconds)
    float iFrameRate;     // shader frame rate
    int   iFrame;         // shader playback frame
};

/////////////////////////////////////////////////////////////////////////
//	Forward Declerations
//
GLuint program_create_fv(const char* const fragmentVertexShader[2]);
Error  file_read(std::string file, std::string& content);
void   texture_create(int width, int height, GLuint& colorTexture);
void   target_texture_create(int width, int height, frame_t& frame);
void   marching_squares(std::vector<rgb_t> const& pixels, int width, int height, float threshold, std::vector<v2f>& out);
void   rdp(const std::vector<v2f>& points, float epsilon, std::vector<v2f>& out);
void   simplify(std::vector<v2f> const& lines, float epsilon, std::vector<v2f>& out);
void   simplify_pixel_grid(std::vector<v2f> const& points, int width, int height, float downscale,  std::vector<v2f>& out);
static GLFWwindow* window_create(const char* title, int w, int h);

int main(int argc, const char* argv[]) 
{
    if (false)
    if (argc <= 1)
    {
        printf("./shader2vertex.exe fragshader\n");
        return 0;
    }
    
    const char* file = "portal.frag"; // argv[1];
    int viewportX = 0, viewportY = 0, viewportW = 64, viewportH = 64;

    std::string content;
    if (file_read(file, content))
    {
        printf("Could not read file.\n");
        return 0;
    }

    std::string preShader = SHADER_VERSION CODE(
        layout(std140, binding = 0) uniform Stuff
        {
            vec3  iResolution; // viewport resolution (in pixels)
            float iTime;       // shader playback time (in seconds)
            float iTimeDelta;  // render time (in seconds)
            float iFrameRate;  // shader frame rate
            int   iFrame;      // shader playback frame
        };) "\n";
    std::string postShader = "\n" CODE(
        out vec4 color;
        in  vec2 coordinate;
        void main()
        {
            mainImage(color, gl_FragCoord.xy);
        }
    );

    content = preShader + content;
    content += postShader;

    std::string quadVert, renderFrag, filterFrag, skeletonizeFrag, lineFrag, lineVert;
    file_read("quad.vert", quadVert);
    file_read("render.frag", renderFrag);
    file_read("filter.frag", filterFrag);
    file_read("skeletonize.frag", skeletonizeFrag);
    file_read("lines.frag", lineFrag);
    file_read("lines.vert", lineVert);

    const char* shaders[] =
    {
        content.c_str(),
        quadVert.c_str(),
        renderFrag.c_str(),
        quadVert.c_str(),
        filterFrag.c_str(),
        quadVert.c_str(),
        skeletonizeFrag.c_str(),
        quadVert.c_str(),
        lineFrag.c_str(),
        lineVert.c_str(),
    };

    int w = 640 << 1, h = 640;
    GLFWwindow* win = window_create("Shader2Vectrex", w, h);
    if (!win)
    {
        glfwTerminate();
        return 0;
    }

    int major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    glDisable(GL_CULL_FACE);

    GLuint program = 0, displayProgram = 0, filterProgram = 0, skeletonizeProgram = 0, linesProgram = 0;
    object_t quad{}, linesMesh;
    GLint sobleThresholdLocation, skeletonizeThresholdLocation;

    frame_t ping, pong;

    float simplifyEps = 1.5;

    GLuint ubo = 0;
    GLint renderImageLocation, filterImageLocation, skeletonizeImageLocation;
    // OGL stuff
    {
        // Create passed program
        program        = program_create_fv(shaders);
        if (!program)
        {
            glfwTerminate();
            return 0;
        }

        glGenBuffers(1, &ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(uniforms_t), NULL, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, 0, sizeof(uniforms_t));


        // Create display program
        displayProgram = program_create_fv(shaders + 2);
        assert(displayProgram != 0);
        renderImageLocation = glGetUniformLocation(displayProgram, "image");

        filterProgram = program_create_fv(shaders + 4);
        assert(filterProgram != 0);
        filterImageLocation = glGetUniformLocation(filterProgram, "image");
        sobleThresholdLocation = glGetUniformLocation(filterProgram, "threshold");

        skeletonizeProgram = program_create_fv(shaders + 6);
        assert(skeletonizeProgram != 0);
        skeletonizeImageLocation = glGetUniformLocation(skeletonizeProgram, "image");
        skeletonizeThresholdLocation = glGetUniformLocation(skeletonizeProgram, "threshold");

        linesProgram = program_create_fv(shaders + 8);
        assert(linesProgram != 0);

        glEnable(GL_PROGRAM_POINT_SIZE);

        // Create texture to render to 
        target_texture_create(viewportW, viewportH, ping);
        target_texture_create(viewportW, viewportH, pong);

        // Create quad
        const GLfloat quadVertices[] = 
        {
            -1.0f, -1.0f, 0.0f, 0.0f, // 2 floats pos, 2 floats tex coord
             1.0f, -1.0f, 1.0f, 0.0f,
            -1.0f,  1.0f, 0.0f, 1.0f,
             1.0f,  1.0f, 1.0f, 1.0f,
        };
        const GLuint quadIdxs[] = { 0, 1, 2, 2, 1, 3, };
        glGenBuffers(1, &quad.vbo);
        glBindBuffer(GL_ARRAY_BUFFER, quad.vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
        glGenBuffers(1, &quad.ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIdxs), quadIdxs, GL_STATIC_DRAW);
        // Create vao
        glGenVertexArrays(1, &quad.vao);
        glBindVertexArray(quad.vao);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, false, 4 * sizeof(float), nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, false, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    int x = 300, y = 300;
    uniforms_t  uniforms{};
   
    typedef std::chrono::high_resolution_clock clock_t;
    double start = glfwGetTime();
    double last  = start;
    float sobleThreshold = 3.0, skeletonizeThreshold = 0.05;
    int marchingThreshold = 1, marchingBinaryThreshold = 255;
    float sampleTime = 1.0f / 50.f;
    bool points = false;
    
    std::vector<rgb_t> pixels(viewportW * viewportH);
    std::vector<v2f> lines;
    lines.reserve(0xFF);

    std::string buf(40000, '\0');

    while (!glfwWindowShouldClose(win))
    {
        glfwPollEvents();
        if (glfwGetWindowAttrib(win, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        glfwGetWindowSize(win, &w, &h);

        // Start Frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (ImGui::IsKeyPressed(ImGuiKey_Q))
            glfwSetWindowShouldClose(win, true);

        if (ImGui::Begin("Settings"))
        {

            ImGui::InputTextMultiline("Shader", (char*)buf.data(), buf.size());
            if (ImGui::Button("Compile"))
            {
                glDeleteProgram(program);

                buf = preShader + buf;
                buf.insert(buf.begin() + (buf.find_first_of('\0')), postShader.begin(), postShader.end());

                const char* newShaders[] =
                {
                    buf.c_str(),
                    quadVert.c_str(),
                };
                program = program_create_fv(newShaders);
                buf.clear();
            }


            if (ImGui::SliderFloat("Skeletonize Threshold", &sobleThreshold, 0.f, 5.f))
            {
                glUseProgram(filterProgram);
                glUniform1f(sobleThresholdLocation, sobleThreshold);
                glUseProgram(0);
            }
            if (ImGui::SliderFloat("Soble Threshold", &skeletonizeThreshold, 0.f, 5.0f))
            {
                glUseProgram(skeletonizeProgram);
                glUniform1f(skeletonizeThresholdLocation, skeletonizeThreshold);
                glUseProgram(0);
            }
            if (ImGui::SliderInt("Marching Squares Threshold", &marchingThreshold, 0, 255))
            {

            }
            if (ImGui::SliderInt("Marching Squares Binary Threshold", &marchingBinaryThreshold, 1, 255))
            {
            }
            if (ImGui::SliderFloat("Sample Time (s)", &sampleTime, 1.f / 50.f, 0.5f))
            {

            }

            if (ImGui::Checkbox("Points", &points))
            {

            }
            
            ImGui::SliderFloat("Simplify Epsilon", &simplifyEps, 0.01, 1000);
            ImGui::Text("%llu", lines.size());
        }
        ImGui::End();

        // Update uniforms
        double now              = glfwGetTime();
        uniforms.iResolution[0] = viewportW;
        uniforms.iResolution[1] = viewportH;
        uniforms.iResolution[2] = 1;
        uniforms.iTime          = last - start;
        uniforms.iTimeDelta     = now - last;
        uniforms.iFrameRate     = 1.0f / uniforms.iTimeDelta;
        uniforms.iFrame++;
        static bool firstRun = true;
        if (firstRun || uniforms.iTimeDelta >= sampleTime)
        {
            firstRun = false;
            last = now;
            glBindBuffer(GL_UNIFORM_BUFFER, ubo);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(uniforms), &uniforms);

            // Do ping pong
            // Do inserted fragment shader
            {
                glBindFramebuffer(GL_FRAMEBUFFER, ping.fbo);
                glViewport(0, 0, viewportW, viewportH);
                glClear(GL_COLOR_BUFFER_BIT);
                glClearColor(0.0, 0.0, 0.0, 1.0);
                glUseProgram(program);
                glBindVertexArray(quad.vao);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad.ebo);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }
            // Do filtering
            {
                glBindFramebuffer(GL_FRAMEBUFFER, pong.fbo);
                glViewport(0, 0, viewportW, viewportH);
                glClear(GL_COLOR_BUFFER_BIT);
                glClearColor(0.0, 0.0, 0.0, 1.0);
                glUseProgram(filterProgram);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, ping.tex);
                glUniform1i(filterImageLocation, 0);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }
            // Do skeletonize
            {
                glBindFramebuffer(GL_FRAMEBUFFER, ping.fbo);
                glViewport(0, 0, viewportW, viewportH);
                glClear(GL_COLOR_BUFFER_BIT);
                glClearColor(0.0, 0.0, 0.0, 1.0);
                glUseProgram(skeletonizeProgram);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, pong.tex);
                glUniform1i(skeletonizeImageLocation, 0);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }
            // Do marching squares and simplify
            {
                glReadPixels(0, 0, viewportW, viewportH, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
                for (rgb_t& rgb : pixels)
                {
                    if (rgb.r >= marchingBinaryThreshold)
                        rgb.r = 255;
                }

                marching_squares(pixels, viewportW, viewportH, marchingThreshold, lines);

                std::vector<v2f> simplified;
                simplified.reserve(lines.capacity());
                simplify(lines, simplifyEps, simplified);
                lines = std::move(simplified);

                //rdp(lines, 1, lines);

                vertex_list list = (vertex_list)malloc(sizeof(vertex_list_t) + simplified.size() * sizeof(v2i));

                free(list);

                glDeleteBuffers(1, &linesMesh.vbo);
                glDeleteVertexArrays(1, &linesMesh.vao);

                glGenBuffers(1, &linesMesh.vbo);
                glBindBuffer(GL_ARRAY_BUFFER, linesMesh.vbo);
                glBufferData(GL_ARRAY_BUFFER, lines.size() * sizeof(v2f), lines.data(), GL_DYNAMIC_DRAW);

                glGenVertexArrays(1, &linesMesh.vao);
                glBindVertexArray(linesMesh.vao);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(float), nullptr);
            }
        }
        
        // Draw final Image
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glClear(GL_COLOR_BUFFER_BIT);
            glViewport(0, 0, w / 2, h);
            glUseProgram(displayProgram);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, ping.tex);
            glUniform1i(renderImageLocation, 0);
            glBindVertexArray(quad.vao);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad.ebo);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glUseProgram(0);
            glBindVertexArray(0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

        // Draw final Mesh
        {
            glViewport(w / 2, 0, w / 2, h);
            glUseProgram(linesProgram);
            glBindVertexArray(linesMesh.vao);
            glDrawArrays(points ? GL_POINTS : GL_LINES, 0, lines.size() * 2);
        }

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
        glfwSwapBuffers(win);
    }

    // Im too lazy to clean up opengl stuff
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(win);
    glfwTerminate();
    return 0;
}

GLuint program_create_fv(const char* const fragmentVertexShader[2])
{
    GLint   logLength = 0, compiled;
    GLchar* log = (GLchar*)malloc(0xFF);
    size_t  logSize = 0xFF;
    memset(log, 0, logSize);
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
            if (size > logSize)
            {
                logSize = size;
                free(log);
                log = (GLchar*)malloc(logSize + 1);
                memset(log, 0, logSize);
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
        if (size > logSize)
        {
            logSize = size;
            free(log);
            log = (GLchar*)malloc(logSize);
            memset(log, 0, logSize);
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
    return program;
}

void texture_create(int width, int height, GLuint& colorTexture)
{
    glGenTextures(1, &colorTexture);
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void target_texture_create(int width, int height, frame_t& frame)
{
    // In case of resize
    glDeleteFramebuffers(1, &frame.fbo);
    glDeleteTextures(1, &frame.tex);
    glDeleteRenderbuffers(1, &frame.rbo);

    // Gen framebuffer
    glGenFramebuffers(1, &frame.fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, frame.fbo);

    // Create texture 
    texture_create(width, height, frame.tex);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frame.tex, 0);

    // Create render buffer
    /*glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);*/

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        assert(false); // Oopsie...

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Error file_read(std::string file, std::string& content)
{
    file = SHADERS_PATH "/" + file;
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

constexpr static std::pair<Edge, Edge> g_lines[] =
{
    {Edge_Left, Edge_Bottom},
    {Edge_Bottom, Edge_Right},
    {Edge_Left, Edge_Right},
    {Edge_Top, Edge_Right},
    {Edge_Top, Edge_Left},
    {Edge_Top, Edge_Bottom},
    {Edge_Left, Edge_Top},
    {Edge_Bottom, Edge_Top},
    {Edge_Right, Edge_Left},
    {Edge_Right, Edge_Bottom}
};

constexpr static int g_lineLUT[16][2] =
{
    { -1, -1 },
    {  0, -1 },
    {  1, -1 },
    {  2, -1 },
    {  3, -1 },
    {  4,  1 },
    {  5, -1 },
    {  4, -1 },
    {  6, -1 },
    {  7, -1 },
    {  1,  6 },
    {  1, -1 },
    {  8, -1 },
    {  9, -1 },
    {  0, -1 },
    { -1, -1 },
};

v2f interpolate(v2f p1, v2f p2, float v1, float v2, float threshold) 
{
    float t = (threshold - v1) / (v2 - v1);
    return 
    {
       (p1.x + t * (p2.x - p1.x)),
       (p1.y + t * (p2.y - p1.y)),
    };
};
v2f interpolateEdge(float x, float y, Edge edge, float tl, float tr, float br, float bl, float threshold)
{
    switch (edge) 
    {
    case Edge_Top:    return interpolate({ x, y + 1 }, { x + 1, y + 1 }, tl, tr, threshold);
    case Edge_Right:  return interpolate({ x + 1, y + 1 }, { x + 1, y }, tr, br, threshold);
    case Edge_Bottom: return interpolate({ x + 1, y }, { x, y }, br, bl, threshold);
    case Edge_Left:   return interpolate({ x, y }, { x, y + 1 }, bl, tl, threshold);
    }
}

void marching_squares(std::vector<rgb_t> const& pixels, int width, int height, float threshold, std::vector<v2f>& out)
{
    out.clear();
    for (size_t y = 0; y < height - 1; y++)
    {
        for (size_t x = 0; x < width - 1; x++)
        {
            float tl = pixels[(y + 1) * width + x].r;
            float tr = pixels[(y + 1) * width + (x + 1)].r;
            float br = pixels[y * width + (x + 1)].r;
            float bl = pixels[y * width + x].r;

            int idx = (tl > threshold) * 8 |
                (tr > threshold) * 4 |
                (br > threshold) * 2 |
                (bl > threshold) * 1;
            const int* li = g_lineLUT[idx];
            for (int i = 0; i < 2; i++, li++)
            {
                if (*li >= 0)
                {
                    std::pair<Edge, Edge> pair = g_lines[*li];
                    v2f p1 = interpolateEdge(x, y, pair.first, tl, tr, br, bl, threshold);
                    v2f p2 = interpolateEdge(x, y, pair.second, tl, tr, br, bl, threshold);
                    out.emplace_back(p1);
                    out.emplace_back(p2);
                }
            }
        }
    }
}


void simplify(std::vector<v2f> const& lines, float epsilon, std::vector<v2f>& out)
{
    struct hash1_t
    {
        std::size_t operator()(const v2f& p) const 
        {
            return std::hash<int>{}(std::round(p.x * 10000.0f)) ^ (std::hash<int>{}(std::round(p.y * 10000.0f)) << 1);
        }
    };
    struct hash2_t 
    {
        std::size_t operator()(const std::pair<v2f, v2f>& segment) const 
        {
            return hash1_t{}(segment.first) ^ hash1_t {}(segment.second);
        }
    };

    std::unordered_map<v2f, std::vector<v2f>, hash1_t> adjacency;
    line_t const* l = (line_t const*)lines.data();
    for (size_t i = 0; i < (lines.size() >> 1); ++i, ++l)
    {
        adjacency[l->start].emplace_back(l->stop);
        adjacency[l->stop].emplace_back(l->start);
    }

    std::vector<std::vector<v2f>> polylines;
    std::unordered_set<std::pair<v2f, v2f>, hash2_t> visited;

    for (auto& entry : adjacency) 
    {
        const v2f& start = entry.first;

        for (const v2f& neighbor : entry.second) 
        {
            auto seg = std::make_pair(start, neighbor);
            if (visited.count(seg)) continue;

            std::vector<v2f> polyline = { start, neighbor };
            visited.insert(seg);
            visited.insert({ neighbor, start });

            v2f current = neighbor;
            v2f previous = start;

            while (true) 
            {
                const auto& nexts = adjacency[current];
                v2f next = {};
                bool found = false;

                for (const auto& candidate : nexts) 
                {
                    if (candidate.x == previous.x && candidate.y == previous.y) 
                        continue;
                    if (!visited.count({ current, candidate })) 
                    {
                        next = candidate;
                        found = true;
                        break;
                    }
                }

                if (!found) 
                    break;

                polyline.push_back(next);
                visited.insert({ current, next });
                visited.insert({ next, current });

                previous = current;
                current = next;
            }

            polylines.push_back(polyline);
        }
    }

    for (auto& line : polylines) 
    {
        std::vector<v2f> simplified;
        rdp(line, epsilon, simplified);
        out.insert(out.end(), simplified.begin(), simplified.end());
        if (out.size() & 1)
            out.pop_back();
    }
}

float perpendicular_distance(v2f point, v2f lineStart, v2f lineEnd) 
{
    float dx = lineEnd.x - lineStart.x;
    float dy = lineEnd.y - lineStart.y;
    if (dx == 0 && dy == 0) return std::hypot(point.x - lineStart.x, point.y - lineStart.y);
    float t = ((point.x - lineStart.x) * dx + (point.y - lineStart.y) * dy) / (dx * dx + dy * dy);
    v2f projection = { lineStart.x + t * dx, lineStart.y + t * dy };
    return std::hypot(point.x - projection.x, point.y - projection.y);
}

void rdp(const std::vector<v2f>& points, float epsilon, std::vector<v2f>& out) 
{
    if (points.size() < 2) return;

    float maxDist = 0;
    size_t index = 0;
    for (size_t i = 1; i < points.size() - 1; ++i) 
    {
        float dist = perpendicular_distance(points[i], points[0], points.back());
        if (dist > maxDist) 
        {
            maxDist = dist;
            index = i;
        }
    }

    if (maxDist > epsilon) 
    {
        std::vector<v2f> left(points.begin(), points.begin() + index + 1);
        std::vector<v2f> right(points.begin() + index, points.end());
        std::vector<v2f> leftResult, rightResult;
        rdp(left, epsilon, leftResult);
        rdp(right, epsilon, rightResult);
        out.clear();
        out.insert(out.end(), leftResult.begin(), leftResult.end() - 1);
        out.insert(out.end(), rightResult.begin(), rightResult.end());
    }
    else {
        out.clear();
        if (points.size() >= 2)
        {
            out.push_back(points.front());
            out.push_back(points.back());
        }
    }
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
    glfwSwapInterval(1); // Enable vsync

    // ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard
        | ImGuiConfigFlags_DockingEnable
        | ImGuiConfigFlags_ViewportsEnable;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(SHADER_VERSION);
    ImGuiStyle& imguiStyle = ImGui::GetStyle();
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        imguiStyle.WindowRounding = 0.0f;
        imguiStyle.TabRounding = 5.f;
        imguiStyle.FrameRounding = 5.f;
        imguiStyle.FramePadding.x = 15.f;
        imguiStyle.FramePadding.y = 17.5f;
        imguiStyle.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    imguiStyle.PopupRounding = 5.f;
    imguiStyle.ItemSpacing.x = 0.f;
    imguiStyle.ItemSpacing.y = 20.f;
    return window;
}

std::vector<bool> g_pixelGrid;
void simplify_pixel_grid(std::vector<v2f> const& points, int width, int height, float downscale, std::vector<v2f>& out)
{
    g_pixelGrid.resize(width * height);
    std::fill(g_pixelGrid.begin(), g_pixelGrid.end(), false);
    for (v2f const& p : points)
    {

    }
}