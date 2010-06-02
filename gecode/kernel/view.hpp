/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Copyright:
 *     Christian Schulte, 2005
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

namespace Gecode {

  /**
   * \brief Base-class for constant views
   * \ingroup TaskVarView
   */
  class ConstViewBase {
  public:
    /// \name Generic view information
    //@{
    /// Return degree (number of subscribed propagators and advisors)
    unsigned int degree(void) const;
    /// Return accumulated failure count (plus degree)
    double afc(void) const;
    /// Return whether this view is derived from a VarViewBase
    static bool varderived(void);
    /// Return dummy variable implementation of view
    VarImpBase* var(void) const;
    //@}

    /// \name Domain tests
    //@{
    /// Test whether view is assigned
    bool assigned(void) const;
    //@}
  };



  /**
   * \brief Base-class for variable views
   * \ingroup TaskVarView
   */
  template<class VarImp>
  class VarViewBase {
  protected:
    /// Pointer to variable implementation
    VarImp* varimp;
    /// Default constructor
    VarViewBase(void);
    /// Initialize with variable implementation x
    VarViewBase(VarImp* x);
  public:
    /// \name Generic view information
    //@{
    /// Return whether this view is derived from a VarViewBase
    static bool varderived(void);
    /// Return variable implementation of view
    VarImp* var(void) const;
    /// Return degree (number of subscribed propagators and advisors)
    unsigned int degree(void) const;
    /// Return accumulated failure count (plus degree)
    double afc(void) const;
    //@}

    /// \name Domain tests
    //@{
    /// Test whether view is assigned
    bool assigned(void) const;
    //@}

    /// \name View-dependent propagator support
    //@{
    /// Schedule propagator \a p with modification event \a me
    static void schedule(Space& home, Propagator& p, ModEvent me);
    /// Return modification event for view type in \a med
    static ModEvent me(const ModEventDelta& med);
    /// Translate modification event \a me to modification event delta for view
    static ModEventDelta med(ModEvent me);
    //@}

    /// \name Dependencies
    //@{
    /**
     * \brief Subscribe propagator \a p with propagation condition \a pc to view
     *
     * In case \a schedule is false, the propagator is just subscribed but
     * not scheduled for execution (this must be used when creating
     * subscriptions during propagation).
     */
    void subscribe(Space& home, Propagator& p, PropCond pc, bool schedule=true);
    /// Cancel subscription of propagator \a p with propagation condition \a pc to view
    void cancel(Space& home, Propagator& p, PropCond pc);
    /// Subscribe advisor \a a to view
    void subscribe(Space& home, Advisor& a);
    /// Cancel subscription of advisor \a a
    void cancel(Space& home, Advisor& a);
    //@}

    /// \name Delta information for advisors
    //@{
    /// Return modification event
    static ModEvent modevent(const Delta& d);
    //@}
  };

  /** \name View comparison
   *  \relates VarViewBase
   */
  //@{
  /// Test whether views \a x and \a y are the same
  template<class VarImpA, class VarImpB>
  bool same(const VarViewBase<VarImpA>& x, const VarViewBase<VarImpB>& y);
  /// Test whether view \a x comes before \a y (arbitrary order)
  template<class ViewA, class ViewB>
  bool before(const ViewA& x, const ViewB& y);
  //@}


  /**
   * \brief Base-class for derived views
   * \ingroup TaskVarView
   */
  template<class View>
  class DerivedViewBase {
  protected:
    /// View from which this view is derived
    View view;
    /// Default constructor
    DerivedViewBase(void);
    /// Initialize with view x
    DerivedViewBase(const View& x);

  public:
    /// The variable type belonging to the \a View
    typedef typename ViewVarImpTraits<View>::VarImp VarImp;

    /// \name Generic view information
    //@{
    /// Return whether this view is derived from a VarViewBase
    static bool varderived(void);
    /// Return variable implementation of view
    VarImp* var(void) const;
    /// Return view from which this view is derived
    View base(void) const;
    /// Return degree (number of subscribed propagators)
    unsigned int degree(void) const;
    /// Return accumulated failure count (plus degree)
    double afc(void) const;
    //@}

