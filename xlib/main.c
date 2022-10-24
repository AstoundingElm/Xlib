#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>
#include <string.h>

static char *labels[] = {"Selection", "flowers" , "pets" , "quit" };


static char* colours [] = { "green" , "pink" , "blue" , "red" } ;

int main(int argc, char **argv){
    
    Display* mydisplay;
    XSetWindowAttributes myat, buttonat, popat;
    Window mywindow, button, optA1, panes[3];
    
    
    XSizeHints wmsize;
    XWMHints wmhints;
    XTextProperty windowName, iconName;
    XEvent myevent;
    XColor exact, closest;
    XGCValues myvalues;
    GC gc, gc2, gc3;
    int screen_num, done, i;
    char* window_name = "Petes window";
    char* icon_name = "Ba";
    unsigned long valuemask;
    
    int labelLength[4], currentWindow;
    unsigned long colourBits[6];
    
    mydisplay = XOpenDisplay("");
    screen_num = DefaultScreen(mydisplay);
    for(i = 0; i < 4; i++)labelLength[i] = strlen(labels[i]);
    colourBits[0] = WhitePixel(mydisplay, screen_num);
    colourBits[1] = BlackPixel(mydisplay, screen_num);
    myat.background_pixel  = colourBits[0];
    myat.border_pixel = colourBits[1];
    valuemask = CWBackPixel | CWBorderPixel;
    mywindow = XCreateWindow(mydisplay, RootWindow(mydisplay, screen_num), 300, 300, 350, 450, 3, DefaultDepth(mydisplay, screen_num), InputOutput, DefaultVisual(mydisplay, screen_num), valuemask, &myat );
    
    
    
    
    wmsize.flags = USPosition | USSize;
    XSetWMNormalHints(mydisplay, mywindow, &wmsize);
    wmhints.initial_state = NormalState;
    wmhints.flags = StateHint;
    XSetWMHints(mydisplay, mywindow, &wmhints);
    XStringListToTextProperty(&window_name, 1, &windowName);
    XSetWMName(mydisplay, mywindow, &windowName);
    XStringListToTextProperty(&icon_name, 1, &iconName);
    XSetWMIconName(mydisplay, mywindow, &iconName);
    
    for(i = 0; i < 4; i++){
        
        XAllocNamedColor(mydisplay, XDefaultColormap(mydisplay, screen_num), colours[i], &exact, &closest);
        
        colourBits[i +2] = exact.pixel;
    }
    
    myvalues.background  = colourBits[2];
    myvalues.foreground = colourBits[3];
    
    valuemask = GCForeground | GCBackground;
    
    gc = XCreateGC(mydisplay, mywindow, valuemask, &myvalues);
    myvalues.background = colourBits[3];
    myvalues.foreground = colourBits[4];
    gc2 = XCreateGC(mydisplay, mywindow, valuemask, &myvalues);
    myvalues.background = colourBits[5];
    gc3 = XCreateGC(mydisplay, mywindow, valuemask, &myvalues);
    
    valuemask = CWBackPixel | CWBorderPixel | CWEventMask;
    buttonat.background_pixel = colourBits[2];
    buttonat.border_pixel  = colourBits[1];
    buttonat.event_mask = ButtonPressMask | ExposureMask | Button1MotionMask;
    
    button = XCreateWindow(mydisplay, mywindow, 20, 50, 70, 30, 2,
                           DefaultDepth(mydisplay, screen_num),InputOutput,DefaultVisual(mydisplay, screen_num),valuemask,
                           &buttonat);
    
    popat.border_pixel = colourBits[1];
    popat.background_pixel = colourBits[3];
    popat.event_mask = 0;
    
    optA1 = XCreateWindow(mydisplay, mywindow, 50, 60, 100, 150, 2,
                          DefaultDepth(mydisplay, screen_num),InputOutput,DefaultVisual(mydisplay, screen_num),valuemask,
                          &popat);
    
    popat.event_mask = ButtonPressMask | EnterWindowMask | LeaveWindowMask | ExposureMask;
    
    for(i = 0; i < 3; i++)
        panes[i] = XCreateWindow(mydisplay, optA1, 0, i* 50, 100, 50, 2, DefaultDepth(mydisplay, screen_num), InputOutput, DefaultVisual(mydisplay, screen_num), valuemask, &popat);
    
    XMapWindow( mydisplay , mywindow ) ;
    XMapWindow( mydisplay , button ) ;
    done = 0;
    int buttoncount = 0;
    while(done == 0){
        
        XNextEvent(mydisplay, &myevent);
        
        switch(myevent.type){
            case Expose:
            XDrawImageString(mydisplay, button, gc, 10, 17, labels[0], labelLength[0]);
            break;
            case ButtonPress:
            buttoncount ++;
            if(buttoncount > 1){done = 1;}
            
            
            XMapWindow(mydisplay, optA1);
            currentWindow = 0;
            for(i = 0; i < 3; i++){
                XMapWindow(mydisplay, panes[i]);
                XDrawImageString(mydisplay, panes[i], gc2, 0, 10, labels[i + 1], labelLength[i + 1]);
            }
            
            if(myevent.xbutton.window == panes[2]) done = 1;
            
            break;
            case ButtonRelease:
            buttoncount++;
            case EnterNotify:
            XSetWindowBackground(mydisplay, panes[currentWindow], colourBits[3]);
            XClearWindow(mydisplay, panes[currentWindow]);
            XDrawImageString(mydisplay, panes[currentWindow], gc2, 0, 10, labels[currentWindow + 1], labelLength[currentWindow + 1]);
            for(i = 0; i < 3; i++)
                if(panes[i] == myevent.xcrossing.window){
                
                currentWindow = i;
                break;
                
            }
            XSetWindowBackground(mydisplay, myevent.xcrossing.window, colourBits[5]);
            XClearWindow(mydisplay, myevent.xcrossing.window);
            XDrawImageString(mydisplay, panes[currentWindow],
                             gc3, 0, 10, labels[currentWindow +1], labelLength[currentWindow + 1]);
            break;
            
        }
    }
    
    
    XUnmapWindow(mydisplay, mywindow);
    XDestroyWindow(mydisplay, mywindow);
    XCloseDisplay(mydisplay);
    
}