#ifndef PLATFORM_H
#define PLATFORM_H

// Functions that's different on glfw/win etc. but have same signature (but if a function only used in win.cpp you can keep in win.h)

#include "rwcore.h"
#include "skeleton.h"

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

#define GLFW_GAMEPAD_BUTTON_A               0
#define GLFW_GAMEPAD_BUTTON_B               1
#define GLFW_GAMEPAD_BUTTON_X               2
#define GLFW_GAMEPAD_BUTTON_Y               3
#define GLFW_GAMEPAD_BUTTON_LEFT_BUMPER     4
#define GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER    5
#define GLFW_GAMEPAD_BUTTON_BACK            6
#define GLFW_GAMEPAD_BUTTON_START           7
#define GLFW_GAMEPAD_BUTTON_GUIDE           8
#define GLFW_GAMEPAD_BUTTON_LEFT_THUMB      9
#define GLFW_GAMEPAD_BUTTON_RIGHT_THUMB     10
#define GLFW_GAMEPAD_BUTTON_DPAD_UP         11
#define GLFW_GAMEPAD_BUTTON_DPAD_RIGHT      12
#define GLFW_GAMEPAD_BUTTON_DPAD_DOWN       13
#define GLFW_GAMEPAD_BUTTON_DPAD_LEFT       14
#define GLFW_GAMEPAD_BUTTON_LAST            GLFW_GAMEPAD_BUTTON_DPAD_LEFT

#define GLFW_GAMEPAD_BUTTON_CROSS       GLFW_GAMEPAD_BUTTON_A
#define GLFW_GAMEPAD_BUTTON_CIRCLE      GLFW_GAMEPAD_BUTTON_B
#define GLFW_GAMEPAD_BUTTON_SQUARE      GLFW_GAMEPAD_BUTTON_X
#define GLFW_GAMEPAD_BUTTON_TRIANGLE    GLFW_GAMEPAD_BUTTON_Y

#define GLFW_GAMEPAD_AXIS_LEFT_X        0
#define GLFW_GAMEPAD_AXIS_LEFT_Y        1
#define GLFW_GAMEPAD_AXIS_RIGHT_X       2
#define GLFW_GAMEPAD_AXIS_RIGHT_Y       3
#define GLFW_GAMEPAD_AXIS_LEFT_TRIGGER  4
#define GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER 5
#define GLFW_GAMEPAD_AXIS_LAST          GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER

#define GLFW_RELEASE                0
#define GLFW_PRESS                  1

typedef struct GLFWgamepadstate
{
    unsigned char buttons[15];
    float axes[6];
} GLFWgamepadstate;

unsigned char* glfwGetJoystickButtons(int jid, int* count);
float* glfwGetJoystickAxes(int jid, int* count);
int glfwGetGamepadState(int jid, GLFWgamepadstate* state);
int glfwJoystickPresent(int jid);
int glfwJoystickIsGamepad(int jid);

#ifdef _WIN32
extern RwUInt32 psTimer(void);
#else
extern double psTimer(void);
#endif

extern RwBool psInitialize(void);
extern void   psTerminate(void);

extern void psCameraShowRaster(RwCamera *camera);
extern RwBool psCameraBeginUpdate(RwCamera *camera);
extern RwImage *psGrabScreen(RwCamera *camera);

extern void psMouseSetPos(RwV2d *pos);

extern RwBool psSelectDevice();

extern RwMemoryFunctions *psGetMemoryFunctions(void);

/* install the platform specific file system */
extern RwBool psInstallFileSystem(void);


/* Handle native texture support */
extern RwBool psNativeTextureSupport(void);

extern void _InputTranslateShiftKeyUpDown(RsKeyCodes* rs);
extern long _InputInitialiseMouse(); // returns HRESULT on Windows actually
extern void _InputInitialiseJoys();

extern void HandleExit();

extern void _psSelectScreenVM(RwInt32 videoMode);

extern void InitialiseLanguage();

extern RwBool _psSetVideoMode(RwInt32 subSystem, RwInt32 videoMode);

extern RwChar** _psGetVideoModeList();

extern RwInt32 _psGetNumVideModes();
#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#endif /* PLATFORM_H */
