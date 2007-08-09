/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Copyright:
 *     Christian Schulte, 2004
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

#ifndef __GECODE_INT_REGULAR_HH__
#define __GECODE_INT_REGULAR_HH__

#include "gecode/int.hh"

/**
 * \namespace Gecode::Int::Regular
 * \brief %Regular propagators
 */

namespace Gecode { namespace Int { namespace Regular {

  class Layer;
  class State;

  /**
   * \brief Domain-consistent regular propagator
   *
   * The algorithm for the regular propagator is based on:
   *   Gilles Pesant, A Regular Language Membership Constraint
   *   for Finite Sequences of Variables, CP 2004.
   *   Pages 482-495, LNCS 3258, Springer-Verlag, 2004.
   *
   * The propagator is not capable of multiple occurences of the same
   * view, see gecode/regular.cc for the necessary preprocessing.
   *
   * Requires \code #include "gecode/int/regular.hh" \endcode
   * \ingroup FuncIntProp
   */
  template <class View>
  class Dom : public Propagator {
  protected:
    /// Advisors for views (by position in array)
    class Index : public Advisor {
    public:
      /// The position of the view in the view array
      int i;
      /// Create index advisor
      Index(Space* home, Propagator* p, Council<Index>& c, int i);
      /// Clone index advisor \a a
      Index(Space* home, bool share, Index& a);
      /// Dispose advisor
      void dispose(Space* home, Council<Index>& c);
    };
    /// Range approximation of which positions have changed
    class IndexRange {
    private:
      int _fst; ///< First index
      int _lst; ///< Last index
    public:
      /// Initialize range as empty
      IndexRange(void);
      /// Reset range to be empty
      void reset(void);
      /// Add index \a i to range
      void add(int i);
      /// Return first position
      int fst(void) const;
      /// Return last position
      int lst(void) const;
    };
    /// The advisor council
    Council<Index> c;
    /// The views
    ViewArray<View> x;
    /// The %DFA describing the language
    DFA dfa;
    /// The start state for graph construction
    int start;
    /// The layers of the graph
    Layer* layers;
    /// The states used in the graph
    State* states;
    /// Index range with in-degree modifications
    IndexRange i_ch;
    /// Index range with out-degree modifications
    IndexRange o_ch;
    
    /// Test whether layered graph has already been constructed
    bool constructed(void) const;
    /// Eliminate assigned prefix, return how many layers removed
    void eliminate(void);
    /// Construct layered graph
    ExecStatus construct(Space* home);
    /// Prune incrementally
    ExecStatus prune(Space* home);

    /// Constructor for cloning \a p
    Dom(Space* home, bool share, Dom<View>& p);
  public:
    /// Constructor for posting
    Dom(Space* home, ViewArray<View>& x, DFA& d);
    /// Copy propagator during cloning
    virtual Actor* copy(Space* home, bool share);
    /// Cost function (defined as dynamic PC_LINEAR_HI)
    virtual PropCost cost(void) const;
    /// Give advice to propagator
    virtual ExecStatus advise(Space* home, Advisor* a, const Delta* d);
    /// Perform propagation
    virtual ExecStatus propagate(Space* home);
    /// Name of this propagator
    virtual const char* name(void) const;    
    /// Delete propagator and return its size
    virtual size_t dispose(Space* home);
    /// Post propagator on views \a x and DFA \a d
    static ExecStatus post(Space* home, ViewArray<View>& x, DFA& d);
  };

}}}

#include "gecode/int/regular/dom.icc"

#endif

// STATISTICS: int-prop

