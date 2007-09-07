/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Guido Tack <tack@gecode.org>
 *     Christian Schulte <schulte@gecode.org>
 *     Mikael Lagerkvist <lagerkvist@gecode.org>
 *
 *  Copyright:
 *     Guido Tack, 2005
 *     Christian Schulte, 2005
 *     Mikael Lagerkvist, 2005
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

#include "test/set.hh"
#include "test/log.hh"
#include "gecode/iter.hh"
#include <algorithm>

CountableSet::CountableSet(const IntSet& d0) : d(d0), //curd(IntSet::empty),
                                               cur(0) {
  IntSetRanges isr(d);
  lubmax =
    static_cast<unsigned int>(pow(static_cast<double>(2.0),
                                  static_cast<int>(Iter::Ranges::size(isr))));
}

void CountableSet::operator++(void) {
  cur++;
}

void CountableSet::init(const IntSet& d0) {
  d = d0;
  cur = 0;
  IntSetRanges isr(d);
  lubmax =
    static_cast<unsigned int>(pow(static_cast<double>(2.0),
                                  static_cast<int>(Iter::Ranges::size(isr))));
}

int CountableSet::val(void) const {
  return cur;
}

SetAssignment::SetAssignment(int n0, const IntSet& d0, int _withInt)
  : n(n0), dsv(new CountableSet[n]), ir(_withInt, d0), done(false), lub(d0), withInt(_withInt) {
  for (int i=n; i--; )
    dsv[i].init(lub);
}

void
SetAssignment::operator++(void) {
  int i = n-1;
  while (true) {
    ++dsv[i];
    if (dsv[i]())
      return;
    dsv[i].init(lub);
    --i;
    if (i<0) {
      if (withInt==0) {
        done = true;
        return;
      }
      ++ir;
      if (ir()) {
        i = n-1;
        for (int j=n; j--; )
          dsv[j].init(lub);
      } else {
        done = true;
        return;
      }
    }
  }
}

std::ostream&
operator<<(std::ostream& os, const SetAssignment& a) {
  int n = a.size();
  os << "{";
  for (int i=0; i<n; i++) {
    CountableSetRanges csv(a.lub, a[i]);
    IntSet icsv(csv);
    os << icsv << ((i!=n-1) ? "," : "}");
  }
  if (a.withInt > 0)
    os << a.ints();
  return os;
}

#define FORCE_FIX                               \
do {                                            \
  if (Base::rand(opt.fixprob) == 0) {        \
    Log::fixpoint();                            \
    if (status() == SS_FAILED) return;          \
  }                                             \
} while(false)

class SetTestSpace : public Space {
public:
  SetVarArray x;
  IntVarArray y;
private:
  const Options opt;
  int withInt;
public:
  SetTestSpace(int n, IntSet& d, int _withInt, const Options& o)
    : x(this, n, IntSet::empty, d), y(this, _withInt, d),
      opt(o), withInt(_withInt) {
    Log::initial(x, "x");
  }
  SetTestSpace(bool share, SetTestSpace& s) : Space(share,s), opt(s.opt),
                                              withInt(s.withInt) {
    x.update(this, share, s.x);
    y.update(this, share, s.y);
  }
  virtual Space* copy(bool share) {
    return new SetTestSpace(share,*this);
  }
  bool failed(void) {
    return status() == SS_FAILED;
  }
  void assign(const SetAssignment& a) {
    for (int i=a.size(); i--; ) {
      CountableSetRanges csv(a.lub, a[i]);
      IntSet ai(csv);
      Log::assign(Log::mk_name("x", i), ai);
      dom(this, x[i], SRT_EQ, ai);
      FORCE_FIX;
    }
    for (int i=withInt; i--; ) {
      Log::assign(Log::mk_name("y", i), a.ints()[i]);
      rel(this, y[i], IRT_EQ, a.ints()[i]);
      FORCE_FIX;
    }
  }
  bool assigned(void) const {
    for (int i=x.size(); i--; )
      if (!x[i].assigned())
        return false;
    for (int i=y.size(); i--; )
      if (!y[i].assigned())
        return false;
    return true;
  }

