/******** build_params.c *********/
/* MIMD version 6 */


#include "arb_dirac_inv_includes.h"

/* linear parameterization */
static Real l0[5]={
2.256756e+00, -1.122217e-01, -3.232121e-02, -1.445011e-02, -7.244655e-03
};
static Real r0[5]={
0.0,  -1.464761e-01, -3.297375e-02, -1.015422e-02, -3.518406e-03
};
static Real bl[5]={
-0.9863, 7.414769e-02, 2.715511e-02, 1.410759e-02, 7.689565e-03
};
static Real br[5]={
0.0,  1.300626e-01, 3.039772e-02, 9.629827e-03, 3.355035e-03
};

static Real u0[2]={0.802,0.844};

void build_params(Real mass_0)
{
int j;
Real cl1,cl2;
Real rn,as,d;
Real cpauli[4];
Real fpauli[4];
Real boost_term;
Real an,ax,rx;
Real c123,c132;

Real beta,u00;

Real rt,d1,d2,c40,c01,numer;


/* lambda and rho terms */
	for(j=1;j<5;j++) {
        lambda[j]= 0.0;
        rho[j]= 0.0;
	}
        lambda[0]= 4.0 + mass_0;
        lambda[1]=rho[1]= -0.5;


        for(j=0;j<5;j++)
		if(this_node==0) printf("lambda[%d] = %e\n",j,lambda[j]);
        for(j=1;j<5;j++)
		if(this_node==0) printf("rho[%d] = %e\n",j,rho[j]);

/* to renormalize to MILC conventions */
        clover_term = 0.5;



 /* boost clover term */ 
	boost_term=1.0;
        clover_term *= boost_term;
if(this_node==0)printf("boosting clover term by %e to %e\n",boost_term,
clover_term);

if(this_node==0)printf("clover normalization %e\n",clover_term);


} /* build_params */
