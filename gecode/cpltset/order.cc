/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Patrick Pekczynski <pekczynski@ps.uni-sb.de>
 *
 *  Copyright:
 *     Patrick Pekczynski, 2006
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

#include "gecode/cpltset.hh"
using namespace Gecode::CpltSet;

namespace Gecode {

  void 
  hls_order(const CpltSetVarArray& x) {
    // std::cout << "start order\n";
    BddMgr* mgr = x[0].manager();
    unsigned int var_in_tab = mgr->allocated();
    
    // std::cout << "var_in_tab = " << var_in_tab << "\n";

    // constraint order as specified by Hawkins, Lagoon and Stuckey
    GECODE_AUTOARRAY(int, hls_order, var_in_tab);
    
    unsigned int min_offset = x[0].offset();
    unsigned int max_width  = x[0].table_width();
    for (int i = 0; i < x.size(); i++) {
      if (x[i].offset() < min_offset) {
        min_offset = x[i].offset();
      }
      if (x[i].table_width() > max_width) {
        max_width = x[i].table_width();
      }
    }
    
//     std::cout << "max_width = " << max_width  << " min_offset = " << min_offset << "\n";
    

    // we do not care about variables 
    // that are not in the scope of the constraint
    int c = 0;
    for (unsigned int i = 0; i < min_offset; i++, c++) {
      hls_order[i] = i;
    }
    // ordering those that lie in the scope
    for (unsigned int f = 0; f < max_width; f++) {
      for (int i = 0; i < x.size(); i++) {
        int xo = x[i].offset();
        // std::cout << "order " << xo << "\n";
        int xw = x[i].table_width();
        int cur= xo + f;
        if (cur <= xo + xw) {
          hls_order[c] = cur;
          c++;
        }
      }
    }

    // IMPORTANT DO NOT FORGET REMAINING LARGER VARIABLES
    for (unsigned int i = c; i < var_in_tab; i++, c++) {
      hls_order[i] = i;
    }   
    // std::cout << "set order\n";
    mgr->setorder(&hls_order[0]);
    // std::cout << "ordering done\n";
  }

}

// STATISTICS: bdd-other