  void removeFromLub(int v, SetVar& x, int i, const SetAssignment& a) {
    SetVarUnknownRanges ur(x);
    CountableSetRanges air(a.lub, a[i]);
    Iter::Ranges::Diff<SetVarUnknownRanges, CountableSetRanges> diff(ur, air);
    Iter::Ranges::ToValues<Iter::Ranges::Diff
      <SetVarUnknownRanges, CountableSetRanges> > diffV(diff);
    for (int j=0; j<v; j++, ++diffV);
    Log::prune(x, Log::mk_name("x", i), SRT_DISJ, diffV.val());
    dom(this, x, SRT_DISJ, diffV.val());
    Log::prune_result(x);
  }

  void addToGlb(int v, SetVar& x, int i, const SetAssignment& a) {
    SetVarUnknownRanges ur(x);
    CountableSetRanges air(a.lub, a[i]);
    Iter::Ranges::Inter<SetVarUnknownRanges, CountableSetRanges>
      inter(ur, air);
    Iter::Ranges::ToValues<Iter::Ranges::Inter
      <SetVarUnknownRanges, CountableSetRanges> > interV(inter);
    for (int j=0; j<v; j++, ++interV);
    Log::prune(x, Log::mk_name("x", i), SRT_SUP, interV.val());
    dom(this, x, SRT_SUP, interV.val());
    Log::prune_result(x);
  }

  bool fixprob(SetTest& st, bool r, BoolVar& b) {
    Log::fixpoint();                                
    if (status() == SS_FAILED)
      return true;
    SetTestSpace* c = static_cast<SetTestSpace*>(clone());
    Log::print(c->x, "x");
    if (c->y.size() > 0) Log::print(c->y, "y");
    if (!r) {
      st.post(c,c->x,c->y);
      Log::fixpoint();
      if (c->status() == SS_FAILED) {
        Log::print(c->x, "x");
        if (c->y.size() > 0) Log::print(c->y, "y");
        delete c;
        return false;
      }
      for (int i=x.size(); i--; ) {
        if (x[i].glbSize() != c->x[i].glbSize() ||
            x[i].lubSize() != c->x[i].lubSize() ||
            x[i].cardMin() != c->x[i].cardMin() ||
            x[i].cardMax() != c->x[i].cardMax()) {
          Log::print(c->x, "x");
          if (c->y.size() > 0) Log::print(c->y, "y");
          delete c;
          return false;
        }
      }
    } else {
      Log::print(b, "b");
      BoolVar cb(c,0,1);
      Log::initial(cb, "cb");
      st.post(c,c->x,c->y,cb);
      Log::fixpoint();
      if (c->status() == SS_FAILED) {
        Log::print(c->x, "x");
        if (c->y.size() > 0) Log::print(c->y, "y");
        Log::print(cb, "cb");
        delete c;
        return false;
      }
      if (cb.size() != b.size()) {
        Log::print(c->x, "x");
        if (c->y.size() > 0) Log::print(c->y, "y");
        Log::print(cb, "cb");
        delete c;
        return false;
      }
      for (int i=x.size(); i--; )
        if (x[i].glbSize() != c->x[i].glbSize() ||
            x[i].lubSize() != c->x[i].lubSize() ||
            x[i].cardMin() != c->x[i].cardMin() ||
            x[i].cardMax() != c->x[i].cardMax() ) {
          Log::print(c->x, "x");
          if (c->y.size() > 0) Log::print(c->y, "y");
          Log::print(cb, "cb");
          delete c;
          return false;
        }
    }
    delete c;
    return true;
  }

  static BoolVar unused;

