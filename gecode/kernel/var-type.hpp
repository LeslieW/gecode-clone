/*
 *  CAUTION:
 *    This file has been automatically generated. Do not edit,
 *    edit the following files instead:
 *     - ./gecode/int/var-imp/int.vis
 *     - ./gecode/int/var-imp/bool.vis
 *
 *  This file contains generated code fragments which are
 *  copyrighted as follows:
 *
 *  Main author:
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Copyright:
 *     Christian Schulte, 2007
 *
 *  The generated code fragments are part of Gecode, the generic
 *  constraint development environment:
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

#ifdef GECODE_HAS_INT_VARS
namespace Gecode { namespace Int { 
  /**
   * \defgroup TaskActorIntMEPC Integer modification events and propagation conditions
   * \ingroup TaskActorInt
   */
  //@{
  /// Domain operation has resulted in failure
  const Gecode::ModEvent ME_INT_FAILED = Gecode::ME_GEN_FAILED;
  /// Domain operation has not changed domain
  const Gecode::ModEvent ME_INT_NONE = Gecode::ME_GEN_NONE;
  /// Domain operation has resulted in a value (assigned variable)
  const Gecode::ModEvent ME_INT_VAL = Gecode::ME_GEN_ASSIGNED;
  /** 
   * \brief Domain operation has changed the minimum or maximum of the domain
   *
   * Note that this implies that the domain has not resulted in a value.
   *
   * If a propagator subscribes to this variable, it will be processed
   * assuming a ME_INT_BND modification event.
   */
  const Gecode::ModEvent ME_INT_BND = Gecode::ME_GEN_ASSIGNED + 1;
  /** 
   * \brief Domain operation has changed the domain
   *
   * Note that this implies that the domain has not resulted in a value
   * and that also the minimum and maximum of the domain have not changed.
   */
  const Gecode::ModEvent ME_INT_DOM = Gecode::ME_GEN_ASSIGNED + 2;
  /// Propagation condition to be ignored (convenience)
  const Gecode::PropCond PC_INT_NONE = Gecode::PC_GEN_NONE;
  /**
   * \brief Propagate when a view becomes assigned (single value)
   *
   * If a propagator \a p depends on a view \a x with propagation
   * condition PC_INT_VAL, then \a p is propagated when a domain
   * update operation on \a x returns the modification event ME_INT_VAL.
   */
  const Gecode::PropCond PC_INT_VAL = Gecode::PC_GEN_ASSIGNED;
  /**
   * \brief Propagate when minimum or maximum of a view changes
   *
   * If a propagator \a p depends on a view \a x with propagation
   * condition PC_INT_BND, then \a p is propagated when a domain
   * update operation on \a x returns the modification events ME_INT_VAL
   * or ME_INT_BND.
   */
  const Gecode::PropCond PC_INT_BND = Gecode::PC_GEN_ASSIGNED + 1;
  /**
   * \brief Propagate when domain changes
   *
   * If a propagator \a p depends on a view \a x with propagation
   * condition PC_INT_DOM, then \a p is propagated when a domain
   * update operation on \a x returns the modification event ME_INT_VAL,
   * ME_INT_BND, or ME_INT_DOM.
   */
  const Gecode::PropCond PC_INT_DOM = Gecode::PC_GEN_ASSIGNED + 2;
  //@}
}}
#endif
#ifdef GECODE_HAS_INT_VARS
namespace Gecode { namespace Int { 
  /**
   * \defgroup TaskActorIntMEPC Integer modification events and propagation conditions
   * \ingroup TaskActorInt
   */
  //@{
  /// Domain operation has resulted in failure
  const Gecode::ModEvent ME_BOOL_FAILED = Gecode::ME_GEN_FAILED;
  /// Domain operation has not changed domain
  const Gecode::ModEvent ME_BOOL_NONE = Gecode::ME_GEN_NONE;
  /// Domain operation has resulted in a value (assigned variable)
  const Gecode::ModEvent ME_BOOL_VAL = Gecode::ME_GEN_ASSIGNED;
  /// Propagation condition to be ignored (convenience)
  const Gecode::PropCond PC_BOOL_NONE = Gecode::PC_GEN_NONE;
  /**
   * \brief Propagate when a view becomes assigned (single value)
   *
   * If a propagator \a p depends on a view \a x with propagation
   * condition PC_BOOL_VAL, then \a p is propagated when a domain
   * update operation on \a x returns the modification event ME_BOOL_VAL.
   */
  const Gecode::PropCond PC_BOOL_VAL = Gecode::PC_GEN_ASSIGNED;
  //@}
}}
#endif
#ifdef GECODE_HAS_INT_VARS
namespace Gecode { namespace Int { 
  /// Configuration for Int-variable implementations
  class IntVarImpConf {
  public:
    /// Index for cloning
    static const int idx_c = 0;
    /// Index for disposal
    static const int idx_d = -1;
    /// Maximal propagation condition
    static const Gecode::PropCond pc_max = PC_INT_DOM;
    /// Freely available bits
    static const int free_bits = 0;
    /// Start of bits for modification event delta
    static const int med_fst = 0;
    /// End of bits for modification event delta
    static const int med_lst = med_fst + 2;
    /// Bitmask for modification event delta
    static const int med_mask = ((1 << 2) - 1) << med_fst;
    /// Combine modification events \a me1 and \a me2
    static Gecode::ModEvent me_combine(Gecode::ModEvent me1, Gecode::ModEvent me2);
    /// Update modification even delta \a med by \a me, return true on change
    static bool med_update(Gecode::ModEventDelta& med, Gecode::ModEvent me);
  };
}}
#else
namespace Gecode { namespace Int { 
  /// Dummy configuration for Int-variable implementations
  class IntVarImpConf {
  public:
    /// Index for cloning
    static const int idx_c = -1;
    /// Index for disposal
    static const int idx_d = -1;
    /// End of bits for modification event delta
    static const int med_lst = 0;
  };
}}
#endif
#ifdef GECODE_HAS_INT_VARS
namespace Gecode { namespace Int { 
  /// Configuration for Bool-variable implementations
  class BoolVarImpConf {
  public:
    /// Index for cloning
    static const int idx_c = Gecode::Int::IntVarImpConf::idx_c+1;
    /// Index for disposal
    static const int idx_d = Gecode::Int::IntVarImpConf::idx_d;
    /// Maximal propagation condition
    static const Gecode::PropCond pc_max = PC_BOOL_VAL;
    /// Freely available bits
    static const int free_bits = 2;
    /// Start of bits for modification event delta
    static const int med_fst = Gecode::Int::IntVarImpConf::med_lst;
    /// End of bits for modification event delta
    static const int med_lst = med_fst + 1;
    /// Bitmask for modification event delta
    static const int med_mask = ((1 << 1) - 1) << med_fst;
    /// Combine modification events \a me1 and \a me2
    static Gecode::ModEvent me_combine(Gecode::ModEvent me1, Gecode::ModEvent me2);
    /// Update modification even delta \a med by \a me, return true on change
    static bool med_update(Gecode::ModEventDelta& med, Gecode::ModEvent me);
  };
}}
#else
namespace Gecode { namespace Int { 
  /// Dummy configuration for Bool-variable implementations
  class BoolVarImpConf {
  public:
    /// Index for cloning
    static const int idx_c = Gecode::Int::IntVarImpConf::idx_c;
    /// Index for disposal
    static const int idx_d = Gecode::Int::IntVarImpConf::idx_d;
    /// End of bits for modification event delta
    static const int med_lst = Gecode::Int::IntVarImpConf::med_lst;
  };
}}
#endif

