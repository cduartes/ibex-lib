//============================================================================
//                                  I B E X                                   
// File        : ibex_NormalizedSystem.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Dec 20, 2013
// Last Update : Dec 20, 2013
//============================================================================

#ifndef __IBEX_NORMALIZED_SYSTEM_H__
#define __IBEX_NORMALIZED_SYSTEM_H__

#include "ibex_System.h"

namespace ibex {

/**
 * \ingroup system
 *
 * \brief Normalized system
 *
 * The normalization of a system is a copy where all inequalities
 * are under the form f_i(x)<=0. Thick equalities f(x)=[a,b] are
 * also transformed into two inequalities, f(x)<=b and -f(x)<=-a
 * (in this precise order).
 * Other equalities f=0 are transformed into f<=eps and -f<=eps,
 * where eps is the value given in 3rd argument of the constructor.
 * Note that the number of constraints of the resulting system
 * in this case is greater than the number or the original one.
 *
 * The index of a constraint in the original system can be retreived
 * via original_index(int).
 */
class NormalizedSystem : public System {
public:
	/**
	 * \brief Normalize \a sys.
	 *
	 * \param eps - Transforms an equation f=0 into two
	 *              inequalities: f<=eps and -f<=eps.
	 */
	NormalizedSystem(const System& sys, double eps=0);

	/**
	 * \brief Delete this.
	 */
	virtual ~NormalizedSystem();

	/**
	 * \brief Get the original index of a constraint.
	 */
	int original_index(int i) const;

protected:
	int* _orig_index;
};


/*============================================ inline implementation ============================================ */

inline int NormalizedSystem::original_index(int i) const {
	assert(i<nb_ctr);
	return _orig_index[i];
}

} // end namespace ibex
#endif // __IBEX_NORMALIZED_SYSTEM_H__
