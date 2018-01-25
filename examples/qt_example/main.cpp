// ImGui - standalone example application for Qt + OpenGL 2, using fixed pipeline
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.

#include <imgui.h>
#include "imgui_impl_qt.h"
#include <stdio.h>

// Qt
#include <qelapsedtimer.h>
#include <qopenglcontext.h>
#include <qopenglpaintdevice.h>
#include <qpainter.h>
#include <qwindow.h>
#include <qopenglfunctions.h>
#include <qevent.h>
#include <qapplication.h>


class QImgui // Imgui interface
{
public:
    virtual QPoint cursorPos() = 0;
    virtual void setCursor(const QCursor &newCursor) = 0;
    virtual bool isActive() = 0;
    virtual qreal devicePixelRatio() = 0;
    virtual int mouseButtonState(int button) = 0;
    virtual int width() = 0;
    virtual int height() = 0;
};

class QtWindow : public QWindow, protected QOpenGLFunctions
{
    Q_OBJECT
    typedef void (^RenderBlock)();
public:
    class qtImgui : public QImgui
    {
        QPoint cursorPos() {}
        void setCursor(const QCursor &newCursor) {}
        bool isActive() {}
        qreal devicePixelRatio() {}
        int mouseButtonState(int button) {}
        int width() {}
        int height() {};
    } qtImguiWin;
private:
    bool m_update_pending;
    bool m_auto_refresh;
    bool m_should_close;
	bool m_done;
	QPoint m_cursor_pos;
	int m_last_mouse_button;

    QOpenGLContext *m_context;
    QOpenGLPaintDevice *m_device;
    RenderBlock m_render;
public:
    Q_PROPERTY(bool done READ done WRITE setDone)
    Q_PROPERTY(bool alwaysRefresh READ alwaysRefresh WRITE setAlwaysRefresh)
    Q_PROPERTY(bool shouldClose READ shouldClose WRITE setShouldClose)
    Q_PROPERTY(QPoint cursorPos READ cursorPos)
	
    bool alwaysRefresh() const { return m_auto_refresh; }
    void setAlwaysRefresh(bool alwaysRefresh) { m_auto_refresh = alwaysRefresh; }
	void setAutoRefresh(bool alwaysRefresh) { m_auto_refresh = alwaysRefresh; }

    bool shouldClose() const { return m_should_close; }
    void setShouldClose(bool shouldClose) { m_should_close = shouldClose; }

    QPoint cursorPos() const { return m_cursor_pos; }

	void setRenderBlock(RenderBlock r) { m_render = r; }

    bool done() const { return m_done; }
    void setDone(bool done) { m_done = done; }

    QImgui *imgui() { return &qtImguiWin; }
public:
	QtWindow(QWindow *parent = 0) : QWindow(parent)
    , m_update_pending(false)
    , m_auto_refresh(true)
    , m_should_close(true)
    , m_context(0)
    , m_device(0)
    , m_render(0)
    , m_done(false)
    , m_last_mouse_button(QT_RELEASE)
	{
    	setSurfaceType(QWindow::OpenGLSurface);
	}
	~QtWindow() { delete m_device; }

	void render(QPainter *painter) {
		Q_UNUSED(painter);
		if (m_render)
			m_render();
	}

