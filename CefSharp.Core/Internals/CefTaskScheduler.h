// Copyright � 2010-2013 The CefSharp Project. All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be found in the LICENSE file.

#pragma once

#include "Stdafx.h"

#include <include/cef_runnable.h>
#include <include/cef_task.h>

#include "CefTaskWrapper.h"

using namespace System;
using namespace System::Threading::Tasks;
using namespace System::Runtime::InteropServices;

using namespace CefSharp;

namespace CefSharp
{
    namespace Internals
    {
        public ref class CefTaskScheduler : TaskScheduler, ITaskScheduler
        {
        public:
            CefThreadId _thread;

            CefTaskScheduler(CefThreadId thread) :
                _thread(thread)
            {
            };

            virtual void QueueTask(Task^ task) override
            {
                auto taskwrapper = CefRefPtr<CefTaskWrapper>(new CefTaskWrapper(task, this));

                CefPostTask(_thread, NewCefRunnableMethod(taskwrapper.get(), &CefTaskWrapper::Execute));
            };

            virtual void ExecuteTask(Task^ task)
            {
                TryExecuteTask(task);
            };

            static bool CurrentlyOnThread(CefThreadId threadId)
            {
                return CefCurrentlyOn(threadId);
            }

            static void EnsureOn(CefThreadId threadId, String^ context)
            {
                if (!CefCurrentlyOn(threadId))
                {
                    throw gcnew InvalidOperationException(String::Format("Executed '{0}' on incorrect thread. This method expects to run on the CEF {1} thread!", context, ((CefThreadIds)threadId).ToString()));
                }
            }

            static void EnsureOn(CefThreadIds threadId, String^ context)
            {
                EnsureOn((CefThreadId)threadId, context);
            }

        protected:
            virtual bool TryExecuteTaskInline(Task^ task, bool taskWasPreviouslyQueued) override
            {
                // You might think this method might not get called,
                // but a .ContinueWith(..., TaskContinuationOpation.ExecuteSyncronously)
                // will probably end up calling this method,
                // so assume the callers know what they're doing and execute the task
                // inline on this thread (if the current thread is correct for this scheduler)
                if (CefCurrentlyOn(_thread))
                {
                    return TryExecuteTask(task);
                }
                return false;
            };

            virtual IEnumerable<Task^>^ GetScheduledTasks() override
            {
                return nullptr;
            };
        };
    }
}