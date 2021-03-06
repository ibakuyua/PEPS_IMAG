/* sgeev.f -- translated by f2c (version 20061008).
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
static int c__0 = 0;
static int c_n1 = -1;

 int sgeev_(char *jobvl, char *jobvr, int *n, float *a, 
	int *lda, float *wr, float *wi, float *vl, int *ldvl, float *vr, 
	int *ldvr, float *work, int *lwork, int *info)
{
    /* System generated locals */
    int a_dim1, a_offset, vl_dim1, vl_offset, vr_dim1, vr_offset, i__1, 
	    i__2, i__3;
    float r__1, r__2;

    /* Builtin functions */
    double sqrt(double);

    /* Local variables */
    int i__, k;
    float r__, cs, sn;
    int ihi;
    float scl;
    int ilo;
    float dum[1], eps;
    int ibal;
    char side[1];
    float anrm;
    int ierr, itau, iwrk, nout;
    extern  int srot_(int *, float *, int *, float *, 
	    int *, float *, float *);
    extern double snrm2_(int *, float *, int *);
    extern int lsame_(char *, char *);
    extern  int sscal_(int *, float *, float *, int *);
    extern double slapy2_(float *, float *);
    extern  int slabad_(float *, float *);
    int scalea;
    float cscale;
    extern  int sgebak_(char *, char *, int *, int *, 
	    int *, float *, int *, float *, int *, int *), sgebal_(char *, int *, float *, int *, 
	    int *, int *, float *, int *);
    extern double slamch_(char *), slange_(char *, int *, 
	    int *, float *, int *, float *);
    extern  int sgehrd_(int *, int *, int *, float 
	    *, int *, float *, float *, int *, int *), xerbla_(char 
	    *, int *);
    extern int ilaenv_(int *, char *, char *, int *, int *, 
	    int *, int *);
    int select[1];
    float bignum;
    extern  int slascl_(char *, int *, int *, float *, 
	    float *, int *, int *, float *, int *, int *);
    extern int isamax_(int *, float *, int *);
    extern  int slacpy_(char *, int *, int *, float *, 
	    int *, float *, int *), slartg_(float *, float *, 
	    float *, float *, float *), sorghr_(int *, int *, int *, 
	    float *, int *, float *, float *, int *, int *), shseqr_(
	    char *, char *, int *, int *, int *, float *, int *
, float *, float *, float *, int *, float *, int *, int *), strevc_(char *, char *, int *, int *, 
	    float *, int *, float *, int *, float *, int *, int *
, int *, float *, int *);
    int minwrk, maxwrk;
    int wantvl;
    float smlnum;
    int hswork;
    int lquery, wantvr;


/*  -- LAPACK driver routine (version 3.2) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley and NAG Ltd.. */
/*     November 2006 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  SGEEV computes for an N-by-N float nonsymmetric matrix A, the */
/*  eigenvalues and, optionally, the left and/or right eigenvectors. */

/*  The right eigenvector v(j) of A satisfies */
/*                   A * v(j) = lambda(j) * v(j) */
/*  where lambda(j) is its eigenvalue. */
/*  The left eigenvector u(j) of A satisfies */
/*                u(j)**H * A = lambda(j) * u(j)**H */
/*  where u(j)**H denotes the conjugate transpose of u(j). */

/*  The computed eigenvectors are normalized to have Euclidean norm */
/*  equal to 1 and largest component float. */

/*  Arguments */
/*  ========= */

/*  JOBVL   (input) CHARACTER*1 */
/*          = 'N': left eigenvectors of A are not computed; */
/*          = 'V': left eigenvectors of A are computed. */

/*  JOBVR   (input) CHARACTER*1 */
/*          = 'N': right eigenvectors of A are not computed; */
/*          = 'V': right eigenvectors of A are computed. */

/*  N       (input) INTEGER */
/*          The order of the matrix A. N >= 0. */

/*  A       (input/output) REAL array, dimension (LDA,N) */
/*          On entry, the N-by-N matrix A. */
/*          On exit, A has been overwritten. */

/*  LDA     (input) INTEGER */
/*          The leading dimension of the array A.  LDA >= MAX(1,N). */

