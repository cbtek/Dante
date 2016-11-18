///
/// CBTek LLC ("COMPANY") CONFIDENTIAL
/// Copyright (c) 2016 CBTek, All Rights Reserved.
///
/// NOTICE:  All information contained herein is, and remains the property of COMPANY. The intellectual and technical concepts contained
/// herein are proprietary to COMPANY and may be covered by U.S. and Foreign Patents, patents in process, and are protected by trade secret or copyright law.
/// Dissemination of this information or reproduction of this material is strictly forbidden unless prior written permission is obtained
/// from COMPANY.  Access to the source code contained herein is hereby forbidden to anyone except current COMPANY employees, managers or contractors who have executed
/// Confidentiality and Non-disclosure agreements explicitly covering such access.
///
/// The copyright notice above does not evidence any actual or intended publication or disclosure  of  this source code, which includes
/// information that is confidential and/or proprietary, and is a trade secret, of  COMPANY.   ANY REPRODUCTION, MODIFICATION, DISTRIBUTION, PUBLIC  PERFORMANCE,
/// OR PUBLIC DISPLAY OF OR THROUGH USE  OF THIS  SOURCE CODE  WITHOUT  THE EXPRESS WRITTEN CONSENT OF COMPANY IS STRICTLY PROHIBITED, AND IN VIOLATION OF APPLICABLE
/// LAWS AND INTERNATIONAL TREATIES.  THE RECEIPT OR POSSESSION OF  THIS SOURCE CODE AND/OR RELATED INFORMATION DOES NOT CONVEY OR IMPLY ANY RIGHTS
/// TO REPRODUCE, DISCLOSE OR DISTRIBUTE ITS CONTENTS, OR TO MANUFACTURE, USE, OR SELL ANYTHING THAT IT  MAY DESCRIBE, IN WHOLE OR IN PART.
///


#include "utility/inc/StringUtils.hpp"
#include "utility/inc/FileUtils.hpp"
#include "utility/inc/Exception.hpp"

#include <chrono>

#ifdef _WIN32
#   include <windows.h>
#   include <shlobj.h>
#   include <Lmcons.h>
#else
#   include <dirent.h>
#   include <unistd.h>
#   include <sys/types.h>
#   include <sys/stat.h>
#   include <fcntl.h>
#   include <pwd.h>
#   include <dlfcn.h>
#   include <X11/Xlib.h>
#   include <X11/keysym.h>
#   include <X11/keysymdef.h>
// Function to create a keyboard event
static inline XKeyEvent createKeyEvent(Display *display, Window &win,
                                       Window &winRoot, bool press,
                                       int keycode,
                                       int modifiers)
    {
       XKeyEvent event;
       event.display     = display;
       event.window      = win;
       event.root        = winRoot;
       event.subwindow   = None;
       event.time        = CurrentTime;
       event.x           = 1;
       event.y           = 1;
       event.x_root      = 1;
       event.y_root      = 1;
       event.same_screen = True;
       event.keycode     = XKeysymToKeycode(display, keycode);
       event.state       = modifiers;
       event.type        = (press?KeyPress:KeyRelease);
       return event;
    }
#endif
#ifndef c_MAX_PATH
#define c_MAX_PATH 1024
#endif

