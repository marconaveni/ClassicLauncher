#ifndef RL_WRAP_H
#define RL_WRAP_H

// Wrapper minimalista para isolar a raylib.
// Incluir este header no projeto no lugar de <raylib.h>.
// A implementação (rl_wrap.cpp) inclui <raylib.h> e chama as funções nativas.
//
// Escopo: apenas funções observadas em uso no projeto.
// Namespace: rlw

#include <cstdarg>
#include <cstdint>

namespace rlw
{

    // ---- Flags de janela (espelham raylib) ----
    inline constexpr unsigned int FLAG_VSYNC_HINT = 0x00000040;
    inline constexpr unsigned int FLAG_WINDOW_RESIZABLE = 0x00000004;
    inline constexpr unsigned int FLAG_WINDOW_UNDECORATED = 0x00000008;

#define C_LIGHTGRAY Color{ 200, 200, 200, 255 }  // Light Gray
#define C_GRAY Color{ 130, 130, 130, 255 }       // Gray
#define C_DARKGRAY Color{ 80, 80, 80, 255 }      // Dark Gray
#define C_YELLOW Color{ 253, 249, 0, 255 }       // Yellow
#define C_GOLD Color{ 255, 203, 0, 255 }         // Gold
#define C_ORANGE Color{ 255, 161, 0, 255 }       // Orange
#define C_PINK Color{ 255, 109, 194, 255 }       // Pink
#define C_RED Color{ 230, 41, 55, 255 }          // Red
#define C_MAROON Color{ 190, 33, 55, 255 }       // Maroon
#define C_GREEN Color{ 0, 228, 48, 255 }         // Green
#define C_LIME Color{ 0, 158, 47, 255 }          // Lime
#define C_DARKGREEN Color{ 0, 117, 44, 255 }     // Dark Green
#define C_SKYBLUE Color{ 102, 191, 255, 255 }    // Sky Blue
#define C_BLUE Color{ 0, 121, 241, 255 }         // Blue
#define C_DARKBLUE Color{ 0, 82, 172, 255 }      // Dark Blue
#define C_PURPLE Color{ 200, 122, 255, 255 }     // Purple
#define C_VIOLET Color{ 135, 60, 190, 255 }      // Violet
#define C_DARKPURPLE Color{ 112, 31, 126, 255 }  // Dark Purple
#define C_BEIGE Color{ 211, 176, 131, 255 }      // Beige
#define C_BROWN Color{ 127, 106, 79, 255 }       // Brown
#define C_DARKBROWN Color{ 76, 63, 47, 255 }     // DarkBrown
#define C_WHITE Color{ 255, 255, 255, 255 }      // White
#define C_BLACK Color{ 0, 0, 0, 255 }            // Black
#define C_BLANK Color{ 0, 0, 0, 0 }              // Blank (Transparent)
#define C_MAGENTA Color{ 255, 0, 255, 255 }      // Magenta
#define C_RAYWHITE Color{ 245, 245, 245, 255 }   // My own White (raylib logo)

    // Mouse buttons
    typedef enum
    {
        MOUSE_BUTTON_LEFT = 0,     // Mouse button left
        MOUSE_BUTTON_RIGHT = 1,    // Mouse button right
        MOUSE_BUTTON_MIDDLE = 2,   // Mouse button middle (pressed wheel)
        MOUSE_BUTTON_SIDE = 3,     // Mouse button side (advanced mouse device)
        MOUSE_BUTTON_EXTRA = 4,    // Mouse button extra (advanced mouse device)
        MOUSE_BUTTON_FORWARD = 5,  // Mouse button forward (advanced mouse device)
        MOUSE_BUTTON_BACK = 6,     // Mouse button back (advanced mouse device)
    } MouseButton;

    // Mouse cursor
    typedef enum
    {
        MOUSE_CURSOR_DEFAULT = 0,        // Default pointer shape
        MOUSE_CURSOR_ARROW = 1,          // Arrow shape
        MOUSE_CURSOR_IBEAM = 2,          // Text writing cursor shape
        MOUSE_CURSOR_CROSSHAIR = 3,      // Cross shape
        MOUSE_CURSOR_POINTING_HAND = 4,  // Pointing hand cursor
        MOUSE_CURSOR_RESIZE_EW = 5,      // Horizontal resize/move arrow shape
        MOUSE_CURSOR_RESIZE_NS = 6,      // Vertical resize/move arrow shape
        MOUSE_CURSOR_RESIZE_NWSE = 7,    // Top-left to bottom-right diagonal resize/move arrow shape
        MOUSE_CURSOR_RESIZE_NESW = 8,    // The top-right to bottom-left diagonal resize/move arrow shape
        MOUSE_CURSOR_RESIZE_ALL = 9,     // The omnidirectional resize/move cursor shape
        MOUSE_CURSOR_NOT_ALLOWED = 10    // The operation-not-allowed shape
    } MouseCursor;

