/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Patrick Pekczynski <pekczynski@ps.uni-sb.de>
 *
 *  Copyright:
 *     Patrick Pekczynski, 2004/2005
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

namespace Gecode { namespace Int { namespace GCC {

  // for posting
  template <class View, class Card, bool isView, bool shared>
  inline
  BndImp<View, Card, isView, shared>::
  BndImp(Space& home, ViewArray<View>& x0, ViewArray<Card>& k0,
         bool cf,  bool nolbc) :
    Propagator(home), x(x0), k(k0), lps(NULL), ups(NULL),
    card_fixed(cf), skip_lbc(nolbc) {
    home.notice(*this,AP_DISPOSE);
    x.subscribe(home, *this, PC_INT_BND);
    k.subscribe(home, *this, PC_INT_BND);
  }

  // for cloning
  template <class View, class Card, bool isView, bool shared>
  forceinline
  BndImp<View, Card, isView, shared>::
  BndImp(Space& home, bool share, BndImp<View, Card, isView, shared>& p)
    : Propagator(home, share, p), lps(NULL), ups(NULL),
      card_fixed(p.card_fixed), skip_lbc(p.skip_lbc) {
    x.update(home, share, p.x);
    k.update(home, share, p.k);
  }

  template <class View, class Card, bool isView, bool shared>
  size_t
  BndImp<View, Card, isView, shared>::dispose(Space& home){
    home.ignore(*this,AP_DISPOSE);
    if (!home.failed()) {
      x.cancel(home,*this, PC_INT_BND);
      k.cancel(home,*this, PC_INT_BND);
    }
    if (lps != NULL) {
      delete lps;
    }
    if (ups != NULL) {
      delete ups;
    }
    (void) Propagator::dispose(home);
    return sizeof(*this);
  }

  template <class View, class Card, bool isView, bool shared>
  size_t
  BndImp<View, Card, isView, shared>::allocated(void) const {
    size_t s = 0;
    if (lps != NULL)
      s += lps->allocated();
    if (ups != NULL)
      s += ups->allocated();
    return s;
  }

  template <class View, class Card, bool isView, bool shared>
  Actor*
  BndImp<View, Card, isView, shared>::copy(Space& home, bool share){
    return new (home) BndImp<View, Card, isView, shared>
      (home, share, *this);
  }

  template <class View, class Card, bool isView, bool shared>
  PropCost
  BndImp<View, Card, isView, shared>::cost(const Space&, const ModEventDelta&) const {
    /*
     * The bounds consistent version of the Global Cardinality constraint
     * has a theoretical complexity of
     *   O(t + n\cdot log(n)) with
     *   n = number of variables
     *   t = time needed to sort the domain bounds of the variables
     */
    return PropCost::linear(PropCost::HI,x.size());
  }

