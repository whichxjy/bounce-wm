#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

int main() {
    // open connection with the display server
    Display *dpy = XOpenDisplay(NULL);
    if (dpy == NULL) {
        fprintf(stderr, "Cannot open display\n");
        exit(EXIT_FAILURE);
    }

    // screen number
    int screen_num =  XDefaultScreen(dpy);
    
    // the size of the screen
    int display_width = XDisplayWidth(dpy, screen_num);
    int display_height = XDisplayHeight(dpy, screen_num);

    // key code of "e"
    KeyCode e_keycode = XKeysymToKeycode(dpy, XStringToKeysym("e"));
    // key code of "q"
    KeyCode q_keycode = XKeysymToKeycode(dpy, XStringToKeysym("q"));

    // grab "ctrl + e"
    XGrabKey(dpy,
             e_keycode,
             ControlMask,
             DefaultRootWindow(dpy),
             True, GrabModeAsync, GrabModeAsync);

    // grab "ctrl + q"
    XGrabKey(dpy,
             q_keycode,
             ControlMask,
             DefaultRootWindow(dpy),
             True, GrabModeAsync, GrabModeAsync);

    // subwindow
    Window sub_win = None;

    // attributes of the subwindow
    XWindowAttributes attr;

    // the position of the subwindow
    int x_pos = 0;
    int y_pos = 0;

    // the size of the subwindow
    int win_width = 0;
    int win_height = 0;

    // the speed of the subwindow
    int x_speed = 10;
    int y_speed = 10;

    // keyboard event
    XEvent ev;

    while (1) {
        // pick next event
        XNextEvent(dpy, &ev);

        if (ev.type != KeyPress) {
            continue;
        }

        if (ev.xkey.subwindow != None) {
            // set current subwindow
            sub_win = ev.xkey.subwindow;

            // update attributes
            XGetWindowAttributes(dpy, ev.xkey.subwindow, &attr);

            // update position
            x_pos = attr.x;
            y_pos = attr.y;

            // update size
            win_width = attr.width;
            win_height = attr.height;
        }

        if (ev.xkey.keycode == e_keycode && sub_win != None) {
            // next position
            x_pos = MAX(MIN(x_pos + x_speed, display_width - win_width), 0);
            y_pos = MAX(MIN(y_pos + y_speed, display_height - win_height), 0);

            // collision detection
            if (x_pos == 0 || (x_pos + win_width) == display_width) {
                x_speed = -x_speed;
            }
            if (y_pos == 0 || (y_pos + win_height) == display_height) {
                y_speed = -y_speed;
            }

            // move subwindow
            XMoveWindow(dpy, sub_win, x_pos, y_pos);
        }
        else if (ev.xkey.keycode == q_keycode) {
            // quit
            break;
        }
    }

    // close connection to display server
    XCloseDisplay(dpy);

    return EXIT_SUCCESS;
}