    // Gamepad buttons
    typedef enum
    {
        GAMEPAD_BUTTON_UNKNOWN = 0,       // Unknown button, just for error checking
        GAMEPAD_BUTTON_LEFT_FACE_UP,      // Gamepad left DPAD up button
        GAMEPAD_BUTTON_LEFT_FACE_RIGHT,   // Gamepad left DPAD right button
        GAMEPAD_BUTTON_LEFT_FACE_DOWN,    // Gamepad left DPAD down button
        GAMEPAD_BUTTON_LEFT_FACE_LEFT,    // Gamepad left DPAD left button
        GAMEPAD_BUTTON_RIGHT_FACE_UP,     // Gamepad right button up (i.e. PS3: Triangle, Xbox: Y)
        GAMEPAD_BUTTON_RIGHT_FACE_RIGHT,  // Gamepad right button right (i.e. PS3: Circle, Xbox: B)
        GAMEPAD_BUTTON_RIGHT_FACE_DOWN,   // Gamepad right button down (i.e. PS3: Cross, Xbox: A)
        GAMEPAD_BUTTON_RIGHT_FACE_LEFT,   // Gamepad right button left (i.e. PS3: Square, Xbox: X)
        GAMEPAD_BUTTON_LEFT_TRIGGER_1,    // Gamepad top/back trigger left (first), it could be a trailing button
        GAMEPAD_BUTTON_LEFT_TRIGGER_2,    // Gamepad top/back trigger left (second), it could be a trailing button
        GAMEPAD_BUTTON_RIGHT_TRIGGER_1,   // Gamepad top/back trigger right (first), it could be a trailing button
        GAMEPAD_BUTTON_RIGHT_TRIGGER_2,   // Gamepad top/back trigger right (second), it could be a trailing button
        GAMEPAD_BUTTON_MIDDLE_LEFT,       // Gamepad center buttons, left one (i.e. PS3: Select)
        GAMEPAD_BUTTON_MIDDLE,            // Gamepad center buttons, middle one (i.e. PS3: PS, Xbox: XBOX)
        GAMEPAD_BUTTON_MIDDLE_RIGHT,      // Gamepad center buttons, right one (i.e. PS3: Start)
        GAMEPAD_BUTTON_LEFT_THUMB,        // Gamepad joystick pressed button left
        GAMEPAD_BUTTON_RIGHT_THUMB        // Gamepad joystick pressed button right
    } GamepadButton;

    // Gamepad axis
    typedef enum
    {
        GAMEPAD_AXIS_LEFT_X = 0,        // Gamepad left stick X axis
        GAMEPAD_AXIS_LEFT_Y = 1,        // Gamepad left stick Y axis
        GAMEPAD_AXIS_RIGHT_X = 2,       // Gamepad right stick X axis
        GAMEPAD_AXIS_RIGHT_Y = 3,       // Gamepad right stick Y axis
        GAMEPAD_AXIS_LEFT_TRIGGER = 4,  // Gamepad back trigger left, pressure level: [1..-1]
        GAMEPAD_AXIS_RIGHT_TRIGGER = 5  // Gamepad back trigger right, pressure level: [1..-1]
    } GamepadAxis;

    // Material map index
    typedef enum
    {
        MATERIAL_MAP_ALBEDO = 0,  // Albedo material (same as: MATERIAL_MAP_DIFFUSE)
        MATERIAL_MAP_METALNESS,   // Metalness material (same as: MATERIAL_MAP_SPECULAR)
        MATERIAL_MAP_NORMAL,      // Normal material
        MATERIAL_MAP_ROUGHNESS,   // Roughness material
        MATERIAL_MAP_OCCLUSION,   // Ambient occlusion material
        MATERIAL_MAP_EMISSION,    // Emission material
        MATERIAL_MAP_HEIGHT,      // Heightmap material
        MATERIAL_MAP_CUBEMAP,     // Cubemap material (NOTE: Uses GL_TEXTURE_CUBE_MAP)
        MATERIAL_MAP_IRRADIANCE,  // Irradiance material (NOTE: Uses GL_TEXTURE_CUBE_MAP)
        MATERIAL_MAP_PREFILTER,   // Prefilter material (NOTE: Uses GL_TEXTURE_CUBE_MAP)
        MATERIAL_MAP_BRDF         // Brdf material
    } MaterialMapIndex;

#define MATERIAL_MAP_DIFFUSE MATERIAL_MAP_ALBEDO
#define MATERIAL_MAP_SPECULAR MATERIAL_MAP_METALNESS

