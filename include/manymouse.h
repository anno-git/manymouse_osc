/*
 * ManyMouse main header. Include this from your app.
 *
 * Please see the file LICENSE.txt in the source's root directory.
 *
 *  This file written by Ryan C. Gordon.
 */

#ifndef _INCLUDE_MANYMOUSE_H_
#define _INCLUDE_MANYMOUSE_H_

#define MANYMOUSE_DLL

#ifdef MANYMOUSE_DLL
#ifdef _WIN32
#define MANYMOUSE_DLL_EXPORT __declspec(dllexport)
#else
#define MANYMOUSE_DLL_EXPORT
#endif // _WIN32
#endif // MANYMOUSE_DLL

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MANYMOUSE_VERSION "0.0.3"

typedef enum
{
    MANYMOUSE_EVENT_ABSMOTION = 0,
    MANYMOUSE_EVENT_RELMOTION,
    MANYMOUSE_EVENT_BUTTON,
    MANYMOUSE_EVENT_SCROLL,
    MANYMOUSE_EVENT_DISCONNECT,
    MANYMOUSE_EVENT_MAX
} ManyMouseEventType;

typedef struct
{
    ManyMouseEventType type;
    unsigned int device;
    unsigned int item;
    int value;
    int minval;
    int maxval;
} ManyMouseEvent;


/* internal use only. */
typedef struct
{
    const char *driver_name;
    int (*init)(void);
    void (*quit)(void);
    const char *(*name)(unsigned int index);
    int (*poll)(ManyMouseEvent *event);
} ManyMouseDriver;

MANYMOUSE_DLL_EXPORT bool ManyMouse_IsAlreadyInit(void);
MANYMOUSE_DLL_EXPORT int ManyMouse_Count(void);
MANYMOUSE_DLL_EXPORT int ManyMouse_Init(void);
MANYMOUSE_DLL_EXPORT const char *ManyMouse_DriverName(void);
MANYMOUSE_DLL_EXPORT void ManyMouse_Quit(void);
MANYMOUSE_DLL_EXPORT const char *ManyMouse_DeviceName(unsigned int index);
MANYMOUSE_DLL_EXPORT int ManyMouse_PollEvent(ManyMouseEvent *event);

#ifdef __cplusplus
}
#endif

#endif  /* !defined _INCLUDE_MANYMOUSE_H_ */

/* end of manymouse.h ... */

