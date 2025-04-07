#include <Application.h>
#include <Archivable.h>
#include <Box.h>
#include <Dragger.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <Message.h>
#include <Path.h>
#include <ScrollView.h>
#include <Shelf.h>
#include <Window.h>

#include <iostream>
#include <ostream>



class App : public BApplication {
public:
	App();
};


class Window : public BWindow {
public:
	Window();
	void MessageReceived(BMessage* message) override;
	void AttachTermView();
	
private:
	BView* fTermView;
};


int main()
{
	App app;
	app.Run();
	return 0;
}


// App
App::App()
	:BApplication("application/x-vnd-terminal-replicant")
{
	Window *window = new Window();
	window->Show();
}


// Window
Window::Window()
	:
	BWindow(BRect(100, 100, 400, 360), "RepliTerminal",
			B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS|B_QUIT_ON_WINDOW_CLOSE),
	fTermView(NULL)
{
	BMessage* exec = new BMessage(B_EXECUTE_PROPERTY);
	exec->AddSpecifier("command");
	exec->AddString("argv", "/bin/sh");
	exec->AddString("argv", "-c");
	exec->AddString("argv", "ls");
	exec->AddBool("clear", true);
	
	BMenuBar* menuBar = new BMenuBar(Bounds(), "menubar");
	BMenu* menu = new BMenu("command");
	BMenuItem* item = new BMenuItem("restart terminal", exec);
	menu->AddItem(item);
	menuBar->AddItem(menu);
	AddChild(menuBar);

	AttachTermView();
	
	menu->SetTargetForItems(fTermView);
}


void
Window::MessageReceived(BMessage* message)
{
	switch (message->what) {
		default:
			BWindow::MessageReceived(message);
			break;
	}
}


void
Window::AttachTermView()
{
	// BMessage containing the class name and the app_signature
	// for Terminal and TermView
	BMessage message;				
	message.AddString("class", "TermView");
	message.AddString("add_on", "application/x-vnd.Haiku-Terminal");
	message.AddBool("use_rect", true);

	BRect rect = Bounds();
	rect.top += 32;
	message.AddRect("_frame", rect);
	fTermView = dynamic_cast<BView *>(instantiate_object(&message));
	if (fTermView == NULL) {
		std::cout << "ERROR" << std::endl;
		return;
	}
	fTermView->SetResizingMode(B_FOLLOW_ALL);

	AddChild(fTermView);
}