    // Shader location index
    typedef enum
    {
        SHADER_LOC_VERTEX_POSITION = 0,  // Shader location: vertex attribute: position
        SHADER_LOC_VERTEX_TEXCOORD01,    // Shader location: vertex attribute: texcoord01
        SHADER_LOC_VERTEX_TEXCOORD02,    // Shader location: vertex attribute: texcoord02
        SHADER_LOC_VERTEX_NORMAL,        // Shader location: vertex attribute: normal
        SHADER_LOC_VERTEX_TANGENT,       // Shader location: vertex attribute: tangent
        SHADER_LOC_VERTEX_COLOR,         // Shader location: vertex attribute: color
        SHADER_LOC_MATRIX_MVP,           // Shader location: matrix uniform: model-view-projection
        SHADER_LOC_MATRIX_VIEW,          // Shader location: matrix uniform: view (camera transform)
        SHADER_LOC_MATRIX_PROJECTION,    // Shader location: matrix uniform: projection
        SHADER_LOC_MATRIX_MODEL,         // Shader location: matrix uniform: model (transform)
        SHADER_LOC_MATRIX_NORMAL,        // Shader location: matrix uniform: normal
        SHADER_LOC_VECTOR_VIEW,          // Shader location: vector uniform: view
        SHADER_LOC_COLOR_DIFFUSE,        // Shader location: vector uniform: diffuse color
        SHADER_LOC_COLOR_SPECULAR,       // Shader location: vector uniform: specular color
        SHADER_LOC_COLOR_AMBIENT,        // Shader location: vector uniform: ambient color
        SHADER_LOC_MAP_ALBEDO,           // Shader location: sampler2d texture: albedo (same as: SHADER_LOC_MAP_DIFFUSE)
        SHADER_LOC_MAP_METALNESS,        // Shader location: sampler2d texture: metalness (same as: SHADER_LOC_MAP_SPECULAR)
        SHADER_LOC_MAP_NORMAL,           // Shader location: sampler2d texture: normal
        SHADER_LOC_MAP_ROUGHNESS,        // Shader location: sampler2d texture: roughness
        SHADER_LOC_MAP_OCCLUSION,        // Shader location: sampler2d texture: occlusion
        SHADER_LOC_MAP_EMISSION,         // Shader location: sampler2d texture: emission
        SHADER_LOC_MAP_HEIGHT,           // Shader location: sampler2d texture: height
        SHADER_LOC_MAP_CUBEMAP,          // Shader location: samplerCube texture: cubemap
        SHADER_LOC_MAP_IRRADIANCE,       // Shader location: samplerCube texture: irradiance
        SHADER_LOC_MAP_PREFILTER,        // Shader location: samplerCube texture: prefilter
        SHADER_LOC_MAP_BRDF,             // Shader location: sampler2d texture: brdf
        SHADER_LOC_VERTEX_BONEIDS,       // Shader location: vertex attribute: boneIds
        SHADER_LOC_VERTEX_BONEWEIGHTS,   // Shader location: vertex attribute: boneWeights
        SHADER_LOC_BONE_MATRICES         // Shader location: array of matrices uniform: boneMatrices
    } ShaderLocationIndex;

#define SHADER_LOC_MAP_DIFFUSE SHADER_LOC_MAP_ALBEDO
#define SHADER_LOC_MAP_SPECULAR SHADER_LOC_MAP_METALNESS

    // Shader uniform data type
    typedef enum
    {
        SHADER_UNIFORM_FLOAT = 0,  // Shader uniform type: float
        SHADER_UNIFORM_VEC2,       // Shader uniform type: vec2 (2 float)
        SHADER_UNIFORM_VEC3,       // Shader uniform type: vec3 (3 float)
        SHADER_UNIFORM_VEC4,       // Shader uniform type: vec4 (4 float)
        SHADER_UNIFORM_INT,        // Shader uniform type: int
        SHADER_UNIFORM_IVEC2,      // Shader uniform type: ivec2 (2 int)
        SHADER_UNIFORM_IVEC3,      // Shader uniform type: ivec3 (3 int)
        SHADER_UNIFORM_IVEC4,      // Shader uniform type: ivec4 (4 int)
        SHADER_UNIFORM_SAMPLER2D   // Shader uniform type: sampler2d
    } ShaderUniformDataType;

    // Shader attribute data types
    typedef enum
    {
        SHADER_ATTRIB_FLOAT = 0,  // Shader attribute type: float
        SHADER_ATTRIB_VEC2,       // Shader attribute type: vec2 (2 float)
        SHADER_ATTRIB_VEC3,       // Shader attribute type: vec3 (3 float)
        SHADER_ATTRIB_VEC4        // Shader attribute type: vec4 (4 float)
    } ShaderAttributeDataType;

