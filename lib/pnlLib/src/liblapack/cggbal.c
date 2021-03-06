/* cggbal.f -- translated by f2c (version 20061008).
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

static int c__1 = 1;
static float c_b36 = 10.f;
static float c_b72 = .5f;

 int cggbal_(char *job, int *n, complex *a, int *lda, 
	complex *b, int *ldb, int *ilo, int *ihi, float *lscale, 
	float *rscale, float *work, int *info)
{
    /* System generated locals */
    int a_dim1, a_offset, b_dim1, b_offset, i__1, i__2, i__3, i__4;
    float r__1, r__2, r__3;

    /* Builtin functions */
    double r_lg10(float *), r_imag(complex *), c_abs(complex *), r_sign(float *,
	     float *), pow_ri(float *, int *);

    /* Local variables */
    int i__, j, k, l, m;
    float t;
    int jc;
    float ta, tb, tc;
    int ir;
    float ew;
    int it, nr, ip1, jp1, lm1;
    float cab, rab, ewc, cor, sum;
    int nrp2, icab, lcab;
    float beta, coef;
    int irab, lrab;
    float basl, cmax;
    extern double sdot_(int *, float *, int *, float *, int *);
    float coef2, coef5, gamma, alpha;
    extern int lsame_(char *, char *);
    extern  int sscal_(int *, float *, float *, int *);
    float sfmin;
    extern  int cswap_(int *, complex *, int *, 
	    complex *, int *);
    float sfmax;
    int iflow, kount;
    extern  int saxpy_(int *, float *, float *, int *, 
	    float *, int *);
    float pgamma;
    extern int icamax_(int *, complex *, int *);
    extern double slamch_(char *);
    extern  int csscal_(int *, float *, complex *, int 
	    *), xerbla_(char *, int *);
    int lsfmin, lsfmax;


/*  -- LAPACK routine (version 3.2) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley and NAG Ltd.. */
/*     November 2006 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  CGGBAL balances a pair of general complex matrices (A,B).  This */
/*  involves, first, permuting A and B by similarity transformations to */
/*  isolate eigenvalues in the first 1 to ILO$-$1 and last IHI+1 to N */
/*  elements on the diagonal; and second, applying a diagonal similarity */
/*  transformation to rows and columns ILO to IHI to make the rows */
/*  and columns as close in norm as possible. Both steps are optional. */

/*  Balancing may reduce the 1-norm of the matrices, and improve the */
/*  accuracy of the computed eigenvalues and/or eigenvectors in the */
/*  generalized eigenvalue problem A*x = lambda*B*x. */

/*  Arguments */
/*  ========= */

/*  JOB     (input) CHARACTER*1 */
/*          Specifies the operations to be performed on A and B: */
/*          = 'N':  none:  simply set ILO = 1, IHI = N, LSCALE(I) = 1.0 */
/*                  and RSCALE(I) = 1.0 for i=1,...,N; */
/*          = 'P':  permute only; */
/*          = 'S':  scale only; */
/*          = 'B':  both permute and scale. */

/*  N       (input) INTEGER */
/*          The order of the matrices A and B.  N >= 0. */

/*  A       (input/output) COMPLEX array, dimension (LDA,N) */
/*          On entry, the input matrix A. */
/*          On exit, A is overwritten by the balanced matrix. */
/*          If JOB = 'N', A is not referenced. */

/*  LDA     (input) INTEGER */
/*          The leading dimension of the array A. LDA >= MAX(1,N). */

/*  B       (input/output) COMPLEX array, dimension (LDB,N) */
/*          On entry, the input matrix B. */
/*          On exit, B is overwritten by the balanced matrix. */
/*          If JOB = 'N', B is not referenced. */

/*  LDB     (input) INTEGER */
/*          The leading dimension of the array B. LDB >= MAX(1,N). */

/*  ILO     (output) INTEGER */
/*  IHI     (output) INTEGER */
/*          ILO and IHI are set to ints such that on exit */
/*          A(i,j) = 0 and B(i,j) = 0 if i > j and */
/*          j = 1,...,ILO-1 or i = IHI+1,...,N. */
/*          If JOB = 'N' or 'S', ILO = 1 and IHI = N. */

