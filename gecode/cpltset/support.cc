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

namespace Gecode { namespace CpltSet {

  void
  conv_hull_project(bdd& robdd, 
                    bdd& hull, bdd& poshull) {
    if (manager.leaf(robdd)) { hull = robdd; return; }
    if (manager.leaf(robdd)) { poshull = robdd; return; }
    assert(robdd != bdd_true());
    assert(robdd != bdd_false());

    BddIterator bi(robdd);
    while(bi()) {
      int stat = bi.status();
      int lev  = bi.label();
      bdd cur = manager.bddpos(lev);
      // in order to be able to quantify ALL bounds out
      // we have to store the variables not in the lub
      // in their positive form
      if (stat == FIX_GLB) {
        assert(!manager.marked(cur));
        hull    &= cur;
        poshull &= cur;
      }
      if (stat == FIX_NOT_LUB) {
        poshull &= cur;
        hull    &= manager.negbddpos(lev);
      }
      ++bi;
    }
    return;
  }

  void
  conv_hull(bdd& robdd, bdd& hull) {
    if (manager.leaf(robdd)) { hull = robdd; return; }
    BddIterator bi(robdd);
    while(bi()) {
      int stat = bi.status();
      int lev  = bi.label();
      // in order to be able to quantify ALL bounds out
      // we have to store the variables not in the lub
      // in their positive form
      if (stat == FIX_GLB)     { hull &= manager.bddpos(lev); }
      if (stat == FIX_NOT_LUB) { hull &= manager.negbddpos(lev); }
      ++bi;
    }
    return;
  }
  
  void
  conv_project(bdd& robdd, bdd& poshull) {
    if (manager.leaf(robdd)) { poshull = robdd; return; }
    BddIterator bi(robdd);
    while(bi()) {
      int stat = bi.status();
      int lev  = bi.label();
      // in order to be able to quantify ALL bounds out
      // we have to store the variables not in the lub
      // in their positive form
      if (stat == FIX_GLB)     { poshull &= manager.bddpos(lev); }
      if (stat == FIX_NOT_LUB) { poshull &= manager.bddpos(lev); }
      ++bi;
    }
    return;
  }

  bdd 
  bdd_vars(bdd& robdd) {

    bdd allvars    = bdd_true();
    BddIterator bi(robdd);
    while(bi()) {
      int lev  = bi.label();
      // in order to be able to quantify ALL bounds out
      // we have to store the variables not in the lub
      // in their positive form
      bdd cur = manager.bddpos(lev);
      assert(!manager.marked(cur));
      allvars &= cur;
      ++bi;
    }
    
    return allvars;
  }

  bdd 
  cardeq(int offset, int c, int n, int r) {
    GECODE_AUTOARRAY(bdd, layer, n);
    // the use of autoarray now requires explicit initialization
    // otherwise the bdd nodes are not known in the global table
    for (int i = n; i--;) 
      layer[i].init();

    // build the nodes of the lowest layer
    layer[0] = bdd_true();
    for (int i = 1; i <= c; i++) {
      layer[i].init();
      layer[i] = manager.bddpos(offset + r - i + 1);
    }

    // connect the lowest layer
    for (int i = 1; i < n; i++) {
      layer[i] = manager.ite(layer[i], layer[i - 1], bdd_false());
    }

    // build layers on top
    for (int k = r + 1; --k; ) {
      int col = k;
      for (int i = 0; i < n; i++) {
        bdd t = bdd_true();
        if (i == 0) {
          t = bdd_false();
        } else {
          t = layer[i-1]; 
        }
        layer[i] = manager.ite(manager.bddpos(offset + col), t,layer[i]);
        col--;
        if (col < 0) { k = 1; break;}
      }
    }
    return layer[n - 1];
  }

