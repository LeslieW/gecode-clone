/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Mikael Lagerkvist <lagerkvist@gecode.org>
 *
 *  Copyright:
 *     Mikael Lagerkvist, 2007
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

#include "examples/support.hh"
#include "gecode/minimodel.hh"

#include <iomanip>


int nvars, nclauses;


/**
 * \brief %Example: Satisfiability of set of sat-clauses
 *
 * \ingroup ExProblem
 *
 */
class SAT : public Example {
protected:
  /// Array of Boolean variables
  BoolVarArray x;
public:
  /// Model
  SAT(const Options& opt)
    : x(this,nvars,0,1) {

    BoolVarArgs y(nvars);
    for (int i = nvars; i--; )
      y[i] = post(this, !x[i]);

    //std::string line;
    //while (std::getline(std::cin, line)) {
    //  std::cout << line << std::endl;
    //}
    BoolVarArgs v(nvars);
    while (true) { // Loop over clauses
      int var;
      int csiz = 0;
      while (true) { // Loop over vars in clause
        std::cin >> var;
        if (var == 0) break; // Clause read
        v[csiz++] = var > 0 ? x[var-1] : y[-var-1];
      }
      BoolVarArgs c(csiz);
      for (int i = csiz; i--; )
        c[i] = v[i];
      if (opt.naive)
        rel(this, c, BOT_OR, 1);
      else
        rel(this, c, BOT_OR_WL, 1);
    }
    
    branch(this, x, BVAR_DEGREE_MAX, BVAL_MIN);
  }

  /// Print solution
  virtual void
  print(void) {
    std::cout << "x = [";
    for (int i = 0; i < x.size(); ++i)
      std::cout << x[i] << (i!=x.size()-1 ? ", " : "");
    std::cout << "]" << std::endl;
  }

  /// Constructor for cloning \a s
  SAT(bool share, SAT& s)
    : Example(share,s) {
    x.update(this,share,s.x);
  }

  /// Copy during cloning
  virtual Space*
  copy(bool share) {
    return new SAT(share,*this);
  }

};

/** \brief Main-function
 *  \relates SAT
 */
int
main(int argc, char* argv[]) {
  Options opt("SAT");
  opt.solutions(1);
  opt.size      = 0;
  opt.naive     = true;
  opt.parse(argc,argv);

  std::string s;
  std::cin >> s; std::cin >> s;
  std::cin >> nvars >> nclauses >> std::ws;
  std::cout << "p " << s << " " << nvars << " " << nclauses << std::endl;

  Example::run<SAT,DFS>(opt);
  return 0;
}

// STATISTICS: example-any