/*  WR      (output) REAL array, dimension (N) */
/*  WI      (output) REAL array, dimension (N) */
/*          WR and WI contain the float and imaginary parts, */
/*          respectively, of the computed eigenvalues.  Complex */
/*          conjugate pairs of eigenvalues appear consecutively */
/*          with the eigenvalue having the positive imaginary part */
/*          first. */

/*  VL      (output) REAL array, dimension (LDVL,N) */
/*          If JOBVL = 'V', the left eigenvectors u(j) are stored one */
/*          after another in the columns of VL, in the same order */
/*          as their eigenvalues. */
/*          If JOBVL = 'N', VL is not referenced. */
/*          If the j-th eigenvalue is float, then u(j) = VL(:,j), */
/*          the j-th column of VL. */
/*          If the j-th and (j+1)-st eigenvalues form a complex */
/*          conjugate pair, then u(j) = VL(:,j) + i*VL(:,j+1) and */
/*          u(j+1) = VL(:,j) - i*VL(:,j+1). */

/*  LDVL    (input) INTEGER */
/*          The leading dimension of the array VL.  LDVL >= 1; if */
/*          JOBVL = 'V', LDVL >= N. */

/*  VR      (output) REAL array, dimension (LDVR,N) */
/*          If JOBVR = 'V', the right eigenvectors v(j) are stored one */
/*          after another in the columns of VR, in the same order */
/*          as their eigenvalues. */
/*          If JOBVR = 'N', VR is not referenced. */
/*          If the j-th eigenvalue is float, then v(j) = VR(:,j), */
/*          the j-th column of VR. */
/*          If the j-th and (j+1)-st eigenvalues form a complex */
/*          conjugate pair, then v(j) = VR(:,j) + i*VR(:,j+1) and */
/*          v(j+1) = VR(:,j) - i*VR(:,j+1). */

/*  LDVR    (input) INTEGER */
/*          The leading dimension of the array VR.  LDVR >= 1; if */
/*          JOBVR = 'V', LDVR >= N. */

/*  WORK    (workspace/output) REAL array, dimension (MAX(1,LWORK)) */
/*          On exit, if INFO = 0, WORK(1) returns the optimal LWORK. */

/*  LWORK   (input) INTEGER */
/*          The dimension of the array WORK.  LWORK >= MAX(1,3*N), and */
/*          if JOBVL = 'V' or JOBVR = 'V', LWORK >= 4*N.  For good */
/*          performance, LWORK must generally be larger. */

/*          If LWORK = -1, then a workspace query is assumed; the routine */
/*          only calculates the optimal size of the WORK array, returns */
/*          this value as the first entry of the WORK array, and no error */
/*          message related to LWORK is issued by XERBLA. */

/*  INFO    (output) INTEGER */
/*          = 0:  successful exit */
/*          < 0:  if INFO = -i, the i-th argument had an illegal value. */
/*          > 0:  if INFO = i, the QR algorithm failed to compute all the */
/*                eigenvalues, and no eigenvectors have been computed; */
/*                elements i+1:N of WR and WI contain eigenvalues which */
/*                have converged. */

/*  ===================================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. Local Arrays .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Test the input arguments */

    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --wr;
    --wi;
    vl_dim1 = *ldvl;
    vl_offset = 1 + vl_dim1;
    vl -= vl_offset;
    vr_dim1 = *ldvr;
    vr_offset = 1 + vr_dim1;
    vr -= vr_offset;
    --work;

    /* Function Body */
    *info = 0;
    lquery = *lwork == -1;
    wantvl = lsame_(jobvl, "V");
    wantvr = lsame_(jobvr, "V");
    if (! wantvl && ! lsame_(jobvl, "N")) {
	*info = -1;
    } else if (! wantvr && ! lsame_(jobvr, "N")) {
	*info = -2;
    } else if (*n < 0) {
	*info = -3;
    } else if (*lda < MAX(1,*n)) {
	*info = -5;
    } else if (*ldvl < 1 || wantvl && *ldvl < *n) {
	*info = -9;
    } else if (*ldvr < 1 || wantvr && *ldvr < *n) {
	*info = -11;
    }

