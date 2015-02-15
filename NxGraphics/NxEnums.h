/*
-----------------------------------------------------------------------------
This file is part of NxGraphics, Multimedia Engine.
Author : Stephane Kyles. Developed in Prague, Czech Republic.
		_   __       ______                     __     _            
	   / | / /_  __ / ____/_____ ____ _ ____   / /_   (_)_____ _____
	  /  |/ /| |/_// / __ / ___// __ `// __ \ / __ \ / // ___// ___/
	 / /|  /_>  < / /_/ // /   / /_/ // /_/ // / / // // /__ (__  ) 
	/_/ |_//_/|_| \____//_/    \__,_// .___//_/ /_//_/ \___//____/  
									/_/                             
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#ifndef __NXENUMS_H__
#define __NXENUMS_H__


//NxGui

enum NxWidgetState
{
	NXWIDGET_STATE_NORMAL,
	NXWIDGET_STATE_OVER,
	NXWIDGET_STATE_DOWN,
	NXWIDGET_STATE_SUSTAINED
};

enum NxWidgetType
{
	NxWidgetButton
};

enum NxTextVerticalAlignment {
	NxTextVTop,
	NxTextVMiddle,
	NxTextVBottom
};

enum NxTextHorizontalAlignment {
	NxTextHLeft,
	NxTextHMiddle,
	NxTextHRight
};

//end nxgui

enum NxOrientationMode {
	NXOR_DEGREE_0 = 0,
	NXOR_DEGREE_90 = 1,
	NXOR_DEGREE_180 = 2,
	NXOR_DEGREE_270 = 3,

	NXOR_PORTRAIT = NXOR_DEGREE_0,
	NXOR_LANDSCAPERIGHT = NXOR_DEGREE_90,
	NXOR_LANDSCAPELEFT = NXOR_DEGREE_270
};

enum NxMetricsMode
{
	/// 'left', 'top', 'height' and 'width' are parametrics from 0.0 to 1.0
	NXMM_RELATIVE,
	/// Positions & sizes are in absolute pixels
	NXMM_PIXELS,
	/// Positions & sizes are in virtual pixels
	NXMM_RELATIVE_ASPECT_ADJUSTED
};

enum NxFrameBufferType {
	NXFBT_COLOUR = 0x1,
	NXFBT_DEPTH = 0x2,
	NXFBT_STENCIL = 0x4
};
 
//! Keyboard scan codes
enum NxKeyCode
{
	NXKC_UNASSIGNED  = 0x00, 
	NXKC_ESCAPE      = 0x01,
	NXKC_1           = 0x02,
	NXKC_2           = 0x03,
	NXKC_3           = 0x04,
	NXKC_4           = 0x05,
	NXKC_5           = 0x06,
	NXKC_6           = 0x07,
	NXKC_7           = 0x08,
	NXKC_8           = 0x09,
	NXKC_9           = 0x0A,
	NXKC_0           = 0x0B,
	NXKC_MINUS       = 0x0C,    // - on main keyboard
	NXKC_EQUALS      = 0x0D,
	NXKC_BACK        = 0x0E,    // backspace
	NXKC_TAB         = 0x0F,
	NXKC_Q           = 0x10,
	NXKC_W           = 0x11,
	NXKC_E           = 0x12,
	NXKC_R           = 0x13,
	NXKC_T           = 0x14,
	NXKC_Y           = 0x15,
	NXKC_U           = 0x16,
	NXKC_I           = 0x17,
	NXKC_O           = 0x18,
	NXKC_P           = 0x19,
	NXKC_LBRACKET    = 0x1A,
	NXKC_RBRACKET    = 0x1B,
	NXKC_RETURN      = 0x1C,    // Enter on main keyboard
	NXKC_LCONTROL    = 0x1D,
	NXKC_A           = 0x1E,
	NXKC_S           = 0x1F,
	NXKC_D           = 0x20,
	NXKC_F           = 0x21,
	NXKC_G           = 0x22,
	NXKC_H           = 0x23,
	NXKC_J           = 0x24,
	NXKC_K           = 0x25,
	NXKC_L           = 0x26,
	NXKC_SEMICOLON   = 0x27,
	NXKC_APOSTROPHE  = 0x28,
	NXKC_GRAVE       = 0x29,    // accent
	NXKC_LSHIFT      = 0x2A,
	NXKC_BACKSLASH   = 0x2B,
	NXKC_Z           = 0x2C,
	NXKC_X           = 0x2D,
	NXKC_C           = 0x2E,
	NXKC_V           = 0x2F,
	NXKC_B           = 0x30,
	NXKC_N           = 0x31,
	NXKC_M           = 0x32,
	NXKC_COMMA       = 0x33,
	NXKC_PERIOD      = 0x34,    // . on main keyboard
	NXKC_SLASH       = 0x35,    // / on main keyboard
	NXKC_RSHIFT      = 0x36,
	NXKC_MULTIPLY    = 0x37,    // * on numeric keypad
	NXKC_LMENU       = 0x38,    // left Alt
	NXKC_SPACE       = 0x39,
	NXKC_CAPITAL     = 0x3A,
	NXKC_F1          = 0x3B,
	NXKC_F2          = 0x3C,
	NXKC_F3          = 0x3D,
	NXKC_F4          = 0x3E,
	NXKC_F5          = 0x3F,
	NXKC_F6          = 0x40,
	NXKC_F7          = 0x41,
	NXKC_F8          = 0x42,
	NXKC_F9          = 0x43,
	NXKC_F10         = 0x44,
	NXKC_NUMLOCK     = 0x45,
	NXKC_SCROLL      = 0x46,    // Scroll Lock
	NXKC_NUMPAD7     = 0x47,
	NXKC_NUMPAD8     = 0x48,
	NXKC_NUMPAD9     = 0x49,
	NXKC_SUBTRACT    = 0x4A,    // - on numeric keypad
	NXKC_NUMPAD4     = 0x4B,
	NXKC_NUMPAD5     = 0x4C,
	NXKC_NUMPAD6     = 0x4D,
	NXKC_ADD         = 0x4E,    // + on numeric keypad
	NXKC_NUMPAD1     = 0x4F,
	NXKC_NUMPAD2     = 0x50,
	NXKC_NUMPAD3     = 0x51,
	NXKC_NUMPAD0     = 0x52,
	NXKC_DECIMAL     = 0x53,    // . on numeric keypad
	NXKC_OEM_102     = 0x56,    // < > | on UK/Germany keyboards
	NXKC_F11         = 0x57,
	NXKC_F12         = 0x58,
	NXKC_F13         = 0x64,    //                     (NEC PC98)
	NXKC_F14         = 0x65,    //                     (NEC PC98)
	NXKC_F15         = 0x66,    //                     (NEC PC98)
	NXKC_KANA        = 0x70,    // (Japanese keyboard)
	NXKC_ABNT_C1     = 0x73,    // / ? on Portugese (Brazilian) keyboards
	NXKC_CONVERT     = 0x79,    // (Japanese keyboard)
	NXKC_NOCONVERT   = 0x7B,    // (Japanese keyboard)
	NXKC_YEN         = 0x7D,    // (Japanese keyboard)
	NXKC_ABNT_C2     = 0x7E,    // Numpad . on Portugese (Brazilian) keyboards
	NXKC_NUMPADEQUALS= 0x8D,    // = on numeric keypad (NEC PC98)
	NXKC_PREVTRACK   = 0x90,    // Previous Track (KC_CIRCUMFLEX on Japanese keyboard)
	NXKC_AT          = 0x91,    //                     (NEC PC98)
	NXKC_COLON       = 0x92,    //                     (NEC PC98)
	NXKC_UNDERLINE   = 0x93,    //                     (NEC PC98)
	NXKC_KANJI       = 0x94,    // (Japanese keyboard)
	NXKC_STOP        = 0x95,    //                     (NEC PC98)
	NXKC_AX          = 0x96,    //                     (Japan AX)
	NXKC_UNLABELED   = 0x97,    //                        (J3100)
	NXKC_NEXTTRACK   = 0x99,    // Next Track
	NXKC_NUMPADENTER = 0x9C,    // Enter on numeric keypad
	NXKC_RCONTROL    = 0x9D,
	NXKC_MUTE        = 0xA0,    // Mute
	NXKC_CALCULATOR  = 0xA1,    // Calculator
	NXKC_PLAYPAUSE   = 0xA2,    // Play / Pause
	NXKC_MEDIASTOP   = 0xA4,    // Media Stop
	NXKC_VOLUMEDOWN  = 0xAE,    // Volume -
	NXKC_VOLUMEUP    = 0xB0,    // Volume +
	NXKC_WEBHOME     = 0xB2,    // Web home
	NXKC_NUMPADCOMMA = 0xB3,    // , on numeric keypad (NEC PC98)
	NXKC_DIVIDE      = 0xB5,    // / on numeric keypad
	NXKC_SYSRQ       = 0xB7,
	NXKC_RMENU       = 0xB8,    // right Alt
	NXKC_PAUSE       = 0xC5,    // Pause
	NXKC_HOME        = 0xC7,    // Home on arrow keypad
	NXKC_UP          = 0xC8,    // UpArrow on arrow keypad
	NXKC_PGUP        = 0xC9,    // PgUp on arrow keypad
	NXKC_LEFT        = 0xCB,    // LeftArrow on arrow keypad
	NXKC_RIGHT       = 0xCD,    // RightArrow on arrow keypad
	NXKC_END         = 0xCF,    // End on arrow keypad
	NXKC_DOWN        = 0xD0,    // DownArrow on arrow keypad
	NXKC_PGDOWN      = 0xD1,    // PgDn on arrow keypad
	NXKC_INSERT      = 0xD2,    // Insert on arrow keypad
	NXKC_DELETE      = 0xD3,    // Delete on arrow keypad
	NXKC_LWIN        = 0xDB,    // Left Windows key
	NXKC_RWIN        = 0xDC,    // Right Windows key
	NXKC_APPS        = 0xDD,    // AppMenu key
	NXKC_POWER       = 0xDE,    // System Power
	NXKC_SLEEP       = 0xDF,    // System Sleep
	NXKC_WAKE        = 0xE3,    // System Wake
	NXKC_WEBSEARCH   = 0xE5,    // Web Search
	NXKC_WEBFAVORITES= 0xE6,    // Web Favorites
	NXKC_WEBREFRESH  = 0xE7,    // Web Refresh
	NXKC_WEBSTOP     = 0xE8,    // Web Stop
	NXKC_WEBFORWARD  = 0xE9,    // Web Forward
	NXKC_WEBBACK     = 0xEA,    // Web Back
	NXKC_MYCOMPUTER  = 0xEB,    // My Computer
	NXKC_MAIL        = 0xEC,    // Mail
	NXKC_MEDIASELECT = 0xED     // Media Select
};


enum NxFileType {
	NXFT_BINARY,
	NXFT_XML,
	NXFT_COLLADA
};

enum NxMouseButtonID
{
	NxButtonLeft = 0,
	NxButtonRight,
	NxButtonMiddle,
	NxButton3,
	NxButton4,
	NxButton5,
	NxButton6,
	NxButton7
};

enum NxGraphics_Base_Types
{  
	TYPE_MEDIA,
	TYPE_DEVICE,
	TYPE_MEDIA_PLUGIN,
	TYPE_DEVICE_PLUGIN,
	TYPE_BASE_NONE
};

enum NXTOOLS_MODE {
	NXTOOL_SELECT,
	NXTOOL_MOVE,
	NXTOOL_ROTATE,
	NXTOOL_SCALE,
	NXTOOL_NONE
};

enum NxPolygonMode
{
	NxPolygonModePoints = 1,
	NxPolygonModeWireFrame = 2,
	NxPolygonModeSolid = 3
};

enum NxSpecialCaseRenderQueueMode
{
	/// Render only the queues in the special case list
	NXSCRQM_INCLUDE,
	/// Render all except the queues in the special case list
	NXSCRQM_EXCLUDE
};

enum NxTransformSpace
{
	/// Transform is relative to the local space
	NxLocal,
	/// Transform is relative to the space of the parent node
	NxParent,
	/// Transform is relative to world space
	NxWorld
};

enum NXTOOL_AXIS {
	AXIS_X = 1,    
	AXIS_Y = 2,      
	AXIS_XY = 3,     
	AXIS_Z = 4,    
	AXIS_XZ = 5,    
	AXIS_YZ = 6,     
	AXIS_ALL = 7
};

enum NxEntityQueryFlags
{
	NXENTITY = 1<<0,
	NXTERRAIN = 1<<1,
	NXGIZMO = 1<<2
};

enum NxSceneType
{
	NXST_GENERIC = 1,
	NXST_EXTERIOR_CLOSE = 2,
	NXST_EXTERIOR_FAR = 4,
	NXST_EXTERIOR_REAL_FAR = 8,
	NXST_INTERIOR = 16
};

enum NxEntityMagnetCoordinates 
{ 
	NxCartesian,
	NxSpherical,
	NxCylindrical,
	NxToroidal
};

enum NxEntityEvents 
{ 
	NxEntityTouchStart,
	NxEntityTouchEnd
};

enum NxType
{ 
	NxTypeSceneNode,
	NxTypeEntity,
	NxTypeLine,
	NxTypeText,
	NxTypeCloth,
	NxTypeFluid,
	NxTypeParticleSystem,
	NxTypeLight,
	NxTypeCamera,
	NxTypeMagnet,
	NxTypeCharacter,
	NxTypeTerrain,
	NxTypeVehicule
};

enum CollisionGroup
{
	GROUP_NON_COLLIDABLE,
	GROUP_COLLIDABLE_NON_PUSHABLE,
	GROUP_COLLIDABLE_PUSHABLE,
	GROUP_CHARACTER
};

enum PhysicGroup
{
	GROUP_NONE,
	GROUP_KINEMATIC,
	GROUP_DYNAMIC,
	GROUP_STATIC
};

enum PhysicControllerFlag
{
	COLLISION_SIDES_FLAG	= (1<<0),	//!< Character is colliding to the sides.
	COLLISION_UP_FLAG		= (1<<1),	//!< Character has collision above.
	COLLISION_DOWN_FLAG		= (1<<2),	//!< Character has collision below.
};

enum NavigationMode
{ 
	ModeWalk,
	ModeFly,
	ModeCharacter
};

enum NxLightType
{
	PointLight,
	SpotLight,
	DirectionalLight
};


enum NxImageType
{
	NxITCanvas,
	NxITFilename
};

enum NxRendererType
{ 
	NxGraphics_OpenGL,
	NxGraphics_DirectX,
	NxGraphics_DirectX10,
	NxGraphics_DirectX11,
	NxGraphics_GLES2
};

enum NxRendererGlOptions 
{  
	NxGraphics_OpenGL_FBO,
	NxGraphics_OpenGL_PBuffer,
	NxGraphics_OpenGL_Copy  
};

enum NxGraphics_3D_Object_Mask 
{ 
	NxGraphics_3D_Object_Mask_NotSelectable = 0x23 
};

enum NxQuadVertex
{
	QUAD_UPPER_LEFT,
	QUAD_LOWER_LEFT,
	QUAD_UPPER_RIGHT,
	QUAD_LOWER_RIGHT,
	QUAD_MIDDLE
};

enum NxParticleEmitterType
{
	NxParticleEmitterPoint,
	NxParticleEmitterJong
};

enum NxParticleSystemAffectorTypes
{
	NxParticleSystemAffectorScaler,  
	NxParticleSystemAffectorRotator,  
	NxParticleSystemAffectorLinearForce,
	NxParticleSystemAffectorDirectionRandomiser,
	NxParticleSystemAffectorDeflectorPlane,
	NxParticleSystemAffectorColourInterpolator,
	NxParticleSystemAffectorColourImage,
	NxParticleSystemAffectorColourFader2,
	NxParticleSystemAffectorColourFader,
	NxParticleSystemAffectorInterpolator,
	NxParticleSystemAffectorJong

};


/** Fog modes. */
enum NxFogMode
{
	/// No fog. Duh.
	NXFOG_NONE,
	/// Fog density increases  exponentially from the camera (fog = 1/e^(distance * density))
	NXFOG_EXP,
	/// Fog density increases at the square of FOG_EXP, i.e. even quicker (fog = 1/e^(distance * density)^2)
	NXFOG_EXP2,
	/// Fog density increases linearly between the start and end distances
	NXFOG_LINEAR
};