/*  LSCALE  (output) REAL array, dimension (N) */
/*          Details of the permutations and scaling factors applied */
/*          to the left side of A and B.  If P(j) is the index of the */
/*          row interchanged with row j, and D(j) is the scaling factor */
/*          applied to row j, then */
/*            LSCALE(j) = P(j)    for J = 1,...,ILO-1 */
/*                      = D(j)    for J = ILO,...,IHI */
/*                      = P(j)    for J = IHI+1,...,N. */
/*          The order in which the interchanges are made is N to IHI+1, */
/*          then 1 to ILO-1. */

/*  RSCALE  (output) REAL array, dimension (N) */
/*          Details of the permutations and scaling factors applied */
/*          to the right side of A and B.  If P(j) is the index of the */
/*          column interchanged with column j, and D(j) is the scaling */
/*          factor applied to column j, then */
/*            RSCALE(j) = P(j)    for J = 1,...,ILO-1 */
/*                      = D(j)    for J = ILO,...,IHI */
/*                      = P(j)    for J = IHI+1,...,N. */
/*          The order in which the interchanges are made is N to IHI+1, */
/*          then 1 to ILO-1. */

/*  WORK    (workspace) REAL array, dimension (lwork) */
/*          lwork must be at least MAX(1,6*N) when JOB = 'S' or 'B', and */
/*          at least 1 when JOB = 'N' or 'P'. */

/*  INFO    (output) INTEGER */
/*          = 0:  successful exit */
/*          < 0:  if INFO = -i, the i-th argument had an illegal value. */

/*  Further Details */
/*  =============== */

/*  See R.C. WARD, Balancing the generalized eigenvalue problem, */
/*                 SIAM J. Sci. Stat. Comp. 2 (1981), 141-152. */

/*  ===================================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Statement Functions .. */
/*     .. */
/*     .. Statement Function definitions .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Test the input parameters */

    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    b_dim1 = *ldb;
    b_offset = 1 + b_dim1;
    b -= b_offset;
    --lscale;
    --rscale;
    --work;

    /* Function Body */
    *info = 0;
    if (! lsame_(job, "N") && ! lsame_(job, "P") && ! lsame_(job, "S") 
	    && ! lsame_(job, "B")) {
	*info = -1;
    } else if (*n < 0) {
	*info = -2;
    } else if (*lda < MAX(1,*n)) {
	*info = -4;
    } else if (*ldb < MAX(1,*n)) {
	*info = -6;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("CGGBAL", &i__1);
	return 0;
    }

/*     Quick return if possible */

    if (*n == 0) {
	*ilo = 1;
	*ihi = *n;
	return 0;
    }

    if (*n == 1) {
	*ilo = 1;
	*ihi = *n;
	lscale[1] = 1.f;
	rscale[1] = 1.f;
	return 0;
    }

    if (lsame_(job, "N")) {
	*ilo = 1;
	*ihi = *n;
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    lscale[i__] = 1.f;
	    rscale[i__] = 1.f;
/* L10: */
	}
	return 0;
    }

    k = 1;
    l = *n;
    if (lsame_(job, "S")) {
	goto L190;
    }

    goto L30;

/*     Permute the matrices A and B to isolate the eigenvalues. */

/*     Find row with one nonzero in columns 1 through L */

L20:
    l = lm1;
    if (l != 1) {
	goto L30;
    }

    rscale[1] = 1.f;
    lscale[1] = 1.f;
    goto L190;

L30:
    lm1 = l - 1;
    for (i__ = l; i__ >= 1; --i__) {
	i__1 = lm1;
	for (j = 1; j <= i__1; ++j) {
	    jp1 = j + 1;
	    i__2 = i__ + j * a_dim1;
	    i__3 = i__ + j * b_dim1;
	    if (a[i__2].r != 0.f || a[i__2].i != 0.f || (b[i__3].r != 0.f || 
		    b[i__3].i != 0.f)) {
		goto L50;
	    }
/* L40: */
	}
	j = l;
	goto L70;

L50:
	i__1 = l;
	for (j = jp1; j <= i__1; ++j) {
	    i__2 = i__ + j * a_dim1;
	    i__3 = i__ + j * b_dim1;
	    if (a[i__2].r != 0.f || a[i__2].i != 0.f || (b[i__3].r != 0.f || 
		    b[i__3].i != 0.f)) {
		goto L80;
	    }
/* L60: */
	}
	j = jp1 - 1;

L70:
	m = l;
	iflow = 1;
	goto L160;
L80:
	;
    }
    goto L100;