    /// \name Domain tests
    //@{
    /// Test whether view is assigned
    bool assigned(void) const;
    //@}

    /// \name View-dependent propagator support
    //@{
    /// Schedule propagator \a p with modification event \a me
    static void schedule(Space& home, Propagator& p, ModEvent me);
    /// Return modification event for view type in \a med
    static ModEvent me(const ModEventDelta& med);
    /// Translate modification event \a me to modification event delta for view
    static ModEventDelta med(ModEvent);
    //@}

    /// \name Dependencies
    //@{
    /**
     * \brief Subscribe propagator \a p with propagation condition \a pc to view
     *
     * In case \a schedule is false, the propagator is just subscribed but
     * not scheduled for execution (this must be used when creating
     * subscriptions during propagation).
     */
    void subscribe(Space& home, Propagator& p, PropCond pc, bool schedule=true);
    /// Cancel subscription of propagator \a p with propagation condition \a pc to view
    void cancel(Space& home, Propagator& p, PropCond pc);
    /// Subscribe advisor \a a to view
    void subscribe(Space& home, Advisor& a);
    /// Cancel subscription of advisor \a a
    void cancel(Space& home, Advisor& a);
    //@}

    /// \name Delta information for advisors
    //@{
    /// Return modification event
    static ModEvent modevent(const Delta& d);
    //@}
  };


  /**
   * \brief Test whether views share same variable
   * \ingroup TaskVarView
   */
  bool shared(const ConstViewBase&, const ConstViewBase&);
  /**
   * \brief Test whether views share same variable
   * \ingroup TaskVarView
   */
  template<class VarImp>
  bool shared(const VarViewBase<VarImp>&, const ConstViewBase&);
  /**
   * \brief Test whether views share same variable
   * \ingroup TaskVarView
   */
  template<class ViewA>
  bool shared(const DerivedViewBase<ViewA>&, const ConstViewBase&);
  /**
   * \brief Test whether views share same variable
   * \ingroup TaskVarView
   */
  template<class VarImp>
  bool shared(const ConstViewBase&, const VarViewBase<VarImp>&);
  /**
   * \brief Test whether views share same variable
   * \ingroup TaskVarView
   */
  template<class ViewA>
  bool shared(const ConstViewBase&, const DerivedViewBase<ViewA>&);
  /**
   * \brief Test whether views share same variable
   * \ingroup TaskVarView
   */
  template<class VarImpA, class VarImpB>
  bool shared(const VarViewBase<VarImpA>&, const VarViewBase<VarImpB>&);
  /**
   * \brief Test whether views share same variable
   * \ingroup TaskVarView
   */
  template<class VarImpA, class ViewB>
  bool shared(const VarViewBase<VarImpA>&, const DerivedViewBase<ViewB>&);
  /**
   * \brief Test whether views share same variable
   * \ingroup TaskVarView
   */
  template<class ViewA, class VarImpB>
  bool shared(const DerivedViewBase<ViewA>&, const VarViewBase<VarImpB>&);
  /**
   * \brief Test whether views share same variable
   * \ingroup TaskVarView
   */
  template<class ViewA, class ViewB>
  bool shared(const DerivedViewBase<ViewA>&, const DerivedViewBase<ViewB>&);


  /// Test whether two views are the same
  inline
  bool same(const ConstViewBase&, const ConstViewBase&) {
    return false;
  }
  /// Test whether two views are the same
  template<class VarImp>
  bool same(const VarViewBase<VarImp>&, const ConstViewBase&) {
    return false;
  }
  /// Test whether two views are the same
  template<class ViewA>
  bool same(const ConstViewBase&, const DerivedViewBase<ViewA>&) {
    return false;
  }
  /// Test whether two views are the same
  template<class VarImpA, class ViewB>
  bool same(const VarViewBase<VarImpA>&, const DerivedViewBase<ViewB>&) {
    return false;
  }
  /// Test whether two views are the same
  template<class ViewA, class VarImpB>
  bool same(const DerivedViewBase<ViewA>&, const VarViewBase<VarImpB>&) {
    return false;
  }
  /// Test whether two views are the same
  template<class ViewA, class ViewB>
  bool same(const DerivedViewBase<ViewA>&, const DerivedViewBase<ViewB>&) {
    return false;
  }

