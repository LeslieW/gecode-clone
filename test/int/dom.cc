/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Copyright:
 *     Christian Schulte, 2005
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

#include "test/int.hh"

static const int d1r[4][2] = {
  {-4,-3},{-1,-1},{1,1},{3,5}
};
static IntSet d1(d1r,4);

static IntSet ds_66(-6,6);

class DomRange : public IntTest {
public:
  DomRange(const char* t)
    : IntTest(t,1,ds_66,true) {}
  virtual bool solution(const Assignment& x) const {
    return (x[0] >= -2) && (x[0] <= 2);
  }
  virtual void post(Space* home, IntVarArray& x) {
    Gecode::dom(home, x[0], -2, 2);
  }
  virtual void post(Space* home, IntVarArray& x, BoolVar b) {
    Gecode::dom(home, x[0], -2, 2, b);
  }
};
DomRange _domrange("Dom::Range");


class DomDom : public IntTest {
public:
  DomDom(const char* t)
    : IntTest(t,1,ds_66,true) {}
  virtual bool solution(const Assignment& x) const {
    return
      (((x[0] >= -4) && (x[0] <= -3)) ||
       ((x[0] >= -1) && (x[0] <= -1)) ||
       ((x[0] >=  1) && (x[0] <=  1)) ||
       ((x[0] >=  3) && (x[0] <=  5)));
  }
  virtual void post(Space* home, IntVarArray& x) {
    Gecode::dom(home, x[0], d1);
  }
  virtual void post(Space* home, IntVarArray& x, BoolVar b) {
    Gecode::dom(home, x[0], d1, b);
  }
};
DomDom _domdom("Dom::Dom");

// STATISTICS: test-int

