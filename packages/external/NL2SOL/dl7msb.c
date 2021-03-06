/* dl7msb.f -- translated by f2c (version 19970211).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"

/* Table of constant values */

static integer c_n1 = -1;
static integer c__1 = 1;
static logical c_true = TRUE_;

/* Subroutine */ int dl7msb_(b, d__, g, ierr, ipiv, ipiv1, ipiv2, ka, lmat, 
	lv, p, p0, pc, qtr, rmat, step, td, tg, v, w, wlm, x, x0)
doublereal *b, *d__, *g;
integer *ierr, *ipiv, *ipiv1, *ipiv2, *ka;
doublereal *lmat;
integer *lv, *p, *p0, *pc;
doublereal *qtr, *rmat, *step, *td, *tg, *v, *w, *wlm, *x, *x0;
{
    /* Initialized data */

    static doublereal one = 1.;
    static doublereal zero = 0.;

    /* System generated locals */
    integer step_dim1, step_offset, i__1;

    /* Local variables */
    static doublereal nred, pred;
    static integer i__, j, k, l, kinit, k0, p1;
    extern /* Subroutine */ int dd7mlp_(), ds7bqn_();
    extern doublereal dd7tpr_();
    extern /* Subroutine */ int dv7scp_(), dq7rsh_(), dl7mst_(), dv7ipr_(), 
	    dl7tvm_(), dv2axy_(), dv7cpy_();
    static integer kb, p10, p11;
    extern /* Subroutine */ int dv7vmp_();
    static integer ns;
    static doublereal ds0, rad;


/*  ***  COMPUTE HEURISTIC BOUNDED NEWTON STEP  *** */

/*     DIMENSION LMAT(P*(P+1)/2), RMAT(P*(P+1)/2), WLM(P*(P+5)/2 + 4) */


/*  ***  LOCAL VARIABLES  *** */


/*  ***  V SUBSCRIPTS  *** */


/* /6 */
/*     DATA DST0/3/, DSTNRM/2/, GTSTEP/4/, NREDUC/6/, PREDUC/7/, */
/*    1     RADIUS/8/ */
/* /7 */
/* / */
    /* Parameter adjustments */
    --lmat;
    --v;
    --x0;
    --x;
    --w;
    --tg;
    --td;
    step_dim1 = *p;
    step_offset = step_dim1 + 1;
    step -= step_offset;
    --qtr;
    --ipiv2;
    --ipiv1;
    --ipiv;
    --g;
    --d__;
    b -= 3;
    --rmat;
    --wlm;

    /* Function Body */

/* +++++++++++++++++++++++++++++++  BODY  ++++++++++++++++++++++++++++++++
 */

    p1 = *pc;
    if (*ka < 0) {
	goto L10;
    }
    nred = v[6];
    ds0 = v[3];
    goto L20;
L10:
    *p0 = 0;
    *ka = -1;

L20:
    kinit = -1;
    if (*p0 == p1) {
	kinit = *ka;
    }
    dv7cpy_(p, &x[1], &x0[1]);
    dv7cpy_(p, &td[1], &d__[1]);
/*     *** USE STEP(1,3) AS TEMP. COPY OF QTR *** */
    dv7cpy_(p, &step[step_dim1 * 3 + 1], &qtr[1]);
    dv7ipr_(p, &ipiv[1], &td[1]);
    pred = zero;
    rad = v[8];
    kb = -1;
    v[2] = zero;
    if (p1 > 0) {
	goto L30;
    }
    nred = zero;
    ds0 = zero;
    dv7scp_(p, &step[step_offset], &zero);
    goto L90;

L30:
    dv7vmp_(p, &tg[1], &g[1], &d__[1], &c_n1);
    dv7ipr_(p, &ipiv[1], &tg[1]);
    p10 = p1;
L40:
    k = kinit;
    kinit = -1;
    v[8] = rad - v[2];
    dv7vmp_(&p1, &tg[1], &tg[1], &td[1], &c__1);
    i__1 = p1;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L50: */
	ipiv1[i__] = i__;
    }
    k0 = max(0,k);
    dl7mst_(&td[1], &tg[1], ierr, &ipiv1[1], &k, &p1, &step[step_dim1 * 3 + 1]
	    , &rmat[1], &step[step_offset], &v[1], &wlm[1]);
    dv7vmp_(&p1, &tg[1], &tg[1], &td[1], &c_n1);
    *p0 = p1;
    if (*ka >= 0) {
	goto L60;
    }
    nred = v[6];
    ds0 = v[3];

L60:
    *ka = k;
    v[8] = rad;
    l = p1 + 5;
    if (k <= k0) {
	dd7mlp_(&p1, &lmat[1], &td[1], &rmat[1], &c_n1);
    }
    if (k > k0) {
	dd7mlp_(&p1, &lmat[1], &td[1], &wlm[l], &c_n1);
    }
    ds7bqn_(&b[3], &d__[1], &step[(step_dim1 << 1) + 1], &ipiv[1], &ipiv1[1], 
	    &ipiv2[1], &kb, &lmat[1], lv, &ns, p, &p1, &step[step_offset], &
	    td[1], &tg[1], &v[1], &w[1], &x[1], &x0[1]);
    pred += v[7];
    if (ns == 0) {
	goto L80;
    }
    *p0 = 0;

/*  ***  UPDATE RMAT AND QTR  *** */

    p11 = p1 + 1;
    l = p10 + p11;
    i__1 = p10;
    for (k = p11; k <= i__1; ++k) {
	j = l - k;
	i__ = ipiv2[j];
	if (i__ < j) {
	    dq7rsh_(&i__, &j, &c_true, &qtr[1], &rmat[1], &w[1]);
	}
/* L70: */
    }

L80:
    if (kb > 0) {
	goto L90;
    }

/*  ***  UPDATE LOCAL COPY OF QTR  *** */

    dv7vmp_(&p10, &w[1], &step[(step_dim1 << 1) + 1], &td[1], &c_n1);
    dl7tvm_(&p10, &w[1], &lmat[1], &w[1]);
    dv2axy_(&p10, &step[step_dim1 * 3 + 1], &one, &w[1], &qtr[1]);
    goto L40;

L90:
    v[3] = ds0;
    v[6] = nred;
    v[7] = pred;
    v[4] = dd7tpr_(p, &g[1], &step[step_offset]);

/* L999: */
    return 0;
/*  ***  LAST LINE OF DL7MSB FOLLOWS  *** */
} /* dl7msb_ */