  bool prune(const SetAssignment& a, SetTest& st,
             bool r, BoolVar& b=unused) {
    bool setsAssigned = true;
    for (int j=x.size(); j--; )
      if (!x[j].assigned()) {
        setsAssigned = false;
        break;
      }
    bool intsAssigned = true;
    for (int j=y.size(); j--; )
      if (!y[j].assigned()) {
        intsAssigned = false;
        break;
      }

    // Select variable to be pruned
    int i;
    if (intsAssigned) {
      i = Base::rand(x.size());
    } else if (setsAssigned) {
      i = Base::rand(y.size());
    } else {
      i = Base::rand(x.size()+y.size());
    }

    if (setsAssigned || i>=x.size()) {
      if (i>=x.size())
        i = i-x.size();
      while (y[i].assigned()) {
        i = (i+1) % y.size();
      }
      // Prune int var


      // Select mode for pruning
      int m=Base::rand(3);
      if ((m == 0) && (a.ints()[i] < y[i].max())) {
        int v=a.ints()[i]+1+
          Base::rand(static_cast<unsigned int>(y[i].max()-a.ints()[i]));
        assert((v > a.ints()[i]) && (v <= y[i].max()));
        Log::prune(y[i], Log::mk_name("y", i), IRT_LE, v);
        rel(this, y[i], IRT_LE, v);
        Log::prune_result(y[i]);
      } else if ((m == 1) && (a.ints()[i] > y[i].min())) {
        int v=y[i].min()+
          Base::rand(static_cast<unsigned int>(a.ints()[i]-y[i].min()));
        assert((v < a.ints()[i]) && (v >= y[i].min()));
        Log::prune(y[i], Log::mk_name("y", i), IRT_GR, v);
        rel(this, y[i], IRT_GR, v);
        Log::prune_result(y[i]);
      } else  {
        int v;
        Gecode::Int::ViewRanges<Gecode::Int::IntView> it(y[i]);
        unsigned int skip = Base::rand(y[i].size()-1);
        while (true) {
          if (it.width() > skip) {
            v = it.min() + skip;
            if (v == a.ints()[i]) {
              if (it.width() == 1) {
                ++it; v = it.min();
              } else if (v < it.max()) {
                ++v;
              } else {
                --v;
              }
            }
            break;
          }
          skip -= it.width();
          ++it;
        }
        Log::prune(y[i], Log::mk_name("y", i), IRT_NQ, v);
        rel(this, y[i], IRT_NQ, v);
        Log::prune_result(y[i]);
      }
      if (Base::rand(opt.fixprob) == 0 && !fixprob(st, r, b))
        return false;
      return true;
    }
    while (x[i].assigned()) {
      i = (i+1) % x.size();
    }
    SetVarUnknownRanges ur1(x[i]);
    CountableSetRanges air1(a.lub, a[i]);
    Iter::Ranges::Diff<SetVarUnknownRanges, CountableSetRanges>
      diff(ur1, air1);
    SetVarUnknownRanges ur2(x[i]);
    CountableSetRanges air2(a.lub, a[i]);
    Iter::Ranges::Inter<SetVarUnknownRanges, CountableSetRanges>
      inter(ur2, air2);

    CountableSetRanges aisizer(a.lub, a[i]);
    unsigned int aisize = Iter::Ranges::size(aisizer);

    // Select mode for pruning
    int m=Base::rand(5);
    if (m==0 && inter()) {
      int v = Base::rand(Iter::Ranges::size(inter));
      addToGlb(v, x[i], i, a);
    } else if (m==1 && diff()) {
      int v = Base::rand(Iter::Ranges::size(diff));
      removeFromLub(v, x[i], i, a);
    } else if (m==2 && x[i].cardMin() < aisize) {
      unsigned int newc = x[i].cardMin() + 1 +
        Base::rand(aisize - x[i].cardMin());
      assert( newc > x[i].cardMin() );
      assert( newc <= aisize );
      Log::prune(x[i], Log::mk_name("x", i), newc, Limits::Set::card_max);
      cardinality(this, x[i], newc, Limits::Set::card_max);
      Log::prune_result(x[i]);
    } else if (m==3 && x[i].cardMax() > aisize) {
      unsigned int newc = x[i].cardMax() - 1 -
        Base::rand(x[i].cardMax() - aisize);
      assert( newc < x[i].cardMax() );
      assert( newc >= aisize );
      Log::prune(x[i], Log::mk_name("x", i), 0, newc);
      cardinality(this, x[i], 0, newc);
      Log::prune_result(x[i]);
    } else {
      if (inter()) {
        int v = Base::rand(Iter::Ranges::size(inter));
        addToGlb(v, x[i], i, a);
      } else {
        int v = Base::rand(Iter::Ranges::size(diff));
        removeFromLub(v, x[i], i, a);
      }
    }
    if (Base::rand(opt.fixprob) == 0 && !fixprob(st, r, b))
      return false;

    return true;
  }

};

BoolVar SetTestSpace::unused;


#define CHECK(T,M)                                 \
if (!(T)) {                                         \
  problem = (M);                                 \
  delete s;                                        \
  goto failed;                                         \
}