/*     Find column with one nonzero in rows K through N */

L90:
    ++k;

L100:
    i__1 = l;
    for (j = k; j <= i__1; ++j) {
	i__2 = lm1;
	for (i__ = k; i__ <= i__2; ++i__) {
	    ip1 = i__ + 1;
	    i__3 = i__ + j * a_dim1;
	    i__4 = i__ + j * b_dim1;
	    if (a[i__3].r != 0.f || a[i__3].i != 0.f || (b[i__4].r != 0.f || 
		    b[i__4].i != 0.f)) {
		goto L120;
	    }
/* L110: */
	}
	i__ = l;
	goto L140;
L120:
	i__2 = l;
	for (i__ = ip1; i__ <= i__2; ++i__) {
	    i__3 = i__ + j * a_dim1;
	    i__4 = i__ + j * b_dim1;
	    if (a[i__3].r != 0.f || a[i__3].i != 0.f || (b[i__4].r != 0.f || 
		    b[i__4].i != 0.f)) {
		goto L150;
	    }
/* L130: */
	}
	i__ = ip1 - 1;
L140:
	m = k;
	iflow = 2;
	goto L160;
L150:
	;
    }
    goto L190;

/*     Permute rows M and I */

L160:
    lscale[m] = (float) i__;
    if (i__ == m) {
	goto L170;
    }
    i__1 = *n - k + 1;
    cswap_(&i__1, &a[i__ + k * a_dim1], lda, &a[m + k * a_dim1], lda);
    i__1 = *n - k + 1;
    cswap_(&i__1, &b[i__ + k * b_dim1], ldb, &b[m + k * b_dim1], ldb);

/*     Permute columns M and J */

L170:
    rscale[m] = (float) j;
    if (j == m) {
	goto L180;
    }
    cswap_(&l, &a[j * a_dim1 + 1], &c__1, &a[m * a_dim1 + 1], &c__1);
    cswap_(&l, &b[j * b_dim1 + 1], &c__1, &b[m * b_dim1 + 1], &c__1);

L180:
    switch (iflow) {
	case 1:  goto L20;
	case 2:  goto L90;
    }

L190:
    *ilo = k;
    *ihi = l;

    if (lsame_(job, "P")) {
	i__1 = *ihi;
	for (i__ = *ilo; i__ <= i__1; ++i__) {
	    lscale[i__] = 1.f;
	    rscale[i__] = 1.f;
/* L195: */
	}
	return 0;
    }

    if (*ilo == *ihi) {
	return 0;
    }

/*     Balance the submatrix in rows ILO to IHI. */

    nr = *ihi - *ilo + 1;
    i__1 = *ihi;
    for (i__ = *ilo; i__ <= i__1; ++i__) {
	rscale[i__] = 0.f;
	lscale[i__] = 0.f;

	work[i__] = 0.f;
	work[i__ + *n] = 0.f;
	work[i__ + (*n << 1)] = 0.f;
	work[i__ + *n * 3] = 0.f;
	work[i__ + (*n << 2)] = 0.f;
	work[i__ + *n * 5] = 0.f;
/* L200: */
    }

/*     Compute right side vector in resulting linear equations */

    basl = r_lg10(&c_b36);
    i__1 = *ihi;
    for (i__ = *ilo; i__ <= i__1; ++i__) {
	i__2 = *ihi;
	for (j = *ilo; j <= i__2; ++j) {
	    i__3 = i__ + j * a_dim1;
	    if (a[i__3].r == 0.f && a[i__3].i == 0.f) {
		ta = 0.f;
		goto L210;
	    }
	    i__3 = i__ + j * a_dim1;
	    r__3 = (r__1 = a[i__3].r, ABS(r__1)) + (r__2 = r_imag(&a[i__ + j 
		    * a_dim1]), ABS(r__2));
	    ta = r_lg10(&r__3) / basl;

L210:
	    i__3 = i__ + j * b_dim1;
	    if (b[i__3].r == 0.f && b[i__3].i == 0.f) {
		tb = 0.f;
		goto L220;
	    }
	    i__3 = i__ + j * b_dim1;
	    r__3 = (r__1 = b[i__3].r, ABS(r__1)) + (r__2 = r_imag(&b[i__ + j 
		    * b_dim1]), ABS(r__2));
	    tb = r_lg10(&r__3) / basl;

L220:
	    work[i__ + (*n << 2)] = work[i__ + (*n << 2)] - ta - tb;
	    work[j + *n * 5] = work[j + *n * 5] - ta - tb;
/* L230: */
	}
/* L240: */
    }

    coef = 1.f / (float) (nr << 1);
    coef2 = coef * coef;
    coef5 = coef2 * .5f;
    nrp2 = nr + 2;
    beta = 0.f;
    it = 1;