enum NxPixelFormat
{
	/// Unknown pixel format.
	NXPF_UNKNOWN = 0,
	/// 8-bit pixel format, all bits luminance.
	NXPF_L8 = 1,
	NXPF_BYTE_L = NXPF_L8,
	/// 16-bit pixel format, all bits luminance.
	NXPF_L16 = 2,
	NXPF_SHORT_L = NXPF_L16,
	/// 8-bit pixel format, all bits alpha.
	NXPF_A8 = 3,
	NXPF_BYTE_A = NXPF_A8,
	/// 8-bit pixel format, 4 bits alpha, 4 bits luminance.
	NXPF_A4L4 = 4,
	/// 2 byte pixel format, 1 byte luminance, 1 byte alpha
	NXPF_BYTE_LA = 5,
	/// 16-bit pixel format, 5 bits red, 6 bits green, 5 bits blue.
	NXPF_R5G6B5 = 6,
	/// 16-bit pixel format, 5 bits red, 6 bits green, 5 bits blue.
	NXPF_B5G6R5 = 7,
	/// 8-bit pixel format, 2 bits blue, 3 bits green, 3 bits red.
	NXPF_R3G3B2 = 31,
	/// 16-bit pixel format, 4 bits for alpha, red, green and blue.
	NXPF_A4R4G4B4 = 8,
	/// 16-bit pixel format, 5 bits for blue, green, red and 1 for alpha.
	NXPF_A1R5G5B5 = 9,
	/// 24-bit pixel format, 8 bits for red, green and blue.
	NXPF_R8G8B8 = 10,
	/// 24-bit pixel format, 8 bits for blue, green and red.
	NXPF_B8G8R8 = 11,
	/// 32-bit pixel format, 8 bits for alpha, red, green and blue.
	NXPF_A8R8G8B8 = 12,
	/// 32-bit pixel format, 8 bits for blue, green, red and alpha.
	NXPF_A8B8G8R8 = 13,
	/// 32-bit pixel format, 8 bits for blue, green, red and alpha.
	NXPF_B8G8R8A8 = 14,
	/// 32-bit pixel format, 8 bits for red, green, blue and alpha.
	NXPF_R8G8B8A8 = 28,
	/// 32-bit pixel format, 8 bits for red, 8 bits for green, 8 bits for blue
	/// like PF_A8R8G8B8, but alpha will get discarded
	NXPF_X8R8G8B8 = 26,
	/// 32-bit pixel format, 8 bits for blue, 8 bits for green, 8 bits for red
	/// like PF_A8B8G8R8, but alpha will get discarded
	NXPF_X8B8G8R8 = 27,
#if NXGRAPHICS_ENDIAN == OGRE_ENDIAN_BIG
	/// 3 byte pixel format, 1 byte for red, 1 byte for green, 1 byte for blue
	NXPF_BYTE_RGB = NXPF_R8G8B8,
	/// 3 byte pixel format, 1 byte for blue, 1 byte for green, 1 byte for red
	NXPF_BYTE_BGR = NXPF_B8G8R8,
	/// 4 byte pixel format, 1 byte for blue, 1 byte for green, 1 byte for red and one byte for alpha
	NXPF_BYTE_BGRA = NXPF_B8G8R8A8,
	/// 4 byte pixel format, 1 byte for red, 1 byte for green, 1 byte for blue, and one byte for alpha
	NXPF_BYTE_RGBA = NXPF_R8G8B8A8,
#else
	/// 3 byte pixel format, 1 byte for red, 1 byte for green, 1 byte for blue
	NXPF_BYTE_RGB = NXPF_B8G8R8,
	/// 3 byte pixel format, 1 byte for blue, 1 byte for green, 1 byte for red
	NXPF_BYTE_BGR = NXPF_R8G8B8,
	/// 4 byte pixel format, 1 byte for blue, 1 byte for green, 1 byte for red and one byte for alpha
	NXPF_BYTE_BGRA = NXPF_A8R8G8B8,
	/// 4 byte pixel format, 1 byte for red, 1 byte for green, 1 byte for blue, and one byte for alpha
	NXPF_BYTE_RGBA = NXPF_A8B8G8R8,
#endif        
	/// 32-bit pixel format, 2 bits for alpha, 10 bits for red, green and blue.
	NXPF_A2R10G10B10 = 15,
	/// 32-bit pixel format, 10 bits for blue, green and red, 2 bits for alpha.
	NXPF_A2B10G10R10 = 16,
	/// DDS (DirectDraw Surface) DXT1 format
	NXPF_DXT1 = 17,
	/// DDS (DirectDraw Surface) DXT2 format
	NXPF_DXT2 = 18,
	/// DDS (DirectDraw Surface) DXT3 format
	NXPF_DXT3 = 19,
	/// DDS (DirectDraw Surface) DXT4 format
	NXPF_DXT4 = 20,
	/// DDS (DirectDraw Surface) DXT5 format
	NXPF_DXT5 = 21,
	/// 16-bit pixel format, 16 bits (float) for red
	NXPF_FLOAT16_R = 32,
	/// 48-bit pixel format, 16 bits (float) for red, 16 bits (float) for green, 16 bits (float) for blue
	NXPF_FLOAT16_RGB = 22,
	/// 64-bit pixel format, 16 bits (float) for red, 16 bits (float) for green, 16 bits (float) for blue, 16 bits (float) for alpha
	NXPF_FLOAT16_RGBA = 23,
	// 32-bit pixel format, 32 bits (float) for red
	NXPF_FLOAT32_R = 33,
	/// 96-bit pixel format, 32 bits (float) for red, 32 bits (float) for green, 32 bits (float) for blue
	NXPF_FLOAT32_RGB = 24,
	/// 128-bit pixel format, 32 bits (float) for red, 32 bits (float) for green, 32 bits (float) for blue, 32 bits (float) for alpha
	NXPF_FLOAT32_RGBA = 25,
	/// 32-bit, 2-channel s10e5 floating point pixel format, 16-bit green, 16-bit red
	NXPF_FLOAT16_GR = 35,
	/// 64-bit, 2-channel floating point pixel format, 32-bit green, 32-bit red
	NXPF_FLOAT32_GR = 36,
	/// Depth texture format
	NXPF_DEPTH = 29,
	/// 64-bit pixel format, 16 bits for red, green, blue and alpha
	NXPF_SHORT_RGBA = 30,
	/// 32-bit pixel format, 16-bit green, 16-bit red
	NXPF_SHORT_GR = 34,
	/// 48-bit pixel format, 16 bits for red, green and blue
	NXPF_SHORT_RGB = 37,
	/// PVRTC (PowerVR) RGB 2 bpp
	NXPF_PVRTC_RGB2 = 38,
	/// PVRTC (PowerVR) RGBA 2 bpp
	NXPF_PVRTC_RGBA2 = 39,
	/// PVRTC (PowerVR) RGB 4 bpp
	NXPF_PVRTC_RGB4 = 40,
	/// PVRTC (PowerVR) RGBA 4 bpp
	NXPF_PVRTC_RGBA4 = 41,
	/// PVRTC (PowerVR) Version 2, 2 bpp
	NXPF_PVRTC2_2BPP = 42,
	/// PVRTC (PowerVR) Version 2, 4 bpp
	NXPF_PVRTC2_4BPP = 43,
	/// 32-bit pixel format, 11 bits (float) for red, 11 bits (float) for green, 10 bits (float) for blue
	NXPF_R11G11B10_FLOAT = 44,
	/// 8-bit pixel format, 8 bits red (unsigned int).
	NXPF_R8_UINT = 45,
	/// 16-bit pixel format, 8 bits red (unsigned int), 8 bits blue (unsigned int).
	NXPF_R8G8_UINT = 46,
	/// 24-bit pixel format, 8 bits red (unsigned int), 8 bits blue (unsigned int), 8 bits green (unsigned int).
	NXPF_R8G8B8_UINT = 47,
	/// 32-bit pixel format, 8 bits red (unsigned int), 8 bits blue (unsigned int), 8 bits green (unsigned int), 8 bits alpha (unsigned int).
	NXPF_R8G8B8A8_UINT = 48,
	/// 16-bit pixel format, 16 bits red (unsigned int).
	NXPF_R16_UINT = 49,
	/// 32-bit pixel format, 16 bits red (unsigned int), 16 bits blue (unsigned int).
	NXPF_R16G16_UINT = 50,
	/// 48-bit pixel format, 16 bits red (unsigned int), 16 bits blue (unsigned int), 16 bits green (unsigned int).
	NXPF_R16G16B16_UINT = 51,
	/// 64-bit pixel format, 16 bits red (unsigned int), 16 bits blue (unsigned int), 16 bits green (unsigned int), 16 bits alpha (unsigned int).
	NXPF_R16G16B16A16_UINT = 52,
	/// 32-bit pixel format, 32 bits red (unsigned int).
	NXPF_R32_UINT = 53,
	/// 64-bit pixel format, 32 bits red (unsigned int), 32 bits blue (unsigned int).
	NXPF_R32G32_UINT = 54,
	/// 96-bit pixel format, 32 bits red (unsigned int), 32 bits blue (unsigned int), 32 bits green (unsigned int).
	NXPF_R32G32B32_UINT = 55,
	/// 128-bit pixel format, 32 bits red (unsigned int), 32 bits blue (unsigned int), 32 bits green (unsigned int), 32 bits alpha (unsigned int).
	NXPF_R32G32B32A32_UINT = 56,
	/// 8-bit pixel format, 8 bits red (signed int).
	NXPF_R8_SINT = 57,
	/// 16-bit pixel format, 8 bits red (signed int), 8 bits blue (signed int).
	NXPF_R8G8_SINT = 58,
	/// 24-bit pixel format, 8 bits red (signed int), 8 bits blue (signed int), 8 bits green (signed int).
	NXPF_R8G8B8_SINT = 59,
	/// 32-bit pixel format, 8 bits red (signed int), 8 bits blue (signed int), 8 bits green (signed int), 8 bits alpha (signed int).
	NXPF_R8G8B8A8_SINT = 60,
	/// 16-bit pixel format, 16 bits red (signed int).
	NXPF_R16_SINT = 61,
	/// 32-bit pixel format, 16 bits red (signed int), 16 bits blue (signed int).
	NXPF_R16G16_SINT = 62,
	/// 48-bit pixel format, 16 bits red (signed int), 16 bits blue (signed int), 16 bits green (signed int).
	NXPF_R16G16B16_SINT = 63,
	/// 64-bit pixel format, 16 bits red (signed int), 16 bits blue (signed int), 16 bits green (signed int), 16 bits alpha (signed int).
	NXPF_R16G16B16A16_SINT = 64,
	/// 32-bit pixel format, 32 bits red (signed int).
	NXPF_R32_SINT = 65,
	/// 64-bit pixel format, 32 bits red (signed int), 32 bits blue (signed int).
	NXPF_R32G32_SINT = 66,
	/// 96-bit pixel format, 32 bits red (signed int), 32 bits blue (signed int), 32 bits green (signed int).
	NXPF_R32G32B32_SINT = 67,
	/// 128-bit pixel format, 32 bits red (signed int), 32 bits blue (signed int), 32 bits green (signed int), 32 bits alpha (signed int).
	NXPF_R32G32B32A32_SINT = 68,
	/// 32-bit pixel format, 9 bits for blue, green, red plus a 5 bit exponent.
	NXPF_R9G9B9E5_SHAREDEXP = 69,
	/// DDS (DirectDraw Surface) BC4 format (unsigned normalised)
	NXPF_BC4_UNORM = 70,
	/// DDS (DirectDraw Surface) BC4 format (signed normalised)
	NXPF_BC4_SNORM = 71,
	/// DDS (DirectDraw Surface) BC5 format (unsigned normalised)
	NXPF_BC5_UNORM = 72,
	/// DDS (DirectDraw Surface) BC5 format (signed normalised)
	NXPF_BC5_SNORM = 73,
	/// DDS (DirectDraw Surface) BC6H format (unsigned 16 bit float)
	NXPF_BC6H_UF16 = 74,
	/// DDS (DirectDraw Surface) BC6H format (signed 16 bit float)
	NXPF_BC6H_SF16 = 75,
	/// DDS (DirectDraw Surface) BC7 format (unsigned normalised)
	NXPF_BC7_UNORM = 76,
	/// DDS (DirectDraw Surface) BC7 format (unsigned normalised sRGB)
	NXPF_BC7_UNORM_SRGB = 77,
	/// 8-bit pixel format, all bits red.
	NXPF_R8 = 78,
	/// 16-bit pixel format, 8 bits red, 8 bits green.
	NXPF_RG8 = 79,
	/// 8-bit pixel format, 8 bits red (signed normalised int).
	NXPF_R8_SNORM = 80,
	/// 16-bit pixel format, 8 bits red (signed normalised int), 8 bits blue (signed normalised int).
	NXPF_R8G8_SNORM = 81,
	/// 24-bit pixel format, 8 bits red (signed normalised int), 8 bits blue (signed normalised int), 8 bits green (signed normalised int).
	NXPF_R8G8B8_SNORM = 82,
	/// 32-bit pixel format, 8 bits red (signed normalised int), 8 bits blue (signed normalised int), 8 bits green (signed normalised int), 8 bits alpha (signed normalised int).
	NXPF_R8G8B8A8_SNORM = 83,
	/// 16-bit pixel format, 16 bits red (signed normalised int).
	NXPF_R16_SNORM = 84,
	/// 32-bit pixel format, 16 bits red (signed normalised int), 16 bits blue (signed normalised int).
	NXPF_R16G16_SNORM = 85,
	/// 48-bit pixel format, 16 bits red (signed normalised int), 16 bits blue (signed normalised int), 16 bits green (signed normalised int).
	NXPF_R16G16B16_SNORM = 86,
	/// 64-bit pixel format, 16 bits red (signed normalised int), 16 bits blue (signed normalised int), 16 bits green (signed normalised int), 16 bits alpha (signed normalised int).
	NXPF_R16G16B16A16_SNORM = 87,
	/// ETC1 (Ericsson Texture Compression)
	NXPF_ETC1_RGB8 = 88,
	/// ETC2 (Ericsson Texture Compression)
	NXPF_ETC2_RGB8 = 89,
	/// ETC2 (Ericsson Texture Compression)
	NXPF_ETC2_RGBA8 = 90,
	/// ETC2 (Ericsson Texture Compression)
	NXPF_ETC2_RGB8A1 = 91,
	/// ATC (AMD_compressed_ATC_texture)
	NXPF_ATC_RGB = 92,
	/// ATC (AMD_compressed_ATC_texture)
	NXPF_ATC_RGBA_EXPLICIT_ALPHA = 93,
	/// ATC (AMD_compressed_ATC_texture)
	NXPF_ATC_RGBA_INTERPOLATED_ALPHA = 94,
	// Number of pixel formats currently defined
	NXPF_COUNT = 95
};

 