  bdd 
  cardlqgq(int offset, int cl, int cr, int n, int r) {
    GECODE_AUTOARRAY(bdd, layer, n);
    // the use of autoarray now requires explicit initialization
    // otherwise the bdd nodes are not known in the global table
    for (int i = n; i--;) 
      layer[i].init();

    // creates TOP v(c) v(c-1) ... v(c - cl + 1)
    layer[n - cl - 1] = bdd_true();
    int k = r;
    for (int i = n - cl ; i < n; i++) {
        layer[i] = manager.ite(manager.bddpos(offset + k), layer[i - 1], 
                               bdd_false());
      k--;
    }

    for (k = r; k--; ) {
      int col = k;
      // cl < cr <= tab  ==> n - cl > 0 
      for (int i = n - cl; i < n; i++) { 
        bdd t = layer[i-1]; 
        layer[i] = manager.ite(manager.bddpos(offset + col), t, layer[i]);
        col--;
        if (col < r + 1 - cr) { k = 0; break;}
      }
    }

    if (cr == r + 1) { // cardinality equals table_width, all elements allowed
      return layer[n - 1];
    }

    if (cr == r) {
      int col = r;
      // one single large layer
      bdd t = bdd_true();
      bdd f = bdd_true();
      bdd zerot = bdd_false();
      bdd zerof = t;
      for (int i = 0; i < n; i++) {
        if (i == 0) {
          t = zerot;
          f = zerof;
        } else {
          t = layer[i-1];
          if (i > n - cl - 1) { // connect lower layer
            f = layer[i];
          }
        }
        layer[i] = manager.ite(manager.bddpos(offset + col), t ,f);
        col--;
        if (col < 0) { break;}
      }

      return layer[n- 1];
    }
  
    // connect lower and upper parts of the bdd
    // one single layer in between
    int col = r;
    {
      bdd t = bdd_true();
      bdd f = bdd_true();
      for (int i = 0; i < n; i++) {
        if (i == 0) {
          t = bdd_false();
        } else {
          t = layer[i-1];
          // NOTE: ONLY CONNECT if cl > 0
          if (i > n - cl - 1 && cl > 0) { // connect lower layer
            f = layer[i];
          }
        }
        layer[i] = manager.ite(manager.bddpos(offset + col), t ,f);
        col--;
        if (col < 0) { break;}
      }
    }

    // the remaining layers 
    for (k = r; --k; ) {
      int col = k;
      for (int i = 0; i < n; i++) {
        bdd t = bdd_true();
        if (i == 0) {
          t = bdd_false();
        } else {
          t = layer[i-1]; 
        }
        layer[i] = manager.ite(manager.bddpos(offset + col), t, layer[i]);
        col--;
        if (col < 0) { k = 1; break;}
      }
    }
    return layer[n - 1];
  }

  bdd 
  cardcheck(int xtab, int offset, int cl, int cr) {
    if (cr > xtab) { 
      cr = xtab;
    }
    int r = xtab - 1; // rightmost bit in bitvector
    int n = cr + 1; // layer size
    if (cl > xtab || cl > cr)  // inconsistent cardinality
      return bdd_false();

    if (cr == 0) {    // cl <= cr
      // build the emptyset
      bdd empty = bdd_true();
      for (int i = xtab; i--; ) {
        empty &= manager.negbddpos(offset + i);
      }
      return empty;
    }
  
    if (cl == cr) {
      if (cr == xtab) {
        // build the full set
        bdd full = bdd_true();
        for (int i = xtab; i--; ) {
          full &= manager.bddpos(offset + i);
        }
        return full;
      } else {
        return cardeq(offset, cr, n, r);
      }
    }

    // cl < cr
    if (cr == xtab) {
      if (cl == 0) {   // no cardinality restriction
        return bdd_true();
      }
    }
    return cardlqgq(offset, cl, cr, n, r);
  }

