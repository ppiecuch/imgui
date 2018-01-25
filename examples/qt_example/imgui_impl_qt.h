// ImGui Qt binding with OpenGL
// You can copy and use unmodified imgui_impl_* files in your project. See main.cpp for an example of using this.
// If you use this binding you'll need to call 4 functions: ImGui_ImplXXXX_Init(), ImGui_ImplXXXX_NewFrame(), ImGui::Render() and ImGui_ImplXXXX_Shutdown().
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.
// https://github.com/ocornut/imgui

class QImgui;

extern bool qtGetWindowAttrib(QImgui *window, int attr);
extern void qtGetWindowSize(QImgui *window, int *w, int *h);
extern void qtGetFramebufferSize(QImgui *window, int *w, int *h);
extern void qtGetCursorPos(QImgui *window, double *mx, double *my);
extern int qtGetMouseButton(QImgui *window, int button);
extern void qtSetInputMode(QImgui *window, int input, int mode);
extern double qtGetTime();


IMGUI_API bool        ImGui_ImplQt_Init(QImgui* window);
IMGUI_API void        ImGui_ImplQt_Shutdown();
IMGUI_API void        ImGui_ImplQt_NewFrame();

// Use if you want to reset your rendering device without losing ImGui state.
IMGUI_API void        ImGui_ImplQt_InvalidateDeviceObjects();
IMGUI_API bool        ImGui_ImplQt_CreateDeviceObjects();

// Qt callbacks
// Provided here if you want to chain callbacks.
// You can also handle inputs yourself and use those as a reference.
IMGUI_API void        ImGui_ImplQt_MouseButtonCallback(QImgui* window, int button, int action, int mods);
IMGUI_API void        ImGui_ImplQt_ScrollCallback(QImgui* window, double xoffset, double yoffset);
IMGUI_API void        ImGui_ImplQt_KeyCallback(QImgui* window, int key, int scancode, int action, int mods);
IMGUI_API void        ImGui_ImplQt_CharCallback(QImgui* window, unsigned int c);

enum {
	QT_ERR = 0,
	QT_OK = 1,

	QT_RELEASE = 0,
	QT_PRESS,
	QT_CURSOR,
	QT_CURSOR_HIDDEN,
	QT_CURSOR_NORMAL,
	QT_FOCUSED
};

enum {
	QT_KEY_NONE,
	QT_KEY_LEFT_CONTROL,
	QT_KEY_RIGHT_CONTROL,
	QT_KEY_LEFT_SHIFT,
	QT_KEY_RIGHT_SHIFT,
	QT_KEY_LEFT_ALT,
	QT_KEY_RIGHT_ALT,
	QT_KEY_LEFT,
	QT_KEY_RIGHT,
	QT_KEY_UP,
	QT_KEY_DOWN,
	QT_KEY_PAGE_UP,
	QT_KEY_PAGE_DOWN,
	QT_KEY_DELETE,
	QT_KEY_HOME,
	QT_KEY_END,
	QT_KEY_TAB,
	QT_KEY_ESCAPE,
	QT_KEY_BACKSPACE,
	QT_KEY_ENTER,
	QT_KEY_A,
	QT_KEY_C,
	QT_KEY_V,
	QT_KEY_X,
	QT_KEY_Y,
	QT_KEY_Z
};
