
#include <X11/Xlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void sendFullscreenEvent(Display* display, Window win)
{
	if (win != None && win != DefaultRootWindow(display))
	{
		Atom atomFullscreen = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", True);
		Atom atomState = XInternAtom(display, "_NET_WM_STATE", True);
		XClientMessageEvent event;
		event.type = ClientMessage;
		event.serial = 0;
		event.send_event = True;
		event.window = win;
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
}

int main(int argC, char** argV)
{
	Display* display = XOpenDisplay(getenv("DISPLAY"));
	if (!display)
	{
		puts("Failed to open display\n");
		return -1;
	}

	if (argC > 1)
	{
		if (argC == 2)
		{
			puts("Usage: fullscreenize <command> <argument>\n"
				 "Or   : fullscreenize");
			return 1;
		}
		else
		{
			if (strcmp(argV[1], "--execute") == 0)
			{
				pid_t pid = fork();
				if (pid == 0)
				{
					setsid();
					system(argV[2]);
					exit(0);
				}
				else
				{
					puts("fullscreenize: Will send fullscreen after 2 seconds");
					sleep(2);

					Window currentFocus;
					int revert;
					XGetInputFocus(display, &currentFocus, &revert);

					sendFullscreenEvent(display, currentFocus);
					puts("fullscreenize: Sent fullscreen");
					_exit(0);
				}
			}
			else
			{
				printf("Unknown command: %s\n", argV[1]);
				return 1;
			}
		}
	}
	else
	{
		Window currentFocus;
		int revert;
		XGetInputFocus(display, &currentFocus, &revert);

		sendFullscreenEvent(display, currentFocus);
	}

	XCloseDisplay(display);
	return 0;
}
