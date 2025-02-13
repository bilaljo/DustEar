#pragma once

#include <thread>

#ifdef WIN32
#include <processthreadsapi.h>
#else

#endif

#include <iostream>

namespace helper::thread {
    inline void set_scheduling(std::thread& thread, int priority, [[maybe_unused]] int policy = 0) {
#ifdef WIN32
        HANDLE handle = thread.native_handle();
        SetPriorityClass(GetCurrentProcess(), priority);
        if (!SetThreadPriority(handle, priority)) {
            std::cerr << "Fehler beim Setzen der Priorität: " << GetLastError() << std::endl;
        }
#else
            auto handle = thread.native_handle();
            pthread_setschedparam(handle, policy, priority);
#endif
    };
 }