  bdd 
  cardrec_bin(int j, unsigned int& off1, unsigned int& range1, 
              unsigned int& off2, unsigned int& range2, int l, int u) {
    if ((l <= 0) && ((int) range1 <= u)) {
      return bdd_true();
    }
    if ( ((int) range1 < l) || (u < 0)) {
      return bdd_false();
    }
    bdd cur = bdd_true();
    if (j < (int) range1) {      
      cur = ( manager.bddpos(off1 + j) & manager.bddpos(off2 + j) );
      j++;
    } else {
      if (l > 0 ) {
        return bdd_false();
      } else {
        return bdd_true();
      }
    }

    bdd neg = bdd_true();
    neg &= (!cur & cardrec_bin(j, off1, range1, off2, range2, l, u));
    bdd pos = bdd_true();
    pos &= (cur  & cardrec_bin(j, off1, range1, off2, range2, l - 1, u - 1));
    bdd cv = bdd_true();
    cv &= (neg | pos);
    return cv;
  }


  bdd 
  cardrec(bdd& boundvars, 
          int j, unsigned int& offset, unsigned int& range, int l, int u) {
    bdd b = boundvars;
    if (!manager.leaf(boundvars)) {
      int idx = manager.bddidx(boundvars) - offset;
      while ( j == idx ) {
        j++;
        boundvars = manager.iftrue(boundvars);
        if (manager.leaf(boundvars)) {
          break;
        }
        idx = manager.bddidx(boundvars) - offset;
      }
    }

    if ((l <= 0) && ((int) range <= u)) {
      return bdd_true();
    }
    if ( ((int) range < l) || (u < 0)) {
      return bdd_false();
    }
    bdd cur = bdd_true();
    if (j < (int) range) {      
      cur = manager.bddpos(offset + j);
      j++;
    } else {
      if (l > 0 ) {
        return bdd_false();
      } else {
        return bdd_true();
      }
    }
    b = boundvars;
    bdd neg = (!cur & cardrec(b, j, offset, range, l, u));
    b = boundvars;
    bdd pos = (cur  & cardrec(b, j, offset, range, l - 1, u - 1));
    bdd cv = bdd_true();
    cv &= (neg | pos);
    return cv;
  }

  bdd 
  lexlt(unsigned int& xoff, unsigned int& yoff, unsigned int& range, int n) {
    if (n < 0) { return bdd_false(); }

    bdd cur = bdd_true();
    cur &= ((manager.negbddpos(xoff + n)) & (manager.bddpos(yoff + n)));
    cur |= ( ((manager.bddpos(xoff + n)) % (manager.bddpos(yoff + n))) & 
             lexlt(xoff, yoff, range, n - 1));
    return cur;
  }

  bdd 
  lexlq(unsigned int& xoff, unsigned int& yoff, unsigned int& range, int n) {
    if (n < 0) { return bdd_true(); }

    bdd cur = bdd_true();
    cur &= ((manager.negbddpos(xoff + n)) & (manager.bddpos(yoff + n)));
    cur |= ( ((manager.bddpos(xoff + n)) % (manager.bddpos(yoff + n))) & 
             lexlq(xoff, yoff, range, n - 1));
    return cur;
  }


  bdd 
  lexltrev(unsigned int& xoff, unsigned int& yoff,
           unsigned int& range, int n) {
    if (n > (int) range - 1) { return bdd_false(); }
    bdd cur = bdd_true();
    cur &= ((manager.negbddpos(xoff + n)) & (manager.bddpos(yoff + n)));
    cur |= ( ((manager.bddpos(xoff + n)) % (manager.bddpos(yoff + n))) & 
             lexltrev(xoff, yoff, range, n + 1));
    return cur;
  }

  bdd 
  lexlqrev(unsigned int& xoff, unsigned int& yoff,
           unsigned int& range, int n) {
    if (n > static_cast<int> (range) - 1) { return bdd_true(); }

    bdd cur = bdd_true();
    cur &= ((manager.negbddpos(xoff + n)) & (manager.bddpos(yoff + n)));
    cur |= ( ((manager.bddpos(xoff + n)) % (manager.bddpos(yoff + n))) & 
             lexlqrev(xoff, yoff, range, n + 1));
    return cur;
  }

