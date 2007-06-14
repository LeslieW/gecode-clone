/*
 *  Main authors:
 *     Patrick Pekczynski <pekczynski@ps.uni-sb.de>
 *
 *  Copyright:
 *     Patrick Pekczynski, 2006
 *
 *  Last modified:
 *     $Date: 2006-07-29 15:55:20 +0000 (Sat, 29 Jul 2006) $ by $Author: pekczynski $
 *     $Revision: 3478 $
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

#include "gecode/bdd.hh"
#include "gecode/bdd/bddprop.hh"

using namespace Gecode::Bdd;

namespace Gecode {

  void
  disjointglb(Space* home, const BddVarArgs& x, int index) {
    if (home->failed()) return;

    int n = x.size();
    ViewArray<BddView> bv(home, n);
    for (int i = n; i--; )
      bv[i] = x[i];
    GECODE_ES_FAIL(home, DisjointGlb<BddView>::post(home, bv, index));

  }

  void
  disjointsudoku(Space* home, BddVar x, int order) {
    if (home->failed()) return;
    ViewArray<BddView> bv(home, 1);
    bv[0] = x;
    GECODE_ES_FAIL(home, DisjointSudoku<BddView>::post(home, bv[0], order));

  }

}

// STATISTICS: bdd-post
