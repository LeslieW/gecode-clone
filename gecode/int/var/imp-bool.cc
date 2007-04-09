/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Copyright:
 *     Christian Schulte, 2006
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

#include "gecode/int.hh"
#include "gecode/int/var/imp-bool-body.icc"

namespace Gecode { namespace Int {

  BoolVarImp BoolVarImp::s_one(1);
  BoolVarImp BoolVarImp::s_zero(0);

  /*
   * Advisors
   *
   */
#if GECODE_USE_ADVISORS
  bool
  BoolVarImp::advisors(Space *home) {
    int lo, hi;
    switch (status()) {
    case NONE: GECODE_NEVER; lo=1; hi=0; break;
    case ZERO: lo=hi=1; break;
    case ONE:  lo=hi=0; break;
    }
    for (Advisors a(*this); a(); ++a) 
      switch (static_cast<IntAdvisor*>(a.advisor())
              ->_advise(home,ME_INT_VAL,lo,hi)) {
      case ES_FIX:
      case __ES_SUBSUMED: 
        break;
      case ES_FAILED:     
        return false;
      case ES_NOFIX:
        schedule(home,a.advisor()->parent(),ME_INT_VAL);
        break;
      default:
        GECODE_NEVER;
      }
    return true;
  }
#endif
}}

// STATISTICS: int-var