  void card_count(bdd& remain, bdd& boundvars,
                  unsigned int n, unsigned int& offset, 
                  unsigned int& range, int& l, int& u) {
    // skip variables in the boolean vector that 
    // belong to the fixed part of the bdd
    if (!manager.leaf(boundvars)) {
      unsigned int idx = manager.bddidx(boundvars) - offset;
      while ( n == idx ) {
        n++;
        boundvars = manager.iftrue(boundvars);
        if (manager.leaf(boundvars)) {
          break;
        }
        idx = manager.bddidx(boundvars) - offset;
      }
    }

    if (manager.cfalse(remain)) {
      // mark false branch by 
      // (Limits::Set::card_max, -Limits::Set::card_max)
      l = Limits::Set::card_max;
      u = -Limits::Set::card_max;
      return;
    } else {
      if (manager.ctrue(remain)) {
        if (n == range) {
          // if we have seen all variables in the tree
          // mark the true branch with (0,0)
          l = 0;
          u = 0;
          return;
        } else {
          // if we have not yet seen all variables in the tree
          // mark the true branch with (0, range - n)
          l = 0;
          u = range - n;
          return;
        }
      } else {
            bdd b = boundvars;
        if (manager.bddidx(remain) - offset == n) {
          int lt = 0;
          int ut = 0;
          bdd t = manager.iftrue(remain);
          
          card_count(t,  b, n + 1, offset, range, lt, ut);

          int lf = 0;
          int uf = 0;
          b = boundvars;
          bdd f = manager.iffalse(remain);
          card_count(f,  b, n + 1, offset, range, lf, uf);
          
          l = std::min(lt + 1, lf);
          u = std::max(ut + 1, uf);
          return;
        } else {
          b = boundvars;
          card_count(remain,  b, n + 1, offset, range, l, u);
          u++;
          return;
        }
      }
    }
  }

  // mark all nodes in the dqueue
  void
  extcache_mark(Support::DynamicArray<bdd>& nodes, 
                int n, int& l, int& r, int& markref) {
    // the left side
    if (l > 0) {
      for (int i = 0; i < l; i++) {
        if (!manager.marked(nodes[i])) {
          manager.mark(nodes[i]);
          markref++;
        }
      }
    }
    // the right side
    if (r < n - 1) {
      for (int i = r + 1; i < n; i++) {
        if (!manager.marked(nodes[i])) {
          manager.mark(nodes[i]);
          markref++;
        }
      }
    }
  }

  // unmark all nodes in the dqueue
  void
  extcache_unmark(Support::DynamicArray<bdd>& nodes, 
                  int n, int& l, int& r, int& markref) {
    if (l > 0) {
      for (int i = 0; i < l; i++) {
        if (manager.marked(nodes[i])) {
          manager.unmark(nodes[i]);
          markref--;
        } 
      }
    }
    if (r < n - 1) {
      for (int i = r + 1; i < n; i++) {
        if (manager.marked(nodes[i])) {
          manager.unmark(nodes[i]);
          markref--;
        } 
      }
    }
  }