/*     Start generalized conjugate gradient iteration */

L250:

    gamma = sdot_(&nr, &work[*ilo + (*n << 2)], &c__1, &work[*ilo + (*n << 2)]
, &c__1) + sdot_(&nr, &work[*ilo + *n * 5], &c__1, &work[*ilo + *
	    n * 5], &c__1);

    ew = 0.f;
    ewc = 0.f;
    i__1 = *ihi;
    for (i__ = *ilo; i__ <= i__1; ++i__) {
	ew += work[i__ + (*n << 2)];
	ewc += work[i__ + *n * 5];
/* L260: */
    }

/* Computing 2nd power */
    r__1 = ew;
/* Computing 2nd power */
    r__2 = ewc;
/* Computing 2nd power */
    r__3 = ew - ewc;
    gamma = coef * gamma - coef2 * (r__1 * r__1 + r__2 * r__2) - coef5 * (
	    r__3 * r__3);
    if (gamma == 0.f) {
	goto L350;
    }
    if (it != 1) {
	beta = gamma / pgamma;
    }
    t = coef5 * (ewc - ew * 3.f);
    tc = coef5 * (ew - ewc * 3.f);

    sscal_(&nr, &beta, &work[*ilo], &c__1);
    sscal_(&nr, &beta, &work[*ilo + *n], &c__1);

    saxpy_(&nr, &coef, &work[*ilo + (*n << 2)], &c__1, &work[*ilo + *n], &
	    c__1);
    saxpy_(&nr, &coef, &work[*ilo + *n * 5], &c__1, &work[*ilo], &c__1);

    i__1 = *ihi;
    for (i__ = *ilo; i__ <= i__1; ++i__) {
	work[i__] += tc;
	work[i__ + *n] += t;
/* L270: */
    }

/*     Apply matrix to vector */

    i__1 = *ihi;
    for (i__ = *ilo; i__ <= i__1; ++i__) {
	kount = 0;
	sum = 0.f;
	i__2 = *ihi;
	for (j = *ilo; j <= i__2; ++j) {
	    i__3 = i__ + j * a_dim1;
	    if (a[i__3].r == 0.f && a[i__3].i == 0.f) {
		goto L280;
	    }
	    ++kount;
	    sum += work[j];
L280:
	    i__3 = i__ + j * b_dim1;
	    if (b[i__3].r == 0.f && b[i__3].i == 0.f) {
		goto L290;
	    }
	    ++kount;
	    sum += work[j];
L290:
	    ;
	}
	work[i__ + (*n << 1)] = (float) kount * work[i__ + *n] + sum;
/* L300: */
    }

    i__1 = *ihi;
    for (j = *ilo; j <= i__1; ++j) {
	kount = 0;
	sum = 0.f;
	i__2 = *ihi;
	for (i__ = *ilo; i__ <= i__2; ++i__) {
	    i__3 = i__ + j * a_dim1;
	    if (a[i__3].r == 0.f && a[i__3].i == 0.f) {
		goto L310;
	    }
	    ++kount;
	    sum += work[i__ + *n];
L310:
	    i__3 = i__ + j * b_dim1;
	    if (b[i__3].r == 0.f && b[i__3].i == 0.f) {
		goto L320;
	    }
	    ++kount;
	    sum += work[i__ + *n];
L320:
	    ;
	}
	work[j + *n * 3] = (float) kount * work[j] + sum;
/* L330: */
    }

    sum = sdot_(&nr, &work[*ilo + *n], &c__1, &work[*ilo + (*n << 1)], &c__1) 
	    + sdot_(&nr, &work[*ilo], &c__1, &work[*ilo + *n * 3], &c__1);
    alpha = gamma / sum;

