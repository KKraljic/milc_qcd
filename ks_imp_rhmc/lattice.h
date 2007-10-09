#ifndef _LATTICE_H
#define _LATTICE_H
/****************************** lattice.h ********************************/

/* include file for MIMD version 7
   This file defines global scalars and the fields in the lattice.

   Directory for dynamical improved KS action.  Allow:
	arbitrary paths in quark action 
	arbitrary paths in gauge action (eg Symanzik imp.)

   If "FN" is defined,
     Includes storage for Naik improvement (longlink[4], templongvec[4],
     gen_pt[16], etc.
*/

#include "defines.h"
#include "../include/generic_quark_types.h"
#include "../include/generic_ks.h" /* For fn_links_t and ks_action_paths */
#include "../include/random.h"    /* For double_prn */
#include "../include/macros.h"    /* For MAXFILENAME */
#include "../include/io_lat.h"    /* For gauge_file */

/* Begin definition of site structure */

#include "../include/su3.h"
#include "../include/random.h"   /* For double_prn */

/* The lattice is an array of sites.  */
#define MOM_SITE   /* If there is a mom member of the site struct */
typedef struct {
    /* The first part is standard to all programs */
	/* coordinates of this site */
	short x,y,z,t;
	/* is it even or odd? */
	char parity;
	/* my index in the array */
	int index;
#ifdef SITERAND
	/* The state information for a random number generator */
	double_prn site_prn;
	/* align to double word boundary (kludge for Intel compiler) */
	int space1;
#endif

/* ------------------------------------------------------------ */
/*   Now come the physical fields, program dependent            */
/* ------------------------------------------------------------ */
	/* gauge field */
	su3_matrix link[4];	/* the fundamental field */

 	su3_matrix old_link[4];
	/* For accept/reject */

	/* antihermitian momentum matrices in each direction */
 	anti_hermitmat mom[4];

	/* The Kogut-Susskind phases, which have been absorbed into 
		the matrices.  Also the antiperiodic boundary conditions.  */
 	Real phase[4];

	/* 3 element complex vectors */
 	su3_vector phi[N_PSEUDO]; /* Gaussian random source, each pseudoferm */
 	su3_vector phi1;	/* Gaussian random source, each pseudoferm */
 	su3_vector phi2;	/* Gaussian random source, each pseudoferm */
 	su3_vector xxx1;	/* solution vector = Kinverse * phi, mass1 */
 	su3_vector xxx2;	/* solution vector = Kinverse * phi, mass2 */
 	su3_vector resid;	/* conjugate gradient residual vector */
 	su3_vector cg_p;	/* conjugate gradient change vector */
 	su3_vector ttt;		/* temporary vector, for K*ppp */
 	su3_vector g_rand;	/* Gaussian random vector*/
	/* Use trick of combining xxx=D^adj D)^(-1) on even sites with
	   Dslash times this on odd sites when computing fermion force */
	
#ifdef SPECTRUM
        su3_matrix tempmat1,staple;
	su3_vector propmat[3];	/* For three source colors */
	su3_vector propmat2[3];	/* nl_spectrum() */
	su3_matrix tempmat2;
	/* for spectrum_imp() */
	/**su3_vector quark_source, quark_prop, anti_prop;**/
#define quark_source propmat2[0]
#define quark_prop propmat2[1]
#define anti_prop propmat2[2]
#endif

	/* temporary vectors and matrices */
	su3_vector tempvec[4];	/* One for each direction */
#ifdef FN
	su3_vector templongvec[4];	/* One for each direction */
        su3_vector templongv1;
#endif
#ifdef DM_DU0
        su3_vector dMdu_x;      /* temp vector for <psi-bar(dM/du0)psi>
                                   calculation in 'f_meas.c' */
#endif
#ifdef NPBP_REPS
        su3_vector M_inv;       /* temp vector for M^{-1} g_rand */
#endif
#if defined(CHEM_POT) || defined(D_CHEM_POT)
        su3_vector dM_M_inv;    /* temp vector for dM/dmu M^{-1} g_rand */
        su3_vector deriv[6];
#endif
} site;

/* End definition of site structure */

/* Definition of globals */

#ifdef CONTROL
#define EXTERN 
#else
#define EXTERN extern
#endif