namespace cbtek {
namespace common {
namespace utility {
namespace SystemUtils {

namespace Keys
{
    enum
    {
        KEY_LEFT_CTRL=0xFF00,
        KEY_RIGHT_CTRL,
        KEY_LEFT_SHIFT,
        KEY_RIGHT_SHIFT,
        KEY_LEFT_ALT,
        KEY_RIGHT_ALT,
        KEY_LEFT_ARROW,
        KEY_DOWN_ARROW,
        KEY_RIGHT_ARROW,
        KEY_UP_ARROW,
        KEY_BACKSPACE,
        KEY_ESCAPE,
        KEY_F1,
        KEY_F2,
        KEY_F3,
        KEY_F4,
        KEY_F5,
        KEY_F6,
        KEY_F7,
        KEY_F8,
        KEY_F9,
        KEY_F10,
        KEY_F11,
        KEY_F12,
        KEY_PRINT_SCREEN,
        KEY_TAB,
        KEY_SPACE_BAR,
        KEY_INSERT,
        KEY_HOME,
        KEY_PAGE_UP,
        KEY_PAGE_DOWN,
        KEY_END,
        KEY_DELETE,
        KEY_ENTER,
        KEY_KP_LEFT_ARROW,
        KEY_KP_DOWN_ARROW,
        KEY_KP_RIGHT_ARROW,
        KEY_KP_UP_ARROW,
        KEY_KP_INSERT,
        KEY_KP_HOME,
        KEY_KP_PAGE_UP,
        KEY_KP_PAGE_DOWN,
        KEY_KP_END,
        KEY_KP_DELETE,
        KEY_KP_ENTER
    };
}

static inline std::uint64_t getCurrentMilliseconds()
{
    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
    std::chrono::system_clock::now().time_since_epoch());
    return ms.count();
}



static inline void simulateKeyPress(std::uint64_t key)
{
    #ifdef _WIN32

    #else
    // Obtain the X11 display.
       Display *display = XOpenDisplay(0);
       if(display == NULL)
       {
          return;
       }
        int linuxKeycode = 0;
       switch (key)
       {
           case Keys::KEY_LEFT_CTRL:      linuxKeycode  = XK_Control_L;break;
           case Keys::KEY_RIGHT_CTRL:     linuxKeycode = XK_Control_R;break;
           case Keys::KEY_LEFT_SHIFT:     linuxKeycode = XK_Shift_L;break;
           case Keys::KEY_RIGHT_SHIFT:    linuxKeycode = XK_Shift_R;break;
           case Keys::KEY_LEFT_ALT:       linuxKeycode = XK_Alt_L;break;
           case Keys::KEY_RIGHT_ALT:      linuxKeycode = XK_Alt_R;break;
           case Keys::KEY_LEFT_ARROW:     linuxKeycode = XK_Left;break;
           case Keys::KEY_DOWN_ARROW:     linuxKeycode = XK_Down;break;
           case Keys::KEY_RIGHT_ARROW:    linuxKeycode = XK_Right;break;
           case Keys::KEY_UP_ARROW:       linuxKeycode = XK_Up;break;
           case Keys::KEY_BACKSPACE:      linuxKeycode = XK_BackSpace;break;
           case Keys::KEY_ESCAPE:         linuxKeycode = XK_Escape;break;
           case Keys::KEY_F1:             linuxKeycode = XK_F1;break;
           case Keys::KEY_F2:             linuxKeycode = XK_F2;break;
           case Keys::KEY_F3:             linuxKeycode = XK_F3;break;
           case Keys::KEY_F4:             linuxKeycode = XK_F4;break;
           case Keys::KEY_F5:             linuxKeycode = XK_F5;break;
           case Keys::KEY_F6:             linuxKeycode = XK_F6;break;
           case Keys::KEY_F7:             linuxKeycode = XK_F7;break;
           case Keys::KEY_F8:             linuxKeycode = XK_F8;break;
           case Keys::KEY_F9:             linuxKeycode = XK_F9;break;
           case Keys::KEY_F10:            linuxKeycode = XK_F10;break;
           case Keys::KEY_F11:            linuxKeycode = XK_F11;break;
           case Keys::KEY_F12:            linuxKeycode = XK_F12;break;
           case Keys::KEY_PRINT_SCREEN:   linuxKeycode = XK_Print;break;
           case Keys::KEY_TAB:            linuxKeycode = XK_Tab;break;
           case Keys::KEY_SPACE_BAR:      linuxKeycode = XK_space;break;
           case Keys::KEY_INSERT:         linuxKeycode = XK_Insert;break;
           case Keys::KEY_HOME:           linuxKeycode = XK_Home;break;
           case Keys::KEY_PAGE_UP:        linuxKeycode = XK_Page_Up;break;
           case Keys::KEY_PAGE_DOWN:      linuxKeycode = XK_Page_Down;break;
           case Keys::KEY_END:            linuxKeycode = XK_End;break;
           case Keys::KEY_DELETE:         linuxKeycode = XK_Delete;break;
           case Keys::KEY_ENTER:          linuxKeycode = XK_Return;break;
           case Keys::KEY_KP_LEFT_ARROW:  linuxKeycode = XK_KP_Left;break;
           case Keys::KEY_KP_DOWN_ARROW:  linuxKeycode = XK_KP_Down;break;
           case Keys::KEY_KP_RIGHT_ARROW: linuxKeycode = XK_KP_Right;break;
           case Keys::KEY_KP_UP_ARROW:    linuxKeycode = XK_KP_Up;break;
           case Keys::KEY_KP_INSERT:      linuxKeycode = XK_KP_Insert;break;
           case Keys::KEY_KP_HOME:        linuxKeycode = XK_KP_Home;break;
           case Keys::KEY_KP_PAGE_UP:     linuxKeycode = XK_KP_Page_Up;break;
           case Keys::KEY_KP_PAGE_DOWN:   linuxKeycode = XK_KP_Page_Down;break;
           case Keys::KEY_KP_END:         linuxKeycode = XK_KP_End;break;
           case Keys::KEY_KP_DELETE:      linuxKeycode = XK_KP_Delete;break;
           case Keys::KEY_KP_ENTER:       linuxKeycode = XK_KP_Enter;break;
           default:break;
       }

    // Get the root window for the current display.
       Window winRoot = XDefaultRootWindow(display);

    // Find the window which has the current keyboard focus.
       Window winFocus;
       int    revert;
       XGetInputFocus(display, &winFocus, &revert);

    // Send a fake key press event to the window.
       XKeyEvent event = createKeyEvent(display, winFocus, winRoot, true, linuxKeycode, 0);
       XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent *)&event);