    // Pixel formats
    // NOTE: Support depends on OpenGL version and platform
    typedef enum
    {
        PIXELFORMAT_UNCOMPRESSED_GRAYSCALE = 1,  // 8 bit per pixel (no alpha)
        PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA,     // 8*2 bpp (2 channels)
        PIXELFORMAT_UNCOMPRESSED_R5G6B5,         // 16 bpp
        PIXELFORMAT_UNCOMPRESSED_R8G8B8,         // 24 bpp
        PIXELFORMAT_UNCOMPRESSED_R5G5B5A1,       // 16 bpp (1 bit alpha)
        PIXELFORMAT_UNCOMPRESSED_R4G4B4A4,       // 16 bpp (4 bit alpha)
        PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,       // 32 bpp
        PIXELFORMAT_UNCOMPRESSED_R32,            // 32 bpp (1 channel - float)
        PIXELFORMAT_UNCOMPRESSED_R32G32B32,      // 32*3 bpp (3 channels - float)
        PIXELFORMAT_UNCOMPRESSED_R32G32B32A32,   // 32*4 bpp (4 channels - float)
        PIXELFORMAT_UNCOMPRESSED_R16,            // 16 bpp (1 channel - half float)
        PIXELFORMAT_UNCOMPRESSED_R16G16B16,      // 16*3 bpp (3 channels - half float)
        PIXELFORMAT_UNCOMPRESSED_R16G16B16A16,   // 16*4 bpp (4 channels - half float)
        PIXELFORMAT_COMPRESSED_DXT1_RGB,         // 4 bpp (no alpha)
        PIXELFORMAT_COMPRESSED_DXT1_RGBA,        // 4 bpp (1 bit alpha)
        PIXELFORMAT_COMPRESSED_DXT3_RGBA,        // 8 bpp
        PIXELFORMAT_COMPRESSED_DXT5_RGBA,        // 8 bpp
        PIXELFORMAT_COMPRESSED_ETC1_RGB,         // 4 bpp
        PIXELFORMAT_COMPRESSED_ETC2_RGB,         // 4 bpp
        PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA,    // 8 bpp
        PIXELFORMAT_COMPRESSED_PVRT_RGB,         // 4 bpp
        PIXELFORMAT_COMPRESSED_PVRT_RGBA,        // 4 bpp
        PIXELFORMAT_COMPRESSED_ASTC_4x4_RGBA,    // 8 bpp
        PIXELFORMAT_COMPRESSED_ASTC_8x8_RGBA     // 2 bpp
    } PixelFormat;

    // Texture parameters: filter mode
    // NOTE 1: Filtering considers mipmaps if available in the texture
    // NOTE 2: Filter is accordingly set for minification and magnification
    typedef enum
    {
        TEXTURE_FILTER_POINT = 0,        // No filter, just pixel approximation
        TEXTURE_FILTER_BILINEAR,         // Linear filtering
        TEXTURE_FILTER_TRILINEAR,        // Trilinear filtering (linear with mipmaps)
        TEXTURE_FILTER_ANISOTROPIC_4X,   // Anisotropic filtering 4x
        TEXTURE_FILTER_ANISOTROPIC_8X,   // Anisotropic filtering 8x
        TEXTURE_FILTER_ANISOTROPIC_16X,  // Anisotropic filtering 16x
    } TextureFilter;

    // Texture parameters: wrap mode
    typedef enum
    {
        TEXTURE_WRAP_REPEAT = 0,     // Repeats texture in tiled mode
        TEXTURE_WRAP_CLAMP,          // Clamps texture to edge pixel in tiled mode
        TEXTURE_WRAP_MIRROR_REPEAT,  // Mirrors and repeats the texture in tiled mode
        TEXTURE_WRAP_MIRROR_CLAMP    // Mirrors and clamps to border the texture in tiled mode
    } TextureWrap;

    // Cubemap layouts
    typedef enum
    {
        CUBEMAP_LAYOUT_AUTO_DETECT = 0,      // Automatically detect layout type
        CUBEMAP_LAYOUT_LINE_VERTICAL,        // Layout is defined by a vertical line with faces
        CUBEMAP_LAYOUT_LINE_HORIZONTAL,      // Layout is defined by a horizontal line with faces
        CUBEMAP_LAYOUT_CROSS_THREE_BY_FOUR,  // Layout is defined by a 3x4 cross with cubemap faces
        CUBEMAP_LAYOUT_CROSS_FOUR_BY_THREE   // Layout is defined by a 4x3 cross with cubemap faces
    } CubemapLayout;

