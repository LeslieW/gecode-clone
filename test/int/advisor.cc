/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Mikael Lagerkvist <lagerkvist@gecode.org>
 *
 *  Copyright:
 *     Mikael Lagerkvist, 2006
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
#include "gecode/int/distinct.hh"

namespace {

  using namespace Gecode;
  using namespace Int;

  Gecode::IntSet nl(0,3);

  class Keep : public NaryPropagator<IntView,PC_INT_NONE> {
    using NaryPropagator<IntView,PC_INT_NONE>::x;
    Council<ViewAdvisor<IntView> > c;
    int n;
    /// Constructor for posting
    /// Create and register test
    Keep(Gecode::Space* home, ViewArray<IntView>& x)
      : NaryPropagator<IntView,PC_INT_NONE>(home,x), c(home), n(0) {
      for (int i=x.size(); i--; ) {
        if (x[i].assigned()) {
          IntView::schedule(home,this,ME_INT_VAL);
        } else {
          for (int j=10*(i+1); j--; ) {
            (void) new (home) ViewAdvisor<IntView>(home,this,c,x[i]);
            n++;
          }
          IntView::schedule(home,this,ME_INT_BND);
        }
      }
    }
    /// Constructor for cloning \a p
    /// Create and register test
    Keep(Gecode::Space* home, bool share, Keep& p) 
      : NaryPropagator<IntView,PC_INT_NONE>(home,share,p), n(p.n) {
      c.update(home,share,p.c);
    }
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Gecode::Space* home, bool share) {
      return new (home) Keep(home,share,*this);
    }
    virtual ExecStatus advise(Gecode::Space* home, Advisor* _a, const Delta* d) {
      ViewAdvisor<IntView>* a = static_cast<ViewAdvisor<IntView>*>(_a);
      ModEvent me = IntView::modevent(d);
      if (me == ME_INT_VAL) {
        n--;
        return ES_SUBSUMED_NOFIX(a,home,c);
      }
      if (n > 0)
        return ES_FIX;
      return ES_NOFIX;
    }
    virtual const char* name(void) const { 
      return "Keep"; 
    }
    /// Perform propagation
    virtual ExecStatus propagate(Gecode::Space* home) {
      if (n == 0)
        return ES_SUBSUMED(this,home);
      return ES_FIX;
    }
    /// Post propagator for view array \a x
    /// Post constraint on \a x
    static ExecStatus post(Gecode::Space* home, ViewArray<IntView>& x) {
      (void) new (home) Keep(home,x);
      return ES_OK;
    }
    size_t dispose(Gecode::Space* home) {
      c.dispose(home);
      (void) NaryPropagator<IntView,PC_INT_NONE>::dispose(home);
      return sizeof(*this);
    }
  };

  class AdviseKeep : public IntTest {
  public:
    /// Create and register test
    AdviseKeep(void)
      : IntTest("Advisor::Keep",4,nl,false,Gecode::ICL_DEF) {}
    /// Test whether \a x is solution
    virtual bool solution(const Assignment& x) const {
      return true;
    }
    /// Post constraint on \a x
    virtual void post(Gecode::Space* home, Gecode::IntVarArray& x) {
      if (home->failed())
        return;
      ViewArray<IntView> xv(home,x.size());
      for (int i=x.size(); i--; )
        xv[i]=x[i];
      /// Post constraint on \a x
      Keep::post(home,xv);
    }
  };

  AdviseKeep _ak;

  Gecode::IntSet ds(0,5);

  class AD : public NaryPropagator<IntView,PC_INT_NONE> {
  protected:
    using NaryPropagator<IntView,PC_INT_NONE>::x;
    Council<ViewAdvisor<IntView> > c;
    /// Constructor for posting
    /// Create and register test
    AD(Gecode::Space* home, ViewArray<IntView>& x)
      : NaryPropagator<IntView,PC_INT_NONE>(home,x), c(home) {
      for (int i=x.size(); i--; ) {
        (void) new (home) ViewAdvisor<IntView>(home,this,c,x[i]);
        if (x[i].assigned())
          IntView::schedule(home,this,ME_INT_VAL);
      }
    }
    /// Constructor for cloning \a p
    /// Create and register test
    AD(Gecode::Space* home, bool share, AD& p) 
      : NaryPropagator<IntView,PC_INT_NONE>(home,share,p) {
      c.update(home,share,p.c);
    }
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Gecode::Space* home, bool share) {
      return new (home) AD(home,share,*this);
    }
    virtual const char* name(void) const { return "AD"; }
    virtual ExecStatus advise(Gecode::Space* home, Advisor* _a, const Delta* d) {
      ViewAdvisor<IntView>* a = static_cast<ViewAdvisor<IntView>*>(_a);
      ModEvent me = IntView::modevent(d);
      if (me == ME_INT_VAL)
        return ES_SUBSUMED_NOFIX(a,home,c);
      return ES_FIX;
    }
    /// Perform propagation
    virtual ExecStatus propagate(Gecode::Space* home) {
      GECODE_ES_CHECK((Gecode::Int::Distinct::prop_val<IntView,true>(home,x)));
      return (x.size() < 2) ? ES_SUBSUMED(this,home) : ES_FIX;
    }
    /// Post propagator for view array \a x
    /// Post constraint on \a x
    static ExecStatus post(Gecode::Space* home, ViewArray<IntView>& x) {
      if (x.size() > 1)
        (void) new (home) AD(home,x);
      return ES_OK;
    }
    size_t dispose(Gecode::Space* home) {
      c.dispose(home);
      (void) NaryPropagator<IntView,PC_INT_NONE>::dispose(home);
      return sizeof(*this);
    }
  };

  class AdviseDistinct : public IntTest {
  public:
    /// Create and register test
    AdviseDistinct(void)
      : IntTest("Advisor::Distinct",5,ds,false,Gecode::ICL_DEF) {}
    /// Test whether \a x is solution
    virtual bool solution(const Assignment& x) const {
      for (int i=0; i<x.size(); i++)
        for (int j=i+1; j<x.size(); j++)
          if (x[i]==x[j])
            return false;
      return true;
    }
    /// Post constraint on \a x
    virtual void post(Gecode::Space* home, Gecode::IntVarArray& x) {
      if (home->failed())
        return;
      ViewArray<IntView> xv(home,x.size());
      for (int i=x.size(); i--; )
        xv[i]=x[i];
      /// Post constraint on \a x
      AD::post(home,xv);
    }
  };

  AdviseDistinct _ad;


  class Eq : public Propagator {
  protected:

    class BndAdvisor : public ViewAdvisor<IntView> {
      using ViewAdvisor<IntView>::x;
    public:
      /// Create and register test
      BndAdvisor(Gecode::Space* home, Propagator* p, 
                 Council<BndAdvisor>& c, IntView v) 
        : ViewAdvisor<IntView>(home,p,c,v) {
        if (x.assigned())
          IntView::schedule(home,p,ME_INT_VAL);
        else
          IntView::schedule(home,p,ME_INT_BND);
      }
      /// Create and register test
      BndAdvisor(Gecode::Space* home, bool share, BndAdvisor& a) 
        : ViewAdvisor<IntView>(home,share,a) {}
    };
    
    Council<BndAdvisor> c;
    IntView x0, x1;

    virtual ExecStatus advise(Gecode::Space* home, Advisor* _a, const Delta* d) {
      BndAdvisor* a = static_cast<BndAdvisor*>(_a);
      ModEvent me = IntView::modevent(d);
      if (me == ME_INT_VAL)
        return ES_SUBSUMED_NOFIX(a,home,c);
      return ES_NOFIX;
    }

    /// Constructor for cloning \a p
    Eq(Gecode::Space* home, bool share, Eq& p) 
      : Propagator(home, share, p) {
      c.update(home,share,p.c);
      x0.update(home,share,p.x0);
      x1.update(home,share,p.x1);
    }

    /// Constructor for posting
    Eq(Gecode::Space* home, IntView _x0, IntView _x1) 
      : Propagator(home), c(home), x0(_x0), x1(_x1) {
      (void) new (home) BndAdvisor(home, this, c, x0);
      (void) new (home) BndAdvisor(home, this, c, x1);
    }
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Gecode::Space* home, bool share) {
      return new (home) Eq(home, share, *this);
    }
    virtual const char* name(void) const { return "Eq"; }
    virtual PropCost cost(void) const { 
      return PC_BINARY_LO; 
    }
    /// Perform propagation
    virtual ExecStatus propagate(Gecode::Space* home) {
      while ((x0.min() != x1.min()) &&
             (x0.max() != x1.max())) {
        GECODE_ME_CHECK(x0.lq(home, x1.max()));
        GECODE_ME_CHECK(x0.gq(home, x1.min()));
        GECODE_ME_CHECK(x1.lq(home, x0.max()));
        GECODE_ME_CHECK(x1.gq(home, x0.min()));
      }
      if (x0.assigned() && x1.assigned())
        return ES_SUBSUMED(this, dispose(home));
      return ES_NOFIX;
    }
    /// Post bounds-consistent propagator \f$ x_0=x_1\f$
    /// Post constraint on \a x
    static  ExecStatus  post(Gecode::Space* home, IntView x0, IntView x1) {
      (void) new (home) Eq(home, x0, x1);
      return ES_OK;
    }
    size_t
    dispose(Gecode::Space* home) {
      c.dispose(home);
      (void) Propagator::dispose(home);
      return sizeof(*this);
    }
  };

  class BasicIntAdvisor : public IntTest {
  public:
    /// Create and register test
    BasicIntAdvisor(void)
      : IntTest("Advisor::Eq::Int",2,-5,5) {}
    /// Test whether \a x is solution
    virtual bool solution(const Assignment& x) const {
      return x[0] == x[1];
    }
    /// Post constraint on \a x
    virtual void post(Gecode::Space* home, Gecode::IntVarArray& x) {
      Int::IntView x0(x[0]), x1(x[1]);
      /// Post constraint on \a x
      Eq::post(home, x0, x1);
    }
  };

  BasicIntAdvisor _basicintadvisor;

  Gecode::IntSet bs(0,1);

  class BoolEq : public Propagator {
  protected:
    class BndAdvisor : public ViewAdvisor<BoolView> {
      using ViewAdvisor<BoolView>::x;
    public:
      /// Create and register test
      BndAdvisor(Gecode::Space* home, Propagator* p, Council<BndAdvisor>& c, 
                 BoolView v) 
        : ViewAdvisor<BoolView>(home,p,c,v) {
        if (x.assigned())
          BoolView::schedule(home,p,ME_INT_BND);
      }
      /// Create and register test
      BndAdvisor(Gecode::Space* home, bool share, BndAdvisor& a) 
        : ViewAdvisor<BoolView>(home,share,a) {}
    };
    
    Council<BndAdvisor> c;

    BoolView x0, x1;
    virtual ExecStatus advise(Gecode::Space* home, Advisor* _a, const Delta* d) {
      BndAdvisor* a = static_cast<BndAdvisor*>(_a);
      return ES_SUBSUMED_NOFIX(a,home,c);
    }

    /// Constructor for cloning \a p
    BoolEq(Gecode::Space* home, bool share, BoolEq& p) 
      : Propagator(home, share, p) {
      c.update(home,share,p.c);
      x0.update(home,share,p.x0);
      x1.update(home,share,p.x1);
    }
    /// Constructor for posting
    BoolEq(Gecode::Space* home, BoolView _x0, BoolView _x1) 
      : Propagator(home), c(home), x0(_x0), x1(_x1) {
      (void) new (home) BndAdvisor(home, this, c, x0);
      (void) new (home) BndAdvisor(home, this, c, x1);
    }
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Gecode::Space* home, bool share) {
      return new (home) BoolEq(home, share, *this);
    }
    virtual const char* name(void) const { return "BoolEq"; }
    virtual PropCost cost(void) const { 
      return PC_BINARY_LO; 
    }
    /// Perform propagation
    virtual ExecStatus propagate(Gecode::Space* home) {
      if (x1.assigned()) {
        if (me_failed(x0.eq(home, x1.val())))
          return ES_FAILED;
      } else {
        if (me_failed(x1.eq(home, x0.val())))
          return ES_FAILED;
      }
      return ES_SUBSUMED(this, dispose(home));
    }
    /// Post bounds-consistent propagator \f$ x_0=x_1\f$
    /// Post constraint on \a x
    static  ExecStatus  post(Gecode::Space* home, BoolView x0, BoolView x1) {
      (void) new (home) BoolEq(home, x0, x1);
      return ES_OK;
    }
    size_t
    dispose(Gecode::Space* home) {
      c.dispose(home);
      (void) Propagator::dispose(home);
      return sizeof(*this);
    }
  };

  class BasicBoolAdvisor : public IntTest {
  public:
    /// Create and register test
    BasicBoolAdvisor(void)
      : IntTest("Advisor::Eq::Bool",2,bs) {}
    /// Test whether \a x is solution
    virtual bool solution(const Assignment& x) const {
      return x[0] == x[1];
    }
    /// Post constraint on \a x
    virtual void post(Gecode::Space* home, Gecode::IntVarArray& x) {
      Int::BoolView x0(channel(home,x[0])), x1(channel(home,x[1]));
      /// Post constraint on \a x
      BoolEq::post(home, x0, x1);
    }
  };

  BasicBoolAdvisor _basicbooladvisor;
}


// STATISTICS: test-int