  // iterate to the next level of nodes
  void 
  extcardbounds(int& markref, bdd& c, int& n, int& l, int& r,
                bool& singleton, int& _level, 
                Support::DynamicArray<bdd>& nodes, 
                int& curmin, int& curmax) {
    bdd cur = bdd_true();

    if (((l == 0) && (r == n - 1))) {
      // no more nodes on the stack to be iterated
      singleton = false;
      extcache_unmark(nodes, n, l, r, markref);
      assert(markref == 0);
      return;
    }

    // mark nodes under exploration
    extcache_mark(nodes, n, l, r,markref);
    if (l > 0) {
      _level++;
    }

    // NEW
    bool stackleft = false;
    while (l > 0) {
      stackleft = true;
      /* 
       * if left side contains at least two nodes 
       * L: n_1 n_2 ______
       * if level(n_1) < level(n_2) move n_2 to the right end of the dqueue
       * maintain the invariant: 
       * for all nodes n_i, n_j in L: level(n_i) = level(n_j)
       * difference detected set Gecode::CpltSet::UNDET
       */
      while ((l > 1) && 
             manager.bddidx(nodes[l - 1]) < manager.bddidx(nodes[l - 2])) {
        int shift = l - 2;
        int norm  = l - 1;
        manager.unmark(nodes[shift]); markref--;
        if (r == n - 1) {
          nodes[r] = nodes[shift];
          manager.mark(nodes[r]); markref++;
        } else {    
          for (int i = r; i < n - 1; i++) {
            nodes[i] = nodes[i + 1];
          }
          nodes[n - 1] = nodes[shift];
          manager.mark(nodes[n - 1]); markref++;
        }
        r--;
        nodes[shift] = nodes[norm];
        nodes[norm].init();
        l--;
      }
      // symmetric case
      while ((l > 1) && 
             manager.bddidx(nodes[l - 1]) > manager.bddidx(nodes[l - 2])) {
        int shift = l - 1;
        manager.unmark(nodes[shift]); markref--;
        if (r == n - 1) {
          nodes[r] = nodes[shift];
          manager.mark(nodes[r]); markref++;
        } else {    
          for (int i = r; i < n - 1; i++) {
            nodes[i] = nodes[i + 1];
          }
          nodes[n - 1] = nodes[shift];
          manager.mark(nodes[n - 1]); markref++;
        }
        r--;
        nodes[shift].init();
        l--;
      }

      l--;
      manager.unmark(nodes[l]);
      markref--; 
      cur = nodes[l];
      assert(!manager.marked(cur));
      nodes[l].init();

      // cur is an internal node 
      if (!manager.leaf(cur)) {
        bdd t   = manager.iftrue(cur);
        bdd f   = manager.iffalse(cur);
        // unsigned int cur_idx = manager.bddidx(cur);

        bool leaf_t = manager.leaf(t);
        bool leaf_f = manager.leaf(f);

        if (!leaf_t) {
          if (!manager.marked(t)) {
            // if we encounter different indices in the child nodes
            // we delay inserting the larger one in the dqueue
            nodes[r] = t;
            manager.lbCard(t, manager.lbCard(cur) + 1);
            manager.ubCard(t, manager.ubCard(cur) + 1);

            manager.mark(nodes[r]);
            markref++;
            r--;
          } else {
            int lc = std::min(manager.lbCard(cur) + 1, manager.lbCard(t));
            int uc = std::max(manager.ubCard(cur) + 1, manager.ubCard(t));
            manager.lbCard(t, lc);
            manager.ubCard(t, uc);
          }
        } else {
          // leaf_t true
          if (manager.ctrue(t)) {
            int minval = manager.lbCard(cur) + 1;
            int maxval = manager.ubCard(cur) + 1;
            curmin = std::min(curmin, minval);
            curmax = std::max(curmax, maxval);
            
          }
        }

        if (!leaf_f) {
          if (!manager.marked(f)) {
            nodes[r] = f;
            manager.lbCard(f, manager.lbCard(cur));
            manager.ubCard(f, manager.ubCard(cur));
            manager.mark(nodes[r]);
            markref++; 
            r--;
          } else {
            int lc = std::min(manager.lbCard(cur), manager.lbCard(f));
            int uc = std::max(manager.ubCard(cur), manager.ubCard(f));
            manager.lbCard(f, lc);
            manager.ubCard(f, uc);
          }
        } else {
          // leaf_f true
          if (manager.ctrue(f)) {
            int minval = manager.lbCard(cur);
            int maxval = manager.ubCard(cur);
            curmin = std::min(curmin, minval);
            curmax = std::max(curmax, maxval);
          }
        }
      } else {
        // cur is a leaf node
      }
      if (((l == 0) && (r == n - 1))) {
        // a singleton node can only occur at the left queue end
        // as the root node is always inserted at the left end
        singleton = true;
      }
    }

    // ensure that iterations processes alternately 
    // left and right altnerately
    if (stackleft) {
      extcache_unmark(nodes, n, l, r, markref);
      assert(markref == 0);
      return;
    }
   
    if (r < n - 1) {
      _level++;
    }

    // process right stack half
    while (r < n - 1) {
      while ((r < n - 2) && manager.bddidx(nodes[r + 1]) < 
                            manager.bddidx(nodes[r + 2])) {
        int shift = r + 2;
        int norm  = r + 1;
        manager.unmark(nodes[shift]); markref--;
        if (l == 0) {
          nodes[l] = nodes[shift];
          manager.mark(nodes[l]); markref++;
        } else {    
          for (int i = l; i > 0; i--) {
            nodes[i] = nodes[i - 1];
          }
          nodes[0] = nodes[shift];
          manager.mark(nodes[0]); markref++;
        }
        l++;
        nodes[shift] = nodes[norm];
        nodes[norm].init();
        r++;
      }

      while ((r < n - 2) && manager.bddidx(nodes[r + 1]) > 
                            manager.bddidx(nodes[r + 2])) {
        int shift = r + 1;
        manager.unmark(nodes[shift]); markref--;
        if (l == 0) {
          nodes[l] = nodes[shift];
          manager.mark(nodes[l]); markref++;
        } else {
          for (int i = l; i > 0; i--) {
            nodes[i] = nodes[i - 1];
          }
          nodes[0] = nodes[shift];
          manager.mark(nodes[0]); markref++;
        }
        l++;
        nodes[shift].init();
        r++;
      }


      // check whether right-hand side nodes has fixed vars
      r++;
      manager.unmark(nodes[r]);
      markref--; 
      cur = nodes[r];
      assert(!manager.marked(cur));

      nodes[r].init();
      // cur is internal node, that is cur is neither
      // bdd_false() nor bdd_true()
      if (!manager.leaf(cur)) {
        bdd t   = manager.iftrue(cur);
        bdd f   = manager.iffalse(cur);

        bool leaf_t = manager.leaf(t);
        bool leaf_f = manager.leaf(f);

        if (!leaf_t) {
          if (!manager.marked(t)) {
            nodes[l] = t;
            manager.lbCard(t, manager.lbCard(cur) + 1);
            manager.ubCard(t, manager.ubCard(cur) + 1);
            manager.mark(nodes[l]);
            markref++;
            l++;
          } else {
            int lc = std::min(manager.lbCard(cur) + 1, manager.lbCard(t));
            int uc = std::max(manager.ubCard(cur) + 1, manager.ubCard(t));
            manager.lbCard(t, lc);
            manager.ubCard(t, uc);
          }

        } else {
          // leaf_t true
          if (manager.ctrue(t)) {
            int minval = manager.lbCard(cur) + 1;
            int maxval = manager.ubCard(cur) + 1;
            curmin = std::min(curmin, minval);
            curmax = std::max(curmax, maxval);
          } 
        }

        if (!leaf_f) {
          if (!manager.marked(f)) {
            nodes[l] = f;
            manager.lbCard(f, manager.lbCard(cur));
            manager.ubCard(f, manager.ubCard(cur));
            manager.mark(nodes[l]);
            markref++;
            l++;
          } else {
            int lc = std::min(manager.lbCard(cur), manager.lbCard(f));
            int uc = std::max(manager.ubCard(cur), manager.ubCard(f));
            manager.lbCard(f, lc);
            manager.ubCard(f, uc);
          }
        }  else {
          // leaf_f true
          if (manager.ctrue(f)) {
            int minval = manager.lbCard(cur);
            int maxval = manager.ubCard(cur);
            curmin = std::min(curmin, minval);
            curmax = std::max(curmax, maxval);

          } 
        }
      }
      if (((l == 0) && (r == n - 1))) {
        // a singleton node can only occur at the left queue end
        // as the root node is always inserted at the left end
        singleton = true;
      }

    } // end processing right stack

    extcache_unmark(nodes, n, l, r, markref);
    assert(markref == 0);
  } // end increment op

