/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Guido Tack <tack@gecode.org>
 *
 *  Copyright:
 *     Guido Tack, 2004
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

#include <gecode/driver.hh>
#include <gecode/int.hh>
#include <gecode/set.hh>

using namespace Gecode;

/// \brief %Options for %Golf example
class GolfOptions : public Options {
protected:
  Driver::IntOption _w; //< Number of weeks
  Driver::IntOption _g; //< Number of groups
  Driver::IntOption _s; //< Number of players per group
public:
  /// Constructor
  GolfOptions(void)
    : Options("Golf"),
      _w("-w","number of weeks",9),
      _g("-g","number of groups",8),
      _s("-s","number of players per group",4) {
    add(_w);
    add(_g);
    add(_s);
  }
  /// Return number of weeks
  int w(void) const { return _w.value(); }
  /// Return number of groups
  int g(void) const { return _g.value(); }
  /// Return number of players per group
  int s(void) const { return _s.value(); }
};

/**
 * \brief %Example: %Golf tournament
 *
 * Schedule a golf tournament. This is problem 010 from csplib.
 *
 * \ingroup ExProblem
 *
 */
class Golf : public Script {
public:
  /// Model variants
  enum {
    MODEL_PLAIN,   ///< A simple model
    MODEL_SYMMETRY ///< Model with symmetry breaking
  };
  int g; ///< Number of groups in a week
  int s; ///< Number of players in a group
  int w; ///< Number of weeks

  /// The sets representing the groups
  SetVarArray groups;

  /// Actual model
  Golf(const GolfOptions& opt) : g(opt.g()), s(opt.s()), w(opt.w()),
    groups(*this,g*w,IntSet::empty,0,g*s-1,s,s) {
    Matrix<SetVarArray> schedule(groups,g,w);

    // Groups in one week must be disjoint
    SetVar allPlayers(*this, 0,g*s-1, 0,g*s-1);
    for (int i=0; i<w; i++)
      rel(*this,SOT_DUNION,schedule.row(i),allPlayers);

    // No two golfers play in the same group more than once
    for (int i=0; i<groups.size()-1; i++)
      for (int j=i+1; j<groups.size(); j++) {
        SetVar atmostOne(*this,IntSet::empty,0,g*s-1,0,1);
        rel(*this, groups[i], SOT_INTER, groups[j], SRT_EQ, atmostOne);
      }

    if (opt.model() == MODEL_SYMMETRY) {

      /*
       * Redundant constraints and static symmetry breaking from
       * "Solving Kirkman's Schoolgirl Problem in a Few Seconds"
       * Nicolas Barnier, Pascal Brisset, Constraints, 10, 7-21, 2005
       *
       */

      // Redundant constraint:
      // in each week, one player plays in only one group
      for (int j=0; j<w; j++) {
         for (int p=0; p < g*s; p++) {
           BoolVarArgs b(*this,g,0,1);
           for (int i=0; i<g; i++)
             dom(*this, schedule(i,j), SRT_SUP, p, b[i]);
           linear(*this, b, IRT_EQ, 1);
         }
       }

      // Symmetry breaking: order groups
      for (int j=0; j<w; j++) {
        for (int i=0; i<g-1; i++) {
          IntVar min1(*this, 0, g*s-1);
          IntVar min2(*this, 0, g*s-1);
          min(*this, schedule(i,j), min1);
          min(*this, schedule(i+1,j), min2);
          rel(*this, min1, IRT_LE, min2);
        }
      }

      // Symmetry breaking: order weeks
      // minElem(group(w,0)\{0}) < minElem(group(w+1,0)\{0})
      for (int i=0; i<w-1; i++) {
        SetVar g1(*this, IntSet::empty, 1, g*s-1);
        SetVar g2(*this, IntSet::empty, 1, g*s-1);
        rel(*this, g1, SOT_DUNION, IntSet(0,0), SRT_EQ, schedule(0,i));
        rel(*this, g2, SOT_DUNION, IntSet(0,0), SRT_EQ, schedule(0,i+1));
        IntVar minG1(*this, 0, g*s-1);
        IntVar minG2(*this, 0, g*s-1);
        min(*this, g1, minG1);
        min(*this, g2, minG2);
        rel(*this, minG1, IRT_LE, minG2);
      }

      // Initialize the dual variables:
      // gInv(i,p) is player p's group in week i
      Matrix<IntVarArray> gInv(IntVarArray(*this, w*g*s, 0, g-1),w,g*s);
      for (int i=0; i<w; i++) {
        for (int p=0; p<g*s; p++) {
          SetVar player(*this, p,p, 0, g*s-1);
          element(*this, schedule.row(i), gInv(i,p),player);
        }
      }
      
      // Symmetry breaking: order players
      for (int i=0; i<w; i++)
        for (int p=0; p<g; p++)
          rel(*this, gInv(i,p), IRT_LQ, p);
    }

    branch(*this, groups, SET_VAR_MIN_MIN, SET_VAL_MIN_INC);
  }

  /// Print solution
  virtual void
  print(std::ostream& os) const {
    os << "Tournament plan" << std::endl;
    Matrix<SetVarArray> schedule(groups,g,w);
    for (int j=0; j<w; j++) {
      os << "Week " << j << ": " << std::endl << "    ";
      for (int i=0; i<g; i++) {
        if (schedule(i,j).assigned()) {
          bool first = true;
          os << "(";
          for (SetVarGlbValues glb(schedule(i,j)); glb(); ++glb) {
            if (first) first = false; else os << " ";
            os << glb.val();
          }
          os << ")";
        } else {
          os << "(" << schedule(i,j) << ")";
        }
        if (i < g-1) os << " ";
        if (i > 0 && i % 4 == 0) os << std::endl << "    ";
      }
      os << std::endl;
    }
  }

  /// Constructor for copying \a s
  Golf(bool share, Golf& s) : Script(share,s), g(s.g), s(s.s), w(s.w) {
    groups.update(*this, share, s.groups);
  }
  /// Copy during cloning
  virtual Space*
  copy(bool share) {
    return new Golf(share,*this);
  }
};

/** \brief Main-function
 *  \relates Golf
 */
int
main(int argc, char* argv[]) {
  GolfOptions opt;
  opt.model(Golf::MODEL_PLAIN);
  opt.model(Golf::MODEL_PLAIN, "none", "no symmetry breaking");
  opt.model(Golf::MODEL_SYMMETRY, "symmetry", "static symmetry breaking");
  opt.solutions(1);
  opt.parse(argc,argv);
  Script::run<Golf,DFS,GolfOptions>(opt);
  return 0;
}

// STATISTICS: example-any
