/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Guido Tack <tack@gecode.org>
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Copyright:
 *     Guido Tack, 2004
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

#include "gecode/minimodel.hh"

namespace Gecode {

  namespace MiniModel {

    bool
    BoolExpr::Node::decrement(void) {
      if (--use == 0) {
        if ((l != NULL) && l->decrement())
          delete l;
        if ((r != NULL) && r->decrement())
          delete r;
        return true;
      }
      return false;
    }

    BoolVar
    BoolExpr::Node::post(Space* home, IntConLevel icl, PropKind pk) const {
      if (t == BT_VAR)
        return x;
      BoolVar b(home,0,1);
      post(home,b,icl,pk);
      return b;
    }

    int
    BoolExpr::Node::post(Space* home, NodeType t,
                         BoolVarArgs& b, int i, 
                         IntConLevel icl, PropKind pk) const {
      if (this->t != t) {
        b[i] = post(home,icl,pk);
        return i+1;
      } else {
        return l->post(home,t,b,r->post(home,t,b,i,icl,pk),icl,pk);
      }
    }

    void
    BoolExpr::Node::post(Space* home, BoolVar b,
                         IntConLevel icl, PropKind pk) const {
      assert(t != BT_VAR);
      switch (t) {
      case BT_NOT:
        rel(home,l->post(home,icl,pk),IRT_NQ,b,icl,pk);
        break;
      case BT_AND:
        if (same > 2) {
          BoolVarArgs ba(same);
          (void) post(home,BT_AND,ba,0,icl,pk);
          rel(home,BOT_AND,ba,b,icl,pk);
        } else {
          rel(home,l->post(home,icl,pk),BOT_AND,r->post(home,icl,pk),b,icl,pk);
        }
        break;
      case BT_OR:
        if (same > 2) {
          BoolVarArgs ba(same);
          (void) post(home,BT_OR,ba,0,icl,pk);
          rel(home,BOT_OR,ba,b,icl,pk);
        } else {
          rel(home,l->post(home,icl,pk),BOT_OR,r->post(home,icl,pk),b,icl,pk);
        }
        break;
      case BT_IMP:
        rel(home,l->post(home,icl,pk),BOT_IMP,r->post(home,icl,pk),b,icl,pk);
        break;
      case BT_XOR:
        rel(home,l->post(home,icl,pk),BOT_XOR,r->post(home,icl,pk),b,icl,pk);
        break;
      case BT_EQV:
        rel(home,l->post(home,icl,pk),BOT_EQV,r->post(home,icl,pk),b,icl,pk);
        break;
      default:
        GECODE_NEVER;
      case BT_RLIN:
        rl.post(home,b,icl,pk);
        break;
      }
    }

    void
    BoolExpr::Node::post(Space* home, bool b, 
                         IntConLevel icl, PropKind pk) const {
      if (b) {
        switch (t) {
        case BT_VAR:
          rel(home, x, IRT_EQ, 1);
          break;
        case BT_NOT:
          l->post(home,false,icl,pk);
          break;
        case BT_OR:
          if (same > 2) {
            BoolVarArgs ba(same);
            (void) post(home,BT_OR,ba,0,icl,pk);
            rel(home,BOT_OR,ba,1,icl,pk);
          } else {
            rel(home,l->post(home,icl,pk),BOT_OR,r->post(home,icl,pk),1,icl,pk);
          }
          break;
        case BT_AND:
          l->post(home,true,icl,pk); r->post(home,true,icl,pk);
          break;
        case BT_EQV:
          if ((l->t == BT_VAR) && (r->t != BT_VAR)) {
            r->post(home,l->x,icl,pk);
          } else if ((l->t != BT_VAR) && (r->t == BT_VAR)) {
            l->post(home,r->x,icl,pk);
          } else if ((l->t != BT_VAR) && (r->t != BT_VAR)) {
            BoolVar b(home,0,1);
            l->post(home,b,icl,pk);
            r->post(home,b,icl,pk);
          } else {
            BoolVar b(home,1,1);
            post(home,b,icl,pk);
          }
          break;
        case BT_RLIN:
          rl.post(home,true,icl,pk);
          break;
        default:
          {
            BoolVar b(home,1,1);
            post(home,b,icl,pk);
          }
          break;
        }
      } else {
        switch (t) {
        case BT_VAR:
          rel(home, x, IRT_EQ, 0,icl,pk);
          break;
        case BT_NOT:
          l->post(home,true,icl,pk);
          break;
        case BT_OR:
          l->post(home,false,icl,pk); r->post(home,false,icl,pk);
          break;
        case BT_AND:
          if (same > 2) {
            BoolVarArgs ba(same);
            (void) post(home,BT_AND,ba,0,icl,pk);
            rel(home, BOT_AND, ba, 0,icl,pk);
          } else {
            rel(home, l->post(home,icl,pk), BOT_AND, r->post(home,icl,pk), 0,icl,pk);
          }
          break;
        case BT_IMP:
          l->post(home,true,icl,pk);
          r->post(home,false,icl,pk);
          break;
        case BT_XOR:
          if ((l->t == BT_VAR) && (r->t != BT_VAR)) {
            r->post(home,l->x,icl,pk);
          } else if ((l->t != BT_VAR) && (r->t == BT_VAR)) {
            l->post(home,r->x,icl,pk);
          } else if ((l->t != BT_VAR) && (r->t != BT_VAR)) {
            BoolVar b(home,0,1);
            l->post(home,b,icl,pk);
            r->post(home,b,icl,pk);
          } else {
            BoolVar b(home,0,0);
            post(home,b,icl,pk);
          }
          break;
        case BT_RLIN:
          rl.post(home,false,icl,pk);
          break;
        default:
          {
            BoolVar b(home,0,0);
            post(home,b,icl,pk);
          }
          break;
        }
      }
    }

  }

}

// STATISTICS: minimodel-any
