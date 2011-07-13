/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Copyright:
 *     Christian Schulte, 2011
 *
 *  Last modified:
 *     $Date$ by $Author$
 *     $Revision$
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

#include <gecode/int/no-overlap.hh>

namespace Gecode {

  namespace Int { namespace NoOverlap {

    bool
    optional(const BoolVarArgs& o) {
      for (int i=o.size(); i--; )
        if (o[i].none())
          return true;
      return false;
    }

  }}

  void
  nooverlap(Home home, 
            const IntVarArgs& x, const IntArgs& w, 
            const IntVarArgs& y, const IntArgs& h,
            IntConLevel) {
    using namespace Int;
    using namespace NoOverlap;
    if (x.same(home,y))
      throw ArgumentSame("Int::nooverlap");
    if ((x.size() != w.size()) || (x.size() != y.size()) || 
        (x.size() != h.size()))
      throw ArgumentSizeMismatch("Int::nooverlap");      
    for (int i=x.size(); i--; ) {
      Limits::nonnegative(w[i],"Int::nooverlap");
      Limits::nonnegative(h[i],"Int::nooverlap");
      Limits::check(static_cast<double>(x[i].max()) + w[i],
                    "Int::nooverlap");
      Limits::check(static_cast<double>(y[i].max()) + h[i],
                    "Int::nooverlap");
    }
    if (home.failed()) return;

    ManBox<IntDim,2>* b 
      = static_cast<Space&>(home).alloc<ManBox<IntDim,2> >(x.size());
    for (int i=x.size(); i--; ) {
      b[i][0] = IntDim(x[i],w[i]);
      b[i][1] = IntDim(y[i],h[i]);
    }

    GECODE_ES_FAIL((NoOverlap::ManProp<IntDim,2>::post(home,b,x.size())));
  }

  void
  nooverlap(Home home, 
            const IntVarArgs& x, const IntArgs& w, 
            const IntVarArgs& y, const IntArgs& h,
            const BoolVarArgs& o,
            IntConLevel) {
    using namespace Int;
    using namespace NoOverlap;
    if (x.same(home,y) || o.same(home))
      throw ArgumentSame("Int::nooverlap");
    if ((x.size() != w.size()) || (x.size() != y.size()) ||
        (x.size() != h.size()) || (x.size() != o.size()))
      throw ArgumentSizeMismatch("Int::nooverlap");      
    for (int i=x.size(); i--; ) {
      Limits::nonnegative(w[i],"Int::nooverlap");
      Limits::nonnegative(h[i],"Int::nooverlap");
      Limits::check(static_cast<double>(x[i].max()) + w[i],
                    "Int::nooverlap");
      Limits::check(static_cast<double>(y[i].max()) + h[i],
                    "Int::nooverlap");
    }
    if (home.failed()) return;
    
    if (optional(o)) {
      OptBox<IntDim,2>* b 
        = static_cast<Space&>(home).alloc<OptBox<IntDim,2> >(x.size());
      for (int i=x.size(); i--; ) {
        b[i][0] = IntDim(x[i],w[i]);
        b[i][1] = IntDim(y[i],h[i]);
        b[i].optional(o[i]);
      }
      GECODE_ES_FAIL((NoOverlap::OptProp<IntDim,2>::post(home,b,x.size())));
    } else {
      ManBox<IntDim,2>* b 
        = static_cast<Space&>(home).alloc<ManBox<IntDim,2> >(x.size());
      int n = 0;
      for (int i=0; i<x.size(); i++)
        if (o[i].one()) {
          b[n][0] = IntDim(x[i],w[i]);
          b[n][1] = IntDim(y[i],h[i]);
          n++;
        }
      GECODE_ES_FAIL((NoOverlap::ManProp<IntDim,2>::post(home,b,n)));
    }
  }

