/*
 *  CAUTION:
 *    This file has been automatically generated. Do not edit,
 *    edit the specification file
 *      gecode/int/branch/post-view-int.bs
 *    instead.
 *
 *  This file contains generated code fragments which are
 *  copyrighted as follows:
 *
 *  Main author:
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Copyright:
 *     Christian Schulte, 2008
 *
 *  The generated code fragments are part of Gecode, the generic
 *  constraint development environment:
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

#include "gecode/int/branch.hh"

namespace Gecode { namespace Int { namespace Branch { 

  /// Create virtual view selector for tie-breaking
  void
  virtualize(Gecode::Space* home, IntVarBranch vars,
             const Gecode::VarBranchOptions& o_vars,
             Gecode::ViewSelVirtualBase<IntView>*& v) {
    switch (vars) {
     case INT_VAR_RND:
       v = new (home) ViewSelVirtual<ViewSelRnd<IntView> >(home,o_vars);
       break;
     case INT_VAR_MIN_MIN:
       v = new (home) ViewSelVirtual<ByMinMin>(home,o_vars);
       break;
     case INT_VAR_MIN_MAX:
       v = new (home) ViewSelVirtual<ViewSelInvert<ByMinMin> >(home,o_vars);
       break;
     case INT_VAR_MAX_MIN:
       v = new (home) ViewSelVirtual<ByMaxMin>(home,o_vars);
       break;
     case INT_VAR_MAX_MAX:
       v = new (home) ViewSelVirtual<ViewSelInvert<ByMaxMin> >(home,o_vars);
       break;
     case INT_VAR_SIZE_MIN:
       v = new (home) ViewSelVirtual<BySizeMin>(home,o_vars);
       break;
     case INT_VAR_SIZE_MAX:
       v = new (home) ViewSelVirtual<ViewSelInvert<BySizeMin> >(home,o_vars);
       break;
     case INT_VAR_DEGREE_MIN:
       v = new (home) ViewSelVirtual<ViewSelDegreeMin<IntView> >(home,o_vars);
       break;
     case INT_VAR_DEGREE_MAX:
       v = new (home) ViewSelVirtual<ViewSelInvert<ViewSelDegreeMin<IntView> > >(home,o_vars);
       break;
     case INT_VAR_SIZE_DEGREE_MIN:
       v = new (home) ViewSelVirtual<BySizeDegreeMin>(home,o_vars);
       break;
     case INT_VAR_SIZE_DEGREE_MAX:
       v = new (home) ViewSelVirtual<ViewSelInvert<BySizeDegreeMin> >(home,o_vars);
       break;
     case INT_VAR_REGRET_MIN_MIN:
       v = new (home) ViewSelVirtual<ByRegretMinMin>(home,o_vars);
       break;
     case INT_VAR_REGRET_MIN_MAX:
       v = new (home) ViewSelVirtual<ViewSelInvert<ByRegretMinMin> >(home,o_vars);
       break;
     case INT_VAR_REGRET_MAX_MIN:
       v = new (home) ViewSelVirtual<ByRegretMaxMin>(home,o_vars);
       break;
     case INT_VAR_REGRET_MAX_MAX:
       v = new (home) ViewSelVirtual<ViewSelInvert<ByRegretMaxMin> >(home,o_vars);
       break;
    default:
      throw UnknownBranching("Int::branch");
    }
  }

}}}

namespace Gecode { 

  void
  branch(Gecode::Space* home, const IntVarArgs& x,
         IntVarBranch vars, IntValBranch vals,
         const Gecode::VarBranchOptions& o_vars,
         const Gecode::ValBranchOptions& o_vals) {
    using namespace Gecode;
    using namespace Gecode::Int;
    using namespace Gecode::Int::Branch;


    if (home->failed()) return;
    ViewArray<IntView> xv(home,x);
    switch (vars) {
    case INT_VAR_NONE:
      {
        ViewSelNone<IntView> v(home,o_vars);
        post(home,xv,v,vals,o_vals);
      }
      break;
    case INT_VAR_RND:
      {
        ViewSelRnd<IntView> v(home,o_vars);
        post(home,xv,v,vals,o_vals);
      }
      break;
    case INT_VAR_MIN_MIN:
      {
        ByMinMin v(home,o_vars);
        post(home,xv,v,vals,o_vals);
      }
      break;
    case INT_VAR_MIN_MAX:
      {
        ViewSelInvert<ByMinMin> v(home,o_vars);
        post(home,xv,v,vals,o_vals);
      }
      break;
    case INT_VAR_MAX_MIN:
      {
        ByMaxMin v(home,o_vars);
        post(home,xv,v,vals,o_vals);
      }
      break;
    case INT_VAR_MAX_MAX:
      {
        ViewSelInvert<ByMaxMin> v(home,o_vars);
        post(home,xv,v,vals,o_vals);
      }
      break;
    case INT_VAR_SIZE_MIN:
      {
        BySizeMin v(home,o_vars);
        post(home,xv,v,vals,o_vals);
      }
      break;
    case INT_VAR_SIZE_MAX:
      {
        ViewSelInvert<BySizeMin> v(home,o_vars);
        post(home,xv,v,vals,o_vals);
      }
      break;
    case INT_VAR_DEGREE_MIN:
      {
        ViewSelDegreeMin<IntView> v(home,o_vars);
        post(home,xv,v,vals,o_vals);
      }
      break;
    case INT_VAR_DEGREE_MAX:
      {
        ViewSelInvert<ViewSelDegreeMin<IntView> > v(home,o_vars);
        post(home,xv,v,vals,o_vals);
      }
      break;
    case INT_VAR_SIZE_DEGREE_MIN:
      {
        BySizeDegreeMin v(home,o_vars);
        post(home,xv,v,vals,o_vals);
      }
      break;
    case INT_VAR_SIZE_DEGREE_MAX:
      {
        ViewSelInvert<BySizeDegreeMin> v(home,o_vars);
        post(home,xv,v,vals,o_vals);
      }
      break;
    case INT_VAR_REGRET_MIN_MIN:
      {
        ByRegretMinMin v(home,o_vars);
        post(home,xv,v,vals,o_vals);
      }
      break;
    case INT_VAR_REGRET_MIN_MAX:
      {
        ViewSelInvert<ByRegretMinMin> v(home,o_vars);
        post(home,xv,v,vals,o_vals);
      }
      break;
    case INT_VAR_REGRET_MAX_MIN:
      {
        ByRegretMaxMin v(home,o_vars);
        post(home,xv,v,vals,o_vals);
      }
      break;
    case INT_VAR_REGRET_MAX_MAX:
      {
        ViewSelInvert<ByRegretMaxMin> v(home,o_vars);
        post(home,xv,v,vals,o_vals);
      }
      break;
    default:
      throw UnknownBranching("Int::branch");
    }
  }

  void
  branch(Gecode::Space* home, const IntVarArgs& x,
         const Gecode::TieBreakVarBranch<IntVarBranch>& vars,
         IntValBranch vals,
         const Gecode::TieBreakVarBranchOptions& o_vars,
         const Gecode::ValBranchOptions& o_vals) {
    using namespace Gecode;
    using namespace Gecode::Int;
    using namespace Gecode::Int::Branch;


    if (home->failed()) return;
    if ((vars.a == INT_VAR_NONE) || (vars.a == INT_VAR_RND) ||
        ((vars.b == INT_VAR_NONE) && (vars.c == INT_VAR_NONE) && (vars.d == INT_VAR_NONE))) {
      branch(home,x,vars.a,vals,o_vars.a,o_vals);
      return;
    }
    ViewArray<IntView> xv(home,x);
    Gecode::ViewSelVirtualBase<IntView>* tb[3];
    int n=0;
    if (vars.b != INT_VAR_NONE)
      virtualize(home,vars.b,o_vars.b,tb[n++]);
    if (vars.c != INT_VAR_NONE)
      virtualize(home,vars.c,o_vars.c,tb[n++]);
    if (vars.d != INT_VAR_NONE)
      virtualize(home,vars.d,o_vars.d,tb[n++]);
    assert(n > 0);
    ViewSelTieBreakDynamic<IntView> vbcd(home,tb,n);
    switch (vars.a) {
    case INT_VAR_MIN_MIN:
      {
        ByMinMin va(home,o_vars.a);
        ViewSelTieBreakStatic<ByMinMin,
          ViewSelTieBreakDynamic<IntView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals);
      }
      break;
    case INT_VAR_MIN_MAX:
      {
        ViewSelInvert<ByMinMin> va(home,o_vars.a);
        ViewSelTieBreakStatic<ViewSelInvert<ByMinMin>,
          ViewSelTieBreakDynamic<IntView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals);
      }
      break;
    case INT_VAR_MAX_MIN:
      {
        ByMaxMin va(home,o_vars.a);
        ViewSelTieBreakStatic<ByMaxMin,
          ViewSelTieBreakDynamic<IntView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals);
      }
      break;
    case INT_VAR_MAX_MAX:
      {
        ViewSelInvert<ByMaxMin> va(home,o_vars.a);
        ViewSelTieBreakStatic<ViewSelInvert<ByMaxMin>,
          ViewSelTieBreakDynamic<IntView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals);
      }
      break;
    case INT_VAR_SIZE_MIN:
      {
        BySizeMin va(home,o_vars.a);
        ViewSelTieBreakStatic<BySizeMin,
          ViewSelTieBreakDynamic<IntView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals);
      }
      break;
    case INT_VAR_SIZE_MAX:
      {
        ViewSelInvert<BySizeMin> va(home,o_vars.a);
        ViewSelTieBreakStatic<ViewSelInvert<BySizeMin>,
          ViewSelTieBreakDynamic<IntView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals);
      }
      break;
    case INT_VAR_DEGREE_MIN:
      {
        ViewSelDegreeMin<IntView> va(home,o_vars.a);
        ViewSelTieBreakStatic<ViewSelDegreeMin<IntView>,
          ViewSelTieBreakDynamic<IntView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals);
      }
      break;
    case INT_VAR_DEGREE_MAX:
      {
        ViewSelInvert<ViewSelDegreeMin<IntView> > va(home,o_vars.a);
        ViewSelTieBreakStatic<ViewSelInvert<ViewSelDegreeMin<IntView> >,
          ViewSelTieBreakDynamic<IntView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals);
      }
      break;
    case INT_VAR_SIZE_DEGREE_MIN:
      {
        BySizeDegreeMin va(home,o_vars.a);
        ViewSelTieBreakStatic<BySizeDegreeMin,
          ViewSelTieBreakDynamic<IntView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals);
      }
      break;
    case INT_VAR_SIZE_DEGREE_MAX:
      {
        ViewSelInvert<BySizeDegreeMin> va(home,o_vars.a);
        ViewSelTieBreakStatic<ViewSelInvert<BySizeDegreeMin>,
          ViewSelTieBreakDynamic<IntView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals);
      }
      break;
    case INT_VAR_REGRET_MIN_MIN:
      {
        ByRegretMinMin va(home,o_vars.a);
        ViewSelTieBreakStatic<ByRegretMinMin,
          ViewSelTieBreakDynamic<IntView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals);
      }
      break;
    case INT_VAR_REGRET_MIN_MAX:
      {
        ViewSelInvert<ByRegretMinMin> va(home,o_vars.a);
        ViewSelTieBreakStatic<ViewSelInvert<ByRegretMinMin>,
          ViewSelTieBreakDynamic<IntView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals);
      }
      break;
    case INT_VAR_REGRET_MAX_MIN:
      {
        ByRegretMaxMin va(home,o_vars.a);
        ViewSelTieBreakStatic<ByRegretMaxMin,
          ViewSelTieBreakDynamic<IntView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals);
      }
      break;
    case INT_VAR_REGRET_MAX_MAX:
      {
        ViewSelInvert<ByRegretMaxMin> va(home,o_vars.a);
        ViewSelTieBreakStatic<ViewSelInvert<ByRegretMaxMin>,
          ViewSelTieBreakDynamic<IntView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals);
      }
      break;
    default:
      throw UnknownBranching("Int::branch");
    }
  }

}


// STATISTICS: int-branch