enum NxBodyFlags
{
	/**
	\brief Set if gravity should not be applied on this body

	@see NxBodyDesc.flags NxScene.setGravity()
	*/
	NxBodyFlagsDisableGravity	= (1<<0),
	
	/**	
	\brief Enable/disable freezing for this body/actor. 

	\note This is an EXPERIMENTAL feature which doesn't always work on in all situations, e.g. 
	for actors which have joints connected to them.
	
	To freeze an actor is a way to simulate that it is static. The actor is however still simulated
	as if it was dynamic, it's position is just restored after the simulation has finished. A much
	more stable way to make an actor temporarily static is to raise the NX_BF_KINEMATIC flag.
	*/
	NxBodyFlagsFROZEN_POS_X		= (1<<1),
	NxBodyFlagsFROZEN_POS_Y		= (1<<2),
	NxBodyFlagsFROZEN_POS_Z		= (1<<3),
	NxBodyFlagsFROZEN_ROT_X		= (1<<4),
	NxBodyFlagsFROZEN_ROT_Y		= (1<<5),
	NxBodyFlagsFROZEN_ROT_Z		= (1<<6),
	NxBodyFlagsFROZEN_POS		= NxBodyFlagsFROZEN_POS_X|NxBodyFlagsFROZEN_POS_Y|NxBodyFlagsFROZEN_POS_Z,
	NxBodyFlagsFROZEN_ROT		= NxBodyFlagsFROZEN_ROT_X|NxBodyFlagsFROZEN_ROT_Y|NxBodyFlagsFROZEN_ROT_Z,
	NxBodyFlagsFROZEN			= NxBodyFlagsFROZEN_POS|NxBodyFlagsFROZEN_ROT,