  void
  nooverlap(Home home, 
            const IntVarArgs& x, const IntVarArgs& w, 
            const IntVarArgs& y, const IntVarArgs& h,
            IntConLevel) {
    using namespace Int;
    using namespace NoOverlap;
    if (x.same(home,y) || x.same(home,w) || x.same(home,h))
      throw ArgumentSame("Int::nooverlap");
    if ((x.size() != w.size()) || (x.size() != y.size()) || 
        (x.size() != h.size()))
      throw ArgumentSizeMismatch("Int::nooverlap");      
    if (home.failed()) return;

    for (int i=x.size(); i--; ) {
      GECODE_ME_FAIL(IntView(w[i]).gq(home,0));
      GECODE_ME_FAIL(IntView(h[i]).gq(home,0));
      Limits::check(static_cast<double>(x[i].max()) + w[i].max(),
                    "Int::nooverlap");
      Limits::check(static_cast<double>(y[i].max()) + h[i].max(),
                    "Int::nooverlap");
    }

    if (w.assigned() && h.assigned()) {
      IntArgs wc(x.size()), hc(x.size());
      for (int i=x.size(); i--; ) {
        wc[i] = w[i].val();
        hc[i] = h[i].val();
      }
      nooverlap(home, x, wc, y, hc);
    } else {
      ManBox<ViewDim,2>* b 
        = static_cast<Space&>(home).alloc<ManBox<ViewDim,2> >(x.size());
      for (int i=x.size(); i--; ) {
        b[i][0] = ViewDim(x[i],w[i]);
        b[i][1] = ViewDim(y[i],h[i]);
      }
      GECODE_ES_FAIL((NoOverlap::ManProp<ViewDim,2>::post(home,b,x.size())));
    }
  }

  void
  nooverlap(Home home, 
            const IntVarArgs& x, const IntVarArgs& w, 
            const IntVarArgs& y, const IntVarArgs& h,
            const BoolVarArgs& o,
            IntConLevel) {
    using namespace Int;
    using namespace NoOverlap;
    if (x.same(home,y) || x.same(home,w) || x.same(home,h) || o.same(home))
      throw ArgumentSame("Int::nooverlap");
    if ((x.size() != w.size()) || (x.size() != y.size()) || 
        (x.size() != h.size()) || (x.size() != o.size()))
      throw ArgumentSizeMismatch("Int::nooverlap");      
    if (home.failed()) return;

    for (int i=x.size(); i--; ) {
      GECODE_ME_FAIL(IntView(w[i]).gq(home,0));
      GECODE_ME_FAIL(IntView(h[i]).gq(home,0));
      Limits::check(static_cast<double>(x[i].max()) + w[i].max(),
                    "Int::nooverlap");
      Limits::check(static_cast<double>(y[i].max()) + h[i].max(),
                    "Int::nooverlap");
    }

    if (w.assigned() && h.assigned()) {
      IntArgs wc(x.size()), hc(x.size());
      for (int i=x.size(); i--; ) {
        wc[i] = w[i].val();
        hc[i] = h[i].val();
      }
      nooverlap(home, x, wc, y, hc, o);
    } else if (optional(o)) {
      OptBox<ViewDim,2>* b 
        = static_cast<Space&>(home).alloc<OptBox<ViewDim,2> >(x.size());
      for (int i=x.size(); i--; ) {
        b[i][0] = ViewDim(x[i],w[i]);
        b[i][1] = ViewDim(y[i],h[i]);
        b[i].optional(o[i]);
      }
      GECODE_ES_FAIL((NoOverlap::OptProp<ViewDim,2>::post(home,b,x.size())));
    } else {
      ManBox<ViewDim,2>* b 
        = static_cast<Space&>(home).alloc<ManBox<ViewDim,2> >(x.size());
      int n = 0;
      for (int i=0; i<x.size(); i++)
        if (o[i].one()) {
          b[n][0] = ViewDim(x[i],w[i]);
          b[n][1] = ViewDim(y[i],h[i]);
          n++;
        }
      GECODE_ES_FAIL((NoOverlap::ManProp<ViewDim,2>::post(home,b,n)));
    }
  }

}

// STATISTICS: int-post
