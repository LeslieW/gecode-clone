/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Copyright:
 *     Christian Schulte, 2009
 *
 *  Last modified:
 *     $Date: 2009-02-05 11:48:53 +0100 (Thu, 05 Feb 2009) $ by $Author: schulte $
 *     $Revision: 8155 $
 *
 *  This file is part of Gecode, the generic constraint
 *  development environment:
 *     http://www.gecode.org
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject to
 *  the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include <gecode/support/thread.hh>

#ifdef GECODE_THREADS_WINDOWS

namespace Gecode { namespace Support {

  /*
   * Thread
   */
  /// Function to start execution
  DWORD WINAPI 
  bootstrap(LPVOID p) {
    static_cast<Runnable*>(p)->run();
    return 0;
  }

  Thread::Thread(Runnable& r) 
    : w_h(CreateThread(NULL, 0, bootstrap, &r, 0, NULL)) {}
  
  unsigned int
  Thread::npu(void) {
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    return static_cast<unsigned int>(si.dwNumberOfProcessors);
  }

  /*
   * Mutex
   */

  Mutex::Mutex(void) {
    InitializeCriticalSection(&w_cs);
  }

  Mutex::~Mutex(void) {
    DeleteCriticalSection(&w_cs);
  }

}}

#endif

#ifdef GECODE_THREADS_PTHREADS

namespace Gecode { namespace Support {

  /*
   * Thread
   */
  /// Function to start execution
  void
  bootstrap(void* p) {
    static_cast<Runnable*>(p)->run();
  }

  Thread::Thread(Runnable& r) {
    (void) pthread_create(&p_t, NULL, bootstrap, &r);
  }
  
  unsigned int
  Thread::npu(void) {
    return 0;
  }

  /*
   * Mutex
   */

  Mutex::Mutex(void) {
    pthread_mutex_init(&p_m,NULL);
  }

  Mutex::~Mutex(void) {
    pthread_mutex_destroy(&p_m);    
  }

}}

#endif

// STATISTICS: support-any