	/**
	\brief Enables kinematic mode for the actor.
	
	Kinematic actors are special dynamic actors that are not 
	influenced by forces (such as gravity), and have no momentum. They are considered to have infinite
	mass and can be moved around the world using the moveGlobal*() methods. They will push 
	regular dynamic actors out of the way. Kinematics will not collide with static or other kinematic objects.
	
	Kinematic actors are great for moving platforms or characters, where direct motion control is desired.

	You can not connect Reduced joints to kinematic actors. Lagrange joints work ok if the platform
	is moving with a relatively low, uniform velocity.

	@see NxActor NxActor.raiseActorFlag()
	*/
	NxBodyFlagsKINEMATIC			= (1<<7),		//!< Enable kinematic mode for the body.

	/**
	\brief Enable debug renderer for this body

	@see NxScene.getDebugRenderable() NxDebugRenderable NxParameter
	*/
	NxBodyFlagsVISUALIZATION		= (1<<8),

	NxBodyFlagsDUMMY_0			= (1<<9), // deprecated flag placeholder

	/**
	\brief Filter velocities used keep body awake. The filter reduces rapid oscillations and transient spikes.
	@see NxActor.isSleeping()
	*/
	NxBodyFlagsFILTER_SLEEP_VEL	= (1<<10),

	/**
	\brief Enables energy-based sleeping algorithm.
	@see NxActor.isSleeping() NxBodyDesc.sleepEnergyThreshold 
	*/
	NxBodyFlagsENERGY_SLEEP_TEST	= (1<<11)
};