  template <class View, class Card, bool isView, bool shared>
  ExecStatus
  BndImp<View, Card, isView, shared>::propagate(Space& home, const ModEventDelta&) {
    bool all_assigned = true;
    bool mod          = false;
    int  smin         = 0;
    int  smax         = 0;

    if (isView) {
      // if a cardinality of first or last value is
      // out of the variable bounds (cardvar == 0)
      // reduce cardinality variables

      int m = k.size();
      bool locut = k[0].max() == 0;
      bool hicut = k[m - 1].max() == 0;

      if (locut) {
        int low = k[0].card();
        for (int i = 0; i < x.size(); i++) {
          ModEvent me = x[i].gr(home, low);
          GECODE_ME_CHECK(me);
          mod |= me_modified(me) && (x[i].min() != low + 1);
          mod |= shared && me_modified(me);
        }
      }
      if (hicut) {
        int hi = k[m - 1].card();
        for (int i = 0; i < x.size(); i++) {
          ModEvent me = x[i].le(home, hi);
          GECODE_ME_CHECK(me);
          mod |= me_modified(me) && (x[i].max() != hi - 1);
          mod |= shared && me_modified(me);
        }
      }

      if (locut || hicut) {
        int cmin = k[0].card();
        int cmax = k[m - 1].card();
        if (k[0].max() == 0) {
          cmin = k[1].card();
        }
        if (k[m - 1].max() == 0) {
          cmax = k[m - 2].card();
        }
        reduce_card<Card>(home,cmin, cmax, k);

        if (lps != NULL) {
          delete lps; lps = NULL;
          assert(ups != NULL);
          delete ups; ups = NULL;
        }
      }
    }

    Region r(home);
    int* count = r.alloc<int>(k.size());
    for (int i = k.size(); i--; )
      count[i] = 0;

    int noa = 0;
    int single = 0;
    int xlb = 0;
    int xub = 0;
    for (int i = x.size(); i--; ) {
      bool b = x[i].assigned();
      xlb += x[i].min();
      xub += x[i].max();
      all_assigned &= b;
      if (b) {
        noa++;
        int idx = lookupValue(k,x[i].val());
        // reduction is essential for order on value nodes in dom
        // hence introduce test for failed lookup
        if (idx == -1)
          return ES_FAILED;
        count[idx]++;
      } else {
        single = i;
      }
    }

    if (isView) {
      // before propagation performs inferences on cardinality variables:
      if (noa > 0) {
        int n  = x.size();
        int ks = k.size();

        for (int i = ks; i--; ) {
          if (!k[i].assigned()) {
            int ub = n - (noa - count[i]);
            int lb = count[i];
            ModEvent melq = k[i].lq(home, ub);
            GECODE_ME_CHECK(melq);
            mod |= me_modified(melq) && (k[i].max() != ub);
            mod |= shared && me_modified(melq);

            ModEvent megq = k[i].gq(home, lb);
            GECODE_ME_CHECK(megq);
            mod |= me_modified(megq) && (k[i].min() != lb);
            mod |= shared && me_modified(megq);
          }
        }
      }

      if (!card_consistent<View, Card>(smin, smax, x, k))
        return ES_FAILED;

      // can only modified cardinality variables
      GECODE_ES_CHECK((prop_card<View, Card, shared>(home, x, k, mod)));

      // mimicking linear constraint
      int smax = 0;
      int smin = 0;
      int total_min = 0;
      int total_max = 0;
      for (int i = k.size(); i--; ) {
        smax += k[i].max();
        total_min += k[i].card() * k[i].min();
        total_max += k[i].card() * k[i].max();
      }
      int xsmax = x.size() - smax;
      int xsmin = x.size() - smin;
      smax = 0;
      smin = 0;
      bool card_ass = true;
      for (int i = k.size(); i--; ) {
        int lb = xsmax + k[i].max();
        int ub = xsmin + k[i].min();
        ModEvent me = k[i].gq(home, lb);
        GECODE_ME_CHECK(me);
        mod |= me_modified(me) && (k[i].min() != lb);
        mod |= shared && me_modified(me);
        smax += k[i].max();

        me = k[i].lq(home, ub);
        GECODE_ME_CHECK(me);
        mod |= me_modified(me) && (k[i].max() != ub);
        mod |= shared && me_modified(me);
        card_ass &= k[i].assigned();
      }
      if (card_ass) {
        if (smax < x.size() || smax > x.size())
          return ES_FAILED;

        // redundant linear constraint
        for (int i = x.size(); i--; ) {
          if (!x[i].assigned()) {
            int xmin = xub - x[i].max();
            int xgq  = total_max - xmin;

            int xmax = xlb - x[i].min();
            int xlq  = total_max - xmax;

            ModEvent me = x[i].gq(home, xgq);
            GECODE_ME_CHECK(me);
            mod |= me_modified(me) && (x[i].min() != xgq);
            mod |= shared && me_modified(me);

            me = x[i].lq(home, xlq);
            GECODE_ME_CHECK(me);
            mod |= me_modified(me) && (x[i].max() != xlq);
            mod |= shared && me_modified(me);
          }
        }
      }
    }

    for (int i = k.size(); i--; )
      count[i] = 0;

    all_assigned = true;
    noa = 0;
    single = 0;

    for (int i = x.size(); i--; ) {
      bool b = x[i].assigned();
      xlb += x[i].min();
      xub += x[i].max();
      all_assigned &= b;
      if (b) {
        noa++;
        int idx = lookupValue(k,x[i].val());
        // reduction is essential for order on value nodes in dom
        // hence introduce test for failed lookup
        if (idx == -1)
          return ES_FAILED;
        count[idx]++;
      } else {
        single = i;
      }
    }

    if (all_assigned) {
      for (int i = k.size(); i--; ) {
        int ci = count[i];
        if (! (k[i].min() <= ci && ci <= k[i].max())) {
          return ES_FAILED;
        } else {
          if (isView) {
            if (!k[i].assigned()) {
              ModEvent me = k[i].eq(home, ci);
              GECODE_ME_CHECK(me);
              mod |= k[i].assigned();
            }
            all_assigned &= k[i].assigned();
          }
        }
      }
      if (all_assigned)
        return ES_SUBSUMED(*this,home);
    }

    if (isView) {
      // check again for zero entries at first or last position
      int m = k.size();
      bool locut = k[0].max() == 0;
      bool hicut = k[m - 1].max() == 0;

      if (locut) {
        int low = k[0].card();
        for (int i = 0; i < x.size(); i++) {
          ModEvent me = x[i].gr(home, low);
          GECODE_ME_CHECK(me);
          mod |= me_modified(me) && (x[i].min() != low + 1);
          mod |= shared && me_modified(me);
        }
      }
      if (hicut) {
        int hi = k[m - 1].card();
        for (int i = 0; i < x.size(); i++) {
          ModEvent me = x[i].le(home, hi);
          GECODE_ME_CHECK(me);
          mod |= me_modified(me) && (x[i].max() != hi - 1);
          mod |= shared && me_modified(me);
        }
      }

      if (locut || hicut) {
        int cmin = k[0].card();
        int cmax = k[m - 1].card();
        if (k[0].max() == 0) {
          cmin = k[1].card();
        }
        if (k[m - 1].max() == 0) {
          cmax = k[m - 2].card();
        }
        reduce_card<Card>(home,cmin, cmax, k);

        if (lps != NULL) {
          delete lps;
          lps = NULL;
          assert(ups != NULL);
          delete ups;
          ups = NULL;
        }
      }
    }

    ExecStatus es_ubc = ES_FIX;
    ExecStatus es_lbc = ES_FIX;
    int n = x.size();

    int* mu = r.alloc<int>(n);
    int* nu = r.alloc<int>(n);

    for (int i = n; i--; )
      nu[i] = mu[i] = i;

    // Create sorting permutation mu according to the variables upper bounds
    MaxInc<View> max_inc(x);
    Support::quicksort<int, MaxInc<View> >(mu, n, max_inc);

    // Create sorting permutation nu according to the variables lower bounds
    MinInc<View> min_inc(x);
    Support::quicksort<int, MinInc<View> >(nu, n, min_inc);

    // Sort the cardinality bounds by index
    MinIdx<Card> min_idx;
    Support::quicksort<Card, MinIdx<Card> >(&k[0], k.size(), min_idx);

    if (isView) {
      // assert guaranteed bounds in the set of all values for variable case
      assert(k[0].card() == x[nu[0]].min());
    }

    // ensure that only values are considered lying in the variable domain
    if (lps == NULL) {
      assert (ups == NULL);
      lps = new PartialSum<Card>(k, false);
      ups = new PartialSum<Card>(k, true);
    }

    if (isView) {
      // if there has been a change to the cardinality variables
      // reconstruction of the partial sum structure is necessary
      if (lps->check_update_min(k)) {
        delete lps;
        lps = new PartialSum<Card>(k, false);
      }

      if (ups->check_update_max(k)) {
        delete ups;
        ups = new PartialSum<Card>(k, true);
      }
    }

    // already holds by construction
    assert(lps->minValue() == ups->minValue());
    assert(lps->maxValue() == ups->maxValue());

    bool minima_equal = lps->minValue() == ups->minValue();
    bool maxima_equal = lps->maxValue() == ups->maxValue();

    if (!minima_equal || !maxima_equal ) {
      delete lps;
      lps = new PartialSum<Card>(k, false);
      delete ups;
      ups = new PartialSum<Card>(k, true);
    }

    // assert that the minimal value of the partial sum structure for
    // LBC is consistent with the smallest value a variable can take
    assert(lps->minValue() <= x[nu[0]].min());
    // assert that the maximal value of the partial sum structure for
    // UBC is consistent with the largest value a variable can take
    //std::cerr << x[mu[x.size() - 1]].max() <<"<="<< ups->maxValue() << "\n";
    //assert(x[mu[x.size() - 1]].max() <= ups->maxValue());

    /*
     *  Setup rank and bounds info
     *  Since this implementation is based on the theory of Hall Intervals
     *  additional datastructures are needed in order to represent these
     *  intervals and the "partial-sum" data structure (cf."gcc/gccbndsup.hpp")
     *
     */

    HallInfo* hall = r.alloc<HallInfo>(2 * n + 2);
    Rank* rank = r.alloc<Rank>(n);

    int nb = 0;
    // setup bounds and rank
    int min        = x[nu[0]].min();
    int max        = x[mu[0]].max() + 1;
    int last       = lps->firstValue + 1; //equivalent to last = min -2
    hall[0].bounds = last;

    /*
     * First the algorithm merges the arrays minsorted and maxsorted
     * into bounds i.e. hall[].bounds contains the ordered union
     * of the lower and upper domain bounds including two sentinels
     * at the beginning and at the end of the set
     * ( the upper variable bounds in this union are increased by 1 )
     */

    {
      int i = 0;
      int j = 0;
      while (true) {
        if (i < n && min < max) {
          if (min != last) {
            last = min;
            hall[++nb].bounds = last;
          }
          rank[nu[i]].min = nb;
          if (++i < n) {
            min = x[nu[i]].min();
          }
        } else {
          if (max != last) {
            last = max;
            hall[++nb].bounds = last;
          }
          rank[mu[j]].max = nb;
          if (++j == n) {
            break;
          }
          max = x[mu[j]].max() + 1;
        }
      }
    }

    int rightmost = nb + 1; // rightmost accesible value in bounds
    hall[rightmost].bounds = ups->lastValue + 1 ;

    skip_lbc = true;
    for (int i = k.size(); i--; )
      if (k[i].min() != 0) {
        skip_lbc = false;
        break;
      }

    if (!card_fixed && !skip_lbc) {
      es_lbc = lbc<View, Card, shared>(home, x, nb, hall, rank,lps, mu, nu);
      GECODE_ES_CHECK(es_lbc);
      mod |= (es_lbc == ES_NOFIX);
    }

    es_ubc = ubc<View, Card, shared>(home, x, nb, hall, rank, ups, mu, nu);
    GECODE_ES_CHECK(es_ubc);
    mod |= (es_ubc == ES_NOFIX);

    if (isView) {
      GECODE_ES_CHECK((prop_card<View, Card, shared>(home, x, k, mod)));
    }

    all_assigned = true;
    noa = 0;
    single = 0;
    for (int i = k.size(); i--; ) {
      count[i] = 0;
    }

    for (int i = x.size(); i--; ) {
      bool b = x[i].assigned();
      all_assigned &= b;
      if (b) {
        noa++;
        int idx = lookupValue(k,x[i].val());
        count[idx]++;
      } else {
        single = i;
      }
    }

    if (all_assigned) {
      for (int i = k.size(); i--; ) {
        int ci = count[i];
        if (! (k[i].min() <= ci && ci <= k[i].max())) {
          return ES_FAILED;
        } else {
          if (isView) {
            if (!k[i].assigned()) {
              ModEvent me = k[i].eq(home, ci);
              GECODE_ME_CHECK(me);
              mod |= k[i].assigned();
            }
            all_assigned &= k[i].assigned();
          }
        }
      }
      if (all_assigned)
        return ES_SUBSUMED(*this,home);
    }

    return mod ? ES_NOFIX : ES_FIX;
  }