    // Font type, defines generation method
    typedef enum
    {
        FONT_DEFAULT = 0,  // Default font generation, anti-aliased
        FONT_BITMAP,       // Bitmap font generation, no anti-aliasing
        FONT_SDF           // SDF font generation, requires external shader
    } FontType;

    // Color blending modes (pre-defined)
    typedef enum
    {
        BLEND_ALPHA = 0,          // Blend textures considering alpha (default)
        BLEND_ADDITIVE,           // Blend textures adding colors
        BLEND_MULTIPLIED,         // Blend textures multiplying colors
        BLEND_ADD_COLORS,         // Blend textures adding colors (alternative)
        BLEND_SUBTRACT_COLORS,    // Blend textures subtracting colors (alternative)
        BLEND_ALPHA_PREMULTIPLY,  // Blend premultiplied textures considering alpha
        BLEND_CUSTOM,             // Blend textures using custom src/dst factors (use rlSetBlendFactors())
        BLEND_CUSTOM_SEPARATE     // Blend textures using custom rgb/alpha separate src/dst factors (use rlSetBlendFactorsSeparate())
    } BlendMode;

    // Gesture
    // NOTE: Provided as bit-wise flags to enable only desired gestures
    typedef enum
    {
        GESTURE_NONE = 0,          // No gesture
        GESTURE_TAP = 1,           // Tap gesture
        GESTURE_DOUBLETAP = 2,     // Double tap gesture
        GESTURE_HOLD = 4,          // Hold gesture
        GESTURE_DRAG = 8,          // Drag gesture
        GESTURE_SWIPE_RIGHT = 16,  // Swipe right gesture
        GESTURE_SWIPE_LEFT = 32,   // Swipe left gesture
        GESTURE_SWIPE_UP = 64,     // Swipe up gesture
        GESTURE_SWIPE_DOWN = 128,  // Swipe down gesture
        GESTURE_PINCH_IN = 256,    // Pinch in gesture
        GESTURE_PINCH_OUT = 512    // Pinch out gesture
    } Gesture;

    // Camera system modes
    typedef enum
    {
        CAMERA_CUSTOM = 0,    // Camera custom, controlled by user (UpdateCamera() does nothing)
        CAMERA_FREE,          // Camera free mode
        CAMERA_ORBITAL,       // Camera orbital, around target, zoom supported
        CAMERA_FIRST_PERSON,  // Camera first person
        CAMERA_THIRD_PERSON   // Camera third person
    } CameraMode;