	void initialize() {
		setTitle(QString("ImGui OpenGL2 example [Qt %1 - %2 (%3)]").arg(QT_VERSION_STR).arg((const char*)glGetString(GL_VERSION)).arg((const char*)glGetString(GL_RENDERER)));
    }
	void update() { renderLater(); }
	void render()
	{
	    if (!m_device)
	        m_device = new QOpenGLPaintDevice;
	
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	    m_device->setSize(size());
	
	    QPainter painter(m_device);
	    render(&painter);
	}
	void mousePressEvent(QMouseEvent *event)
	{
    	m_cursor_pos = QPoint(event->x(), event->y());
		Qt::KeyboardModifiers modifiers = event->modifiers();
		if (event->buttons() & Qt::LeftButton) {

		}
		m_last_mouse_button = QT_PRESS;
	}
	void mouseReleaseEvent(QMouseEvent *event)
	{
    	m_cursor_pos = QPoint(event->x(), event->y());
		Qt::KeyboardModifiers modifiers = event->modifiers();
		if (event->button() == Qt::LeftButton) {
			ImGui_ImplQt_MouseButtonCallback(imgui(), 0, m_cursor_pos.x(), m_cursor_pos.y());
		}
		m_last_mouse_button = QT_RELEASE;
	}
	void mouseMoveEvent(QMouseEvent *event)
	{
    	m_cursor_pos = QPoint(event->x(), event->y());
	}
	void keyPressEvent(QKeyEvent* event)
	{
	    switch(event->key())
	    {
	    	case Qt::Key_Escape:
	    		quit();
	        break;
	    	default:
	    		event->ignore();
	        break;
	    }
	}
	void quit() { m_done = true; }
	int mouseButtonState(int button) { return m_last_mouse_button; }
protected:
	void closeEvent(QCloseEvent *event) { if (m_should_close) quit(); }
	bool event(QEvent *event)
	{
	    switch (event->type()) {
	    case QEvent::UpdateRequest:
	        m_update_pending = false;
	        renderNow();
	        return true;
	    default:
	        return QWindow::event(event);
	    }
	}
	void exposeEvent(QExposeEvent *event)
	{
	    Q_UNUSED(event);
	
	    if (isExposed())
	        renderNow();
	}

public slots:
	void renderLater()
	{
	    if (!m_update_pending) {
	        m_update_pending = true;
	        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
	    }
	}
	void renderNow()
	{
	    if (!isExposed())
	        return;
	
	    bool needsInitialize = false;
	
	    if (!m_context) {
	        m_context = new QOpenGLContext(this);
	        m_context->setFormat(requestedFormat());
	        m_context->create();
	
	        needsInitialize = true;
	    }
	
	    m_context->makeCurrent(this);
	
	    if (needsInitialize) {
	        initializeOpenGLFunctions();
	        initialize();
	    }
	
	    render();
	
	    m_context->swapBuffers(this);
	    
	    if (m_auto_refresh)
	    	renderLater();
	}
};

void qtSetInputMode(QImgui *window, int input, int mode)
{
	switch (mode) {
		case QT_CURSOR_HIDDEN: window->setCursor( QCursor( Qt::BlankCursor ) ); break;
		case QT_CURSOR_NORMAL: window->setCursor( QCursor( Qt::ArrowCursor ) ); break;
	}
}

bool qtGetWindowAttrib(QImgui *window, int attr)
{
	switch(attr)
	{
		case QT_FOCUSED: return window->isActive();
	}
	return false;
}

void qtGetWindowSize(QImgui *window, int *w, int *h)
{
	*w = window->width();
	*h = window->height();
}

void qtGetFramebufferSize(QImgui *window, int *w, int *h)
{
	*w = window->width()*window->devicePixelRatio();
	*h = window->height()*window->devicePixelRatio();
}

void qtGetCursorPos(QImgui *window, double *mx, double *my)
{
	*mx = window->cursorPos().x();
	*my = window->cursorPos().y();
}

int qtGetMouseButton(QImgui *window, int button)
{
	return window->mouseButtonState(button);
}

void qtSwapBuffers(QImgui *window)
{
}

double qtGetTime()
{
	static QElapsedTimer timer;
	if (!timer.isValid())
		timer.start();
	return timer.elapsed() / 1000;
}



int main(int argc, char **argv)
{
	QScopedPointer<QApplication> app(new QApplication(argc, argv));

	QtWindow *window = new QtWindow;
	window->resize(1280, 720);
	window->show();

    // Setup ImGui binding
    ImGui_ImplQt_Init(window->imgui());

    // Load Fonts
    // (there is a default font, this is only if you want to change it. see extra_fonts/README.txt for more details)
    //ImGuiIO& io = ImGui::GetIO();
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyClean.ttf", 13.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf", 10.0f);
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

    __block bool show_test_window = true;
    __block bool show_another_window = false;
	window->setRenderBlock(^ void {
    	ImVec4 clear_color = ImColor(114, 144, 154);

        ImGui_ImplQt_NewFrame();

        // 1. Show a simple window
        // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
        {
            static float f = 0.0f;
            ImGui::Text("Hello, world!");
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
            ImGui::ColorEdit3("clear color", (float*)&clear_color);
            if (ImGui::Button("Test Window")) show_test_window ^= 1;
            if (ImGui::Button("Another Window")) show_another_window ^= 1;
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }

        // 2. Show another simple window, this time using an explicit Begin/End pair
        if (show_another_window)
        {
            ImGui::SetNextWindowSize(ImVec2(200,100), ImGuiSetCond_FirstUseEver);
            ImGui::Begin("Another Window", &show_another_window);
            ImGui::Text("Hello");
            ImGui::End();
        }

        // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
        if (show_test_window)
        {
            ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
            ImGui::ShowTestWindow(&show_test_window);
        }

        // Rendering
        int display_w, display_h;
        qtGetFramebufferSize(window->imgui(), &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
    });
    // Main loop
    while (!window->done())
    {
		app->processEvents();
        qtSwapBuffers(window->imgui());
    }

    // Cleanup
    ImGui_ImplQt_Shutdown();

    return 0;
}

#include "main.moc"
