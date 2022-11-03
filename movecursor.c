#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

Display *dpy;
XEvent event;
Window root_window;
Window window_returned;
unsigned int mask_return;
int rand_x, rand_y; 
int width, height, snum;
int root_x, root_y;
int win_x, win_y;
int click;

void mouseClick(int button) {
	dpy = XOpenDisplay(NULL);
	
	if (dpy == NULL) {
		fprintf(stderr, "Error: Could not open display...\n");
		exit(EXIT_FAILURE);
	}
	
	memset(&event, 0x00, sizeof(event));
	
	event.type = ButtonPress;
	event.xbutton.button = button;
	event.xbutton.same_screen = True;
	
	XQueryPointer(dpy, root_window, &event.xbutton.root, &event.xbutton.window, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
	
	event.xbutton.subwindow = event.xbutton.window;
	
	while (event.xbutton.subwindow) {
		event.xbutton.window = event.xbutton.subwindow;
		XQueryPointer(dpy, event.xbutton.window, &event.xbutton.root, &event.xbutton.subwindow, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
	}
	
	if(XSendEvent(dpy, PointerWindow, True, 0xfff, &event) == 0) fprintf(stderr, "Error: Could not send XEvent...\n");
	
	XFlush(dpy);
	
	event.type = ButtonRelease;
	event.xbutton.state = 0x100;
	
	if(XSendEvent(dpy, PointerWindow, True, 0xfff, &event) == 0) fprintf(stderr, "Error: Could not send XEvent...\n");
	
	//XFlush(dpy);
	//XCloseDisplay(dpy);
}

int main() {

  srandom(time(NULL));

  dpy = XOpenDisplay(0);

  if (dpy == NULL) {
    fprintf(stderr, "Error: Could not open the display...\n");
    exit(EXIT_FAILURE);
  }

  root_window = XRootWindow(dpy, 0);
  XSelectInput(dpy, root_window, KeyReleaseMask);
  snum = DefaultScreen(dpy);
  width = DisplayWidth(dpy, snum);
  height = DisplayHeight(dpy, snum);

  XQueryPointer (
    dpy,
    root_window,
    &window_returned,
    &window_returned,
    &root_x,
    &root_y,
    &win_x,
    &win_y,
    &mask_return
  ); 

  while (1){

    click = ((random() % 10) + 1);

    if (click >= 5 && click <= 10) {
      mouseClick(1);
    } else if (click < 4 && click >= 1){
      ;
    }

    int shake_x = root_x + (random() % 10);
    int shake_y = root_y + (random() % 10);
    rand_x = ((random() % width) + 1);
    rand_y = ((random() % height) + 1);
    XWarpPointer(dpy, None, root_window, root_x, root_y, 0, 0, rand_x, rand_y);
    XWarpPointer(dpy, None, root_window, root_x, root_y, 0, 0, shake_x, shake_y); 
    XFlush(dpy); //XFlush updates cursors position
  }
}
