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

  params = read_parameter_file( "p0" );
  n_parameters = process_parameters( params, &sim, 1 );
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
  data[0] = 40;

}

/*****************************************************************************/
