/* slarf.f -- translated by f2c (version 20061008).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#include "pnl/pnl_f2c.h"

/* Table of constant values */

static float c_b4 = 1.f;
static float c_b5 = 0.f;
static int c__1 = 1;

 int slarf_(char *side, int *m, int *n, float *v, 
	int *incv, float *tau, float *c__, int *ldc, float *work)
{
    /* System generated locals */
    int c_dim1, c_offset;
    float r__1;

    /* Local variables */
    int i__;
    int applyleft;
    extern  int sger_(int *, int *, float *, float *, 
	    int *, float *, int *, float *, int *);
    extern int lsame_(char *, char *);
    int lastc;
    extern  int sgemv_(char *, int *, int *, float *, 
	    float *, int *, float *, int *, float *, float *, int *);
    int lastv;
    extern int ilaslc_(int *, int *, float *, int *), ilaslr_(
	    int *, int *, float *, int *);


/*  -- LAPACK auxiliary routine (version 3.2) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley and NAG Ltd.. */
/*     November 2006 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  SLARF applies a float elementary reflector H to a float m by n matrix */
/*  C, from either the left or the right. H is represented in the form */

/*        H = I - tau * v * v' */

/*  where tau is a float scalar and v is a float vector. */

/*  If tau = 0, then H is taken to be the unit matrix. */

/*  Arguments */
/*  ========= */

/*  SIDE    (input) CHARACTER*1 */
/*          = 'L': form  H * C */
/*          = 'R': form  C * H */

/*  M       (input) INTEGER */
/*          The number of rows of the matrix C. */

/*  N       (input) INTEGER */
/*          The number of columns of the matrix C. */

/*  V       (input) REAL array, dimension */
/*                     (1 + (M-1)*ABS(INCV)) if SIDE = 'L' */
/*                  or (1 + (N-1)*ABS(INCV)) if SIDE = 'R' */
/*          The vector v in the representation of H. V is not used if */
/*          TAU = 0. */

/*  INCV    (input) INTEGER */
/*          The increment between elements of v. INCV <> 0. */

/*  TAU     (input) REAL */
/*          The value tau in the representation of H. */

/*  C       (input/output) REAL array, dimension (LDC,N) */
/*          On entry, the m by n matrix C. */
/*          On exit, C is overwritten by the matrix H * C if SIDE = 'L', */
/*          or C * H if SIDE = 'R'. */

/*  LDC     (input) INTEGER */
/*          The leading dimension of the array C. LDC >= MAX(1,M). */

/*  WORK    (workspace) REAL array, dimension */
/*                         (N) if SIDE = 'L' */
/*                      or (M) if SIDE = 'R' */

/*  ===================================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. Executable Statements .. */

    /* Parameter adjustments */
    --v;
    c_dim1 = *ldc;
    c_offset = 1 + c_dim1;
    c__ -= c_offset;
    --work;

    /* Function Body */
    applyleft = lsame_(side, "L");
    lastv = 0;
    lastc = 0;
    if (*tau != 0.f) {
/*     Set up variables for scanning V.  LASTV begins pointing to the end */
/*     of V. */
	if (applyleft) {
	    lastv = *m;
	} else {
	    lastv = *n;
	}
	if (*incv > 0) {
	    i__ = (lastv - 1) * *incv + 1;
	} else {
	    i__ = 1;
	}
/*     Look for the last non-zero row in V. */
	while(lastv > 0 && v[i__] == 0.f) {
	    --lastv;
	    i__ -= *incv;
	}
	if (applyleft) {
/*     Scan for the last non-zero column in C(1:lastv,:). */
	    lastc = ilaslc_(&lastv, n, &c__[c_offset], ldc);
	} else {
/*     Scan for the last non-zero row in C(:,1:lastv). */
	    lastc = ilaslr_(m, &lastv, &c__[c_offset], ldc);
	}
    }
/*     Note that lastc.eq.0 renders the BLAS operations null; no special */
/*     case is needed at this level. */
    if (applyleft) {

/*        Form  H * C */

	if (lastv > 0) {

/*           w(1:lastc,1) := C(1:lastv,1:lastc)' * v(1:lastv,1) */

	    sgemv_("Transpose", &lastv, &lastc, &c_b4, &c__[c_offset], ldc, &
		    v[1], incv, &c_b5, &work[1], &c__1);

/*           C(1:lastv,1:lastc) := C(...) - v(1:lastv,1) * w(1:lastc,1)' */

	    r__1 = -(*tau);
	    sger_(&lastv, &lastc, &r__1, &v[1], incv, &work[1], &c__1, &c__[
		    c_offset], ldc);
	}
    } else {

/*        Form  C * H */

	if (lastv > 0) {

/*           w(1:lastc,1) := C(1:lastc,1:lastv) * v(1:lastv,1) */

	    sgemv_("No transpose", &lastc, &lastv, &c_b4, &c__[c_offset], ldc, 
		     &v[1], incv, &c_b5, &work[1], &c__1);

/*           C(1:lastc,1:lastv) := C(...) - w(1:lastc,1) * v(1:lastv,1)' */

	    r__1 = -(*tau);
	    sger_(&lastc, &lastv, &r__1, &work[1], &c__1, &v[1], incv, &c__[
		    c_offset], ldc);
	}
    }
    return 0;

/*     End of SLARF */

} /* slarf_ */