/*     Compute workspace */
/*      (Note: Comments in the code beginning "Workspace:" describe the */
/*       minimal amount of workspace needed at that point in the code, */
/*       as well as the preferred amount for good performance. */
/*       NB refers to the optimal block size for the immediately */
/*       following subroutine, as returned by ILAENV. */
/*       HSWORK refers to the workspace preferred by SHSEQR, as */
/*       calculated below. HSWORK is computed assuming ILO=1 and IHI=N, */
/*       the worst case.) */

    if (*info == 0) {
	if (*n == 0) {
	    minwrk = 1;
	    maxwrk = 1;
	} else {
	    maxwrk = (*n << 1) + *n * ilaenv_(&c__1, "SGEHRD", " ", n, &c__1, 
		    n, &c__0);
	    if (wantvl) {
		minwrk = *n << 2;
/* Computing MAX */
		i__1 = maxwrk, i__2 = (*n << 1) + (*n - 1) * ilaenv_(&c__1, 
			"SORGHR", " ", n, &c__1, n, &c_n1);
		maxwrk = MAX(i__1,i__2);
		shseqr_("S", "V", n, &c__1, n, &a[a_offset], lda, &wr[1], &wi[
			1], &vl[vl_offset], ldvl, &work[1], &c_n1, info);
		hswork = work[1];
/* Computing MAX */
		i__1 = maxwrk, i__2 = *n + 1, i__1 = MAX(i__1,i__2), i__2 = *
			n + hswork;
		maxwrk = MAX(i__1,i__2);
/* Computing MAX */
		i__1 = maxwrk, i__2 = *n << 2;
		maxwrk = MAX(i__1,i__2);
	    } else if (wantvr) {
		minwrk = *n << 2;
/* Computing MAX */
		i__1 = maxwrk, i__2 = (*n << 1) + (*n - 1) * ilaenv_(&c__1, 
			"SORGHR", " ", n, &c__1, n, &c_n1);
		maxwrk = MAX(i__1,i__2);
		shseqr_("S", "V", n, &c__1, n, &a[a_offset], lda, &wr[1], &wi[
			1], &vr[vr_offset], ldvr, &work[1], &c_n1, info);
		hswork = work[1];
/* Computing MAX */
		i__1 = maxwrk, i__2 = *n + 1, i__1 = MAX(i__1,i__2), i__2 = *
			n + hswork;
		maxwrk = MAX(i__1,i__2);
/* Computing MAX */
		i__1 = maxwrk, i__2 = *n << 2;
		maxwrk = MAX(i__1,i__2);
	    } else {
		minwrk = *n * 3;
		shseqr_("E", "N", n, &c__1, n, &a[a_offset], lda, &wr[1], &wi[
			1], &vr[vr_offset], ldvr, &work[1], &c_n1, info);
		hswork = work[1];
/* Computing MAX */
		i__1 = maxwrk, i__2 = *n + 1, i__1 = MAX(i__1,i__2), i__2 = *
			n + hswork;
		maxwrk = MAX(i__1,i__2);
	    }
	    maxwrk = MAX(maxwrk,minwrk);
	}
	work[1] = (float) maxwrk;

	if (*lwork < minwrk && ! lquery) {
	    *info = -13;
	}
    }

    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("SGEEV ", &i__1);
	return 0;
    } else if (lquery) {
	return 0;
    }

/*     Quick return if possible */

    if (*n == 0) {
	return 0;
    }

/*     Get machine constants */

    eps = slamch_("P");
    smlnum = slamch_("S");
    bignum = 1.f / smlnum;
    slabad_(&smlnum, &bignum);
    smlnum = sqrt(smlnum) / eps;
    bignum = 1.f / smlnum;

/*     Scale A if max element outside range [SMLNUM,BIGNUM] */

    anrm = slange_("M", n, n, &a[a_offset], lda, dum);
    scalea = FALSE;
    if (anrm > 0.f && anrm < smlnum) {
	scalea = TRUE;
	cscale = smlnum;
    } else if (anrm > bignum) {
	scalea = TRUE;
	cscale = bignum;
    }
    if (scalea) {
	slascl_("G", &c__0, &c__0, &anrm, &cscale, n, n, &a[a_offset], lda, &
		ierr);
    }

/*     Balance the matrix */
/*     (Workspace: need N) */

    ibal = 1;
    sgebal_("B", n, &a[a_offset], lda, &ilo, &ihi, &work[ibal], &ierr);

