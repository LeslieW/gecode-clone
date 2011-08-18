/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Copyright:
 *     Christian Schulte, 2011
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

namespace Gecode { namespace Int { namespace NValues {

  /*
   * Value sets
   *
   */
  forceinline
  ValSet::ValSet(void)
    : n(0), fst(NULL), lst(NULL) {}

  forceinline void
  ValSet::add(Space& home, int v) {
    RangeList*  c = fst;
    RangeList** p = &fst;
    while (c != NULL) {
      if (v < c->min()) {
        if (v+1 == c->min()) {
          c->min(v); n++;
          return;
        } else {
          *p = new (home) RangeList(v,v,c); n++; 
          return;
        }
      } else if (v <= c->max()) {
        // Value already included
        return;
      } else if (v == c->max()+1) {
        if ((c->next() != NULL) && (v+1 == c->next()->min())) {
          c->next()->min(c->min());
          *p = c->next();
          c->dispose(home,c);
        } else {
          c->max(v);
        }
        n++;
        return;
      } else {
        // FIXME: HOW TO CAST HERE?
        p = reinterpret_cast<RangeList**>(c->nextRef());
        c = *p;
      }
    }
    *p = new (home) RangeList(v,v,NULL); n++;
    lst = *p;
  }

  forceinline int
  ValSet::size(void) const {
    return n;
  }

  forceinline bool
  ValSet::empty(void) const {
    return n == 0;
  }

  forceinline int
  ValSet::min(void) const {
    if (fst != NULL)
      return fst->min();
    else
      return -1;
  }

  forceinline int
  ValSet::max(void) const {
    if (lst != NULL)
      return lst->max();
    else
      return -1;
  }

  forceinline void
  ValSet::update(Space& home, bool share, ValSet& vs) {
    (void) share;
    if (vs.n > 0) {
      n = vs.n;
      // Count number of ranges
      int m = 0;
      for (RangeList* c = vs.fst; c != NULL; c = c->next())
        m++;
      fst = home.alloc<RangeList>(m);
      lst = fst + (m-1);
      int i=0;
      for (RangeList* c = vs.fst; c != NULL; c = c->next()) {
        fst[i].min(c->min()); fst[i].max(c->max());
        fst[i].next(fst+i+1);
        i++;
      }
      lst->next(NULL);
    }
  }

  forceinline void
  ValSet::dispose(Space& home) {
    if (fst != NULL)
      fst->dispose(home,lst);
  }

  forceinline
  ValSet::Ranges::Ranges(const ValSet& vs) 
    : c(vs.fst) {}

  forceinline bool
  ValSet::Ranges::operator ()(void) const {
    return c != NULL;
  }

  forceinline void
  ValSet::Ranges::operator ++(void) {
    c = c->next();
  }

  forceinline int
  ValSet::Ranges::min(void) const {
    return c->min();
  }
  forceinline int
  ValSet::Ranges::max(void) const {
    return c->max();
  }

  forceinline unsigned int
  ValSet::Ranges::width(void) const {
    return c->width();
  }

}}}

// STATISTICS: int-prop