enum NxContactPairFlags
{
	NxContactPairFlagsIGNORE_PAIR								= (1<<0),	//!< Disable contact generation for this pair

	NxContactPairFlagsNOTIFY_ON_START_TOUCH					= (1<<1),	//!< Pair callback will be called when the pair starts to be in contact
	NxContactPairFlagsNOTIFY_ON_END_TOUCH						= (1<<2),	//!< Pair callback will be called when the pair stops to be in contact
	NxContactPairFlagsNOTIFY_ON_TOUCH							= (1<<3),	//!< Pair callback will keep getting called while the pair is in contact
	NxContactPairFlagsNOTIFY_ON_IMPACT							= (1<<4),	//!< [Not yet implemented] pair callback will be called when it may be appropriate for the pair to play an impact sound
	NxContactPairFlagsNOTIFY_ON_ROLL							= (1<<5),	//!< [Not yet implemented] pair callback will be called when the pair is in contact and rolling.
	NxContactPairFlagsNOTIFY_ON_SLIDE							= (1<<6),	//!< [Not yet implemented] pair callback will be called when the pair is in contact and sliding (and not rolling).
	NxContactPairFlagsNOTIFY_FORCES							= (1<<7),	//!< The (summed total) friction force and normal force will be given in the NxContactPair variable in the contact report.
	NxContactPairFlagsNOTIFY_ON_START_TOUCH_FORCE_THRESHOLD	= (1<<8),	//!< Pair callback will be called when the contact force between two actors exceeds one of the actor-defined force thresholds
	NxContactPairFlagsNOTIFY_ON_END_TOUCH_FORCE_THRESHOLD		= (1<<9),	//!< Pair callback will be called when the contact force between two actors falls below the actor-defined force thresholds
	NxContactPairFlagsNOTIFY_ON_TOUCH_FORCE_THRESHOLD			= (1<<10),	//!< Pair callback will keep getting called while the contact force between two actors exceeds one of the actor-defined force thresholds

