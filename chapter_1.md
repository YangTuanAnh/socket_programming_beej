# Chapter 1 - Intro

## Audience

Intro and foothold,  not the complete and total guide to sockets programming

## Platform and Compiler

gcc, unless Windows (uh oh)

## Official Homepage and Books For Sale

https://beej.us/guide/bgnet/

## Note for Solaris/SunO/ilumos Programmers

ehh

## Note for Windows Programmers (me)

Use WSL

Windows 11 is a menace

Please use Linux, BSD, illumos, ANYTHING but Windows (i need to dual boot)

In case of Windows:

```c
#include <winsock.h>
#include <ws2tcpip.h>
```

`winsock2` is the "new" (circa 1994) version of the Windows socket library.

Unfortunately, if you include `windows.h` , it automatically pulls in the older `winsock.h` (version 1) header file which conflicts with `winsock2.h` ! Fun times.

So if you have to include `windows.h` , you need to define a macro to get it to not include the older header:

```c
#define WIN32_LEAN_AND_MEAN // Say this...

#include <windows.h> // And now we can include that.
#include <winsock2.h> // And this.
```

Wait! You also have to make a call to WSAStartup() before doing anything else with the sockets library. You pass in the Winsock version you desire to this function (e.g. version 2.2). And then you can check the result to make sure that version is available.

```c
#include <winsock2.h>

{
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed.\n");
        exit(1);
    }

    if (LOBYTE(wsaData.wVersion) != 2 ||
        HIBYTE(wsaData.wVersion) != 2 ||) 
    {
        fprint(stderr, "Version 2.2 of Winsock not available.\n");
        WSACleanup();
        exit(2);
    }
}
```

Note that call to WSACleanup() in there. That’s what you want to call when you’re done with the Winsock library.

yeah lets build this thing on WSL.