  /*
   * Constant view: has no variable implementation
   *
   */
  forceinline unsigned int
  ConstViewBase::degree(void) const {
    return 0;
  }
  forceinline double
  ConstViewBase::afc(void) const {
    return 0.0;
  }
  forceinline bool
  ConstViewBase::varderived(void) {
    return false;
  }
  forceinline VarImpBase*
  ConstViewBase::var(void) const {
    return NULL;
  }
  forceinline bool
  ConstViewBase::assigned(void) const {
    return true;
  }

  /*
   * Variable view: contains a pointer to a variable implementation
   *
   */
  template<class VarImp>
  forceinline
  VarViewBase<VarImp>::VarViewBase(void)
    : varimp(NULL) {}
  template<class VarImp>
  forceinline
  VarViewBase<VarImp>::VarViewBase(VarImp* x)
    : varimp(x) {}
  template<class VarImp>
  forceinline bool
  VarViewBase<VarImp>::varderived(void) {
    return true;
  }
  template<class VarImp>
  forceinline VarImp*
  VarViewBase<VarImp>::var(void) const {
    return varimp;
  }
  template<class VarImp>
  forceinline unsigned int
  VarViewBase<VarImp>::degree(void) const {
    return varimp->degree();
  }
  template<class VarImp>
  forceinline double
  VarViewBase<VarImp>::afc(void) const {
    return varimp->afc();
  }
  template<class VarImp>
  forceinline bool
  VarViewBase<VarImp>::assigned(void) const {
    return varimp->assigned();
  }
  template<class VarImp>
  forceinline void
  VarViewBase<VarImp>::subscribe(Space& home, Propagator& p, PropCond pc,
                                 bool process) {
    varimp->subscribe(home,p,pc,process);
  }
  template<class VarImp>
  forceinline void
  VarViewBase<VarImp>::cancel(Space& home, Propagator& p, PropCond pc) {
    varimp->cancel(home,p,pc);
  }
  template<class VarImp>
  forceinline void
  VarViewBase<VarImp>::subscribe(Space& home, Advisor& a) {
    varimp->subscribe(home,a);
  }
  template<class VarImp>
  forceinline void
  VarViewBase<VarImp>::cancel(Space& home, Advisor& a) {
    varimp->cancel(home,a);
  }
  template<class VarImp>
  forceinline void
  VarViewBase<VarImp>::schedule(Space& home, Propagator& p, ModEvent me) {
    return VarImp::schedule(home,p,me);
  }
  template<class VarImp>
  forceinline ModEvent
  VarViewBase<VarImp>::me(const ModEventDelta& med) {
    return VarImp::me(med);
  }
  template<class VarImp>
  forceinline ModEventDelta
  VarViewBase<VarImp>::med(ModEvent me) {
    return VarImp::med(me);
  }

  template<class VarImp>
  forceinline ModEvent
  VarViewBase<VarImp>::modevent(const Delta& d) {
    return VarImp::modevent(d);
  }

  template<class VarImp>
  forceinline bool
  same(const VarViewBase<VarImp>& x, const VarViewBase<VarImp>& y) {
    return x.var() == y.var();
  }
  template<class ViewA, class ViewB>
  forceinline bool
  before(const ViewA& x, const ViewB& y) {
    return x.var() < y.var();
  }

  /*
   * Derived view: contain the base view from which they are derived
   *
   */

  template<class View>
  forceinline
  DerivedViewBase<View>::DerivedViewBase(void) {}

  template<class View>
  forceinline
  DerivedViewBase<View>::DerivedViewBase(const View& x)
    : view(x) {}

  template<class View>
  forceinline bool
  DerivedViewBase<View>::varderived(void) {
    return View::varderived();
  }