    // Keyboard keys (US keyboard layout)
    // NOTE: Use GetKeyPressed() to allow redefining
    // required keys for alternative layouts
    typedef enum
    {
        KEY_NULL = 0,  // Key: NULL, used for no key pressed
        // Alphanumeric keys
        KEY_APOSTROPHE = 39,     // Key: '
        KEY_COMMA = 44,          // Key: ,
        KEY_MINUS = 45,          // Key: -
        KEY_PERIOD = 46,         // Key: .
        KEY_SLASH = 47,          // Key: /
        KEY_ZERO = 48,           // Key: 0
        KEY_ONE = 49,            // Key: 1
        KEY_TWO = 50,            // Key: 2
        KEY_THREE = 51,          // Key: 3
        KEY_FOUR = 52,           // Key: 4
        KEY_FIVE = 53,           // Key: 5
        KEY_SIX = 54,            // Key: 6
        KEY_SEVEN = 55,          // Key: 7
        KEY_EIGHT = 56,          // Key: 8
        KEY_NINE = 57,           // Key: 9
        KEY_SEMICOLON = 59,      // Key: ;
        KEY_EQUAL = 61,          // Key: =
        KEY_A = 65,              // Key: A | a
        KEY_B = 66,              // Key: B | b
        KEY_C = 67,              // Key: C | c
        KEY_D = 68,              // Key: D | d
        KEY_E = 69,              // Key: E | e
        KEY_F = 70,              // Key: F | f
        KEY_G = 71,              // Key: G | g
        KEY_H = 72,              // Key: H | h
        KEY_I = 73,              // Key: I | i
        KEY_J = 74,              // Key: J | j
        KEY_K = 75,              // Key: K | k
        KEY_L = 76,              // Key: L | l
        KEY_M = 77,              // Key: M | m
        KEY_N = 78,              // Key: N | n
        KEY_O = 79,              // Key: O | o
        KEY_P = 80,              // Key: P | p
        KEY_Q = 81,              // Key: Q | q
        KEY_R = 82,              // Key: R | r
        KEY_S = 83,              // Key: S | s
        KEY_T = 84,              // Key: T | t
        KEY_U = 85,              // Key: U | u
        KEY_V = 86,              // Key: V | v
        KEY_W = 87,              // Key: W | w
        KEY_X = 88,              // Key: X | x
        KEY_Y = 89,              // Key: Y | y
        KEY_Z = 90,              // Key: Z | z
        KEY_LEFT_BRACKET = 91,   // Key: [
        KEY_BACKSLASH = 92,      // Key: '\'
        KEY_RIGHT_BRACKET = 93,  // Key: ]
        KEY_GRAVE = 96,          // Key: `
        // Function keys
        KEY_SPACE = 32,           // Key: Space
        KEY_ESCAPE = 256,         // Key: Esc
        KEY_ENTER = 257,          // Key: Enter
        KEY_TAB = 258,            // Key: Tab
        KEY_BACKSPACE = 259,      // Key: Backspace
        KEY_INSERT = 260,         // Key: Ins
        KEY_DELETE = 261,         // Key: Del
        KEY_RIGHT = 262,          // Key: Cursor right
        KEY_LEFT = 263,           // Key: Cursor left
        KEY_DOWN = 264,           // Key: Cursor down
        KEY_UP = 265,             // Key: Cursor up
        KEY_PAGE_UP = 266,        // Key: Page up
        KEY_PAGE_DOWN = 267,      // Key: Page down
        KEY_HOME = 268,           // Key: Home
        KEY_END = 269,            // Key: End
        KEY_CAPS_LOCK = 280,      // Key: Caps lock
        KEY_SCROLL_LOCK = 281,    // Key: Scroll down
        KEY_NUM_LOCK = 282,       // Key: Num lock
        KEY_PRINT_SCREEN = 283,   // Key: Print screen
        KEY_PAUSE = 284,          // Key: Pause
        KEY_F1 = 290,             // Key: F1
        KEY_F2 = 291,             // Key: F2
        KEY_F3 = 292,             // Key: F3
        KEY_F4 = 293,             // Key: F4
        KEY_F5 = 294,             // Key: F5
        KEY_F6 = 295,             // Key: F6
        KEY_F7 = 296,             // Key: F7
        KEY_F8 = 297,             // Key: F8
        KEY_F9 = 298,             // Key: F9
        KEY_F10 = 299,            // Key: F10
        KEY_F11 = 300,            // Key: F11
        KEY_F12 = 301,            // Key: F12
        KEY_LEFT_SHIFT = 340,     // Key: Shift left
        KEY_LEFT_CONTROL = 341,   // Key: Control left
        KEY_LEFT_ALT = 342,       // Key: Alt left
        KEY_LEFT_SUPER = 343,     // Key: Super left
        KEY_RIGHT_SHIFT = 344,    // Key: Shift right
        KEY_RIGHT_CONTROL = 345,  // Key: Control right
        KEY_RIGHT_ALT = 346,      // Key: Alt right
        KEY_RIGHT_SUPER = 347,    // Key: Super right
        KEY_KB_MENU = 348,        // Key: KB menu
        // Keypad keys
        KEY_KP_0 = 320,         // Key: Keypad 0
        KEY_KP_1 = 321,         // Key: Keypad 1
        KEY_KP_2 = 322,         // Key: Keypad 2
        KEY_KP_3 = 323,         // Key: Keypad 3
        KEY_KP_4 = 324,         // Key: Keypad 4
        KEY_KP_5 = 325,         // Key: Keypad 5
        KEY_KP_6 = 326,         // Key: Keypad 6
        KEY_KP_7 = 327,         // Key: Keypad 7
        KEY_KP_8 = 328,         // Key: Keypad 8
        KEY_KP_9 = 329,         // Key: Keypad 9
        KEY_KP_DECIMAL = 330,   // Key: Keypad .
        KEY_KP_DIVIDE = 331,    // Key: Keypad /
        KEY_KP_MULTIPLY = 332,  // Key: Keypad *
        KEY_KP_SUBTRACT = 333,  // Key: Keypad -
        KEY_KP_ADD = 334,       // Key: Keypad +
        KEY_KP_ENTER = 335,     // Key: Keypad Enter
        KEY_KP_EQUAL = 336,     // Key: Keypad =
        // Android key buttons
        KEY_BACK = 4,         // Key: Android back button
        KEY_MENU = 5,         // Key: Android menu button
        KEY_VOLUME_UP = 24,   // Key: Android volume up button
        KEY_VOLUME_DOWN = 25  // Key: Android volume down button
    } KeyboardKey;

    typedef enum
    {
        LOG_ALL = 0,  // Display all logs
        LOG_TRACE,    // Trace logging, intended for internal use only
        LOG_DEBUG,    // Debug logging, used for internal debugging, it should be disabled on release builds
        LOG_INFO,     // Info logging, used for program execution info
        LOG_WARNING,  // Warning logging, used on recoverable failures
        LOG_ERROR,    // Error logging, used on unrecoverable failures
        LOG_FATAL,    // Fatal logging, used to abort program: exit(EXIT_FAILURE)
        LOG_NONE      // Disable logging
    } TraceLogLevel;

