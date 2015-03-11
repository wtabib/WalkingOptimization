/*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mex.h"

#include "main.h"
#include "main2.h"

/*****************************************************************************/
/*****************************************************************************/

extern SIM sim;

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )
{
  if (nrhs != 1)
  {
    mexErrMsgTxt("One input required to run this function");
  }

  const int *dims = mxGetDimensions(prhs[0]);
  const int nDims = mxGetNumberOfDimensions(prhs[0]);
  const int rows = dims[0];
  const int cols = dims[1];

  PARAMETER *params;
  int n_parameters;
  int i;
  init_default_parameters( &sim );
  sim.rand_scale = 0;
  sim.controller_print = 1;
  printf("rows = %d, cols = %d\n", rows, cols);
  double * p = mxGetPr(prhs[0]);
  printf("prhs[1] = %g\n", p[1]);

  /* create a new file with the parameters in it */
  FILE *f = fopen( "p1", "w" );
  fprintf(f, "\nswing_hv1  %g opt end\n", p[0]);
  fprintf(f, "thrust1  %g opt end\n", p[1]);
  fprintf(f, "swing_hip_target  %g opt end\n", p[2]);
  fprintf(f, "swing_hv1 %g opt end\n", p[3]);
  fprintf(f, "swing_ha1 %g opt end\n", p[4]);
  fprintf(f, "swing_knee1 %g opt end\n", p[5]);
  fprintf(f, "swing_kv1 %g opt end\n", p[6]);
  fprintf(f, "swing_ka1 %g opt end\n", p[7]);
  fprintf(f, "swing_knee_target %g opt end\n", p[8]);
  fprintf(f, "swing_kv2 %g opt end\n", p[9]);
  fprintf(f, "swing_ka2 %g opt end\n", p[10]);
  fprintf(f, "stance_hip_target %g opt end\n", p[11]);
  fprintf(f, "stance_hv1 %g opt end\n", p[12]);
  fprintf(f, "stance_ha1 %g opt end\n", p[13]);
  fprintf(f, "pitch_d %g opt end\n", p[14]);
  fprintf(f, "stance_kv1 %g opt end\n", p[15]);
  fprintf(f, "stance_ka1 %g opt end\n", p[16]);
  fprintf(f, "stance_knee_target %g opt end\n", p[17]);
  fprintf(f, "stance_kv2 %g opt end\n", p[18]);
  fprintf(f, "stance_ka2 %g opt end\n", p[19]);
  fprintf(f, "stance_ankle_torque %g opt end\n", p[20]);
  fclose(f);


  params = read_parameter_file( "p1" );
  n_parameters = process_parameters( params, &sim, 1 );
  /*params->pointer = &((&sim)->swing_time); params = params->next;*/
  /*sim.swing_time = *((double *)mxGetData(prhs[0]));
  sim.thrust1 = *((double *)mxGetData(prhs[1]));
  sim.swing_hip_target = *((double *)mxGetData(prhs[2]));
  sim.swing_hv1 = *((double *)mxGetData(prhs[3]));
  sim.swing_ha1 = *((double *)mxGetData(prhs[4]));
  sim.swing_knee1 = *((double *)mxGetData(prhs[5]));
  sim.swing_kv1 = *((double *)mxGetData(prhs[6]));
  sim.swing_ka1 = *((double *)mxGetData(prhs[7]));
  sim.swing_knee_target = *((double *)mxGetData(prhs[8]));
  sim.swing_kv2 = *((double *)mxGetData(prhs[9]));
  sim.swing_ka2 = *((double *)mxGetData(prhs[10]));
  sim.stance_hip_target = *((double *)mxGetData(prhs[11]));
  sim.stance_hv1 = *((double *)mxGetData(prhs[12]));
  sim.stance_ha1 = *((double *)mxGetData(prhs[13]));
  sim.pitch_d = *((double *)mxGetData(prhs[14]));
  sim.stance_kv1 = *((double *)mxGetData(prhs[15]));
  sim.stance_ka1 = *((double *)mxGetData(prhs[16]));
  sim.stance_knee_target = *((double *)mxGetData(prhs[17]));
  sim.stance_kv2 = *((double *)mxGetData(prhs[18]));
  sim.stance_ka2 = *((double *)mxGetData(prhs[19]));
  sim.stance_ankle_torque = *((double *)mxGetData(prhs[20]));
  printf("sim.stance_ankle_torque = %g", sim.stance_ankle_torque);
  return;*/

  init_sim( &sim );
  init_data( &sim );

  for( i = 0; sim.time < sim.duration; i++ )
    {
      controller( &sim );
      save_data( &sim );
      if ( sim.status == CRASHED )
	break;
      integrate_one_time_step( &sim );
    }

  write_the_mrdplot_file( &sim );
  get_score( &sim );

  int dimsOut[2];
  dimsOut[0] = 1;
  dimsOut[1] = 1;
  plhs[0] = mxCreateNumericArray(2, dimsOut, mxDOUBLE_CLASS, mxREAL);
  double * data = (double *) mxGetData(plhs[0]);
  data[0] = get_score( &sim );

}

/*****************************************************************************/