	NxContactPairFlagsNOTIFY_CONTACT_MODIFICATION				= (1<<16),	//!< Generate a callback for all associated contact constraints, making it possible to edit the constraint. This flag is not included in NX_NOTIFY_ALL for performance reasons. \see NxUserContactModify

	NxContactPairFlagsNOTIFY_ALL								= (NxContactPairFlagsNOTIFY_ON_START_TOUCH|NxContactPairFlagsNOTIFY_ON_END_TOUCH|NxContactPairFlagsNOTIFY_ON_TOUCH|NxContactPairFlagsNOTIFY_ON_IMPACT|NxContactPairFlagsNOTIFY_ON_ROLL|NxContactPairFlagsNOTIFY_ON_SLIDE|NxContactPairFlagsNOTIFY_FORCES|
	NxContactPairFlagsNOTIFY_ON_START_TOUCH_FORCE_THRESHOLD|NxContactPairFlagsNOTIFY_ON_END_TOUCH_FORCE_THRESHOLD|NxContactPairFlagsNOTIFY_ON_TOUCH_FORCE_THRESHOLD)
};




/*! 3D scenes types */
enum NxGraphics_3D_Scene_Types
{ 
	Nx3D_Scene_Ogremax,/*!< OgreMax .scene format */
	Nx3D_Scene_Default/*!< Empty scene */  
};

/*! 3D Objects types */
enum Nx3DSceneObjectTypes 
{ 
	Nx3DObjectBox,
	Nx3DObjectSphere,
	Nx3DObjectCapsule,
	Nx3DObjectCone,
	Nx3DObjectPatch
};


enum NxFastCornerDetectionType {
	NxFast9,
	NxFast10,
	NxFast11,
	NxFast12,
	NxFastNonMax
};

typedef int NxTrackVertexColourType;
enum NxTrackVertexColourEnum {
	NxVertexColour_NONE        = 0x0,
	NxVertexColour_AMBIENT     = 0x1,        
	NxVertexColour_DIFFUSE     = 0x2,
	NxVertexColour_SPECULAR    = 0x4,
	NxVertexColour_EMISSIVE    = 0x8
};

enum NxSceneBlendType {
	/// Make the object transparent based on the final alpha values in the texture
	NXBLEND_TRANSPARENT_ALPHA,
	/// Make the object transparent based on the colour values in the texture (brighter = more opaque)
	NXBLEND_TRANSPARENT_COLOUR,
	/// Add the texture values to the existing scene content
	NXBLEND_ADD,
	/// Multiply the 2 colours together
	NXBLEND_MODULATE,
	/// The default blend mode where source replaces destination
	NXBLEND_REPLACE
	// TODO : more
};

enum NxTextureFilterOptions {
	/// Equal to: min=FO_POINT, mag=FO_POINT, mip=FO_NONE
	TFO_NONE,
	/// Equal to: min=FO_LINEAR, mag=FO_LINEAR, mip=FO_POINT
	TFO_BILINEAR,
	/// Equal to: min=FO_LINEAR, mag=FO_LINEAR, mip=FO_LINEAR
	TFO_TRILINEAR,
	/// Equal to: min=FO_ANISOTROPIC, max=FO_ANISOTROPIC, mip=FO_LINEAR
	TFO_ANISOTROPIC
};

enum NxCompareFunction
{
	NXCF_ALWAYS_FAIL,
	NXCF_ALWAYS_PASS,
	NXCF_LESS,
	NXCF_LESS_EQUAL,
	NXCF_EQUAL,
	NXCF_NOT_EQUAL,
	NXCF_GREATER_EQUAL,
	NXCF_GREATER
};

enum NxSceneBlendFactor
{
    NXBF_ONE,
    NXBF_ZERO,
    NXBF_DEST_COLOUR,
    NXBF_SOURCE_COLOUR,
    NXBF_ONE_MINUS_DEST_COLOUR,
    NXBF_ONE_MINUS_SOURCE_COLOUR,
    NXBF_DEST_ALPHA,
    NXBF_SOURCE_ALPHA,
    NXBF_ONE_MINUS_DEST_ALPHA,
    NXBF_ONE_MINUS_SOURCE_ALPHA

};

/** Blending operations controls how objects are blended into the scene. The default operation
	is add (+) but by changing this you can change how drawn objects are blended into the
	existing scene.
*/
enum NxSceneBlendOperation
{
	NXBO_ADD,
	NXBO_SUBTRACT,
	NXBO_REVERSE_SUBTRACT,
	NXBO_MIN,
	NXBO_MAX
};


enum NxFilterType
{
	/// The filter used when shrinking a texture
	NXFT_MIN,
	/// The filter used when magnifying a texture
	NXFT_MAG,
	/// The filter used when determining the mipmap
	NXFT_MIP
};

/** Filtering options for textures / mipmaps. */
enum NxFilterOptions
{
	/// No filtering, used for FT_MIP to turn off mipmapping
	NXFO_NONE,
	/// Use the closest pixel
	NXFO_POINT,
	/// Average of a 2x2 pixel area, denotes bilinear for MIN and MAG, trilinear for MIP
	NXFO_LINEAR,
	/// Similar to FO_LINEAR, but compensates for the angle of the texture plane
	NXFO_ANISOTROPIC
};


	    enum NxUsage 
		    {
                /** Static buffer which the application rarely modifies once created. Modifying 
                the contents of this buffer will involve a performance hit.
                */
                NXHBU_STATIC = 1,
			    /** Indicates the application would like to modify this buffer with the CPU
			    fairly often. 
			    Buffers created with this flag will typically end up in AGP memory rather 
			    than video memory.
			    */
			    NXHBU_DYNAMIC = 2,
			    /** Indicates the application will never read the contents of the buffer back, 
			    it will only ever write data. Locking a buffer with this flag will ALWAYS 
			    return a pointer to new, blank memory rather than the memory associated 
			    with the contents of the buffer; this avoids DMA stalls because you can 
			    write to a new memory area while the previous one is being used. 
			    */
			    NXHBU_WRITE_ONLY = 4,
                /** Indicates that the application will be refilling the contents
                of the buffer regularly (not just updating, but generating the
                contents from scratch), and therefore does not mind if the contents 
                of the buffer are lost somehow and need to be recreated. This
                allows and additional level of optimisation on the buffer.
                This option only really makes sense when combined with 
                HBU_DYNAMIC_WRITE_ONLY.
                */
                NXHBU_DISCARDABLE = 8,
				/// Combination of HBU_STATIC and HBU_WRITE_ONLY
				NXHBU_STATIC_WRITE_ONLY = 5, 
				/** Combination of HBU_DYNAMIC and HBU_WRITE_ONLY. If you use 
                this, strongly consider using HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE
                instead if you update the entire contents of the buffer very 
                regularly. 
                */
				NXHBU_DYNAMIC_WRITE_ONLY = 6,
                /// Combination of HBU_DYNAMIC, HBU_WRITE_ONLY and HBU_DISCARDABLE
                NXHBU_DYNAMIC_WRITE_ONLY_DISCARDABLE = 14


		    };