/* The following are global scalars 
   beta is overall gauge coupling factor
   mass, mass1 and mass2 are quark masses
   u0 is tadpole improvement factor, perhaps (plaq/3)^(1/4)
*/
EXTERN	int nx,ny,nz,nt;	/* lattice dimensions */
EXTERN  int volume;		/* volume of lattice = nx*ny*nz*nt */
#ifdef FIX_NODE_GEOM
EXTERN  int node_geometry[4];  /* Specifies fixed "nsquares" (i.e. 4D
			    hypercubes) for the compute nodes in each
			    coordinate direction.  Must be divisors of
			    the lattice dimensions */
#ifdef FIX_IONODE_GEOM
EXTERN int ionode_geometry[4]; /* Specifies fixed "nsquares" for I/O
			     partitions in each coordinate direction,
			     one I/O node for each square.  The I/O
			     node is at the origin of the square.
			     Must be divisors of the node_geometry. */
#endif
#endif
EXTERN	int iseed;		/* random number seed */
EXTERN	int warms,trajecs,steps,niter,nrestart,propinterval;
EXTERN  int niter_md[N_PSEUDO], niter_fa[N_PSEUDO], niter_gr[N_PSEUDO];
EXTERN  int prec_md[N_PSEUDO], prec_fa[N_PSEUDO], prec_gr[N_PSEUDO];
EXTERN  int npbp_reps_in;
EXTERN  int prec_ff;
EXTERN  int prec_pbp;  /* Precision of pbp measurements */
EXTERN	int nflavors1,nflavors2;  /* number of flavors of types 1 and 2 */
EXTERN	Real epsilon;
EXTERN  Real beta,u0;
EXTERN  Real mass1,mass2;
EXTERN	Real rsqmin_md[N_PSEUDO], rsqmin_fa[N_PSEUDO], rsqmin_gr[N_PSEUDO];
EXTERN  Real rsqprop;
EXTERN	int startflag;	/* beginning lattice: CONTINUE, RELOAD, RELOAD_BINARY,
			   RELOAD_CHECKPOINT, FRESH */
EXTERN	int saveflag;	/* do with lattice: FORGET, SAVE, SAVE_BINARY,
			   SAVE_CHECKPOINT */
EXTERN	char rparamfile[MAXFILENAME];
EXTERN	char startfile[MAXFILENAME],savefile[MAXFILENAME];
EXTERN  double g_ssplaq, g_stplaq;
EXTERN  double_complex linktrsum;
EXTERN  u_int32type nersc_checksum;
EXTERN  char stringLFN[MAXFILENAME];  /** ILDG LFN if applicable **/
EXTERN	int total_iters;
EXTERN  int phases_in; /* 1 if KS and BC phases absorbed into matrices */
EXTERN  int source_start, source_inc, n_sources;
        /* source time, increment for it, and number of source slices */
EXTERN  char spectrum_request[MAX_SPECTRUM_REQUEST]; /* request list for spectral measurements */
/* parameters for spectrum_multimom */
EXTERN  int spectrum_multimom_nmasses;
EXTERN  Real spectrum_multimom_low_mass;
EXTERN  Real spectrum_multimom_mass_step;
/* parameters for fpi */
EXTERN  int fpi_nmasses;
EXTERN  Real fpi_mass[MAX_FPI_NMASSES];

/* Some of these global variables are node dependent */
/* They are set in "make_lattice()" */
EXTERN	int sites_on_node;		/* number of sites on this node */
EXTERN	int even_sites_on_node;	/* number of even sites on this node */
EXTERN	int odd_sites_on_node;	/* number of odd sites on this node */
EXTERN	int number_of_nodes;	/* number of nodes in use */
EXTERN  int this_node;		/* node number of this node */

EXTERN gauge_file *startlat_p;

/* Each node maintains a structure with the pseudorandom number
   generator state */
EXTERN double_prn node_prn ;

/* The lattice is a single global variable - (actually this is the
   part of the lattice on this node) */
EXTERN site *lattice;

/* Vectors for addressing */
/* Generic pointers, for gather routines */
#define N_POINTERS 16
EXTERN char ** gen_pt[N_POINTERS];

/* Storage for definition of the quark action */
EXTERN fn_links_t        fn_links;
EXTERN ks_action_paths ks_act_paths;
EXTERN fn_links_t        fn_links_dmdu0;
EXTERN ks_action_paths ks_act_paths_dmdu0;

#include "params_rhmc.h"
EXTERN int n_pseudo;
EXTERN int max_rat_order;
EXTERN params_rhmc *rparam;

#endif /* _LATTICE_H */
