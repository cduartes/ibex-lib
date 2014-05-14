/* ============================================================================
 * I B E X - Set-membership operations
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : May 13, 2014
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_SET_MEMBERSHIP_H__
#define __IBEX_SET_MEMBERSHIP_H__

#include "ibex_IntervalMatrixArray.h"

namespace ibex {

// just for defining a hierarchy of types
class __Real {
public:
	typedef double SCALAR;
	typedef Vector VECTOR;
	typedef Matrix MATRIX;
	typedef MatrixArray MATRIX_ARRAY;
};

inline bool basic_is_subset(const Interval& x, const Interval& y) {
	return (y.lb()<=x.lb() && y.ub()>=x.ub());
}

inline bool basic_is_strict_subset(const Interval& x, const Interval& y) {
	return (y.lb()==NEG_INFINITY || y.lb()<x.lb()) &&
		   (y.ub()==POS_INFINITY || y.ub()>x.ub());
}

inline bool basic_is_superset(const Interval& x, const Interval& y) {
	return basic_is_subset(y,x);
}

inline bool basic_is_strict_superset(const Interval& x, const Interval& y) {
	return basic_is_strict_subset(y,x);
}

inline bool basic_contains(const Interval& x, double d) {
	return d>=x.lb() && d<=x.ub();
}

inline bool basic_strictly_contains(const Interval& x, double d) {
	return d>x.lb() && d<x.ub();
}

inline bool basic_intersects(const Interval& x, const Interval& y) {
	return x.lb()<=y.ub() && x.ub()>=y.lb();
}

inline bool basic_strictly_intersects(const Interval& x, const Interval& y) {
	return x.lb()<y.ub() && x.ub()>y.lb();
}

inline bool basic_is_disjoint(const Interval& x, const Interval& y) {
	return x.lb()>y.ub() || x.ub()<y.lb();
}

//inline bool basic_is_degenerated(const Interval& x) {
//	return x.lb()==x.ub();
//}
//
//inline bool basic_is_unbounded(const Interval& x) {
//	return x.lb()==NEG_INFINITY || x.ub()==POS_INFINITY;
//}

#define __IBEX_GENERATE_BASIC_SET_OP_AND__(S1,S2,BASIC_COND) \
  \
  inline bool BASIC_COND(const S1::VECTOR& x, const S2::VECTOR& y) { \
	assert(x.size()==y.size()); \
    for (int i=0; i<x.size(); i++) \
      if (!BASIC_COND(x[i],y[i])) return false; \
    return true; \
  } \
  \
  inline bool BASIC_COND(const S1::MATRIX& x, const S2::MATRIX& y) { \
	  assert(x.nb_rows()==y.nb_rows()); \
	  assert(x.nb_cols()==y.nb_cols()); \
	  for (int i=0; i<x.nb_rows(); i++) \
      for (int j=0; j<x.nb_cols(); j++) \
        if (!BASIC_COND(x[i][j],y[i][j])) return false; \
    return true; \
  } \
\
 inline bool BASIC_COND(const S1::MATRIX_ARRAY& x, const S2::MATRIX_ARRAY& y) { \
	  assert(x.size()==y.size()); \
	  assert(x.nb_rows()==y.nb_rows()); \
	  assert(x.nb_cols()==y.nb_cols()); \
	  for (int k=0; k<x.size(); k++) \
	  for (int i=0; i<x.nb_rows(); i++) \
        for (int j=0; j<x.nb_cols(); j++) \
          if (!BASIC_COND(x[i][j],y[i][j])) return false; \
   return true; \
 }

#define __IBEX_GENERATE_BASIC_SET_OP_OR__(S1,S2,BASIC_COND) \
  \
  inline bool BASIC_COND(const S1::VECTOR& x, const S2::VECTOR& y) { \
	assert(x.size()==y.size()); \
    for (int i=0; i<x.size(); i++) \
      if (BASIC_COND(x[i],y[i])) return true; \
    return false; \
  } \
  \
  inline bool BASIC_COND(const S1::MATRIX& x, const S2::MATRIX& y) { \
	  assert(x.nb_rows()==y.nb_rows()); \
	  assert(x.nb_cols()==y.nb_cols()); \
	  for (int i=0; i<x.nb_rows(); i++) \
      for (int j=0; j<x.nb_cols(); j++) \
        if (BASIC_COND(x[i][j],y[i][j])) return true; \
    return false; \
  } \
\
 inline bool BASIC_COND(const S1::MATRIX_ARRAY& x, const S2::MATRIX_ARRAY& y) { \
	  assert(x.size()==y.size()); \
	  assert(x.nb_rows()==y.nb_rows()); \
	  assert(x.nb_cols()==y.nb_cols()); \
	  for (int k=0; k<x.size(); k++) \
	  for (int i=0; i<x.nb_rows(); i++) \
        for (int j=0; j<x.nb_cols(); j++) \
          if (BASIC_COND(x[i][j],y[i][j])) return true; \
   return false; \
 }

__IBEX_GENERATE_BASIC_SET_OP_AND__(Interval,Interval,basic_is_subset)
__IBEX_GENERATE_BASIC_SET_OP_AND__(Interval,Interval,basic_is_strict_subset)
__IBEX_GENERATE_BASIC_SET_OP_AND__(Interval,__Real,  basic_contains)
__IBEX_GENERATE_BASIC_SET_OP_AND__(Interval,__Real,  basic_strictly_contains)
__IBEX_GENERATE_BASIC_SET_OP_AND__(Interval,Interval,basic_intersects)
__IBEX_GENERATE_BASIC_SET_OP_AND__(Interval,Interval,basic_strictly_intersects)

__IBEX_GENERATE_BASIC_SET_OP_OR__ (Interval,Interval,basic_is_disjoint)

template<typename T>
inline bool is_subset(const T& x, const T& y) {
	return x.is_empty() || (!y.is_empty() && basic_is_subset(x,y));
}

template<typename T>
inline bool is_strict_subset(const T& x, const T& y) {
	//return !y.is_empty() && (x.is_empty() || basic_is_strict_subset(x,y));
	return x.is_empty() || (!y.is_empty() && basic_is_strict_subset(x,y));
}

template<typename T>
inline bool is_superset(const T& x, const T& y) {
	return is_subset(y,x);
}

template<typename T>
inline bool is_strict_superset(const T& x, const T& y) {
	return is_strict_subset(y,x);
}

template<typename T,typename S>
inline bool contains(const T& x, const S& y) {
	return !x.is_empty() && basic_contains(x,y);
}

template<typename T,typename S>
inline bool strictly_contains(const T& x, const S& y) {
	return !x.is_empty() && basic_strictly_contains(x,y);
}

template<typename T,typename S>
inline bool intersects(const T& x, const S& y) {
	return !x.is_empty() && !y.is_empty() && basic_intersects(x,y);
}

template<typename T,typename S>
inline bool strictly_intersects(const T& x, const S& y) {
	return !x.is_empty() && !y.is_empty() && basic_strictly_intersects(x,y);
}

template<typename T,typename S>
inline bool is_disjoint(const T& x, const S& y) {
	return x.is_empty() || y.is_empty() || basic_is_disjoint(x,y);
}

#define __IBEX_GENERATE_SET_OP__(S2,COND) \
  \
  inline bool Interval::COND(const S2::SCALAR& x) const                  { return ibex::COND(*this,x); } \
  inline bool IntervalVector::COND(const S2::VECTOR& x) const            { return ibex::COND(*this,x); } \
  inline bool IntervalMatrix::COND(const S2::MATRIX& x) const            { return ibex::COND(*this,x); } \
  inline bool IntervalMatrixArray::COND(const S2::MATRIX_ARRAY& x) const { return ibex::COND(*this,x); }

__IBEX_GENERATE_SET_OP__(Interval,is_subset)
__IBEX_GENERATE_SET_OP__(Interval,is_strict_subset)
__IBEX_GENERATE_SET_OP__(Interval,is_superset)
__IBEX_GENERATE_SET_OP__(Interval,is_strict_superset)
__IBEX_GENERATE_SET_OP__(__Real,contains)
__IBEX_GENERATE_SET_OP__(__Real,strictly_contains)
__IBEX_GENERATE_SET_OP__(Interval,intersects)
__IBEX_GENERATE_SET_OP__(Interval,strictly_intersects)
__IBEX_GENERATE_SET_OP__(Interval,is_disjoint)


// ====================== Old FILIB wrappers =============================

//inline bool Interval::is_subset(const Interval& x) const {
//	return itv.subset(x.itv);
//}
//
///*inline bool Interval::is_strict_subset(const Interval& x) const {
//	return x.itv.interior(itv);
//}*/
//
//inline bool Interval::is_superset(const Interval& x) const {
//	return itv.superset(x.itv);
//}
//
//
//inline bool Interval::contains(double d) const {
//	return itv.contains(d);
//}
//
//inline bool Interval::strictly_contains(double d) const {
//	if (is_empty()) return false;
//		else return d>lb() && d<ub();
//}
//
//inline bool Interval::is_disjoint(const Interval &x) const {
//	return itv.disjoint(x.itv);
//}

// =======================================================================



// ====================== Old GAOL wrappers =============================

//inline bool Interval::is_subset(const Interval& x) const {
//	return x.itv.set_contains(itv);
//}
//
///* the semantic of goal does not match
//inline bool Interval::is_strict_subset(const Interval& x) const {
//	return x.itv.set_strictly_contains(itv);
//} */
//
//inline bool Interval::is_superset(const Interval& x) const {
//	return itv.set_contains(x.itv);
//}
//
///* the semantic of goal does not match
//inline bool Interval::is_strict_superset(const Interval& x) const {
//	return itv.set_strictly_contains(x.itv);
//} */
//
//inline bool Interval::contains(double d) const {
//	return itv.set_contains(d);
//}
//
//inline bool Interval::strictly_contains(double d) const {
//	return itv.set_strictly_contains(d);
//}
//
//inline bool Interval::is_disjoint(const Interval &x) const {
//	return itv.set_disjoint(x.itv);
//}
// =======================================================================



} // namespace ibex

#endif // __IBEX_SET_MEMBERSHIP_H__