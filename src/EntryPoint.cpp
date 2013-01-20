/* Copyright (c) 2012 Gillis Van Ginderachter <supergillis@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <QtGlobal>

#include "Application.h"

Application* application = NULL;

#ifdef Q_OS_WIN
HANDLE thread;
#else
#include <pthread.h>

pthread_t thread;

void hook_constructor() __attribute__((constructor));
void hook_destructor() __attribute__((destructor));
void* hook_thread(void*);
#endif

/**
 * This function runs when the thread is created. Qt runs in this thread.
 */
#ifdef Q_OS_WIN
DWORD WINAPI hook_thread(LPVOID) {
#else
void* hook_thread(void*) {
#endif
    // Create the application and enter the main event loop
    application = new Application();
    application->initialize();
    application->exec();

    // When the application is done executing, clean up
    delete application;

    return 0;
}

/**
 * This function runs when the library is loaded in Tibia.
 */
void hook_constructor() {
    // Create a new thread for Qt to run in
#ifdef Q_OS_WIN
    thread = CreateThread(NULL, 0, hook_thread, NULL, 0, NULL);
#else
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&thread, &attr, hook_thread, NULL);
#endif
}

/**
 * This function runs when the library is unloaded from Tibia.
 */
void hook_destructor() {
    // Put the quit signal on the application event loop
    QMetaObject::invokeMethod(application, "quit", Qt::QueuedConnection);

    // Wait for the Qt thread to finish
#ifdef Q_OS_WIN
    WaitForSingleObject(thread, INFINITE);
#else
    pthread_join(thread, NULL);
#endif
}

#ifdef Q_OS_WIN
/**
 * This function is called when attaching or detaching to a process or thread.
 */
extern "C" __declspec(dllexport) BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID) {
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
        hook_constructor();
        break;
    case DLL_PROCESS_DETACH:
        hook_destructor();
        break;
    }
    return TRUE;
}

/**
 * This function is defined to support injecting with SetWindowsHookEx.
 */
extern "C" __declspec(dllexport) LRESULT CALLBACK DummyCallback(int nCode, WPARAM wParam, LPARAM lParam) {
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}
#endif