    // Send a fake key release event to the window.
       event = createKeyEvent(display, winFocus, winRoot, false, linuxKeycode, 0);
       XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent *)&event);

    // Done.
       XCloseDisplay(display);
    #endif
}


static inline std::string getUserHomeDirectory()
{
#ifdef _WIN32
    char dir[c_MAX_PATH];

    SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, dir);

    std::string dirStr(dir);
    StringUtils::replaceInPlace(dirStr,"\\","/");
    return dirStr;
#else
    return std::string(getenv("HOME"));
#endif
}

static inline std::string getUserAppDirectory()
{
#ifdef _WIN32
    char dir[c_MAX_PATH];

    SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, dir);

    std::string dirStr(dir);
    StringUtils::replaceInPlace(dirStr,"\\","/");
    return dirStr;
#else
    return getUserHomeDirectory() + "/.local/share"; // TODO: Reconsider this
#endif
}

static inline std::string getUserTempDirectory()
{
#ifdef _WIN32
    char dir[c_MAX_PATH];

    GetTempPathA(c_MAX_PATH, dir);
    GetLongPathNameA(dir, dir, c_MAX_PATH);

    std::string dirStr(dir);
    StringUtils::replaceInPlace(dirStr,"\\","/");
    return dirStr;
#else
    return std::string("/tmp");
#endif
}

static inline std::string getUserName()
{
#ifdef _WIN32
    char username[UNLEN+1];
    DWORD username_len = UNLEN+1;
    GetUserNameA(username, &username_len);
    return std::string(username);
#else
    struct passwd *passwd;
    passwd =getpwuid ( getuid());
    return std::string(passwd->pw_name);
#endif
}

static inline int execute(const std::string &command)
{
    int retCode = system(command.c_str());  //one day this will do more
    return retCode;
}

static inline std::string getCurrentDirectory()
{
    char dir[c_MAX_PATH];
    #ifdef _WIN32
        GetCurrentDirectoryA(MAX_PATH, dir);
    #else
        char * ret = getcwd(dir, c_MAX_PATH);
        if (ret)
        {
            delete ret;
        }
    #endif
    return std::string(dir);
}

}}}}//end namespace

