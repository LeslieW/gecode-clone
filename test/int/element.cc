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

#include "gecode/minimodel.hh"

namespace Test { namespace Int { namespace Element {

  /**
   * \defgroup TaskTestIntElement Element constraints
   * \ingroup TaskTestInt
   */
  //@{
  /// Test for element with integer array and integer variables
  class IntInt : public IntTest {
  protected:
    /// Array of integers
    Gecode::IntArgs c;
  public:
    /// Create and register test
    IntInt(const std::string& s, const Gecode::IntArgs& c0)
      : IntTest("Element::Int::Int::"+s,2,-4,8), c(c0) {}
    /// Test whether \a x is solution
    virtual bool solution(const Assignment& x) const {
      return (x[0]>= 0) && (x[0]<c.size()) && c[x[0]]==x[1];
    }
    /// Post constraint on \a x
    virtual void post(Gecode::Space* home, Gecode::IntVarArray& x) {
      Gecode::element(home, c, x[0], x[1], 0);
    }
  };

  /// Test for element with integer array and single shared integer variable
  class IntIntShared : public IntTest {
  protected:
    /// Array of integers
    Gecode::IntArgs c;
  public:
    /// Create and register test
    IntIntShared(const std::string& s, const Gecode::IntArgs& c0)
      : IntTest("Element::Int::Int::Shared::"+s,1,-4,8), c(c0) {}
    /// Test whether \a x is solution
    virtual bool solution(const Assignment& x) const {
      return (x[0]>= 0) && (x[0]<c.size()) && c[x[0]]==x[0];
    }
    /// Post constraint on \a x
    virtual void post(Gecode::Space* home, Gecode::IntVarArray& x) {
      Gecode::element(home, c, x[0], x[0], 0);
    }
  };

  /// Test for element with integer array and integer and Boolean variable
  class IntBool : public IntTest {
  protected:
    /// Array of integers
    Gecode::IntArgs c;
  public:
    /// Create and register test
    IntBool(const std::string& s, const Gecode::IntArgs& c0)
      : IntTest("Element::Int::Bool::"+s,2,-4,8), c(c0) {}
    /// Test whether \a x is solution
    virtual bool solution(const Assignment& x) const {
      return (x[0]>= 0) && (x[0]<c.size()) && c[x[0]]==x[1];
    }
    /// Post constraint on \a x
    virtual void post(Gecode::Space* home, Gecode::IntVarArray& x) {
      Gecode::element(home, c, x[0], Gecode::channel(home,x[1]), 0);
    }
  };


  /*


class Var : public IntTest {
private:
  const int n;
public:
  /// Create and register test
  Var(const char* t, const Gecode::IntSet& is, int n0, Gecode::IntConLevel icl)
    : IntTest(t,n0+2,is,false,icl), n(n0) {}
  /// Test whether \a x is solution
  virtual bool solution(const Assignment& x) const {
    return (x[0]>= 0) && (x[0]<n) && x[2+x[0]]==x[1];
  }
  /// Post constraint on \a x
  virtual void post(Gecode::Space* home, Gecode::IntVarArray& x) {
    IntVarArgs ia(n);
    for (int i=0;i<n;i++)
      ia[i]=x[2+i];
    element(home, ia, x[0], x[1], 0, icl);
  }
};

class ShareVar : public IntTest {
private:
  const int n;
  Gecode::IntConLevel icl;
public:
  // This is actually not domain consistent!
  /// Create and register test
  ShareVar(const char* t, const Gecode::IntSet& is, int n0, Gecode::IntConLevel icl0)
    : IntTest(t,n0+1,is,false), n(n0), icl(icl0) {}
  /// Test whether \a x is solution
  virtual bool solution(const Assignment& x) const {
    return (x[0]>= 0) && (x[0]<n) && x[1+x[0]]==x[0];
  }
  /// Post constraint on \a x
  virtual void post(Gecode::Space* home, Gecode::IntVarArray& x) {
    IntVarArgs ia(n);
    for (int i=0;i<n;i++)
      ia[i]=x[1+i];
    element(home, ia, x[0], x[0], 0, icl);
  }
};
Gecode::IntSet iv1(-1,3);
Var _elvarbnda("Element::Var::Bnd::A",iv1,3,Gecode::ICL_BND);
Var _elvardoma("Element::Var::Dom::A",iv1,3,Gecode::ICL_DOM);
ShareVar _elsvarbnda("Element::Var::Bnd::B",iv1,3,Gecode::ICL_BND);
ShareVar _elsvardoma("Element::Var::Dom::B",iv1,3,Gecode::ICL_DOM);


class VarBool : public IntTest {
private:
  const int n;
public:
  /// Create and register test
  VarBool(const char* t, const Gecode::IntSet& is, int n0)
    : IntTest(t,n0+2,is,false), n(n0) {}
  /// Test whether \a x is solution
  virtual bool solution(const Assignment& x) const {
    for (int i=0;i<n;i++)
      if ((x[2+i] < 0) || (x[2+i]>1))
        return false;
    return (x[0]>= 0) && (x[0]<n) && x[2+x[0]]==x[1] 
      && (x[1]>=0) && (x[1]<=1);
  }
  /// Post constraint on \a x
  virtual void post(Gecode::Space* home, Gecode::IntVarArray& x) {
    Gecode::BoolVarArgs ia(n);
    for (int i=0;i<n;i++)
      ia[i]=channel(home,x[2+i]);
    element(home, ia, x[0], channel(home,x[1]), 0);
  }
};

VarBool _elvarbbnda("Element::Var::Bool",iv1,3);

  */

  Gecode::IntArgs ic1(5, -1,1,-3,3,-4);
  Gecode::IntArgs ic2(8, -1,1,-1,1,-1,1,0,0);
  Gecode::IntArgs ic3(1, -1);
  Gecode::IntArgs ic4(7, 0,-1,2,-2,4,-3,6);

  Gecode::IntArgs bc1(5, 0,1,1,0,1);
  Gecode::IntArgs bc2(8, 1,1,0,1,0,1,0,0);
  Gecode::IntArgs bc3(1, 1);

  IntInt ii1("A",ic1);
  IntInt ii2("B",ic2);
  IntInt ii3("C",ic3);
  IntInt ii4("D",ic4);

  IntIntShared iis1("A",ic1);
  IntIntShared iis2("B",ic2);
  IntIntShared iis3("C",ic3);
  IntIntShared iis4("D",ic4);

  IntBool ib1("A",bc1);
  IntBool ib2("B",bc2);
  IntBool ib3("C",bc3);
  //@}

}}}

// STATISTICS: test-int
