/*
 *  CAUTION:
 *    This file has been automatically generated. Do not edit,
 *    edit the specification file
 *      gecode/float/branch/post-view.bs
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

#include <gecode/float/branch.hh>

namespace Gecode { namespace Float { namespace Branch {

  /// Create virtual view selector for tie-breaking
  void
  virtualize(Gecode::Home home, FloatVarBranch vars,
             const Gecode::VarBranchOptions& o_vars,
             Gecode::ViewSelVirtualBase<FloatView>*& v) {
    switch (vars) {
    case FLOAT_VAR_RND:
      v = new (home) ViewSelVirtual<ViewSelRnd<FloatView> >(home,o_vars);
      break;
    case FLOAT_VAR_MIN_MIN:
      v = new (home) ViewSelVirtual<ViewSelMin<MeritMin> >(home,o_vars);
      break;
    case FLOAT_VAR_MIN_MAX:
      v = new (home) ViewSelVirtual<ViewSelMax<MeritMin> >(home,o_vars);
      break;
    case FLOAT_VAR_MAX_MIN:
      v = new (home) ViewSelVirtual<ViewSelMin<MeritMax> >(home,o_vars);
      break;
    case FLOAT_VAR_MAX_MAX:
      v = new (home) ViewSelVirtual<ViewSelMax<MeritMax> >(home,o_vars);
      break;
    case FLOAT_VAR_SIZE_MIN:
      v = new (home) ViewSelVirtual<ViewSelMin<MeritSize> >(home,o_vars);
      break;
    case FLOAT_VAR_SIZE_MAX:
      v = new (home) ViewSelVirtual<ViewSelMax<MeritSize> >(home,o_vars);
      break;
    case FLOAT_VAR_DEGREE_MIN:
      v = new (home) ViewSelVirtual<ViewSelMin<MeritDegree<FloatView> > >(home,o_vars);
      break;
    case FLOAT_VAR_DEGREE_MAX:
      v = new (home) ViewSelVirtual<ViewSelMax<MeritDegree<FloatView> > >(home,o_vars);
      break;
    case FLOAT_VAR_AFC_MIN:
      v = new (home) ViewSelVirtual<ViewSelMin<MeritAfc<FloatView> > >(home,o_vars);
      break;
    case FLOAT_VAR_AFC_MAX:
      v = new (home) ViewSelVirtual<ViewSelMax<MeritAfc<FloatView> > >(home,o_vars);
      break;
    case FLOAT_VAR_ACTIVITY_MIN:
      v = new (home) ViewSelVirtual<ViewSelMin<MeritActivity<FloatView> > >(home,o_vars);
      break;
    case FLOAT_VAR_ACTIVITY_MAX:
      v = new (home) ViewSelVirtual<ViewSelMax<MeritActivity<FloatView> > >(home,o_vars);
      break;
    case FLOAT_VAR_SIZE_DEGREE_MIN:
      v = new (home) ViewSelVirtual<ViewSelMin<MeritSizeDegree> >(home,o_vars);
      break;
    case FLOAT_VAR_SIZE_DEGREE_MAX:
      v = new (home) ViewSelVirtual<ViewSelMax<MeritSizeDegree> >(home,o_vars);
      break;
    case FLOAT_VAR_SIZE_AFC_MIN:
      v = new (home) ViewSelVirtual<ViewSelMin<MeritSizeAfc> >(home,o_vars);
      break;
    case FLOAT_VAR_SIZE_AFC_MAX:
      v = new (home) ViewSelVirtual<ViewSelMax<MeritSizeAfc> >(home,o_vars);
      break;
    case FLOAT_VAR_SIZE_ACTIVITY_MIN:
      v = new (home) ViewSelVirtual<ViewSelMin<MeritSizeActivity> >(home,o_vars);
      break;
    case FLOAT_VAR_SIZE_ACTIVITY_MAX:
      v = new (home) ViewSelVirtual<ViewSelMax<MeritSizeActivity> >(home,o_vars);
      break;
    default:
      throw UnknownBranching("Float::branch");
    }
  }

}}}

namespace Gecode {

  void
  branch(Gecode::Home home, const FloatVarArgs& x,
         FloatVarBranch vars, FloatValBranch vals,
         const Gecode::VarBranchOptions& o_vars,
         const Gecode::ValBranchOptions& o_vals) {
    using namespace Gecode;
    using namespace Gecode::Float;
    using namespace Gecode::Float::Branch;


    if (home.failed()) return;
    if (o_vars.activity.initialized() &&
        (o_vars.activity.size() != x.size()))
      throw ActivityWrongArity("branch");
    ViewArray<FloatView> xv(home,x);
    switch (vars) {
    case FLOAT_VAR_NONE:
      {
        ViewSelNone<FloatView> v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case FLOAT_VAR_RND:
      {
        ViewSelRnd<FloatView> v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case FLOAT_VAR_MIN_MIN:
      {
        ViewSelMin<MeritMin> v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case FLOAT_VAR_MIN_MAX:
      {
        ViewSelMax<MeritMin> v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case FLOAT_VAR_MAX_MIN:
      {
        ViewSelMin<MeritMax> v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case FLOAT_VAR_MAX_MAX:
      {
        ViewSelMax<MeritMax> v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case FLOAT_VAR_SIZE_MIN:
      {
        ViewSelMin<MeritSize> v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case FLOAT_VAR_SIZE_MAX:
      {
        ViewSelMax<MeritSize> v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case FLOAT_VAR_DEGREE_MIN:
      {
        ViewSelMin<MeritDegree<FloatView> > v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case FLOAT_VAR_DEGREE_MAX:
      {
        ViewSelMax<MeritDegree<FloatView> > v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case FLOAT_VAR_AFC_MIN:
      {
        ViewSelMin<MeritAfc<FloatView> > v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case FLOAT_VAR_AFC_MAX:
      {
        ViewSelMax<MeritAfc<FloatView> > v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case FLOAT_VAR_ACTIVITY_MIN:
      {
        ViewSelMin<MeritActivity<FloatView> > v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case FLOAT_VAR_ACTIVITY_MAX:
      {
        ViewSelMax<MeritActivity<FloatView> > v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case FLOAT_VAR_SIZE_DEGREE_MIN:
      {
        ViewSelMin<MeritSizeDegree> v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case FLOAT_VAR_SIZE_DEGREE_MAX:
      {
        ViewSelMax<MeritSizeDegree> v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case FLOAT_VAR_SIZE_AFC_MIN:
      {
        ViewSelMin<MeritSizeAfc> v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case FLOAT_VAR_SIZE_AFC_MAX:
      {
        ViewSelMax<MeritSizeAfc> v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case FLOAT_VAR_SIZE_ACTIVITY_MIN:
      {
        ViewSelMin<MeritSizeActivity> v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case FLOAT_VAR_SIZE_ACTIVITY_MAX:
      {
        ViewSelMax<MeritSizeActivity> v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    default:
      throw UnknownBranching("Float::branch");
    }
  }

  void
  branch(Gecode::Home home, const FloatVarArgs& x,
         const Gecode::TieBreakVarBranch<FloatVarBranch>& vars,
         FloatValBranch vals,
         const Gecode::TieBreakVarBranchOptions& o_vars,
         const Gecode::ValBranchOptions& o_vals) {
    using namespace Gecode;
    using namespace Gecode::Float;
    using namespace Gecode::Float::Branch;


    if (home.failed()) return;
    if ((vars.a == FLOAT_VAR_NONE) || (vars.a == FLOAT_VAR_RND) ||
        ((vars.b == FLOAT_VAR_NONE) && (vars.c == FLOAT_VAR_NONE) && (vars.d == FLOAT_VAR_NONE))) {
      branch(home,x,vars.a,vals,o_vars.a,o_vals);
      return;
    }
    if (o_vars.a.activity.initialized() &&
        (o_vars.a.activity.size() != x.size()))
      throw ActivityWrongArity("branch (option a)");
    if (o_vars.b.activity.initialized() &&
        (o_vars.b.activity.size() != x.size()))
      throw ActivityWrongArity("branch (option b)");
    if (o_vars.c.activity.initialized() &&
        (o_vars.c.activity.size() != x.size()))
      throw ActivityWrongArity("branch (option c)");
    if (o_vars.d.activity.initialized() &&
        (o_vars.d.activity.size() != x.size()))
      throw ActivityWrongArity("branch (option d)");
    ViewArray<FloatView> xv(home,x);
    Gecode::ViewSelVirtualBase<FloatView>* tb[3];
    int n=0;
    if (vars.b != FLOAT_VAR_NONE)
      virtualize(home,vars.b,o_vars.b,tb[n++]);
    if (vars.c != FLOAT_VAR_NONE)
      virtualize(home,vars.c,o_vars.c,tb[n++]);
    if (vars.d != FLOAT_VAR_NONE)
      virtualize(home,vars.d,o_vars.d,tb[n++]);
    assert(n > 0);
    ViewSelTieBreakDynamic<FloatView> vbcd(home,tb,n);
    switch (vars.a) {
    case FLOAT_VAR_MIN_MIN:
      {
        ViewSelMin<MeritMin> va(home,o_vars.a);
        ViewSelTieBreakStatic<ViewSelMin<MeritMin>,
          ViewSelTieBreakDynamic<FloatView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    case FLOAT_VAR_MIN_MAX:
      {
        ViewSelMax<MeritMin> va(home,o_vars.a);
        ViewSelTieBreakStatic<ViewSelMax<MeritMin>,
          ViewSelTieBreakDynamic<FloatView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    case FLOAT_VAR_MAX_MIN:
      {
        ViewSelMin<MeritMax> va(home,o_vars.a);
        ViewSelTieBreakStatic<ViewSelMin<MeritMax>,
          ViewSelTieBreakDynamic<FloatView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    case FLOAT_VAR_MAX_MAX:
      {
        ViewSelMax<MeritMax> va(home,o_vars.a);
        ViewSelTieBreakStatic<ViewSelMax<MeritMax>,
          ViewSelTieBreakDynamic<FloatView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    case FLOAT_VAR_SIZE_MIN:
      {
        ViewSelMin<MeritSize> va(home,o_vars.a);
        ViewSelTieBreakStatic<ViewSelMin<MeritSize>,
          ViewSelTieBreakDynamic<FloatView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    case FLOAT_VAR_SIZE_MAX:
      {
        ViewSelMax<MeritSize> va(home,o_vars.a);
        ViewSelTieBreakStatic<ViewSelMax<MeritSize>,
          ViewSelTieBreakDynamic<FloatView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    case FLOAT_VAR_DEGREE_MIN:
      {
        ViewSelMin<MeritDegree<FloatView> > va(home,o_vars.a);
        ViewSelTieBreakStatic<ViewSelMin<MeritDegree<FloatView> >,
          ViewSelTieBreakDynamic<FloatView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    case FLOAT_VAR_DEGREE_MAX:
      {
        ViewSelMax<MeritDegree<FloatView> > va(home,o_vars.a);
        ViewSelTieBreakStatic<ViewSelMax<MeritDegree<FloatView> >,
          ViewSelTieBreakDynamic<FloatView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    case FLOAT_VAR_AFC_MIN:
      {
        ViewSelMin<MeritAfc<FloatView> > va(home,o_vars.a);
        ViewSelTieBreakStatic<ViewSelMin<MeritAfc<FloatView> >,
          ViewSelTieBreakDynamic<FloatView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    case FLOAT_VAR_AFC_MAX:
      {
        ViewSelMax<MeritAfc<FloatView> > va(home,o_vars.a);
        ViewSelTieBreakStatic<ViewSelMax<MeritAfc<FloatView> >,
          ViewSelTieBreakDynamic<FloatView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    case FLOAT_VAR_ACTIVITY_MIN:
      {
        ViewSelMin<MeritActivity<FloatView> > va(home,o_vars.a);
        ViewSelTieBreakStatic<ViewSelMin<MeritActivity<FloatView> >,
          ViewSelTieBreakDynamic<FloatView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    case FLOAT_VAR_ACTIVITY_MAX:
      {
        ViewSelMax<MeritActivity<FloatView> > va(home,o_vars.a);
        ViewSelTieBreakStatic<ViewSelMax<MeritActivity<FloatView> >,
          ViewSelTieBreakDynamic<FloatView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    case FLOAT_VAR_SIZE_DEGREE_MIN:
      {
        ViewSelMin<MeritSizeDegree> va(home,o_vars.a);
        ViewSelTieBreakStatic<ViewSelMin<MeritSizeDegree>,
          ViewSelTieBreakDynamic<FloatView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    case FLOAT_VAR_SIZE_DEGREE_MAX:
      {
        ViewSelMax<MeritSizeDegree> va(home,o_vars.a);
        ViewSelTieBreakStatic<ViewSelMax<MeritSizeDegree>,
          ViewSelTieBreakDynamic<FloatView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    case FLOAT_VAR_SIZE_AFC_MIN:
      {
        ViewSelMin<MeritSizeAfc> va(home,o_vars.a);
        ViewSelTieBreakStatic<ViewSelMin<MeritSizeAfc>,
          ViewSelTieBreakDynamic<FloatView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    case FLOAT_VAR_SIZE_AFC_MAX:
      {
        ViewSelMax<MeritSizeAfc> va(home,o_vars.a);
        ViewSelTieBreakStatic<ViewSelMax<MeritSizeAfc>,
          ViewSelTieBreakDynamic<FloatView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    case FLOAT_VAR_SIZE_ACTIVITY_MIN:
      {
        ViewSelMin<MeritSizeActivity> va(home,o_vars.a);
        ViewSelTieBreakStatic<ViewSelMin<MeritSizeActivity>,
          ViewSelTieBreakDynamic<FloatView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    case FLOAT_VAR_SIZE_ACTIVITY_MAX:
      {
        ViewSelMax<MeritSizeActivity> va(home,o_vars.a);
        ViewSelTieBreakStatic<ViewSelMax<MeritSizeActivity>,
          ViewSelTieBreakDynamic<FloatView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    default:
      throw UnknownBranching("Float::branch");
    }
  }

}


// STATISTICS: float-branch