  void getcardbounds(bdd& c, int& curmin, int& curmax) {
    // try to extract the card info
    int markref = 0;
    int csize   = manager.bddsize(c);
    int l       = 0;
    int r       = csize - 1;
    bool singleton = (csize == 1);
    int _level = -1;
    Support::DynamicArray<bdd> nodes(csize);
    
    // the given ROBDD c has internal nodes
    if (!manager.leaf(c)) {

      for (int i = csize; i--; ) {       
        nodes[i].init();
      }

      nodes[l] = c;
      manager.lbCard(nodes[l], 0);
      manager.ubCard(nodes[l], 0);
      manager.mark(nodes[l]);
      markref++;
      l++;      
    } else {
      // \todo FIXME implement cardoutput for leaf!
      std::cerr << "NOT IMPLEMENTED CARDOUTPUT FOR LEAF !\n";
    }

    // contains the set of all allowed cardinalities
    if (! ((l == 0) && (r == csize - 1)) // ! empty
        || singleton) {
      while (! ((l == 0) && (r == csize - 1)) || singleton) {
        extcardbounds(markref, c, csize, l, r, 
                      singleton, _level, nodes, 
                      curmin, curmax// , out
                      );
      }
    }
  }
  /// END NEW STRUCTURE

  // END CARDINALITY COUNTING FOR EXTRACTING BOUNDS

  
  bdd lex_lb(bdd& remain, bdd& boundvars, 
              unsigned int n, unsigned int& offset, unsigned int& range) {
    // skip variables in the boolean vector that 
    // belong to the fixed part of the bdd
    if (!manager.leaf(boundvars)) {
      unsigned int idx = manager.bddidx(boundvars) - offset;
      while ( n == idx ) {
        n++;
        boundvars = manager.iftrue(boundvars);
        if (manager.leaf(boundvars)) {
          break;
        }
        idx = manager.bddidx(boundvars) - offset;
      }
    }
    if (n == range || manager.ctrue(remain)) {
      return bdd_true();
    }
    
    assert(!manager.leaf(remain));
    bdd recresult = bdd_true();
    unsigned int cidx = manager.bddidx(remain);
    bdd cur     = manager.bddpos(cidx);
    if (n == cidx - offset) {
      bdd t   = manager.iftrue(remain);
      bdd f   = manager.iffalse(remain);

      if (manager.cfalse(f)) {
        return (cur & lex_lb(t, boundvars, n + 1, offset, range));
      } else {
        return (cur | (!cur & lex_lb(f, boundvars, n + 1, offset, range)));
      }
    } else {
      return (cur | (!cur & lex_lb(remain, boundvars, n + 1, offset, range)));
    }
  }