SetAssignment*
SetTest::make_assignment(void) {
  return new SetAssignment(arity,lub,withInt);
}

bool
SetTest::run(const Options& opt) {
  const char* test    = "NONE";
  const char* problem = "NONE";

  SetAssignment* ap = make_assignment();
  SetAssignment& a = *ap;
  while (a()) {
    bool is_sol = solution(a);
    {
      test = "Assignment (after posting)";
      Log::reset();
      SetTestSpace* s = new SetTestSpace(arity,lub,withInt,opt);
      post(s,s->x,s->y); s->assign(a);
      if (is_sol) {
        CHECK(!s->failed(), "Failed on solution");
        CHECK(s->propagators()==0, "No subsumtion");
      } else {
        CHECK(s->failed(), "Solved on non-solution");
      }
      delete s;
    }
    {
      test = "Assignment (before posting)";
      Log::reset();
      SetTestSpace* s = new SetTestSpace(arity,lub,withInt,opt);
      s->assign(a); post(s,s->x,s->y);
      if (is_sol) {
        CHECK(!s->failed(), "Failed on solution");
        CHECK(s->propagators()==0, "No subsumtion");
      } else {
        CHECK(s->failed(), "Solved on non-solution");
      }
      delete s;
    }
    if (reified) {
      test = "Assignment reified (before posting)";
      Log::reset();
      SetTestSpace* s = new SetTestSpace(arity,lub,withInt,opt);
      BoolVar b(s,0,1);
      s->assign(a); post(s,s->x,s->y,b);
      CHECK(!s->failed(), "Failed");
      CHECK(s->propagators()==0, "No subsumtion");
      CHECK(b.assigned(), "Control variable unassigned");
      if (is_sol) {
        CHECK(b.val()==1, "Zero on solution");
      } else {
        CHECK(b.val()==0, "One on non-solution");
      }
      delete s;
    }
    if (reified) {
      test = "Assignment reified (after posting)";
      Log::reset();
      SetTestSpace* s = new SetTestSpace(arity,lub,withInt,opt);
      BoolVar b(s,0,1);
      post(s,s->x,s->y,b); s->assign(a);
      CHECK(!s->failed(), "Failed");
      CHECK(s->propagators()==0, "No subsumtion");
      CHECK(b.assigned(), "Control variable unassigned");
      if (is_sol) {
        CHECK(b.val()==1, "Zero on solution");
      } else {
        CHECK(b.val()==0, "One on non-solution");
      }
      delete s;
    }
    {
      test = "Prune";
      Log::reset();
      SetTestSpace* s = new SetTestSpace(arity,lub,withInt,opt);
      post(s,s->x,s->y);
      while (!s->failed() && !s->assigned())
         if (!s->prune(a,*this,false)) {
           problem = "No fixpoint";
           delete s;
           goto failed;
         }
      s->assign(a);
      if (is_sol) {
        CHECK(!s->failed(), "Failed on solution");
        CHECK(s->propagators()==0, "No subsumtion");
      } else {
        CHECK(s->failed(), "Solved on non-solution");
      }
      delete s;
    }
    if (reified) {
      test = "Prune reified";
      Log::reset();
      SetTestSpace* s = new SetTestSpace(arity,lub,withInt,opt);
      BoolVar b(s,0,1);
      post(s,s->x,s->y,b);
      while (!s->assigned() && !b.assigned())
         if (!s->prune(a,*this,true,b)) {
           problem = "No fixpoint";
           delete s;
           goto failed;
         }
      CHECK(!s->failed(), "Failed");
      CHECK(s->propagators()==0, "No subsumtion");
      CHECK(b.assigned(), "Control variable unassigned");
      if (is_sol) {
        CHECK(b.val()==1, "Zero on solution");
      } else {
        CHECK(b.val()==0, "One on non-solution");
      }
      delete s;
    }
    ++a;
  }
  delete ap;
  return true;
 failed:
  std::cout << "FAILURE" << std::endl
            << "\t" << "Test:       " << test << std::endl
            << "\t" << "Problem:    " << problem << std::endl
            << "\t" << "SetAssignment: " << a << std::endl;
  delete ap;
  return false;
}

#undef CHECK

// STATISTICS: test-set