    // Vector2, 2 components
    struct Vector2
    {
        float x;  // Vector x component
        float y;  // Vector y component
    };

    // Vector3, 3 components
    struct Vector3
    {
        float x;  // Vector x component
        float y;  // Vector y component
        float z;  // Vector z component
    };

    // Vector4, 4 components
    struct Vector4
    {
        float x;  // Vector x component
        float y;  // Vector y component
        float z;  // Vector z component
        float w;  // Vector w component
    };

    struct Color
    {
        unsigned char r, g, b, a;
    };

    struct Image
    {
        void* data;
        int width;
        int height;
        int mipmaps;
        int format;
    };

    struct Texture2D
    {
        unsigned int id;
        int width;
        int height;
        int mipmaps;
        int format;
    };

    struct RenderTexture2D
    {
        unsigned int id;
        Texture2D texture;
        Texture2D depth;  // compat com raylib >=4.x
    };

    typedef RenderTexture2D RenderTexture;
    typedef Texture2D Texture;

    typedef struct rAudioBuffer rAudioBuffer;

    struct AudioStream
    {
        void* buffer;  // opaco
        unsigned int sampleRate;
        unsigned int sampleSize;
        unsigned int channels;
    };

    struct Music
    {
        void* _native{};
        unsigned int frameCount;
        bool looping;
        int ctxType;
        void* ctxData;

        Music() = default;
        Music(Music&& o) noexcept
            : _native(o._native), frameCount(o.frameCount), looping(o.looping), ctxType(o.ctxType), ctxData(o.ctxData)
        {
            o._native = nullptr;
        }
        Music& operator=(Music&& o) noexcept
        {
            if (this != &o)
            {
                _native = o._native;
                frameCount = o.frameCount;
                looping = o.looping;
                ctxType = o.ctxType;
                ctxData = o.ctxData;
                o._native = nullptr;
            }
            return *this;
        }
        Music(const Music&) = delete;
        Music& operator=(const Music&) = delete;
    };

    struct Sound
    {
        void* _native{};  // -> ::Sound*
        unsigned int frameCount;

        Sound() = default;
        Sound(Sound&& o) noexcept
            : _native(o._native), frameCount(o.frameCount)
        {
            o._native = nullptr;
        }
        Sound& operator=(Sound&& o) noexcept
        {
            if (this != &o)
            {
                _native = o._native;
                frameCount = o.frameCount;
                o._native = nullptr;
            }
            return *this;
        }
        Sound(const Sound&) = delete;
        Sound& operator=(const Sound&) = delete;
    };

    struct FilePathList
    {
        unsigned int count;
        char** paths;
    };

    struct Rectangle
    {
        float x, y, width, height;
    };

    struct Font
    {
        int baseSize{};
        int glyphCount{};
        int glyphPadding{};
        // ponteiro pro nativo (::Font*)
        void* _native{};
        bool _owned{ false };  // true: veio de LoadFontEx (descarrega); false: default font (não descarrega)
    };