  bdd lex_ub(bdd& remain, bdd& boundvars, 
              unsigned int n, unsigned int& offset, unsigned int& range) {
    // skip variables in the boolean vector that 
    // belong to the fixed part of the bdd
    if (!manager.leaf(boundvars)) {
      unsigned int idx = manager.bddidx(boundvars) - offset;
      while ( n == idx ) {
        n++;
        boundvars = manager.iftrue(boundvars);
        if (manager.leaf(boundvars)) {
          break;
        }
        idx = manager.bddidx(boundvars) - offset;
      }
    }

    if (n == range || manager.ctrue(remain)) {
      return bdd_true();
    }
    
    assert(!manager.leaf(remain));
    unsigned int cidx = manager.bddidx(remain);
    bdd cur     = manager.bddpos(cidx);
    bdd recresult = bdd_true();

    if (n == cidx - offset) {
      bdd t   = manager.iftrue(remain);
      bdd f   = manager.iffalse(remain);

      if (manager.cfalse(t)) {
        return !cur & lex_ub(f, boundvars,  n + 1, offset, range);
      } else {
        return (!cur | (cur & lex_ub(t, boundvars,  n + 1, offset, range)));
      }
    } else {     
      return (!cur | (cur & lex_ub(remain, boundvars,  n + 1, offset, range)));
    }
  }

}}

// STATISTICS: cpltset-support