  /**
   * \brief Sharing test for the bounds consistent global cardinality
   * propagator.
   *
   * Due to parametrization on the problem views, \a View1 and the
   * cardinality views, \a View2 the test can be applied for both cases
   * of fixed cardinalities and cardinality variables.
   */
  template <class View1, class View2>
  class SharingTest {
  public:
    /**
     * \brief Test whether the problem views in \a x0 or the
     *  cardinality views in \a k0 contain shared views.
     */
    static bool shared(Space& home, ViewArray<View1>& x0,
                       ViewArray<View2>& k0) {
      ViewArray<View1> xc(home, x0.size()+k0.size());
      for (int i = 0; i < x0.size(); i++) {
        xc[i] = x0[i];
      }
      for (int i = x0.size(); i < x0.size()+k0.size(); i++) {
        xc[i] = k0[i - x0.size()].intview();
      }
      return xc.shared(home);
    }
  };

  /**
   * \brief Specialization of class SharingTest for the case of fixed
   * cardinalities using IntView as \a View1 and OccurBndsView as \a View2
   */
  template <>
  class SharingTest<IntView,OccurBndsView> {
  public:
    /// Test whether the problem views in \a x0 contain shared views.
    static bool shared(Space& home,
                       ViewArray<IntView>& xs, ViewArray<OccurBndsView>&) {
      return xs.shared(home);
    }
  };

  template <class View, class Card, bool isView>
  ExecStatus
  Bnd<View, Card, isView>::post(Space& home,
                                ViewArray<View>& x0,
                                ViewArray<Card>& k0) {
    bool cardfix = true;
    bool nolbc = true;
    for (int i = k0.size(); i--; ) {
      cardfix &= k0[i].assigned();
      nolbc &= (k0[i].min() == 0);
    }
    if (SharingTest<View,Card>::shared(home,x0,k0)) {
      new (home) BndImp<View, Card, isView, true>
        (home, x0, k0, cardfix, nolbc);
    } else {
      new (home) BndImp<View, Card, isView, false>
        (home, x0, k0, cardfix, nolbc);
    }
    return ES_OK;
  }

}}}

// STATISTICS: int-prop