/*     Determine correction to current iteration */

    cmax = 0.f;
    i__1 = *ihi;
    for (i__ = *ilo; i__ <= i__1; ++i__) {
	cor = alpha * work[i__ + *n];
	if (ABS(cor) > cmax) {
	    cmax = ABS(cor);
	}
	lscale[i__] += cor;
	cor = alpha * work[i__];
	if (ABS(cor) > cmax) {
	    cmax = ABS(cor);
	}
	rscale[i__] += cor;
/* L340: */
    }
    if (cmax < .5f) {
	goto L350;
    }

    r__1 = -alpha;
    saxpy_(&nr, &r__1, &work[*ilo + (*n << 1)], &c__1, &work[*ilo + (*n << 2)]
, &c__1);
    r__1 = -alpha;
    saxpy_(&nr, &r__1, &work[*ilo + *n * 3], &c__1, &work[*ilo + *n * 5], &
	    c__1);

    pgamma = gamma;
    ++it;
    if (it <= nrp2) {
	goto L250;
    }

/*     End generalized conjugate gradient iteration */

L350:
    sfmin = slamch_("S");
    sfmax = 1.f / sfmin;
    lsfmin = (int) (r_lg10(&sfmin) / basl + 1.f);
    lsfmax = (int) (r_lg10(&sfmax) / basl);
    i__1 = *ihi;
    for (i__ = *ilo; i__ <= i__1; ++i__) {
	i__2 = *n - *ilo + 1;
	irab = icamax_(&i__2, &a[i__ + *ilo * a_dim1], lda);
	rab = c_abs(&a[i__ + (irab + *ilo - 1) * a_dim1]);
	i__2 = *n - *ilo + 1;
	irab = icamax_(&i__2, &b[i__ + *ilo * b_dim1], ldb);
/* Computing MAX */
	r__1 = rab, r__2 = c_abs(&b[i__ + (irab + *ilo - 1) * b_dim1]);
	rab = MAX(r__1,r__2);
	r__1 = rab + sfmin;
	lrab = (int) (r_lg10(&r__1) / basl + 1.f);
	ir = lscale[i__] + r_sign(&c_b72, &lscale[i__]);
/* Computing MIN */
	i__2 = MAX(ir,lsfmin), i__2 = MIN(i__2,lsfmax), i__3 = lsfmax - lrab;
	ir = MIN(i__2,i__3);
	lscale[i__] = pow_ri(&c_b36, &ir);
	icab = icamax_(ihi, &a[i__ * a_dim1 + 1], &c__1);
	cab = c_abs(&a[icab + i__ * a_dim1]);
	icab = icamax_(ihi, &b[i__ * b_dim1 + 1], &c__1);
/* Computing MAX */
	r__1 = cab, r__2 = c_abs(&b[icab + i__ * b_dim1]);
	cab = MAX(r__1,r__2);
	r__1 = cab + sfmin;
	lcab = (int) (r_lg10(&r__1) / basl + 1.f);
	jc = rscale[i__] + r_sign(&c_b72, &rscale[i__]);
/* Computing MIN */
	i__2 = MAX(jc,lsfmin), i__2 = MIN(i__2,lsfmax), i__3 = lsfmax - lcab;
	jc = MIN(i__2,i__3);
	rscale[i__] = pow_ri(&c_b36, &jc);
/* L360: */
    }

/*     Row scaling of matrices A and B */

    i__1 = *ihi;
    for (i__ = *ilo; i__ <= i__1; ++i__) {
	i__2 = *n - *ilo + 1;
	csscal_(&i__2, &lscale[i__], &a[i__ + *ilo * a_dim1], lda);
	i__2 = *n - *ilo + 1;
	csscal_(&i__2, &lscale[i__], &b[i__ + *ilo * b_dim1], ldb);
/* L370: */
    }

/*     Column scaling of matrices A and B */

    i__1 = *ihi;
    for (j = *ilo; j <= i__1; ++j) {
	csscal_(ihi, &rscale[j], &a[j * a_dim1 + 1], &c__1);
	csscal_(ihi, &rscale[j], &b[j * b_dim1 + 1], &c__1);
/* L380: */
    }

    return 0;

/*     End of CGGBAL */

} /* cggbal_ */