enum NxTextureUsage
    {
		/// @copydoc HardwareBuffer::Usage
		NXTU_STATIC = NXHBU_STATIC,
		NXTU_DYNAMIC = NXHBU_DYNAMIC,
		NXTU_WRITE_ONLY = NXHBU_WRITE_ONLY,
		NXTU_STATIC_WRITE_ONLY = NXHBU_STATIC_WRITE_ONLY, 
		NXTU_DYNAMIC_WRITE_ONLY = NXHBU_DYNAMIC_WRITE_ONLY,
		NXTU_DYNAMIC_WRITE_ONLY_DISCARDABLE = NXHBU_DYNAMIC_WRITE_ONLY_DISCARDABLE,
		/// Mipmaps will be automatically generated for this texture
		NXTU_AUTOMIPMAP = 16,
		/** This texture will be a render target, i.e. used as a target for render to texture
		    setting this flag will ignore all other texture usages except TU_AUTOMIPMAP */
		NXTU_RENDERTARGET = 32,
		/// Default to automatic mipmap generation static textures
		NXTU_DEFAULT = NXTU_AUTOMIPMAP | NXTU_STATIC_WRITE_ONLY
    };


enum NxTextureType
{
	/// 1D texture, used in combination with 1D texture coordinates
	NXTEX_TYPE_1D = 1,
	/// 2D texture, used in combination with 2D texture coordinates (default)
	NXTEX_TYPE_2D = 2,
	/// 3D volume texture, used in combination with 3D texture coordinates
	NXTEX_TYPE_3D = 3,
	/// 3D cube map, used in combination with 3D texture coordinates
	NXTEX_TYPE_CUBE_MAP = 4,
	/// 2D texture array
	NXTEX_TYPE_2D_ARRAY = 5,
	/// 2D texture array
	TEX_TYPE_2D_RECT = 6
};
 
enum NxEnvMapType
{
	/// Envmap based on vector from camera to vertex position, good for planar geometry
	ENV_PLANAR,
	/// Envmap based on dot of vector from camera to vertex and vertex normal, good for curves
	ENV_CURVED,
	/// Envmap intended to supply reflection vectors for cube mapping
	ENV_REFLECTION,
	/// Envmap intended to supply normal vectors for cube mapping
	ENV_NORMAL
};

enum NxTextureAddressingMode
{
	/// Texture wraps at values over 1.0
	TEXTURE_WRAP,
	/// Texture mirrors (flips) at joins over 1.0
	TEXTURE_MIRROR,
	/// Texture clamps at 1.0
	TEXTURE_CLAMP,
	/// Texture coordinates outside the range [0.0, 1.0] are set to the border colour
	TEXTURE_BORDER
};

enum NxCullingMode
{
	/// Hardware never culls triangles and renders everything it receives.
	NXCULL_NONE = 1,
	/// Hardware culls triangles whose vertices are listed clockwise in the view (default).
	NXCULL_CLOCKWISE = 2,
	/// Hardware culls triangles whose vertices are listed anticlockwise in the view.
	NXCULL_ANTICLOCKWISE = 3
};


enum NxManualCullingMode
{
	/// No culling so everything is sent to the hardware.
	NXMANUAL_CULL_NONE = 1,
	/// Cull triangles whose normal is pointing away from the camera (default).
	NXMANUAL_CULL_BACK = 2,
	/// Cull triangles whose normal is pointing towards the camera.
	NXMANUAL_CULL_FRONT = 3
};


enum NxLayerBlendOperation {
	NXLBO_REPLACE,
	NXLBO_ADD,
	NXLBO_MODULATE,
	NXLBO_ALPHA_BLEND
};

enum NxLayerBlendOperationEx {
	NXLBX_SOURCE1,
	NXLBX_SOURCE2,
	NXLBX_MODULATE,
	NXLBX_MODULATE_X2,
	NXLBX_MODULATE_X4,
	NXLBX_ADD,
	NXLBX_ADD_SIGNED,
	NXLBX_ADD_SMOOTH,
	NXLBX_SUBTRACT,
	NXLBX_BLEND_DIFFUSE_ALPHA,
	NXLBX_BLEND_TEXTURE_ALPHA,
	NXLBX_BLEND_CURRENT_ALPHA,
	NXLBX_BLEND_MANUAL,
	NXLBX_DOTPRODUCT,
	NXLBX_BLEND_DIFFUSE_COLOUR
};

enum NxLayerBlendSource {
	NXLBS_CURRENT,
	NXLBS_TEXTURE,
	NXLBS_DIFFUSE,
	NXLBS_SPECULAR,
	NXLBS_MANUAL
};

enum NxAutoConstantType {
	ACT_WORLD_MATRIX,
	ACT_INVERSE_WORLD_MATRIX,
	ACT_TRANSPOSE_WORLD_MATRIX,
	ACT_INVERSE_TRANSPOSE_WORLD_MATRIX,
	
	ACT_WORLD_MATRIX_ARRAY_3x4,
	ACT_WORLD_MATRIX_ARRAY,
	ACT_WORLD_DUALQUATERNION_ARRAY_2x4,
	ACT_WORLD_SCALE_SHEAR_MATRIX_ARRAY_3x4,
	
	ACT_VIEW_MATRIX,
	ACT_INVERSE_VIEW_MATRIX,
	ACT_TRANSPOSE_VIEW_MATRIX,
	ACT_INVERSE_TRANSPOSE_VIEW_MATRIX,
	
	
	ACT_PROJECTION_MATRIX,
	ACT_INVERSE_PROJECTION_MATRIX,
	ACT_TRANSPOSE_PROJECTION_MATRIX,
	ACT_INVERSE_TRANSPOSE_PROJECTION_MATRIX,
	
	
	ACT_VIEWPROJ_MATRIX,
	ACT_INVERSE_VIEWPROJ_MATRIX,
	ACT_TRANSPOSE_VIEWPROJ_MATRIX,
	ACT_INVERSE_TRANSPOSE_VIEWPROJ_MATRIX,
	
	
	ACT_WORLDVIEW_MATRIX,
	ACT_INVERSE_WORLDVIEW_MATRIX,
	ACT_TRANSPOSE_WORLDVIEW_MATRIX,
	ACT_INVERSE_TRANSPOSE_WORLDVIEW_MATRIX,
	
 
	ACT_WORLDVIEWPROJ_MATRIX,
	ACT_INVERSE_WORLDVIEWPROJ_MATRIX,
	ACT_TRANSPOSE_WORLDVIEWPROJ_MATRIX,
	ACT_INVERSE_TRANSPOSE_WORLDVIEWPROJ_MATRIX,
	
	
	
	ACT_RENDER_TARGET_FLIPPING,
	
	ACT_VERTEX_WINDING,
	
	ACT_FOG_COLOUR,
	ACT_FOG_PARAMS,
	
	
	ACT_SURFACE_AMBIENT_COLOUR,
	ACT_SURFACE_DIFFUSE_COLOUR,
	ACT_SURFACE_SPECULAR_COLOUR,
	ACT_SURFACE_EMISSIVE_COLOUR,
	ACT_SURFACE_SHININESS,
	ACT_SURFACE_ALPHA_REJECTION_VALUE,
	
	
	ACT_LIGHT_COUNT,
	
	
	ACT_AMBIENT_LIGHT_COLOUR,
	