  template<class View>
  forceinline typename ViewVarImpTraits<View>::VarImp*
  DerivedViewBase<View>::var(void) const {
    return view.var();
  }

  template<class View>
  forceinline View
  DerivedViewBase<View>::base(void) const {
    return view;
  }

  template<class View>
  forceinline unsigned int
  DerivedViewBase<View>::degree(void) const {
    return view.degree();
  }
  template<class View>
  forceinline double
  DerivedViewBase<View>::afc(void) const {
    return view.afc();
  }
  template<class View>
  forceinline bool
  DerivedViewBase<View>::assigned(void) const {
    return view.assigned();
  }

  template<class View>
  forceinline void
  DerivedViewBase<View>::schedule(Space& home, Propagator& p, ModEvent me) {
    return View::schedule(home,p,me);
  }
  template<class View>
  forceinline ModEvent
  DerivedViewBase<View>::me(const ModEventDelta& med) {
    return View::me(med);
  }
  template<class View>
  forceinline ModEventDelta
  DerivedViewBase<View>::med(ModEvent me) {
    return View::med(me);
  }

  template<class View>
  forceinline void
  DerivedViewBase<View>::subscribe(Space& home, Propagator& p, PropCond pc,
                                   bool schedule) {
    view.subscribe(home,p,pc,schedule);
  }
  template<class View>
  forceinline void
  DerivedViewBase<View>::cancel(Space& home, Propagator& p, PropCond pc) {
    view.cancel(home,p,pc);
  }
  template<class View>
  forceinline void
  DerivedViewBase<View>::subscribe(Space& home, Advisor& a) {
    view.subscribe(home,a);
  }
  template<class View>
  forceinline void
  DerivedViewBase<View>::cancel(Space& home, Advisor& a) {
    view.cancel(home,a);
  }

  template<class View>
  forceinline ModEvent
  DerivedViewBase<View>::modevent(const Delta& d) {
    return View::modevent(d);
  }

  /*
   * Testing whether two views share the same variable
   *
   */

  forceinline bool
  shared(const ConstViewBase&, const ConstViewBase&) {
    return false;
  }

  template<class VarImp>
  forceinline bool
  shared(const VarViewBase<VarImp>&, const ConstViewBase&) {
    return false;
  }

  template<class View>
  forceinline bool
  shared(const DerivedViewBase<View>&, const ConstViewBase&) {
    return false;
  }

  template<class VarImp>
  forceinline bool
  shared(const ConstViewBase&, const VarViewBase<VarImp>&) {
    return false;
  }

  template<class View>
  forceinline bool
  shared(const ConstViewBase&, const DerivedViewBase<View>&) {
    return false;
  }

  template<class VarImpA, class VarImpB>
  forceinline bool
  shared(const VarViewBase<VarImpA>& x, const VarViewBase<VarImpB>& y) {
    return (static_cast<VarImpBase*>(x.var()) ==
            static_cast<VarImpBase*>(y.var()));
  }
  template<class VarImpA, class ViewB>
  forceinline bool
  shared(const VarViewBase<VarImpA>& x, const DerivedViewBase<ViewB>& y) {
    return (ViewB::varderived() &&
            static_cast<VarImpBase*>(x.var()) ==
            static_cast<VarImpBase*>(y.var()));
  }
  template<class ViewA, class VarImpB>
  forceinline bool
  shared(const DerivedViewBase<ViewA>& x, const VarViewBase<VarImpB>& y) {
    return (ViewA::varderived() &&
            static_cast<VarImpBase*>(x.var()) ==
            static_cast<VarImpBase*>(y.var()));
  }
  template<class ViewA, class ViewB>
  forceinline bool
  shared(const DerivedViewBase<ViewA>& x, const DerivedViewBase<ViewB>& y) {
    return (ViewA::varderived() && ViewB::varderived() &&
            static_cast<VarImpBase*>(x.var()) ==
            static_cast<VarImpBase*>(y.var()));
  }

}

// STATISTICS: kernel-var
