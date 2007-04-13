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
 *  See the file "LICENSE" for information on usage and
 *  redistribution of this file, and for a
 *     DISCLAIMER OF ALL WARRANTIES.
 *
 */

#include "test/int.hh"
#include "gecode/minimodel.hh"

static IntSet ds(0,1);

class BoolEq : public IntTest {
public:
  BoolEq(const char* t)
    : IntTest(t,2,ds) {}
  virtual bool solution(const Assignment& x) const {
    return x[0]==x[1];
  }
  virtual void post(Space* home, IntVarArray& x) {
    rel(home, channel(home,x[0]), IRT_EQ, channel(home,x[1]));
  }
};
BoolEq _booleq("Bool::Eq");

class BoolNot : public IntTest {
public:
  BoolNot(const char* t)
    : IntTest(t,2,ds) {}
  virtual bool solution(const Assignment& x) const {
    return x[0]!=x[1];
  }
  virtual void post(Space* home, IntVarArray& x) {
    rel(home, channel(home,x[0]), IRT_NQ, channel(home,x[1]));
  }
};
BoolNot _boolnot("Bool::Not");

class BoolAnd : public IntTest {
public:
  BoolAnd(const char* t)
    : IntTest(t,3,ds) {}
  virtual bool solution(const Assignment& x) const {
    return (x[0]&x[1])==x[2];
  }
  virtual void post(Space* home, IntVarArray& x) {
    rel(home, channel(home,x[0]), BOT_AND, channel(home,x[1]), channel(home,x[2]));
  }
};
BoolAnd _booland("Bool::And::Binary");

class BoolOr : public IntTest {
public:
  BoolOr(const char* t)
    : IntTest(t,3,ds) {}
  virtual bool solution(const Assignment& x) const {
    return (x[0]|x[1])==x[2];
  }
  virtual void post(Space* home, IntVarArray& x) {
    rel(home, channel(home,x[0]), BOT_OR, channel(home,x[1]), channel(home,x[2]));
  }
};
BoolOr _boolor("Bool::Or::Binary");

class BoolImp : public IntTest {
public:
  BoolImp(const char* t)
    : IntTest(t,3,ds) {}
  virtual bool solution(const Assignment& x) const {
    return ((x[0] == 0 ? 1 : 0)|x[1])==x[2];
  }
  virtual void post(Space* home, IntVarArray& x) {
    rel(home, channel(home,x[0]), BOT_IMP, channel(home,x[1]), channel(home,x[2]));
  }
};
BoolImp _boolimp("Bool::Imp");

class BoolEqv : public IntTest {
public:
  BoolEqv(const char* t)
    : IntTest(t,3,ds) {}
  virtual bool solution(const Assignment& x) const {
    return (x[0] == x[1])==x[2];
  }
  virtual void post(Space* home, IntVarArray& x) {
    rel(home, channel(home,x[0]), BOT_EQV, channel(home,x[1]), channel(home,x[2]));
  }
};
BoolEqv _booleqv("Bool::Eqv");

class BoolXor : public IntTest {
public:
  BoolXor(const char* t)
    : IntTest(t,3,ds) {}
  virtual bool solution(const Assignment& x) const {
    return (x[0] != x[1])==x[2];
  }
  virtual void post(Space* home, IntVarArray& x) {
    rel(home, channel(home,x[0]), BOT_XOR, channel(home,x[1]), channel(home,x[2]));
  }
};
BoolXor _boolxor("Bool::Xor");


class BoolAndNary : public IntTest {
public:
  BoolAndNary(const char* t)
    : IntTest(t,14,ds) {}
  virtual bool solution(const Assignment& x) const {
    for (int i = x.size()-1; i--; )
      if (x[i] == 0)
        return x[x.size()-1] == 0;
    return x[x.size()-1] == 1;
  }
  virtual void post(Space* home, IntVarArray& x) {
    BoolVarArgs b(2*(x.size()-1));
    for (int i=x.size()-1; i--; ) {
      BoolVar bx = channel(home,x[i]);
      b[2*i+0] = bx; b[2*i+1] = bx;
    }
    rel(home, b, BOT_AND, channel(home,x[x.size()-1]));
  }
};
BoolAndNary _boolandnary("Bool::And::Nary");

class BoolOrNary : public IntTest {
public:
  BoolOrNary(const char* t)
    : IntTest(t,14,ds) {}
  virtual bool solution(const Assignment& x) const {
    for (int i = x.size()-1; i--; )
      if (x[i] == 1)
        return x[x.size()-1] == 1;
    return x[x.size()-1] == 0;
  }
  virtual void post(Space* home, IntVarArray& x) {
    BoolVarArgs b(2*(x.size()-1));
    for (int i=x.size()-1; i--; ) {
      BoolVar bx = channel(home,x[i]);
      b[2*i+0] = bx; b[2*i+1] = bx;
    }
    rel(home, b, BOT_OR, channel(home,x[x.size()-1]));
  }
};
BoolOrNary _boolornary("Bool::Or::Nary");

class BoolOrTrueNary : public IntTest {
public:
  BoolOrTrueNary(const char* t)
    : IntTest(t,14,ds) {}
  virtual bool solution(const Assignment& x) const {
    for (int i = x.size(); i--; )
      if (x[i] == 1)
        return true;
    return false;
  }
  virtual void post(Space* home, IntVarArray& x) {
    BoolVarArgs b(2*(x.size()));
    for (int i=x.size(); i--; ) {
      BoolVar bx = channel(home,x[i]);
      b[2*i+0] = bx; b[2*i+1] = bx;
    }
    rel(home, b, BOT_OR, 1);
  }
};
BoolOrTrueNary _boolortruenary("Bool::Or::True::Nary");

#ifdef GECODE_USE_ADVISORS
class BoolOrWLNary : public IntTest {
public:
  BoolOrWLNary(const char* t)
    : IntTest(t,14,ds) {}
  virtual bool solution(const Assignment& x) const {
    for (int i = x.size()-1; i--; )
      if (x[i] == 1)
        return x[x.size()-1] == 1;
    return x[x.size()-1] == 0;
  }
  virtual void post(Space* home, IntVarArray& x) {
    BoolVarArgs b(2*(x.size()-1));
    for (int i=x.size()-1; i--; ) {
      BoolVar bx = channel(home,x[i]);
      b[2*i+0] = bx; b[2*i+1] = bx;
    }
    rel(home, b, BOT_OR_WL, channel(home,x[x.size()-1]));
  }
};
BoolOrNary _boolorwlnary("Bool::Advisors::WL::Or::Nary");

class BoolOrWLTrueNary : public IntTest {
public:
  BoolOrWLTrueNary(const char* t)
    : IntTest(t,14,ds) {}
  virtual bool solution(const Assignment& x) const {
    for (int i = x.size(); i--; )
      if (x[i] == 1)
        return true;
    return false;
  }
  virtual void post(Space* home, IntVarArray& x) {
    BoolVarArgs b(2*(x.size()));
    for (int i=x.size(); i--; ) {
      BoolVar bx = channel(home,x[i]);
      b[2*i+0] = bx; b[2*i+1] = bx;
    }
    rel(home, b, BOT_OR_WL, 1);
  }
};
BoolOrWLTrueNary _boolorwltruenary("Bool::Advisors::WL::Or::True::Nary");
#endif

// STATISTICS: test-int