	ACT_LIGHT_DIFFUSE_COLOUR,
	ACT_LIGHT_SPECULAR_COLOUR,
	ACT_LIGHT_ATTENUATION,
	ACT_SPOTLIGHT_PARAMS,
	ACT_LIGHT_POSITION,
	ACT_LIGHT_POSITION_OBJECT_SPACE,
	ACT_LIGHT_POSITION_VIEW_SPACE,
	ACT_LIGHT_DIRECTION,
	ACT_LIGHT_DIRECTION_OBJECT_SPACE,
	ACT_LIGHT_DIRECTION_VIEW_SPACE,
	ACT_LIGHT_DISTANCE_OBJECT_SPACE,
	ACT_LIGHT_POWER_SCALE,
	 ACT_LIGHT_DIFFUSE_COLOUR_POWER_SCALED,
	 ACT_LIGHT_SPECULAR_COLOUR_POWER_SCALED,
	ACT_LIGHT_DIFFUSE_COLOUR_ARRAY,
	ACT_LIGHT_SPECULAR_COLOUR_ARRAY,
	ACT_LIGHT_DIFFUSE_COLOUR_POWER_SCALED_ARRAY,
	ACT_LIGHT_SPECULAR_COLOUR_POWER_SCALED_ARRAY,
	ACT_LIGHT_ATTENUATION_ARRAY,
	ACT_LIGHT_POSITION_ARRAY,
	ACT_LIGHT_POSITION_OBJECT_SPACE_ARRAY,
	ACT_LIGHT_POSITION_VIEW_SPACE_ARRAY,
	ACT_LIGHT_DIRECTION_ARRAY,
	ACT_LIGHT_DIRECTION_OBJECT_SPACE_ARRAY,
	ACT_LIGHT_DIRECTION_VIEW_SPACE_ARRAY,
	ACT_LIGHT_DISTANCE_OBJECT_SPACE_ARRAY,
	ACT_LIGHT_POWER_SCALE_ARRAY,
	ACT_SPOTLIGHT_PARAMS_ARRAY,
	
	ACT_DERIVED_AMBIENT_LIGHT_COLOUR,
	ACT_DERIVED_SCENE_COLOUR,
	 
	ACT_DERIVED_LIGHT_DIFFUSE_COLOUR,
	ACT_DERIVED_LIGHT_SPECULAR_COLOUR,
	 
	ACT_DERIVED_LIGHT_DIFFUSE_COLOUR_ARRAY,
	ACT_DERIVED_LIGHT_SPECULAR_COLOUR_ARRAY,
	ACT_LIGHT_NUMBER,
	ACT_LIGHT_CASTS_SHADOWS,
	ACT_LIGHT_CASTS_SHADOWS_ARRAY,
	
	
	ACT_SHADOW_EXTRUSION_DISTANCE,
	ACT_CAMERA_POSITION,
	ACT_CAMERA_POSITION_OBJECT_SPACE,
	ACT_TEXTURE_VIEWPROJ_MATRIX,
	ACT_TEXTURE_VIEWPROJ_MATRIX_ARRAY,
	ACT_TEXTURE_WORLDVIEWPROJ_MATRIX,
	ACT_TEXTURE_WORLDVIEWPROJ_MATRIX_ARRAY,
	ACT_SPOTLIGHT_VIEWPROJ_MATRIX,
	ACT_SPOTLIGHT_VIEWPROJ_MATRIX_ARRAY,
	ACT_SPOTLIGHT_WORLDVIEWPROJ_MATRIX,
	ACT_SPOTLIGHT_WORLDVIEWPROJ_MATRIX_ARRAY,
	 ACT_CUSTOM,
	ACT_TIME,
	ACT_TIME_0_X,
	ACT_COSTIME_0_X,
	 ACT_SINTIME_0_X,
	ACT_TANTIME_0_X,
	ACT_TIME_0_X_PACKED,
	ACT_TIME_0_1,
	ACT_COSTIME_0_1,
	ACT_SINTIME_0_1,
	ACT_TANTIME_0_1,
	ACT_TIME_0_1_PACKED,
	ACT_TIME_0_2PI,
	ACT_COSTIME_0_2PI,
	 ACT_SINTIME_0_2PI,
	ACT_TANTIME_0_2PI,
	ACT_TIME_0_2PI_PACKED,
	 ACT_FRAME_TIME,
	 ACT_FPS,
	
	ACT_VIEWPORT_WIDTH,
	ACT_VIEWPORT_HEIGHT,
	ACT_INVERSE_VIEWPORT_WIDTH,
	ACT_INVERSE_VIEWPORT_HEIGHT,
	ACT_VIEWPORT_SIZE,
	
	 
	ACT_VIEW_DIRECTION,
	ACT_VIEW_SIDE_VECTOR,
	ACT_VIEW_UP_VECTOR,
	ACT_FOV,
	ACT_NEAR_CLIP_DISTANCE,
	ACT_FAR_CLIP_DISTANCE,
	
	ACT_PASS_NUMBER,
	
	ACT_PASS_ITERATION_NUMBER,
		
		
	 ACT_ANIMATION_PARAMETRIC,
		 
	ACT_TEXEL_OFFSETS,
	 
	ACT_SCENE_DEPTH_RANGE,

	ACT_SHADOW_SCENE_DEPTH_RANGE,
	
	ACT_SHADOW_SCENE_DEPTH_RANGE_ARRAY,
	
	 ACT_SHADOW_COLOUR,
	ACT_TEXTURE_SIZE,
	 ACT_INVERSE_TEXTURE_SIZE,
	ACT_PACKED_TEXTURE_SIZE,
	
	ACT_TEXTURE_MATRIX,
	 
	ACT_LOD_CAMERA_POSITION,
	ACT_LOD_CAMERA_POSITION_OBJECT_SPACE,
	ACT_LIGHT_CUSTOM,
	
	ACT_UNKNOWN = 999
 };




enum NxGraphics_Plugin_Effect_Types
{
	TYPE_FLASH,
	TYPE_VIDEO,
	TYPE_DMX,
	TYPE_PLUGIN_3D,
	TYPE_PLUGIN_AUDIO 
};

enum NxGraphics_Plugin_Effect_SubTypes
{ 
	TYPE_FLASH_VARS ,
	TYPE_FLASH_BACKGROUND ,
	TYPE_FLASH_VARIABLE_VALUE ,
	TYPE_DMX_PIXELS_OUT,
	TYPE_VIDEO_PIXELBUFFER ,
	TYPE_3D_TWIST,
	TYPE_AUDIO_VST,
	TYPE_AUDIO_EFFECT,
	TYPE_VIDEO_FREEFRAME
};

enum NxGraphics_Effects_Library_Types
{
	Effect_COLOR_SPACE,
	Effect_SLIT_SCAN,
	//Effect_FREEFRAME,
	Effect_OPENCV_TRACKER,
	Effect_OPENCV_DIFF,
	Effect_OPENCV_CONTOUR,
	Effect3D_TWIST,
	Effect3D_RIBBON,
	Effect3D_CLOTH,
	Effect3D_STEREO,
	Effect_FaceTrack,
	Effect_DMX_PIXEL_OUT,
	Effect3D_PERLIN
};

enum NxGraphics_Layer_Types
{ 
	TYPE_VIDEO_CAPTURE,
	TYPE_3D,
	TYPE_IMAGE,
	TYPE_AVI,
	TYPE_FLASH_FILE,
	TYPE_FLASH_FILE_FLV,
	TYPE_FLASH_CREATOR,
	TYPE_STREAM_JPEG,
	TYPE_STREAM_HTTP,
	TYPE_STREAM_YOUTUBE,
	TYPE_IMAGE_ANIM,
	TYPE_AUDIO,
	TYPE_CANVAS,
	TYPE_BUFFER
};



#endif