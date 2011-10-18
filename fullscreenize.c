
#include <X11/Xlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	Display* display = XOpenDisplay(getenv("DISPLAY"));
	if (!display)
	{
		puts("Failed to open display\n");
		return -1;
	}

	Window currentFocus;
	int revert;
	XGetInputFocus(display, &currentFocus, &revert);

	if (currentFocus != None && currentFocus != DefaultRootWindow(display))
	{
		Atom atomFullscreen = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", True);
		Atom atomState = XInternAtom(display, "_NET_WM_STATE", True);
		XClientMessageEvent event;
		event.type = ClientMessage;
		event.serial = 0;
		event.send_event = True;
		event.window = currentFocus;
		event.message_type = atomState;
		event.format = 32;
		event.data.l[0] = 1;
		event.data.l[1] = atomFullscreen;
		event.data.l[2] = 0;
		XSendEvent(display, DefaultRootWindow(display), False,
				   SubstructureRedirectMask | SubstructureNotifyMask,
				   (XEvent*)&event);
		XFlush(display);
	}

	XCloseDisplay(display);
	return 0;
}