/*     Reduce to upper Hessenberg form */
/*     (Workspace: need 3*N, prefer 2*N+N*NB) */

    itau = ibal + *n;
    iwrk = itau + *n;
    i__1 = *lwork - iwrk + 1;
    sgehrd_(n, &ilo, &ihi, &a[a_offset], lda, &work[itau], &work[iwrk], &i__1, 
	     &ierr);

    if (wantvl) {

/*        Want left eigenvectors */
/*        Copy Householder vectors to VL */

	*(unsigned char *)side = 'L';
	slacpy_("L", n, n, &a[a_offset], lda, &vl[vl_offset], ldvl)
		;

/*        Generate orthogonal matrix in VL */
/*        (Workspace: need 3*N-1, prefer 2*N+(N-1)*NB) */

	i__1 = *lwork - iwrk + 1;
	sorghr_(n, &ilo, &ihi, &vl[vl_offset], ldvl, &work[itau], &work[iwrk], 
		 &i__1, &ierr);

/*        Perform QR iteration, accumulating Schur vectors in VL */
/*        (Workspace: need N+1, prefer N+HSWORK (see comments) ) */

	iwrk = itau;
	i__1 = *lwork - iwrk + 1;
	shseqr_("S", "V", n, &ilo, &ihi, &a[a_offset], lda, &wr[1], &wi[1], &
		vl[vl_offset], ldvl, &work[iwrk], &i__1, info);

	if (wantvr) {

/*           Want left and right eigenvectors */
/*           Copy Schur vectors to VR */

	    *(unsigned char *)side = 'B';
	    slacpy_("F", n, n, &vl[vl_offset], ldvl, &vr[vr_offset], ldvr);
	}

    } else if (wantvr) {

/*        Want right eigenvectors */
/*        Copy Householder vectors to VR */

	*(unsigned char *)side = 'R';
	slacpy_("L", n, n, &a[a_offset], lda, &vr[vr_offset], ldvr)
		;

/*        Generate orthogonal matrix in VR */
/*        (Workspace: need 3*N-1, prefer 2*N+(N-1)*NB) */

	i__1 = *lwork - iwrk + 1;
	sorghr_(n, &ilo, &ihi, &vr[vr_offset], ldvr, &work[itau], &work[iwrk], 
		 &i__1, &ierr);

/*        Perform QR iteration, accumulating Schur vectors in VR */
/*        (Workspace: need N+1, prefer N+HSWORK (see comments) ) */

	iwrk = itau;
	i__1 = *lwork - iwrk + 1;
	shseqr_("S", "V", n, &ilo, &ihi, &a[a_offset], lda, &wr[1], &wi[1], &
		vr[vr_offset], ldvr, &work[iwrk], &i__1, info);

    } else {

/*        Compute eigenvalues only */
/*        (Workspace: need N+1, prefer N+HSWORK (see comments) ) */

	iwrk = itau;
	i__1 = *lwork - iwrk + 1;
	shseqr_("E", "N", n, &ilo, &ihi, &a[a_offset], lda, &wr[1], &wi[1], &
		vr[vr_offset], ldvr, &work[iwrk], &i__1, info);
    }