    // --- Utilitários de cor comuns ---
    inline Color ColorRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        return Color{ r, g, b, a };
    }

    // --- Logging / Config / Janela ---
    void SetTraceLogCallback(void (*callback)(int logLevel, const char* text, va_list args));
    void SetTraceLogLevel(int logLevel);
    void SetConfigFlags(unsigned int flags);

    void InitWindow(int width, int height, const char* title);
    bool WindowShouldClose();
    void CloseWindow();

    void SetWindowState(unsigned int flags);
    void ClearWindowState(unsigned int flags);
    bool IsWindowState(unsigned int flag);
    void SetWindowSize(int width, int height);
    void SetWindowPosition(int x, int y);
    void SetWindowIcons(Image* images, int count);
    void SetExitKey(int key);
    void SetTargetFPS(int fps);
    int GetFPS();
    float GetFrameTime();

    int GetScreenWidth();
    int GetScreenHeight();

    int GetCurrentMonitor();
    int GetMonitorWidth(int monitor);
    int GetMonitorHeight(int monitor);
    Vector2 GetMonitorPosition(int monitor);

    Vector2 GetWindowPosition();
    bool IsWindowFullscreen();
    void ToggleFullscreen();

    // --- Imagem / Textura ---
    Image LoadImage(const char* fileName);
    void UnloadImage(Image image);
    void ImageResize(Image* image, int newWidth, int newHeight);
    void ImageResizeNN(Image* image, int newWidth, int newHeight);

    Texture2D LoadTextureFromImage(Image image);
    void UnloadTexture(Texture2D texture);
    void SetTextureFilter(Texture2D texture, int filter);

    // --- Render alvo ---
    RenderTexture2D LoadRenderTexture(int width, int height);
    void UnloadRenderTexture(RenderTexture2D target);
    bool IsRenderTextureValid(RenderTexture2D target);

    void BeginTextureMode(RenderTexture2D target);
    void EndTextureMode();

    // --- Desenho 2D ---
    void BeginDrawing();
    void EndDrawing();
    void ClearBackground(Color color);

    void DrawTexturePro(Texture2D texture,
                        /*source*/ float srcX,
                        float srcY,
                        float srcW,
                        float srcH,
                        /*dest*/ float dstX,
                        float dstY,
                        float dstW,
                        float dstH,
                        /*origin*/ float originX,
                        float originY,
                        float rotation);

    // Scissor
    void BeginScissorMode(int x, int y, int width, int height);
    void EndScissorMode();

    // Retângulos
    void DrawRectangle(int x, int y, int width, int height, Color color);
    void DrawRectangleLinesEx(Rectangle rec, float lineThick, Color color);

    // Colisão
    bool CheckCollisionPointRec(Vector2 point, Rectangle rec);
    bool CheckCollisionRecs(Rectangle rec1, Rectangle rec2);

    // Texturas simples e "pro"
    void DrawTexture(Texture2D texture, int posX, int posY, Color tint);
    void DrawTexturePro(Texture2D texture, Rectangle src, Rectangle dst, Vector2 origin, float rotation, Color tint);

    bool IsImageValid(Image image);      // mapeia para IsImageReady
    bool IsTextureValid(Texture2D tex);  // mapeia para IsTextureReady
    Image GenImageColor(int width, int height, Color color);
    Image ImageCopy(Image src);
    void UpdateTexture(Texture2D texture, const void* pixels);

    bool IsFontValid(Font font);  // mapeia para IsFontReady
    void UnloadFont(Font font);
    Font LoadFontEx(const char* fileName, int fontSize, int* codepoints, int codepointCount);
    void DrawTextEx(Font font, const char* text, Vector2 position, float fontSize, float spacing, Color tint);
    Vector2 MeasureTextEx(Font font, const char* text, float fontSize, float spacing);

    bool IsMouseButtonPressed(int button);

    // --- Entrada ---
    bool IsKeyPressed(int key);
    bool IsKeyDown(int key);
    bool IsKeyReleased(int key);
    bool IsKeyUp(int key);

    bool IsGamepadButtonPressed(int gamepad, int button);
    bool IsGamepadButtonDown(int gamepad, int button);
    bool IsGamepadButtonReleased(int gamepad, int button);
    bool IsGamepadButtonUp(int gamepad, int button);

    Vector2 GetMousePosition();

    // --- Math (raymath) ---
    Vector2 Vector2Clamp(Vector2 value, Vector2 min, Vector2 max);

    // --- Áudio ---
    void InitAudioDevice();
    void CloseAudioDevice();

    // Music (stream)
    Music LoadMusicStream(const char* fileName);
    bool IsMusicValid(Music music);
    void UnloadMusicStream(Music music);

    void PlayMusicStream(Music music);
    void PauseMusicStream(Music music);
    void StopMusicStream(Music music);
    void SeekMusicStream(Music music, float position);
    void UpdateMusicStream(Music music);

    float GetMusicTimeLength(Music music);
    float GetMusicTimePlayed(Music music);

    // Sound (efeitos)
    Sound LoadSound(const char* fileName);
    bool IsSoundValid(Sound sound);
    void PlaySound(Sound sound);

    // --- FS Utils ---
    const char* GetApplicationDirectory();
    const char* GetWorkingDirectory();
    bool ChangeDirectory(const char* path);
    const char* GetFileNameWithoutExt(const char* filePath);

    // --- Directory listing ---
    FilePathList LoadDirectoryFiles(const char* dirPath);
    void UnloadDirectoryFiles(FilePathList files);

    const char* TextFormat(const char* text, ...);

    Font GetFontDefault();

    const char* GetDirectoryPath(const char* filePath);
    bool FileExists(const char* fileName);
    bool DirectoryExists(const char* dirPath);
    bool MakeDirectory(const char* dirPath);

    void WaitTime(double seconds);

    void* MemAlloc(unsigned int size);
    void MemFree(void* ptr);

    Image MakeImage(void* data, int width, int height, int mipmaps, int format);

    //// isso vai sair daqui

    // Áudio – sanity checks e debug
    bool IsAudioDeviceReady();           // proxy de raylib
    void LogMusicState(const Music& m);  // imprime campos úteis do Music

}  // namespace rlw

#endif  // RL_WRAP_H