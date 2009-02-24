/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Patrick Pekczynski <pekczynski@ps.uni-sb.de>
 *
 *  Copyright:
 *     Patrick Pekczynski, 2005
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

  /**
   * \brief Bounds constraint (BC) type
   *
   * If BC = UBC, then we argue about the Upper Bounds Constraint
   * else we use the functions for the Lower Bounds Constraint
   */
  enum BC {UBC = 1, LBC = 0};

  class Edge;
  /// Base class for nodes in the variable-value-graph
  class VVGNode {
  private:
    /// stores all incident edges on the node
    Edge* e;
    /// returns the first edge
    Edge* fst;
    /// returns the last edge
    Edge* lst;
    /// single incoming edge used for storing a path in the algorithms
    Edge* ie;
    /// Mark as matching edge in LBC
    bool  lm;
    /// Mark as matching edge in UBC
    bool  um;
    /// Explicit node type
    bool  type;
  public:
    /// \name Constructors and initialization
    //@{
    /// Default constructor
    VVGNode(void);
    //@}
    /// Destructor
    virtual ~VVGNode(void){};
    /// Create a new node
    void*  operator new(size_t, void*);

    /// \name Access
    //@{
    /// return reference to the incident edges
    Edge** adj(void);
    /// return pointer to the first incident edge
    Edge*  first(void);
    /// return pointer to the last incident edge
    Edge*  last(void);
    /// return pointer to the node's inedge
    Edge*  inedge(void);
    /// return the type of the node
    bool   get_type(void);
    /// access the matching flag of the node
    template <BC>
    bool   get_match_flag(void);
    /// get the information on the node (either var-index or value)
    virtual int  get_info(void) = 0;
    /// test whether the node is matched
    virtual bool is_matched(BC) = 0;

    /// check whether a node has been removed from the graph
    virtual bool removed(void) = 0;
    //@}

    /// \name Update
    //@{
    /// set the first edge pointer to \a p
    void   first(Edge* p);
    /// set the last edge pointer to \a p
    void   last(Edge* p);
    /// set the inedge pointer to \a p
    void   inedge(Edge* p);
    /// set the node type to \a t
    void   set_type(bool t);
    /// set the matching flag to \a f
    template <BC>
    void   set_match_flag(bool f);
    /// set the node information to \a i
    virtual void set_info(int i) = 0;
    //@}
  };

  /// %Variable Node
  class VarNode : public VVGNode {
  private:
    /// stores the matching edge on this node in the UBC
    Edge* ubm;
    /// stores the matching edge on this node in the LBC
    Edge* lbm;

  public:
    /// stores the variable index of the node
    int var;
    /// stores the number of incident edges on the node
    int noe;

    /// stores the variable index of the node
    int xindex;

    /// \name Constructors and initialization
    //@{
    /// Creates a variable node with index i
    VarNode(int i, int oidx);
    //@}

    /// \name Access
    //@{
    /// return the matching edge on the node
    template <BC>
    Edge* get_match(void);
    /// return the variable index of the node
    int   get_info(void);
    /// returns whether the node is still matchable
    bool  is_matched(BC);
    /// tests whether the node is matched or not
    template <BC>
    bool  matched(void);

    /// check for removal from graph
    bool removed(void);
    //@}

    /// \name Update
    //@{
    /// set the node info to \a i
    void  set_info(int i);
    /// set the pointer of the matching edge to m
    template <BC>
    void  set_match(Edge* m);
    /// match the node
    template <BC>
    void  match(void);
    /// unmatch the node
    template <BC>
    void  unmatch(void);
    //@}
  };

  /// Value node
  class ValNode : public VVGNode {
  private:
    /// minimal required occurence of the value as stored in k
    int _klb;
    /// maximal required occurence of the value as stored in k
    int _kub;
    /// index to acces the value via cardinality array k
    int _kidx;
    /// stores the current number of occurences of the value
    int _kcount;


    /// store numbre of conflicting matching edges
    int noc;

    /**
     * \brief Minimal capacity of the value node
     *
     * Stores who often the value node can be matched in a LBC-matching
     */
    int lb;
    int ublow;
    /**
     * \brief Maximal capacity of the value node
     *
     * Stores who often the value node can be matched in a UBC-matching
     */

    int ub;

  public:
    /// stores the value of the node
    int val;
    /// stores the index of the node
    int idx;
    /// stores the number of incident edges on the node
    int noe;

    /// \name Constructors and destructors
    //@{
    /**
     * \brief Constructor for value node
     *
     * with minimal capacity \a min,
     * maximal capacity \a max,
     * the value \a value and the index \a k_access in \a k
     */
    ValNode(int min, int max, int value, int kidx, int kshift, int count);
    //@}

    /// \name Access
    //@{

    /// set the max cap for LBC
    void set_maxlow(int i);
    /// get max cap for LBC
    int  get_maxlow(void);


    /// Mark the value node as conflicting in case of variable cardinalities
    void card_conflict(int c);
    /// Check whether the value node is conflicting
    int card_conflict(void);
    /// Reduce the conflict counter
    void red_conflict(void);

    /// check for removal from graph
    bool removed(void);

    /// increases the value counter
    void inc(void);
    /// returns the current number of occurences of the value
    int kcount(void);
    /// returns the number of incident matching edges on a value node
    template <BC>
    int incid_match(void);
    /// returns the index in cardinality array k
    int kindex(void);
    /// return the node information
    int  get_info(void);
    /// returns \a true if the node is matched in BC, \a false otherwise
    template <BC>
    bool matched(void);
    /// tests whether the node is a sink
    bool sink(void);
    /// tests whether the node is a source
    bool source(void);
    /// return the minimal node capacity as stored in \a k
    int  kmin(void);
    /// return the maximal node capacity as stored in \a k
    int  kmax(void);
    /// return minimal or maximal capacity
    template <BC>
    int  kbound(void);

    /// returns whether the node is still matchable
    bool is_matched(BC);
    //@}

    /// \name Update
    //@{
    void kcount(int);
    /// changes the index in the cardinality array k
    void kindex(int);
    /// decrease the node-capacity
    template <BC>
    void dec(void);
    /// increase the node-capacity
    template <BC>
    void inc(void);
    /// return the the node-capacity
    template <BC>
    int  cap(void);
    /// set the node-capacity to \a c
    template <BC>
    void set_cap(int c);
    /// match the node
    template <BC>
    void match(void);
    /// unmatch the node
    template <BC>
    void unmatch(void);
    /// node reset to original capacity values
    void reset(void);
    /// set the node infomration to \a i
    void set_info(int i);
    /// set the minimal k-capacity to min
    void set_kmin(int min);
    /// set the maximal k-capacity to max
    void set_kmax(int max);
    //@}
  };

  /// Class for edges \f$ e(x,v) \f$ in the variable-value-graph
  class Edge{
  private:
    /// pointer to the variable node
    VarNode* x;
    /// pointer to the value node
    ValNode* v;
    /// pointer to the next edge incident on the same variable node
    Edge* next_edge;
    /// pointer to the previous edge incident on the same variable node
    Edge* prev_edge;
    /// pointer to the next edge on the same value node
    Edge* next_vedge;
    /// pointer to the previous edge on the same value node
    Edge* prev_vedge;
    /**
     * \brief stores whether the edge is used in LBC
     * Flag storing whether the edge is used in any maximum LBC matching
     * in the variable-value-graph
     */
    bool  mrklb;
    /**
     * \brief stores whether the edge is used in UBC
     * Flag storing whether the edge is used in any maximum UBC matching
     * in the variable-value-graph
     */
    bool  mrkub;
    /// stores whether the edge is matched in LBC matching
    bool  um;
    /// stores whether the edge is matched in UBC matching
    bool  lm;
    /// stores whether the edge has been deleted during syncronization
    bool  deleted;   // del

  public:
    /// \name Constructors
    //@{
    /**
     * \brief Construct edge \f$e(x,v)\f$ from variable node \a x
     *  and value node \a y
     */
    Edge(VarNode* x, ValNode* v);
    /// Create a new edge in memory
    void* operator new(size_t, void*);
    //@}

    /// \name Access
    //@{
    /**
     * \brief return whether the edge is used
     *
     * An edge can be used in a matching on the graph,
     * a path on the graph or a cycle in the graph.
     *
     */
    template <BC>
    bool used(void);
    /// return whether the edge is matched
    template <BC>
    bool matched(void);
    /// return whether the edge has been deleted from the graph
    bool is_deleted(void);
    /**
     * \brief return a pointer to the next edge
     *  If \a t is false the function returns the next edge incident on \a x
     *  otherwise it returns the next edge incident on \a v
     */
    Edge*    next(bool t) const;
    /// return the pointer to the next edge incident on \a x
    Edge*    next(void) const;
    /// return the pointer to the previous edge incident on \a x
    Edge*    prev(void) const;
    /// return the pointer to the next edge incident on \a v
    Edge*    vnext(void) const;
    /// return the pointer to the previous edge incident on \a v
    Edge*    vprev(void) const;
    /// return the pointer to the variable node \a x of this edge
    VarNode* getVar(void);
    /// return the pointer to the value node \a v of this edge
    ValNode* getVal(void);
    /**
     * \brief return pointer to \a x if \a t = true otherwise return \a v
     *
     */
    VVGNode* getMate(bool t);
    //@}

    /// Update
    //@{
    /// Mark the edge as used
    template <BC>
    void use(void);
    /// Mark the edge as unused
    template <BC>
    void free(void);
    /**
     * \brief Reset the edge ( free the edge, and unmatch
     * the edge including its end-nodes
     *
     */
    template <BC>
    void reset(void);
    /// Match the edge
    template <BC>
    void match(void);
    /// Unmatch the edge and the incident nodes
    template <BC>
    void unmatch(void);
    /// Unmatch the edge and  ( \a x if t=false,  \a v otherwise )
    template <BC>
    void unmatch(bool t);
    /// Unlink the edge from the linked list of edges
    void unlink(void);
    /// Mark the edge as deleted during synchronization
    void del_edge(void);
    ///        Insert the edge again
    void insert_edge(void);
    /// return the reference to the next edge incident on \a x
    Edge**   next_ref(void);
    /// return the reference to the previous edge incident on \a x
    Edge**   prev_ref(void);
    /// return the reference to the next edge incident on \a v
    Edge**   vnext_ref(void);
    /// return the reference to the previous edge incident on \a v
    Edge**   vprev_ref(void);
    //@}
  };


  /**
   * \brief Variable-value-graph used during propagation
   *
   */
  template <class View, class Card, bool isView>
  class VarValGraph {
  private:
    /// failure flag
    bool fail;
    /// Allocated memory for nodes and edges
    char* mem;
    /// How much memory is allocated
    size_t _allocated;
    /// Problem variables
    ViewArray<View>& x;
    /// Copy keeping track of removed variables
    ViewArray<View>& y;
    /// Occurences
    ViewArray<Card>& k;
    /// Variable partition representing the problem variables
    VarNode** vars;
    /**
     * \brief Value partition
     *  For each value
     *  \f$ v_i\in V=\left(\bigcup_\{0, \dots, |x|-1\}\right) D_i \f$
     *  in the domains of the
     *  problem variables there is a node in the graph.
     */
    ValNode** vals;           // value partition    De
    /// the edges connecting the variable and the value partition
    Edge* edges;              // edges e
    /// cardinality of the variable partition
    int n_var;
    /**
     * \brief  cardinality of the value partition
     *
     * Computed as \f$ |V| = \left(\bigcup_\{0, \dots, |x|-1\}\right) D_i \f$
     */
    int n_val;
    /// the number of edges in the graph
    int n_edge;
    /// total number of nodes in the graph
    int node_size;
    /**
     * \brief The sum over the minimal capacities of all value nodes
     *
     *  \f$sum_min = \sum_{v_i \in V} l_i= k[i].min() \f$
     */
    int sum_min;
    /**
     * \brief The sum over the maximal capacities of all value nodes
     *
     * \f$sum_max = \sum_{v_i \in V} l_i= k[i].max() \f$
     */
    int sum_max;
  public:
    /// \name Constructors and Destructors
    //@{
    VarValGraph(ViewArray<View>&, ViewArray<View>&, ViewArray<Card>&,
                int , int , int );
    /// Destructor
    ~VarValGraph(void);
    //@}
    size_t allocated(void) const;
    /// \name Graph-interface
    //@{
    /**
     * \brief Check whether propagation failed
     *  This is actually needed to cope with failures
     *  occuring in functions returing only a boolean value
     *  and not an ExecStatus.
     */
    bool failed(void) const;
    /**
     * \brief Set the failure flag of the graph
     */
    void failed(bool b);

    /**
     * \brief Check whether minimum requirements shrink variable domains
     *
     */
    bool min_require(Space& home);

    /**
     * \brief Synchronization of the graph
     *
     * If the graph has already been constructed and some edges have
     * been removed during propagation \fn sync removes those edges
     * that do not longer belong to the graph associated with the current
     * variable domains.
     */
    bool sync(Space& home);

    /// Allocate memory for the graph
    void* operator new(size_t t);
    /// Free the memory for the graph
    void operator delete(void* p);

    /**
     * \brief Remove all those edges from the graph
     *        that do not belong
     *        to any possible maximum matching on the graph
     *
     */

    template <BC>
    bool narrow(Space& home);

    /** \brief Compute a maximum matching M on the graph
     *
     *  - If BC=UBC then \f$|M|= |X|\f$
     *  - If BC=LBC then \f$|M|= \sum_{i\in \{ 0, \dots, |X|-1\}}
     *    k[i].min()\f$
     */
    template <BC>
    bool maximum_matching(Space& home);

    /// Compute possible free alternating paths in the graph
    template <BC>
    void free_alternating_paths(Space& home);
    /// Compute possible strongly connected components of the graph
    template <BC>
    void strongly_connected_components(Space& home);
    /**
     * \brief Test whether the current maximal matching on the graph
     * can be augmented by an alternating path starting and ending with
     * a free node.
     */
    template <BC>
    bool augmenting_path(Space& home, VVGNode*);

  protected:
    /**
     * \brief Perform depth-first search on the graph
     *
     * Depth first search used to compute the
     * strongly connected components of the graph.
     */
    template <BC>
    void dfs(VVGNode*,
             bool[], bool[], int[],
             Support::StaticStack<VVGNode*,Region>&,
             Support::StaticStack<VVGNode*,Region>&,
             int&);

    //@}
  };

  forceinline
  VVGNode::VVGNode(void){} //no-op

  forceinline void*
  VVGNode::operator new(size_t, void* p){
    return p;
  }

  forceinline Edge**
  VVGNode::adj(void){
    return &e;
  }

  forceinline  Edge*
  VVGNode::first(void){
    return fst;
  }

  forceinline Edge*
  VVGNode::last(void){
    return lst;
  }

  forceinline void
  VVGNode::first(Edge* p){
    fst = p;
  }

  forceinline void
  VVGNode::last(Edge* p){
    lst = p;
  }

  forceinline bool
  VVGNode::get_type(void){
    return type;
  }

  forceinline void
  VVGNode::set_type(bool b){
    type = b;
  }

  forceinline Edge*
  VVGNode::inedge(void){
    return ie;
  }

  forceinline void
  VVGNode::inedge(Edge* p){
    ie = p;
  }

  template <BC direction>
  forceinline void
  VVGNode::set_match_flag(bool b){
    if (direction == UBC) {
      um = b;
    } else {
      lm = b;
    }
  }

  template <BC direction>
  forceinline bool
  VVGNode::get_match_flag(void){
    if (direction == UBC) {
      return um;
    } else {
      return lm;
    }
  }


  /// Variable Node

  forceinline bool
  VarNode::removed(void) {
    return noe == 0;
  }



  forceinline
  VarNode::VarNode(int x, int orig_idx) :
    ubm(NULL), lbm(NULL), var(x), noe(0), xindex(orig_idx){
    first(NULL);
    last(NULL);
    inedge(NULL);
    unmatch<LBC>();
    unmatch<UBC>();
    set_type(false);
  }

  forceinline bool
  VarNode::is_matched(BC d) {
    if (d == UBC) {
      return matched<UBC>();
    } else {
      return matched<LBC>();
    }
  }

  template <BC direction>
  forceinline bool
  VarNode::matched(void){
    return get_match_flag<direction>();
  }

  template <BC direction>
  forceinline void
  VarNode::match(void){
    set_match_flag<direction>(true);
  }

  template <BC direction>
  forceinline void
  VarNode::unmatch(void){
    set_match_flag<direction>(false);
    set_match<direction>(NULL);
  }

  template <BC direction>
  forceinline void
  VarNode::set_match(Edge* p){
    if (direction == UBC) {
      ubm = p;
    } else {
      lbm = p;
    }
  }

  template <BC direction>
  forceinline Edge*
  VarNode::get_match(void){
    if (direction == UBC) {
      return ubm;
    } else {
      return lbm;
    }
  }

  forceinline void
  VarNode::set_info(int i){
    var = i;
  }

  forceinline int
  VarNode::get_info(void){
    return var;
  }

  /// Value Node

  forceinline void
  ValNode::set_maxlow(int i){
    assert(i >= lb);
    ublow = i;
  }

  forceinline int
  ValNode::get_maxlow(void) {
    if (_klb == _kub) {
      assert(ublow == lb);
    }

    return ublow;
  }


  forceinline void
  ValNode::card_conflict(int c){
    noc = c;
  }

  forceinline void
  ValNode::red_conflict(void){
    noc--;
    assert(noc >= 0);
  }

  forceinline int
  ValNode::card_conflict(void){
    return noc;
  }

  forceinline bool
  ValNode::removed(void) {
    return noe == 0;
  }

  forceinline bool
  ValNode::is_matched(BC d) {
    if (d == UBC) {
      return matched<UBC>();
    } else {
      return ublow == 0;
    }
  }

  forceinline void
  ValNode::reset(void){
    lb = _klb;
    ublow = _kub;
    ub = _kub;
    noe = 0;
  }

  template <BC direction>
  forceinline int
  ValNode::kbound(void){
    if (direction == UBC) {
      return _kub;
    } else {
      return _klb;
    }
  }

  forceinline int
  ValNode::kmax(void){
    return _kub;
  }

  forceinline int
  ValNode::kmin(void){
    return _klb;
  }

  forceinline void
  ValNode::set_kmin(int klb){
    _klb = klb;
  }

  forceinline void
  ValNode::set_kmax(int kub){
    _kub = kub;
  }

  template <BC direction>
  forceinline int
  ValNode::cap(void){
    if (direction == UBC) {
      return ub;
    } else {
      return lb;
    }
  }

  template <BC direction>
  forceinline void
  ValNode::dec(void){
    if (direction == UBC) {
      ub--;
    } else {
      lb--;
      ublow--;
    }
  }

  template <BC direction>
  forceinline void
  ValNode::inc(void){
    if (direction == UBC) {
      ub++;
    } else {
      lb++;
      ublow++;
    }
  }

  template <BC direction>
  forceinline void
  ValNode::match(void){
    dec<direction>();
  }

  template <BC direction>
  forceinline void
  ValNode::unmatch(void){
    inc<direction>();
  }

  template <BC direction>
  forceinline bool
  ValNode::matched(void){
    return ( cap<direction>() == 0);
  }

  forceinline
  ValNode::ValNode(int min, int max, int value,
                   int kidx, int kshift, int count) :
    _klb(min), _kub(max), _kidx(kidx), _kcount(count),
    noc(0),
    lb(min), ublow(max), ub(max),
    val(value), idx(kshift), noe(0) {
    first(NULL);
    last(NULL);
    inedge(NULL);
    Edge** vadjacent = adj();
    *vadjacent = NULL;
    set_type(true);
  }

  template<BC direction>
  forceinline void
  ValNode::set_cap(int c){
    if (direction == UBC) {
      ub = c;
    } else {
      lb = c;
//       ublow = c;
    }
  }

  forceinline void
  ValNode::set_info(int i){
    idx = i;
  }

  forceinline int
  ValNode::get_info(void){
    return idx;
  }

  forceinline void
  ValNode::inc(void) {
    _kcount++;
  }

  forceinline int
  ValNode::kcount(void) {
    return _kcount;
  }

  forceinline void
  ValNode::kcount(int c) {
    _kcount = c;
  }

  forceinline void
  ValNode::kindex(int i){
    _kidx = i;
  }

  forceinline int
  ValNode::kindex(void){
    return _kidx;
  }

  /// Returs the number of incident matching edges on the node
  template <BC direction>
  forceinline int
  ValNode::incid_match(void){
    if (direction == LBC) {
      // std::cout << "LBC: "<< _kub <<"-"<<ublow <<"+"<<_kcount<<"\n";
      return _kub - ublow + _kcount;
    } else {
      // std::cout << "UBC: "<< _kub <<"-"<<ub <<"+"<<_kcount<<"\n";
      return _kub - ub + _kcount;
    }
  }


  forceinline bool
  ValNode::sink(void){
    // there are only incoming edges
    // in case of the UBC-matching
    bool is_sink = false;
    is_sink = (_kub - ub == noe);
    return is_sink;
  }

  forceinline bool
  ValNode::source(void){
    // there are only incoming edges
    // in case of the UBC-matching
    bool is_sink = false;
    is_sink = (_klb - lb == noe);
    return is_sink;
  }

  forceinline void
  Edge::unlink(void){
    // unlink from variable side
    Edge* p = prev_edge;
    Edge* n = next_edge;

    if (p != NULL) {
      Edge** pnext = p->next_ref();
      *pnext = n;
    }

    if (n != NULL) {
      Edge** nprev = n->prev_ref();
      *nprev = p;
    }

    if (this == x->first()) {
      Edge** ref = x->adj();
      *ref = n;
      x->first(n);
    }

    if (this == x->last()) {
      x->last(p);
    }

    // unlink from value side
    Edge* pv = prev_vedge;
    Edge* nv = next_vedge;

    if (pv != NULL) {
      Edge** pvnext = pv->vnext_ref();
      *pvnext = nv;
    }

    if (nv != NULL) {
      Edge** nvprev = nv->vprev_ref();
      *nvprev = pv;
    }

    if (this == v->first()) {
      Edge** ref = v->adj();
      *ref = nv;
      v->first(nv);
    }

    if (this == v->last()) {
      v->last(pv);
    }
  }

  forceinline
  Edge::Edge(VarNode* var, ValNode* val) :
    x(var), v(val),
    next_edge(NULL), prev_edge(NULL),
    next_vedge(NULL), prev_vedge(NULL),
    mrklb(false), mrkub(false),
    um(false), lm(false), deleted(false) {}

  forceinline void*
  Edge::operator new(size_t, void* p){ //why is there no argument?
    return p;
  }

  template <BC direction>
  forceinline void
  Edge::use(void){
    if (direction == UBC) {
      mrkub = true;
    } else {
      mrklb = true;
    }
  }

  template <BC direction>
  forceinline void
  Edge::free(void){
    /// the failure is here, capacity is not increased for value nodes
    if (direction == UBC) {
      mrkub = false;
    } else {
      mrklb = false;
    }
  }

  template <BC direction>
  forceinline void
  Edge::reset(void){
    this->free<direction>();
    this->unmatch<direction>();
  }

  template <BC direction>
  forceinline bool
  Edge::used(void){
    if (direction == UBC){
      return mrkub;
    } else {
      return mrklb;
    }
  }

  forceinline Edge*
  Edge::next(void) const{
    return next_edge;
  }

  forceinline Edge*
  Edge::next(bool t) const{
    if (t) {
      return next_vedge;
    } else {
      return next_edge;
    }
  }

  forceinline Edge*
  Edge::vnext(void) const{
    return next_vedge;
  }

  forceinline Edge**
  Edge::vnext_ref(void) {
    return &next_vedge;
  }

  forceinline Edge*
  Edge::prev(void) const{
    return prev_edge;
  }

  forceinline Edge**
  Edge::prev_ref(void) {
    return &prev_edge;
  }

  forceinline Edge*
  Edge::vprev(void) const{
    return prev_vedge;
  }

  forceinline Edge**
  Edge::vprev_ref(void) {
    return &prev_vedge;
  }

  forceinline Edge**
  Edge::next_ref(void){
    return &next_edge;
  }
  forceinline VarNode*
  Edge::getVar(void){
    assert(x != NULL);
    return x;
  }

  forceinline ValNode*
  Edge::getVal(void){
    assert(v != NULL);
    return v;
  }

  forceinline VVGNode*
  Edge::getMate(bool type){
    if (type) {
      return x;
    } else {
      return v;
    }
  }

  template <BC direction>
  forceinline void
  Edge::unmatch(void){
    if (direction == UBC) {
      um = false;
    } else {
      lm = false;
    }
    x->unmatch<direction>();
    v->unmatch<direction>();
  }

  template <BC direction>
  forceinline void
  Edge::unmatch(bool node){
    if (direction == UBC) {
      um = false;
    } else {
      lm = false;
    }
    if (node) {
      v->template unmatch<direction>();
    } else {
      x->template unmatch<direction>();
    }
  }

  template <BC direction>
  forceinline void
  Edge::match(void){
    if (direction == UBC) {
      um = true;
      x->template match<direction>();
      x->template set_match<direction>(this);
      v->template match<direction>();
    } else {
      lm = true;
      x->template match<direction>();
      x->template set_match<direction>(this);
      assert(x->template matched<direction>());
      v->template match<direction>();
//       assert(v->template matched<direction>());
    }
  }

  template <BC direction>
  forceinline bool
  Edge::matched(void){
    if (direction == UBC) {
      return um;
    } else {
      return lm;
    }
  }

  forceinline void
  Edge::del_edge(void){
    deleted = true;
  }

  forceinline void
  Edge::insert_edge(void){
    deleted = false;
  }


  forceinline bool
  Edge::is_deleted(void){
    return deleted;
  }

  /**
   * \brief Constructor for the variable-value-graph
   *
   * The variable parition is initialized with the variables from \a x,
   * the value partition is initialized with the values from \a v,
   * where \f$ \forall v_i\in V:\f$ \a val_idx[i] is the
   * index of \f$ v_i\f$ in \f$ k \f$. \a nov denotes the cardinality of the
   * value partition in the graph, \a noe the number of edges in the
   * graph, \a min_occ the sum over the minimal occurences
   * of all values and \a max_occ the sum over the maximal occurences
   * of all values.
   **/

  template <class View, class Card, bool isView>
  VarValGraph<View, Card, isView>::VarValGraph(ViewArray<View>& xref,
                                               ViewArray<View>& yref,
                                               ViewArray<Card>& kref,
                                               int noe,
                                               int smin, int smax)
    : fail(false),
      x(xref),
      y(yref),
      k(kref),
      n_var(x.size()),
      n_val(k.size()),
      n_edge(noe),
      node_size(n_var + n_val),
      sum_min(smin),
      sum_max(smax) {

    //memory allocation
    size_t  edge_size      = sizeof(Edge)     * n_edge;
    size_t  var_size       = sizeof(VarNode)  * n_var;
    size_t  var_ptr_size   = sizeof(VarNode*) * n_var;
    size_t  val_size       = sizeof(ValNode)  * n_val;
    size_t  val_ptr_size   = sizeof(ValNode*) * n_val;
    size_t  size           = edge_size + var_size + var_ptr_size +
      val_size + val_ptr_size;

    _allocated = size;

    mem      = static_cast<char*>
      (heap.ralloc(size));
    edges    = Support::ptr_cast<Edge*>
      (mem);
    VarNode* vars_ptr = Support::ptr_cast<VarNode*>
      (mem + edge_size);
    ValNode* vals_ptr = Support::ptr_cast<ValNode*>
      (mem + edge_size + var_size);
    vars     = Support::ptr_cast<VarNode**>
      (mem + edge_size + var_size + val_size);
    vals     = Support::ptr_cast<ValNode**>
      (mem + edge_size + var_size + val_size + var_ptr_size);

    for (int i = n_val; i--; ){
      int kmi = k[i].min();
      int kma = k[i].max();
      int kc  = k[i].counter();
      if (kc != kma) {
        if (kmi >= kc) {
          kmi -=kc;
          assert(kmi >=0);
        } else {
          kmi = 0;
        }
        kma -= kc;
        assert (kma > 0);
        vals[i] = new (vals_ptr + i)
          ValNode(kmi, kma, k[i].card(), i, i + n_var, kc);
      } else {
        vals[i] = new (vals_ptr + i)
          ValNode(0, 0, k[i].card(), i, i + n_var, kc);
      }
    }

    for (int i = n_var; i--; ){

      vars[i]          = new (vars_ptr + i) VarNode(i, i);
      VarNode* vrn     = vars[i];
      // get the space for the edges of the varnode
      Edge** xadjacent = vrn->adj();

      ViewValues<View> xiter(x[i]);
      int j = 0;
      for (; xiter(); ++xiter){
        int v = xiter.val();
        // get the correct index for the value
        while(vals[j]->val < v){
          j++;
        }
        ValNode* vln = vals[j];
        *xadjacent = new (edges) Edge(vars_ptr + i, vals_ptr + j);
        vrn->noe++;
        if (vrn->first() == NULL) {
          vrn->first(*xadjacent);
        }
        Edge* oldprev  = vrn->last();
        vrn->last(*xadjacent);
        Edge** newprev = vrn->last()->prev_ref();
        *newprev       = oldprev;

        if (vln->first() == NULL) {
          vln->first(*xadjacent);
          vln->last(*xadjacent);
          vln->noe++;
        } else {
          Edge* old      = vln->first();
          vln->first(*xadjacent);
          Edge** newnext = vln->first()->vnext_ref();
          *newnext       = old;
          Edge** setprev = old->vprev_ref();
          *setprev       = vln->first();
          vln->noe++;
        }
        edges++;
        xadjacent = (*xadjacent)->next_ref();
      }
      *xadjacent = NULL;
    }
  }

  template <class View, class Card, bool isView>
  forceinline size_t
  VarValGraph<View, Card, isView>::allocated(void) const {
    return _allocated + sizeof(VarValGraph<View, Card, isView>);
  }

  template <class View, class Card, bool isView>
  forceinline bool
  VarValGraph<View, Card, isView>::failed(void) const {
    return fail;
  }

  template <class View, class Card, bool isView>
  forceinline void
  VarValGraph<View, Card, isView>::failed(bool b){
    fail = b;
  }



  template <class View, class Card, bool isView>
  inline bool
  VarValGraph<View, Card, isView>::min_require(Space& home){
    bool modified = false;
    for (int i = n_val; i--; ) {
      ValNode* vln = vals[i];
      if (vln->noe > 0) {
        if (k[i].min() == vln->noe) {
          // all variable nodes reachable from vln should be equal to vln->val
          for (Edge* e = vln->first(); e != NULL; e = e->vnext()) {
            VarNode* vrn = e->getVar();
            int vi = vrn->get_info();
            for (Edge* f = vrn->first(); f != NULL; f = f->next()) {
              if (f != e) {
                ValNode* w = f->getVal();
                w->noe--;
                vrn->noe--;
                f->del_edge();
                f->unlink();
              }
            }
            assert(vrn->noe == 1);

            ModEvent me = x[vi].eq(home, vln->val);
            if (me_failed(me))
              failed(true);
            if (me_modified(me))
              modified = true;

            vars[vi] = vars[--n_var];
            vars[vi]->set_info(vi);

            int n = x.size();
            x[vi] = x[--n];
            x.size(n);

            node_size--;
            vln->noe--;
          }


          int vidx = vln->kindex();
          if (isView) {
            ModEvent me = k[vidx].eq(home, k[vidx].min());
            if (me_failed(me))
              failed(true);
            if (me_modified(me))
              modified = true;
          }

          k[vidx].counter(k[vidx].min());

          vln->template set_cap<UBC>(0);
          vln->template set_cap<LBC>(0);
          vln->set_maxlow(0);

          if (sum_min && sum_min >= k[vidx].min()) {
            sum_min -= k[vidx].min();
          }
          assert(sum_min >=0);

          if (sum_max && sum_max >= k[vidx].max()) {
            sum_max -= k[vidx].max();
          }
          assert(sum_max >=0);
        }
      } else {
        vals[i]->template set_cap<UBC>(0);
        vals[i]->template set_cap<LBC>(0);
        vals[i]->set_maxlow(0);
        vals[i]->set_kmax(0);
        vals[i]->set_kmin(0);

      }

      if (isView) {
        if (k[i].counter() == 0) {
          ModEvent me = k[i].lq(home, vals[i]->noe);
          if (me_failed(me))
            failed(true);
          if (me_modified(me) && k[i].max() != vals[i]->noe)
            modified = true;
        }
      }
    }

    for (int i = n_val; i--; )
      vals[i]->set_info(n_var + i);

    return modified;
  }

  template <class View, class Card, bool isView>
  inline bool
  VarValGraph<View, Card, isView>::sync(Space& home) {
    Region r(home);
    VVGNode** re = r.alloc<VVGNode*>(node_size);
    int n_re = 0;

    // synchronize cardinality variables
    if (isView) {
      for (int i = n_val; i--; ) {
        ValNode* v  = vals[i];
        int inc_ubc = v->template incid_match<UBC>();
        int inc_lbc = v->template incid_match<LBC>();
        if (v->noe == 0) {
          inc_ubc = 0;
          inc_lbc = 0;
        }
        int rm = v->kmax() - k[i].max();
        // the cardinality bounds have been modified
        if (k[i].max() < v->kmax() || k[i].min() > v->kmin()) {
          if (k[i].max() != k[i].counter() || k[i].max() == 0) {
            // update the bounds
            v->set_kmax(k[i].max());
            v->set_kmin(k[i].min());

            //everything is fine
            if (inc_ubc <= k[i].max()) {
              // adjust capacities
              v->template set_cap<UBC>(k[i].max() - (inc_ubc));
              v->set_maxlow(k[i].max() - (inc_lbc));
              if (v->kmin() == v->kmax()) {
                v->template set_cap<LBC>(k[i].max() - (inc_lbc));
              }
            } else {
              // set cap to max and resolve conflicts on view side
              // set to full capacity for later rescheduling
              if (v->template cap<UBC>()) {
                v->template set_cap<UBC>(k[i].max());
              }
              v->set_maxlow(k[i].max() - (inc_lbc));
              if (v->kmin() == v->kmax()) {
                v->template set_cap<LBC>(k[i].max() - (inc_lbc));
              }
              v->card_conflict(rm);
            }
          }
        }
        if (inc_lbc < k[i].min() && v->noe > 0) {

          v->template set_cap<LBC>(k[i].min() - inc_lbc);
          re[n_re] = v;
          n_re++;
        }
      }

      for (int i = n_var; i--; ) {
        Edge* mub = vars[i]->template get_match<UBC>();
        if (mub != NULL) {
          ValNode* vu = mub->getVal();
          if (! (vars[i]->noe == 1) ) {
            if (vu->card_conflict()) {
              vu->red_conflict();
              mub->template unmatch<UBC>(vars[i]->get_type());
              re[n_re] = vars[i];
              n_re++;
            }
          }
        }
      }
    }

    // go on with synchronization
    assert(x.size() == n_var);
    for (int i = n_var; i--; ) {

      VarNode* vrn = vars[i];
      if(static_cast<int>(x[i].size()) != vrn->noe){
        // if the variable is already assigned
        if (x[i].assigned()) {
          int  v = x[i].val();
          ValNode* rv = NULL;
          int rv_idx  = 0;
          Edge* mub = vrn->template get_match<UBC>();
          if (mub != NULL) {
            if (v != mub->getVal()->val) {
              mub->template unmatch<UBC>();
              re[n_re] = vars[i];
              n_re++;
            }
          }

          Edge* mlb = vrn->template get_match<LBC>();
          if (mlb != NULL) {
            ValNode* vln = mlb->getVal();
            if (v != vln->val) {
              mlb->template unmatch<LBC>();
              int nom = vln->template incid_match<LBC>();
              // less values than required
              bool cond = nom < vln->kmin();
              if (cond) {
                re[n_re] = vln;
                n_re++;
              }
            }
          }

          for (Edge* e = vrn->first(); e != NULL; e = e->next()){
            ValNode* vln = e->getVal();
            if (vln->val != v) {
              vrn->noe--;
              e->getVal()->noe--;
              e->del_edge();
              e->unlink();
            } else {
              rv = e->getVal();
              rv_idx = rv->kindex();
            }
          }
        } else {

          // delete the edge
          ViewValues<View> xiter(x[i]);
          Edge*  mub = vrn->template get_match<UBC>();
          Edge*  mlb = vrn->template get_match<LBC>();
          Edge** p   = vrn->adj();
          Edge*  e   = *p;
          do {
            // search the edge that has to be deleted
            while (e != NULL && e->getVal()->val < xiter.val()) {
              // Skip edge
              e->getVal()->noe--;
              vrn->noe--;
              e->del_edge();
              e->unlink();
              e = e ->next();
              *p = e;
            }

            assert(xiter.val() == e->getVal()->val);

            // This edge must be kept
            e->template free<UBC>();
            e->template free<LBC>();
            ++xiter;
            p = e->next_ref();
            e = e->next();
          } while (xiter());
          *p = NULL;
          while (e) {
            e->getVar()->noe--;
            e->getVal()->noe--;
            e->del_edge();
            e->unlink();
            e = e->next();
          }

          if (mub != NULL){
            if (mub->is_deleted()) {
              mub->template unmatch<UBC>();
              re[n_re] = vars[i];
              n_re++;
            }
          }

          //lower bound matching can be zero
          if (mlb != NULL) {
            if (mlb->is_deleted()) {
              ValNode* vln = mlb->getVal();
              mlb->template unmatch<LBC>();
              int nom   = vln->template incid_match<LBC>();
              bool cond = nom < vln->kmin();
              if (cond) {
                re[n_re] = vln;
                n_re++;
              }
            }
          }
        }
      }
      vars[i]->set_info(i);
    }

    for (int i = n_val; i--; ) {
      if (k[i].min() > vals[i]->noe && k[i].counter() == 0) {
        failed(true);
        return false;
      }
      vals[i]->set_info(n_var + i);
    }

    if (n_re == 0) {
      // no repair needed
      return true;
    } else {
      // start repair

      bool repaired = true;
      while (n_re ) {
        n_re--;
        assert(re[n_re] != NULL);
        if (!(re[n_re]->removed())) {
          if (!(re[n_re]->get_type())) {
            VarNode* vrn = static_cast<VarNode*>(re[n_re]);
            if (!vrn->template matched<UBC>()) {
              repaired &= augmenting_path<UBC>(home,vrn);
            }
          } else {
            assert(re[n_re]->get_type());
            ValNode* vln = static_cast<ValNode*>(re[n_re]);
            while(!vln->template matched<LBC>()) {
              repaired &= augmenting_path<LBC>(home,vln);
              if (!repaired) {
                break;
              }
            }
          }
        }
      }
      return repaired;
    }
  }

  template <class View, class Card, bool isView> template <BC direction>
  inline bool
  VarValGraph<View, Card, isView>::narrow(Space& home) {
    bool modified  = false;
    for (int i = n_var; i--; ) {
      VarNode* vrn = vars[i];
      if (vrn->noe == 1) {
        Edge* e = vrn->first();
        ValNode* v = e->getVal();
        e->template free<direction>();
        ModEvent me = x[i].eq(home, v->val);
        if (me_failed(me)) {
          failed(true);
          return false;
        }
        if (me_modified(me))
          modified = true;
        v->inc();
      }
    }
    for (int i = n_val; i--; ) {
      ValNode* v = vals[i];
      if (isView)
        if (k[i].counter() == 0)
          if (me_failed(k[i].lq(home, v->noe))) {
            failed(true);
            return false;
          }

      if (v->noe > 0) {

        if (isView)
          if (me_failed(k[i].lq(home, v->noe))) {
            failed(true);
            return false;
          }

        // If the maximum number of occurences of a value is reached
        // it cannot be consumed by another view

        if (v->kcount() == v->kmax()) {
          int vidx = v->kindex();

          k[i].counter(v->kcount());

          if (isView) {
            if (!k[i].assigned()) {
              ModEvent me = k[i].eq(home, k[i].counter());
              if (me_failed(me)) {
                failed(true);
                return false;
              }
            }
          }

          bool delall = false;
          if (v->card_conflict() &&
              v->noe > v->kmax()) {
            delall = true;

          }

          for (Edge* e = v->last(); e != NULL; e = e->vprev()) {
            VarNode* vrn = e->getVar();
            if (vrn->noe == 1) {
              vrn->noe--;
              v->noe--;
              int vi= vrn->get_info();

              int n = x.size();
              x[vi] = x[--n];
              //              x[vi].index(vi);
              x.size(n);

              vars[vi] = vars[--n_var];
              vars[vi]->set_info(vi);
              node_size--;
              e->del_edge();
              e->unlink();

            } else {
              if (delall) {
                if (me_failed(x[vrn->get_info()].nq(home, v->val))) {
                  failed(true);
                  return false;
                }
                vrn->noe--;
                v->noe--;
                e->del_edge();
                e->unlink();
              }
            }
          }
          v->template set_cap<UBC>(0);
          v->template set_cap<LBC>(0);
          v->set_maxlow(0);
          if (sum_min && sum_min >= k[vidx].min()) {
            sum_min -= k[vidx].min();
          }
          assert(sum_min >=0);

          if (sum_max && sum_max >= k[vidx].max()) {
            sum_max -= k[vidx].max();
          }
          assert(sum_max >=0);

        } else {
          int cur = v->kcount();
          if (cur > 0) {
            v->kcount(0);
          }
        }
      }
    }
    for (int i = n_var; i--; )
      vars[i]->set_info(i);

    for (int i = n_val; i--; ) {
      if (vals[i]->noe == 0) {
        vals[i]->template set_cap<UBC>(0);
        vals[i]->template set_cap<LBC>(0);
        vals[i]->set_maxlow(0);
      }
      vals[i]->set_info(n_var + i);
    }

    for (int i = n_var; i--; )
      if (vars[i]->noe > 1)
        for (Edge* e = vars[i]->first(); e != NULL; e = e->next()) {
          bool keepedge = false;
          keepedge =  (e->template matched<direction>() ||
                       e->template used<direction>());
          if (!keepedge) {
            ValNode* v = e->getVal();
            ModEvent me = x[i].nq(home, v->val);
            if (me_failed(me)) {
              failed(true);
              return false;
            }
            if (me_modified(me))
              modified = true;
          } else {
            e->template free<direction>();
          }
        }
    return modified;
  }

  template <class View, class Card, bool isView>  template <BC direction>
  inline bool
  VarValGraph<View, Card, isView>::maximum_matching(Space& home) {

    int required_size = 0;
    int card_match    = 0;

    if (direction == UBC) {
      required_size = n_var;
    } else {
      required_size = sum_min;
    }

    // find an intial matching in O(n*d)
    // greedy algorithm

    for (int i = n_val; i--; ) {
      ValNode* vln = vals[i];
      for (Edge* e = vln->first(); e != NULL ; e = e->vnext()) {
        VarNode* vrn = e->getVar();
        if (!vrn->template matched<direction>() &&
            !vln->template matched<direction>()) {
          e->template match<direction>();
          card_match++;
        }
      }
    }

    Region r(home);
    if (card_match < required_size) {
      if (direction == LBC) {
        // collect free value nodes
        ValNode** free = r.alloc<ValNode*>(n_val);
        int f = 0;
        // find failed nodes
        for (int i = n_val; i--; ) {
          ValNode* vln = vals[i];
          if (!vln->template matched<direction>()) {
            free[f++] = vln;
          }
        }

        for (int i = 0; i < f; i++) {
          while(!free[i]->template matched<direction>()) {
            if (augmenting_path<direction>(home,free[i])) {
              card_match++;
            } else {
              break;
            }
          }
        }
      } else {
        VarNode** free = r.alloc<VarNode*>(n_var);
        int f = 0;
        // find failed nodes
        for (int i = n_var; i--; ) {
          VarNode* vrn = vars[i];
          if (!vrn->template matched<direction>()) {
            free[f++] = vrn;
          }
        }

        for (int i = 0; i < f; i++) {
          if (!free[i]->template matched<direction>()) {
            if (augmenting_path<direction>(home,free[i])) {
              card_match++;
            }
          }
        }
      }
    }
    return (card_match >= required_size);
  }

  template <class View, class Card, bool isView> template<BC direction>
  inline bool
  VarValGraph<View, Card, isView>::augmenting_path(Space& home, VVGNode* v) {
    Region r(home);
    Support::StaticStack<VVGNode*,Region> ns(r,node_size);
    bool* visited = r.alloc<bool>(node_size);
    Edge** start = r.alloc<Edge*>(node_size);

    // augmenting path starting in a free var node
    assert(!v->is_matched(direction));

    // keep track of the nodes that have already been visited
    VVGNode* sn = v;

    // mark the start partition
    bool sp = sn->get_type();

    // nodes in sp only follow free edges
    // nodes in V - sp only follow matched edges

    for (int i = node_size; i--; )
      visited[i] = false;

    for (int i = node_size; i--; )
      if (i >= n_var) {
        vals[i - n_var]->inedge(NULL);
        start[i]   = vals[i - n_var]->first();
      } else {
        vars[i]->inedge(NULL);
        start[i]   = vars[i]->first();
      }

    v->inedge(NULL);
    ns.push(v);
    visited[v->get_info()] = true;
    while (!ns.empty()) {
      VVGNode* v = ns.top();
      Edge* e = NULL;
      if (v->get_type() == sp) {
        // follow next free edge
        e = start[v->get_info()];
        while (e != NULL && e->template matched<direction>()) {
          e = e->next(v->get_type());
        }
      } else {
        e = start[v->get_info()];
        while (e != NULL && !e->template matched<direction>()) {
          e = e->next(v->get_type());
        }
        start[v->get_info()] = e;
      }
      if (e != NULL) {
        start[v->get_info()] = e->next(v->get_type());
        VVGNode* w = e->getMate(v->get_type());
        if (!visited[w->get_info()]) {
          // unexplored path
          if (!w->is_matched(direction) && w->get_type() != sp) {
            if (v->inedge() != NULL) {
              // augmenting path of length l > 1
              e->template match<direction>();
              break;
            } else {
              // augmenting path of length l = 1
              e->template match<direction>();
              ns.pop();
              return true;
            }
          } else {
            w->inedge(e);
            visited[w->get_info()] = true;
            // find matching edge m incident with w
            ns.push(w);
          }
        }
      } else {
        // tried all outgoing edges without finding an augmenting path
        ns.pop();
      }
    }

    bool pathfound = false;
    if (!ns.empty()) {
      pathfound = true;
    }

    while (!ns.empty()) {
      VVGNode* t = ns.top();
      if (t != sn) {
        Edge* in   = t->inedge();
        if (t->get_type() != sp) {
          assert(in != NULL);
          in->template match<direction>();
        } else {
          // avoid defects
          if (!sp) {
            in->template unmatch<direction>(!sp);
          } else {
            in->template unmatch<direction>();
          }
        }
        ns.pop();
      } else {
        ns.pop();
      }
    }
    return pathfound;
  }

  template <class View, class Card, bool isView> template<BC direction>
  inline void
  VarValGraph<View, Card, isView>::free_alternating_paths(Space& home) {
    Region r(home);
    Support::StaticStack<VVGNode*,Region> ns(r,node_size);
    bool* visited = r.alloc<bool>(node_size);
    // keep track of the nodes that have already been visited
    for (int i = node_size; i--; )
      visited[i] = false;

    if (direction == LBC) {
      // after a maximum matching on the value nodes there still can be
      // free value nodes, hence we have to consider ALL nodes whether
      // they are the starting point of an even alternating path in G
      for (int i = n_var; i--; ){
        if(!vars[i]->is_matched(LBC)){
          // unmatched var-node
          ns.push(vars[i]);
          visited[vars[i]->get_info()] = true;
        }
      }
      for (int i = n_val; i--; ){
        if(!vals[i]->is_matched(LBC)){
          // unmatched val-node
          ns.push(vals[i]);
          visited[vals[i]->get_info()] = true;
        }
      }

    } else {
      // clearly, after a maximum matching on the x variables
      // corresponding to a set cover on x there are NO free var nodes
      //       std::cout << "alt_path for ubm: \n";
      // after  max_match_ub there can only be free val-nodes
      for (int i = n_val; i--; ){
        if(!vals[i]->is_matched(UBC)){
          // still capacities left
          ns.push(vals[i]);
          visited[vals[i]->get_info()] = true;
        }
      }
    }

    while (!ns.empty()){
      VVGNode* node = ns.top();
      ns.pop();
      if (node->get_type()) {
        // ValNode
        ValNode* vln = static_cast<ValNode*>(node);
        for (Edge* cur = vln->first(); cur != NULL; cur = cur->vnext()) {
          VarNode* mate = cur->getVar();
          bool follow = false;
          switch (direction) {
            // edges in M_l are directed from values to variables
          case LBC:
            follow = cur->template matched<direction>();
            break;
          case UBC:
            follow = !cur->template matched<direction>();
            break;
          default: GECODE_NEVER;
          }
          if (follow) {
            // mark the edge
            cur->template use<direction>();
            if (!visited[mate->get_info()]) {
              ns.push(mate);
              visited[mate->get_info()] = true;
            }
          }
        }
      } else {
        // VarNode
        VarNode* vrn = static_cast<VarNode*>(node);
        switch (direction) {
          // after LBC-matching we can follow every unmatched edge
        case LBC: {
          for (Edge* cur = vrn->first(); cur != NULL; cur = cur->next()){
            ValNode* mate = cur->getVal();
            if (!cur->template matched<LBC>()) {
              cur->template use<LBC>();
              if (!visited[mate->get_info()]) {
                ns.push(mate);
                visited[mate->get_info()] = true;
              }
            }
          }
          break;
        }
          // after ub-matching we can only follow a matched edge
        case UBC: {
          Edge* cur = vrn->template get_match<UBC>();
          if (cur != NULL) {
            cur->template use<UBC>();
            ValNode* mate = cur->getVal();
            if (!visited[mate->get_info()]) {
              ns.push(mate);
              visited[mate->get_info()] = true;
            }
          }
          break;
        }
        default: GECODE_NEVER;
        }
      }
    }
  }

  template <class View, class Card, bool isView> template <BC direction>
  inline void
  VarValGraph<View, Card, isView>::dfs(VVGNode* v,
                                       bool inscc[],
                                       bool in_unfinished[],
                                       int dfsnum[],
                                       Support::StaticStack<VVGNode*,Region>& roots,
                                       Support::StaticStack<VVGNode*,Region>& unfinished,
                                       int& count){
    count++;
    int v_index            = v->get_info();
    dfsnum[v_index]        = count;
    inscc[v_index]         = true;
    in_unfinished[v_index] = true;

    unfinished.push(v);
    roots.push(v);
    for (Edge* e = v->first(); e != NULL; e = e->next(v->get_type())) {
      bool condition = false;
      // LBC-matching
      if (direction == LBC) {
        // ValNode
        if (v->get_type()) {
          condition = e->template matched<LBC>();
        } else {
          condition = !e->template matched<LBC>();
        }
        // UBC - matching
      } else {
        if (v->get_type()) {
          // in an upper bound matching a valnode only can follow unmatched edges
          condition = !e->template matched<UBC>();
        } else {
          condition = e->template matched<UBC>();
        }
      }
      if (condition) {
        VVGNode* w = e->getMate(v->get_type());
        int w_index = w->get_info();

        assert(w_index < node_size);
        if (!inscc[w_index]) {
          // w is an uncompleted scc
          w->inedge(e);
          dfs<direction>(w, inscc, in_unfinished, dfsnum,
                         roots, unfinished, count);
        } else {
          if (in_unfinished[w_index]) {
            // even alternating cycle found mark the edge closing the cycle,
            // completing the scc
            e->template use<direction>();
            // if w belongs to an scc we detected earlier
            // merge components
            assert(roots.top()->get_info() < node_size);
            while (dfsnum[roots.top()->get_info()] > dfsnum[w_index]) {
              roots.pop();
            }
          }
        }
      }
    }

    if (v == roots.top()) {
      while (v != unfinished.top()) {
        // w belongs to the scc with root v
        VVGNode* w = unfinished.top();
        Edge* ie = w->inedge();
        ie->template use<direction>();
        in_unfinished[w->get_info()] = false;
        unfinished.pop();
      }
      assert(v == unfinished.top());
      in_unfinished[v_index] = false;
      roots.pop();
      unfinished.pop();
    }
  }

  template <class View, class Card, bool isView> template <BC direction>
  inline void
  VarValGraph<View, Card, isView>::strongly_connected_components(Space& home) {
    Region r(home);
    bool* inscc = r.alloc<bool>(node_size);
    bool* in_unfinished = r.alloc<bool>(node_size);
    int* dfsnum = r.alloc<int>(node_size);

    for (int i = node_size; i--; ) {
      inscc[i]         = false;
      in_unfinished[i] = false;
      dfsnum[i]        = 0;
    }

    int count = 0;
    Support::StaticStack<VVGNode*,Region> roots(r,node_size);
    Support::StaticStack<VVGNode*,Region> unfinished(r,node_size);

    for (int i = n_var; i--; )
      dfs<direction>(vars[i], inscc, in_unfinished, dfsnum,
                     roots, unfinished, count);
  }

  template <class View, class Card, bool isView>
  forceinline
  VarValGraph<View, Card, isView>::~VarValGraph(void){
    heap.rfree(mem);
  }

  template <class View, class Card, bool isView>
  forceinline void*
  VarValGraph<View, Card, isView>::operator new(size_t t){
    return heap.ralloc(t);
  }

  template <class View, class Card, bool isView>
  forceinline void
  VarValGraph<View, Card, isView>::operator delete(void* p){
    heap.rfree(p);
  }


}}}

// STATISTICS: int-prop