/*     If INFO > 0 from SHSEQR, then quit */

    if (*info > 0) {
	goto L50;
    }

    if (wantvl || wantvr) {

/*        Compute left and/or right eigenvectors */
/*        (Workspace: need 4*N) */

	strevc_(side, "B", select, n, &a[a_offset], lda, &vl[vl_offset], ldvl, 
		 &vr[vr_offset], ldvr, n, &nout, &work[iwrk], &ierr);
    }

    if (wantvl) {

/*        Undo balancing of left eigenvectors */
/*        (Workspace: need N) */

	sgebak_("B", "L", n, &ilo, &ihi, &work[ibal], n, &vl[vl_offset], ldvl, 
		 &ierr);

/*        Normalize left eigenvectors and make largest component float */

	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    if (wi[i__] == 0.f) {
		scl = 1.f / snrm2_(n, &vl[i__ * vl_dim1 + 1], &c__1);
		sscal_(n, &scl, &vl[i__ * vl_dim1 + 1], &c__1);
	    } else if (wi[i__] > 0.f) {
		r__1 = snrm2_(n, &vl[i__ * vl_dim1 + 1], &c__1);
		r__2 = snrm2_(n, &vl[(i__ + 1) * vl_dim1 + 1], &c__1);
		scl = 1.f / slapy2_(&r__1, &r__2);
		sscal_(n, &scl, &vl[i__ * vl_dim1 + 1], &c__1);
		sscal_(n, &scl, &vl[(i__ + 1) * vl_dim1 + 1], &c__1);
		i__2 = *n;
		for (k = 1; k <= i__2; ++k) {
/* Computing 2nd power */
		    r__1 = vl[k + i__ * vl_dim1];
/* Computing 2nd power */
		    r__2 = vl[k + (i__ + 1) * vl_dim1];
		    work[iwrk + k - 1] = r__1 * r__1 + r__2 * r__2;
/* L10: */
		}
		k = isamax_(n, &work[iwrk], &c__1);
		slartg_(&vl[k + i__ * vl_dim1], &vl[k + (i__ + 1) * vl_dim1], 
			&cs, &sn, &r__);
		srot_(n, &vl[i__ * vl_dim1 + 1], &c__1, &vl[(i__ + 1) * 
			vl_dim1 + 1], &c__1, &cs, &sn);
		vl[k + (i__ + 1) * vl_dim1] = 0.f;
	    }
/* L20: */
	}
    }

    if (wantvr) {

/*        Undo balancing of right eigenvectors */
/*        (Workspace: need N) */

	sgebak_("B", "R", n, &ilo, &ihi, &work[ibal], n, &vr[vr_offset], ldvr, 
		 &ierr);

/*        Normalize right eigenvectors and make largest component float */

	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    if (wi[i__] == 0.f) {
		scl = 1.f / snrm2_(n, &vr[i__ * vr_dim1 + 1], &c__1);
		sscal_(n, &scl, &vr[i__ * vr_dim1 + 1], &c__1);
	    } else if (wi[i__] > 0.f) {
		r__1 = snrm2_(n, &vr[i__ * vr_dim1 + 1], &c__1);
		r__2 = snrm2_(n, &vr[(i__ + 1) * vr_dim1 + 1], &c__1);
		scl = 1.f / slapy2_(&r__1, &r__2);
		sscal_(n, &scl, &vr[i__ * vr_dim1 + 1], &c__1);
		sscal_(n, &scl, &vr[(i__ + 1) * vr_dim1 + 1], &c__1);
		i__2 = *n;
		for (k = 1; k <= i__2; ++k) {
/* Computing 2nd power */
		    r__1 = vr[k + i__ * vr_dim1];
/* Computing 2nd power */
		    r__2 = vr[k + (i__ + 1) * vr_dim1];
		    work[iwrk + k - 1] = r__1 * r__1 + r__2 * r__2;
/* L30: */
		}
		k = isamax_(n, &work[iwrk], &c__1);
		slartg_(&vr[k + i__ * vr_dim1], &vr[k + (i__ + 1) * vr_dim1], 
			&cs, &sn, &r__);
		srot_(n, &vr[i__ * vr_dim1 + 1], &c__1, &vr[(i__ + 1) * 
			vr_dim1 + 1], &c__1, &cs, &sn);
		vr[k + (i__ + 1) * vr_dim1] = 0.f;
	    }
/* L40: */
	}
    }

/*     Undo scaling if necessary */

L50:
    if (scalea) {
	i__1 = *n - *info;
/* Computing MAX */
	i__3 = *n - *info;
	i__2 = MAX(i__3,1);
	slascl_("G", &c__0, &c__0, &cscale, &anrm, &i__1, &c__1, &wr[*info + 
		1], &i__2, &ierr);
	i__1 = *n - *info;
/* Computing MAX */
	i__3 = *n - *info;
	i__2 = MAX(i__3,1);
	slascl_("G", &c__0, &c__0, &cscale, &anrm, &i__1, &c__1, &wi[*info + 
		1], &i__2, &ierr);
	if (*info > 0) {
	    i__1 = ilo - 1;
	    slascl_("G", &c__0, &c__0, &cscale, &anrm, &i__1, &c__1, &wr[1], 
		    n, &ierr);
	    i__1 = ilo - 1;
	    slascl_("G", &c__0, &c__0, &cscale, &anrm, &i__1, &c__1, &wi[1], 
		    n, &ierr);
	}
    }

    work[1] = (float) maxwrk;
    return 0;

/*     End of SGEEV */

} /* sgeev_ */