namespace Gecode {

  /// Configuration for all variable implementations
  class AllVarConf {
  public:
    /// Index for cloning
    static const int idx_c = Gecode::Int::BoolVarImpConf::idx_c+1;
    /// Index for dispose
    static const int idx_d = Gecode::Int::BoolVarImpConf::idx_d+1;
    /// Combine modification event delta \a med1 with \a med2
    static ModEventDelta med_combine(ModEventDelta med1, ModEventDelta med2);
  };

}

#ifdef GECODE_HAS_INT_VARS
namespace Gecode { namespace Int { 
  forceinline Gecode::ModEvent
  IntVarImpConf::me_combine(Gecode::ModEvent me1, Gecode::ModEvent me2) {
    static const Gecode::ModEvent me_c = (
      (
        (ME_INT_NONE <<  0) |  // [ME_INT_NONE][ME_INT_NONE]
        (ME_INT_VAL  <<  2) |  // [ME_INT_NONE][ME_INT_VAL ]
        (ME_INT_BND  <<  4) |  // [ME_INT_NONE][ME_INT_BND ]
        (ME_INT_DOM  <<  6)    // [ME_INT_NONE][ME_INT_DOM ]
      ) |
      (
        (ME_INT_VAL  <<  8) |  // [ME_INT_VAL ][ME_INT_NONE]
        (ME_INT_VAL  << 10) |  // [ME_INT_VAL ][ME_INT_VAL ]
        (ME_INT_VAL  << 12) |  // [ME_INT_VAL ][ME_INT_BND ]
        (ME_INT_VAL  << 14)    // [ME_INT_VAL ][ME_INT_DOM ]
      ) |
      (
        (ME_INT_BND  << 16) |  // [ME_INT_BND ][ME_INT_NONE]
        (ME_INT_VAL  << 18) |  // [ME_INT_BND ][ME_INT_VAL ]
        (ME_INT_BND  << 20) |  // [ME_INT_BND ][ME_INT_BND ]
        (ME_INT_BND  << 22)    // [ME_INT_BND ][ME_INT_DOM ]
      ) |
      (
        (ME_INT_DOM  << 24) |  // [ME_INT_DOM ][ME_INT_NONE]
        (ME_INT_VAL  << 26) |  // [ME_INT_DOM ][ME_INT_VAL ]
        (ME_INT_BND  << 28) |  // [ME_INT_DOM ][ME_INT_BND ]
        (ME_INT_DOM  << 30)    // [ME_INT_DOM ][ME_INT_DOM ]
      )
    );
    return ((me_c >> (me2 << 3)) >> (me1 << 1)) & 3;
  }
  forceinline bool
  IntVarImpConf::med_update(Gecode::ModEventDelta& med, Gecode::ModEvent me) {
    switch (me) {
    case ME_INT_NONE:
      return false;
    case ME_INT_VAL:
      {
        Gecode::ModEventDelta med_int = med & med_mask;
        if (med_int == (ME_INT_VAL << med_fst))
          return false;
        med ^= med_int;
        med ^= ME_INT_VAL << med_fst;
        break;
      }
    case ME_INT_BND:
      {
        static const Gecode::ModEvent me_c = (
          ((ME_INT_NONE ^ ME_INT_BND ) <<  0) |
          ((ME_INT_VAL  ^ ME_INT_VAL ) <<  4) |
          ((ME_INT_BND  ^ ME_INT_BND ) <<  8) |
          ((ME_INT_DOM  ^ ME_INT_BND ) << 12)
        );
        Gecode::ModEvent me_o = (med & med_mask) >> med_fst;
        Gecode::ModEvent me_n = (me_c >> (me_o << 2)) & (med_mask >> med_fst);
        if (me_n == 0)
          return false;
        med ^= me_n << med_fst;
        break;
      }
    case ME_INT_DOM:
      {
        Gecode::ModEventDelta med_int = med & med_mask;
        if (med_int != 0)
          return false;
        med |= ME_INT_DOM << med_fst;
        break;
      }
    default: GECODE_NEVER;
    }
    return true;
  }

}}
#endif
#ifdef GECODE_HAS_INT_VARS
namespace Gecode { namespace Int { 
  forceinline Gecode::ModEvent
  BoolVarImpConf::me_combine(Gecode::ModEvent me1, Gecode::ModEvent me2) {
    return me1 | me2;
  }
  forceinline bool
  BoolVarImpConf::med_update(Gecode::ModEventDelta& med, Gecode::ModEvent me) {
    switch (me) {
    case ME_BOOL_NONE:
      return false;
    case ME_BOOL_VAL:
      if ((med & (ME_BOOL_VAL << med_fst)) != 0)
        return false;
      med |= ME_BOOL_VAL << med_fst;
      break;
    default: GECODE_NEVER;
    }
    return true;
  }

}}
#endif
namespace Gecode {
  forceinline ModEventDelta
  AllVarConf::med_combine(ModEventDelta med1, ModEventDelta med2) {
#ifdef GECODE_HAS_INT_VARS
    (void) Gecode::Int::IntVarImpConf::med_update(med1,(med2 & Gecode::Int::IntVarImpConf::med_mask) >> Gecode::Int::IntVarImpConf::med_fst);
#endif
#ifdef GECODE_HAS_INT_VARS
    (void) Gecode::Int::BoolVarImpConf::med_update(med1,(med2 & Gecode::Int::BoolVarImpConf::med_mask) >> Gecode::Int::BoolVarImpConf::med_fst);
#endif
    return med1;
  }
}

// STATISTICS: kernel-var
