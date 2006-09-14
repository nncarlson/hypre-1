/*BHEADER**********************************************************************
 * Copyright (c) 2006   The Regents of the University of California.
 * Produced at the Lawrence Livermore National Laboratory.
 * Written by the HYPRE team <hypre-users@llnl.gov>, UCRL-CODE-222953.
 * All rights reserved.
 *
 * This file is part of HYPRE (see http://www.llnl.gov/CASC/hypre/).
 * Please see the COPYRIGHT_and_LICENSE file for the copyright notice, 
 * disclaimer and the GNU Lesser General Public License.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License (as published by the Free
 * Software Foundation) version 2.1 dated February 1999.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the IMPLIED WARRANTY OF MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the terms and conditions of the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * $Revision$
 ***********************************************************************EHEADER*/


/******************************************************************************
 *
 * AMG transpose solve routines
 *
 *****************************************************************************/

#include "headers.h"
#include "par_amg.h"

/*--------------------------------------------------------------------
 * hypre_BoomerAMGSolveT
 *--------------------------------------------------------------------*/

int
hypre_BoomerAMGSolveT( void               *amg_vdata,
                   hypre_ParCSRMatrix *A,
                   hypre_ParVector    *f,
                   hypre_ParVector    *u         )
{

   MPI_Comm 	      comm = hypre_ParCSRMatrixComm(A);   

   hypre_ParAMGData   *amg_data = amg_vdata;

   /* Data Structure variables */

   int      amg_print_level;
   int      amg_logging;
   double  *num_coeffs;
   int     *num_variables;
   double   cycle_op_count;
   int      num_levels;
   /* int      num_unknowns; */
   double   tol;
   char    *file_name;
   hypre_ParCSRMatrix **A_array;
   hypre_ParVector    **F_array;
   hypre_ParVector    **U_array;

   /*  Local variables  */

   /*FILE    *fp;*/

   int      j;
   int      Solve_err_flag;
   int      min_iter;
   int      max_iter;
   int      cycle_count;
   double   total_coeffs;
   int      total_variables;
   int      num_procs, my_id;

   double   alpha = 1.0;
   double   beta = -1.0;
   double   cycle_cmplxty;
   double   operat_cmplxty;
   double   grid_cmplxty;
   double   conv_factor;
   double   resid_nrm;
   double   resid_nrm_init;
   double   relative_resid;
   double   rhs_norm;
   double   old_resid;

   hypre_ParVector  *Vtemp;
   hypre_ParVector  *Residual;

   MPI_Comm_size(comm, &num_procs);   
   MPI_Comm_rank(comm,&my_id);

   amg_print_level = hypre_ParAMGDataPrintLevel(amg_data);
   amg_logging   = hypre_ParAMGDataLogging(amg_data);
   if ( amg_logging>1 )
      Residual = hypre_ParAMGDataResidual(amg_data);
   file_name     = hypre_ParAMGDataLogFileName(amg_data);
   /* num_unknowns  = hypre_ParAMGDataNumUnknowns(amg_data); */
   num_levels    = hypre_ParAMGDataNumLevels(amg_data);
   A_array       = hypre_ParAMGDataAArray(amg_data);
   F_array       = hypre_ParAMGDataFArray(amg_data);
   U_array       = hypre_ParAMGDataUArray(amg_data);

   tol           = hypre_ParAMGDataTol(amg_data);
   min_iter      = hypre_ParAMGDataMinIter(amg_data);
   max_iter      = hypre_ParAMGDataMaxIter(amg_data);

   num_coeffs = hypre_CTAlloc(double, num_levels);
   num_variables = hypre_CTAlloc(int, num_levels);
   num_coeffs[0]    = hypre_ParCSRMatrixDNumNonzeros(A_array[0]);
   num_variables[0] = hypre_ParCSRMatrixGlobalNumRows(A_array[0]);
 
   A_array[0] = A;
   F_array[0] = f;
   U_array[0] = u;

/*   Vtemp = hypre_ParVectorCreate(hypre_ParCSRMatrixComm(A_array[0]),
                                 hypre_ParCSRMatrixGlobalNumRows(A_array[0]),
                                 hypre_ParCSRMatrixRowStarts(A_array[0]));
   hypre_ParVectorInitialize(Vtemp);
   hypre_ParVectorSetPartitioningOwner(Vtemp,0);
   hypre_ParAMGDataVtemp(amg_data) = Vtemp;
*/
   Vtemp = hypre_ParAMGDataVtemp(amg_data);
   for (j = 1; j < num_levels; j++)
   {
      num_coeffs[j]    = hypre_ParCSRMatrixDNumNonzeros(A_array[j]);
      num_variables[j] = hypre_ParCSRMatrixGlobalNumRows(A_array[j]);
   }

   /*-----------------------------------------------------------------------
    *    Write the solver parameters
    *-----------------------------------------------------------------------*/


   if (my_id == 0 && amg_print_level > 1)
      hypre_BoomerAMGWriteSolverParams(amg_data); 



   /*-----------------------------------------------------------------------
    *    Initialize the solver error flag and assorted bookkeeping variables
    *-----------------------------------------------------------------------*/

   Solve_err_flag = 0;

   total_coeffs = 0;
   total_variables = 0;
   cycle_count = 0;
   operat_cmplxty = 0;
   grid_cmplxty = 0;

   /*-----------------------------------------------------------------------
    *     open the log file and write some initial info
    *-----------------------------------------------------------------------*/

   if (my_id == 0 && amg_print_level > 1)
   { 
      /*fp = fopen(file_name, "a");*/

      printf("\n\nAMG SOLUTION INFO:\n");

   }

   /*-----------------------------------------------------------------------
    *    Compute initial fine-grid residual and print to logfile
    *-----------------------------------------------------------------------*/

   if ( amg_logging > 1 ) {
      hypre_ParVectorCopy(F_array[0], Residual );
      hypre_ParCSRMatrixMatvecT(alpha, A_array[0], U_array[0], beta, Residual );
      resid_nrm = sqrt(hypre_ParVectorInnerProd( Residual, Residual ));
   }
   else {
      hypre_ParVectorCopy(F_array[0], Vtemp);
      hypre_ParCSRMatrixMatvecT(alpha, A_array[0], U_array[0], beta, Vtemp);
      resid_nrm = sqrt(hypre_ParVectorInnerProd(Vtemp, Vtemp));
   }


   resid_nrm_init = resid_nrm;
   rhs_norm = sqrt(hypre_ParVectorInnerProd(f, f));
   relative_resid = 9999;
   if (rhs_norm)
   {
      relative_resid = resid_nrm_init / rhs_norm;
   }

   if (my_id ==0 && (amg_print_level > 1))
   {     
      printf("                                            relative\n");
      printf("               residual        factor       residual\n");
      printf("               --------        ------       --------\n");
      printf("    Initial    %e                 %e\n",resid_nrm_init,
              relative_resid);
   }

   /*-----------------------------------------------------------------------
    *    Main V-cycle loop
    *-----------------------------------------------------------------------*/
   
   while ((relative_resid >= tol || cycle_count < min_iter)
          && cycle_count < max_iter 
          && Solve_err_flag == 0)
   {
      hypre_ParAMGDataCycleOpCount(amg_data) = 0;   
      /* Op count only needed for one cycle */

      Solve_err_flag = hypre_BoomerAMGCycleT(amg_data, F_array, U_array); 

      old_resid = resid_nrm;

      /*---------------------------------------------------------------
       *    Compute  fine-grid residual and residual norm
       *----------------------------------------------------------------*/

      if ( amg_logging > 1 ) {
         hypre_ParVectorCopy(F_array[0], Residual );
         hypre_ParCSRMatrixMatvecT(alpha, A_array[0], U_array[0], beta, Residual );
         resid_nrm = sqrt(hypre_ParVectorInnerProd( Residual, Residual ));
      }
      else {
         hypre_ParVectorCopy(F_array[0], Vtemp);
         hypre_ParCSRMatrixMatvecT(alpha, A_array[0], U_array[0], beta, Vtemp);
         resid_nrm = sqrt(hypre_ParVectorInnerProd(Vtemp, Vtemp));
      }

      conv_factor = resid_nrm / old_resid;
      relative_resid = 9999;
      if (rhs_norm)
      {
         relative_resid = resid_nrm / rhs_norm;
      }

      ++cycle_count;

      if (my_id == 0 && (amg_print_level > 1))
      { 
         printf("    Cycle %2d   %e    %f     %e \n", cycle_count,
                 resid_nrm, conv_factor, relative_resid);
      }
   }

   if (cycle_count == max_iter) Solve_err_flag = 1;

   /*-----------------------------------------------------------------------
    *    Compute closing statistics
    *-----------------------------------------------------------------------*/

   conv_factor = pow((resid_nrm/resid_nrm_init),(1.0/((double) cycle_count)));


   for (j=0;j<hypre_ParAMGDataNumLevels(amg_data);j++)
   {
      total_coeffs += num_coeffs[j];
      total_variables += num_variables[j];
   }

   cycle_op_count = hypre_ParAMGDataCycleOpCount(amg_data);

   if (num_variables[0])
      grid_cmplxty = ((double) total_variables) / ((double) num_variables[0]);
   if (num_coeffs[0])
   {
      operat_cmplxty = total_coeffs / num_coeffs[0];
      cycle_cmplxty = cycle_op_count / num_coeffs[0];
   }

   if (my_id == 0 && amg_print_level > 1)
   {
      if (Solve_err_flag == 1)
      {
         printf("\n\n==============================================");
         printf("\n NOTE: Convergence tolerance was not achieved\n");
         printf("      within the allowed %d V-cycles\n",max_iter);
         printf("==============================================");
      }
      printf("\n\n Average Convergence Factor = %f",conv_factor);
      printf("\n\n     Complexity:    grid = %f\n",grid_cmplxty);
      printf("                operator = %f\n",operat_cmplxty);
      printf("                   cycle = %f\n\n",cycle_cmplxty);
   }

   /*----------------------------------------------------------
    * Close the output file (if open)
    *----------------------------------------------------------*/

   /*if (my_id == 0 && amg_print_level >= 1)
   { 
      fclose(fp); 
   }*/

   hypre_TFree(num_coeffs);
   hypre_TFree(num_variables);

   return(Solve_err_flag);
}

/******************************************************************************
 *
 * ParAMG cycling routine
 *
 *****************************************************************************/

/*--------------------------------------------------------------------------
 * hypre_BoomerAMGCycleT
 *--------------------------------------------------------------------------*/

int
hypre_BoomerAMGCycleT( void              *amg_vdata, 
                   hypre_ParVector  **F_array,
                   hypre_ParVector  **U_array   )
{
   hypre_ParAMGData *amg_data = amg_vdata;

   /* Data Structure variables */

   hypre_ParCSRMatrix    **A_array;
   hypre_ParCSRMatrix    **P_array;
   hypre_ParCSRMatrix    **R_array;
   hypre_ParVector    *Vtemp;

   int     **CF_marker_array;
   /* int     **unknown_map_array; */
   /* int     **point_map_array; */
   /* int     **v_at_point_array; */

   double    cycle_op_count;   
   int       cycle_type;
   int       num_levels;
   /* int       num_unknowns; */

   double   *num_coeffs;
   int      *num_grid_sweeps;   
   int      *grid_relax_type;   
   int     **grid_relax_points;  
 
   /* Local variables  */

   int      *lev_counter;
   int       Solve_err_flag;
   int       k;
   int       j;
   int       level;
   int       cycle_param;
   int       coarse_grid;
   int       fine_grid;
   int       Not_Finished;
   int       num_sweep;
   int       relax_type;
   int       relax_points;
   double   *relax_weight;

   double    alpha;
   double    beta;
#if 0
   double   *D_mat;
   double   *S_vec;
#endif
   
   /* Acquire data and allocate storage */

   A_array           = hypre_ParAMGDataAArray(amg_data);
   P_array           = hypre_ParAMGDataPArray(amg_data);
   R_array           = hypre_ParAMGDataRArray(amg_data);
   CF_marker_array   = hypre_ParAMGDataCFMarkerArray(amg_data);
   /* unknown_map_array = hypre_ParAMGDataUnknownMapArray(amg_data); */
   /* point_map_array   = hypre_ParAMGDataPointMapArray(amg_data); */
   /* v_at_point_array  = hypre_ParAMGDataVatPointArray(amg_data); */
   Vtemp             = hypre_ParAMGDataVtemp(amg_data);
   num_levels        = hypre_ParAMGDataNumLevels(amg_data);
   cycle_type        = hypre_ParAMGDataCycleType(amg_data);
   /* num_unknowns      =  hypre_ParCSRMatrixNumRows(A_array[0]); */

   num_grid_sweeps     = hypre_ParAMGDataNumGridSweeps(amg_data);
   grid_relax_type     = hypre_ParAMGDataGridRelaxType(amg_data);
   grid_relax_points   = hypre_ParAMGDataGridRelaxPoints(amg_data);
   relax_weight        = hypre_ParAMGDataRelaxWeight(amg_data); 

   cycle_op_count = hypre_ParAMGDataCycleOpCount(amg_data);

   lev_counter = hypre_CTAlloc(int, num_levels);

   /* Initialize */

   Solve_err_flag = 0;

   num_coeffs = hypre_CTAlloc(double, num_levels);
   num_coeffs[0]    = hypre_ParCSRMatrixDNumNonzeros(A_array[0]);

   for (j = 1; j < num_levels; j++)
      num_coeffs[j] = hypre_ParCSRMatrixDNumNonzeros(A_array[j]);

   /*---------------------------------------------------------------------
    *    Initialize cycling control counter
    *
    *     Cycling is controlled using a level counter: lev_counter[k]
    *     
    *     Each time relaxation is performed on level k, the
    *     counter is decremented by 1. If the counter is then
    *     negative, we go to the next finer level. If non-
    *     negative, we go to the next coarser level. The
    *     following actions control cycling:
    *     
    *     a. lev_counter[0] is initialized to 1.
    *     b. lev_counter[k] is initialized to cycle_type for k>0.
    *     
    *     c. During cycling, when going down to level k, lev_counter[k]
    *        is set to the max of (lev_counter[k],cycle_type)
    *---------------------------------------------------------------------*/

   Not_Finished = 1;

   lev_counter[0] = 1;
   for (k = 1; k < num_levels; ++k) 
   {
      lev_counter[k] = cycle_type;
   }

   level = 0;
   cycle_param = 0;

   /*---------------------------------------------------------------------
    * Main loop of cycling
    *--------------------------------------------------------------------*/
  
   while (Not_Finished)
   {
      num_sweep = num_grid_sweeps[cycle_param];
      relax_type = grid_relax_type[cycle_param];

      /*------------------------------------------------------------------
       * Do the relaxation num_sweep times
       *-----------------------------------------------------------------*/

      for (j = 0; j < num_sweep; j++)
      {
         relax_points =   grid_relax_points[cycle_param][j];

         /*-----------------------------------------------
          * VERY sloppy approximation to cycle complexity
          *-----------------------------------------------*/

         if (level < num_levels -1)
         {
            switch (relax_points)
            {
               case 1:
               cycle_op_count += num_coeffs[level+1];
               break;
  
               case -1: 
               cycle_op_count += (num_coeffs[level]-num_coeffs[level+1]); 
               break;
            }
         }
	 else
         {
            cycle_op_count += num_coeffs[level]; 
         }


         Solve_err_flag = hypre_BoomerAMGRelaxT(A_array[level], 
                                            F_array[level],
                                            CF_marker_array[level],
                                            relax_type,
                                            relax_points,
                                            relax_weight[level],
                                            U_array[level],
                                            Vtemp);

 
         if (Solve_err_flag != 0)
            return(Solve_err_flag);
      }


      /*------------------------------------------------------------------
       * Decrement the control counter and determine which grid to visit next
       *-----------------------------------------------------------------*/

      --lev_counter[level];
       
      if (lev_counter[level] >= 0 && level != num_levels-1)
      {
                               
         /*---------------------------------------------------------------
          * Visit coarser level next.  Compute residual using hypre_ParCSRMatrixMatvec.
          * Use interpolation (since transpose i.e. P^TATR instead of
          * RAP) using hypre_ParCSRMatrixMatvecT.
          * Reset counters and cycling parameters for coarse level
          *--------------------------------------------------------------*/

         fine_grid = level;
         coarse_grid = level + 1;

         hypre_ParVectorSetConstantValues(U_array[coarse_grid], 0.0);
          
         hypre_ParVectorCopy(F_array[fine_grid],Vtemp);
         alpha = -1.0;
         beta = 1.0;
         hypre_ParCSRMatrixMatvecT(alpha, A_array[fine_grid], U_array[fine_grid],
                         beta, Vtemp);

         alpha = 1.0;
         beta = 0.0;

         hypre_ParCSRMatrixMatvecT(alpha,P_array[fine_grid],Vtemp,
                          beta,F_array[coarse_grid]);

         ++level;
         lev_counter[level] = hypre_max(lev_counter[level],cycle_type);
         cycle_param = 1;
         if (level == num_levels-1) cycle_param = 3;
      }

      else if (level != 0)
      {
                            
         /*---------------------------------------------------------------
          * Visit finer level next.
          * Use restriction (since transpose i.e. P^TA^TR instead of RAP)
          * and add correction using hypre_ParCSRMatrixMatvec.
          * Reset counters and cycling parameters for finer level.
          *--------------------------------------------------------------*/

         fine_grid = level - 1;
         coarse_grid = level;
         alpha = 1.0;
         beta = 1.0;

         hypre_ParCSRMatrixMatvec(alpha, R_array[fine_grid], U_array[coarse_grid],
                         beta, U_array[fine_grid]);            
 
         --level;
         cycle_param = 2;
         if (level == 0) cycle_param = 0;
      }
      else
      {
         Not_Finished = 0;
      }
   }

   hypre_ParAMGDataCycleOpCount(amg_data) = cycle_op_count;

   hypre_TFree(lev_counter);
   hypre_TFree(num_coeffs);

   return(Solve_err_flag);
}

/******************************************************************************
 *
 * Relaxation scheme
 *
 *****************************************************************************/

/*--------------------------------------------------------------------------
 * hypre_BoomerAMGRelaxT
 *--------------------------------------------------------------------------*/

int  hypre_BoomerAMGRelaxT( hypre_ParCSRMatrix *A,
                        hypre_ParVector    *f,
                        int                *cf_marker,
                        int                 relax_type,
                        int                 relax_points,
                        double              relax_weight,
                        hypre_ParVector    *u,
                        hypre_ParVector    *Vtemp )
{
   hypre_CSRMatrix *A_diag = hypre_ParCSRMatrixDiag(A);
   double         *A_diag_data  = hypre_CSRMatrixData(A_diag);
   int            *A_diag_i     = hypre_CSRMatrixI(A_diag);

   int             n_global= hypre_ParCSRMatrixGlobalNumRows(A);
   int             n       = hypre_CSRMatrixNumRows(A_diag);
   int	      	   first_index = hypre_ParVectorFirstIndex(u);
   
   hypre_Vector   *u_local = hypre_ParVectorLocalVector(u);
   double         *u_data  = hypre_VectorData(u_local);

   hypre_Vector   *Vtemp_local = hypre_ParVectorLocalVector(Vtemp);
   double         *Vtemp_data = hypre_VectorData(Vtemp_local);

   hypre_CSRMatrix *A_CSR;
   int		   *A_CSR_i;   
   int		   *A_CSR_j;
   double	   *A_CSR_data;
   
   hypre_Vector    *f_vector;
   double	   *f_vector_data;

   int             i;
   int             jj;
   int             column;
   int             relax_error = 0;

   double         *A_mat;
   double         *b_vec;

   double          zero = 0.0;
  
   /*-----------------------------------------------------------------------
    * Switch statement to direct control based on relax_type:
    *     relax_type = 2 -> Jacobi (uses ParMatvec)
    *     relax_type = 9 -> Direct Solve
    *-----------------------------------------------------------------------*/
   
   switch (relax_type)
   {            

      case 7: /* Jacobi (uses ParMatvec) */
      {
 
         /*-----------------------------------------------------------------
          * Copy f into temporary vector.
          *-----------------------------------------------------------------*/
        
         hypre_ParVectorCopy(f,Vtemp); 
 
         /*-----------------------------------------------------------------
          * Perform MatvecT Vtemp=f-A^Tu
          *-----------------------------------------------------------------*/
 
            hypre_ParCSRMatrixMatvecT(-1.0,A, u, 1.0, Vtemp);
            for (i = 0; i < n; i++)
            {
 
               /*-----------------------------------------------------------
                * If diagonal is nonzero, relax point i; otherwise, skip it.
                *-----------------------------------------------------------*/
           
               if (A_diag_data[A_diag_i[i]] != zero)
               {
                  u_data[i] += relax_weight * Vtemp_data[i] 
				/ A_diag_data[A_diag_i[i]];
               }
            }
      }
      break;
      
      
      case 9: /* Direct solve: use gaussian elimination */
      {

         /*-----------------------------------------------------------------
          *  Generate CSR matrix from ParCSRMatrix A
          *-----------------------------------------------------------------*/

	 if (n)
	 {
	    A_CSR = hypre_ParCSRMatrixToCSRMatrixAll(A);
	    f_vector = hypre_ParVectorToVectorAll(f);
 	    A_CSR_i = hypre_CSRMatrixI(A_CSR);
 	    A_CSR_j = hypre_CSRMatrixJ(A_CSR);
 	    A_CSR_data = hypre_CSRMatrixData(A_CSR);
   	    f_vector_data = hypre_VectorData(f_vector);

            A_mat = hypre_CTAlloc(double, n_global*n_global);
            b_vec = hypre_CTAlloc(double, n_global);    

            /*---------------------------------------------------------------
             *  Load transpose of CSR matrix into A_mat.
             *---------------------------------------------------------------*/

            for (i = 0; i < n_global; i++)
            {
               for (jj = A_CSR_i[i]; jj < A_CSR_i[i+1]; jj++)
               {
                  column = A_CSR_j[jj];
                  A_mat[column*n_global+i] = A_CSR_data[jj];
               }
               b_vec[i] = f_vector_data[i];
            }

            relax_error = gselim(A_mat,b_vec,n_global);

            for (i = 0; i < n; i++)
            {
               u_data[i] = b_vec[first_index+i];
            }

	    hypre_TFree(A_mat); 
            hypre_TFree(b_vec);
            hypre_CSRMatrixDestroy(A_CSR);
            A_CSR = NULL;
            hypre_SeqVectorDestroy(f_vector);
            f_vector = NULL;
         
         }
      }
      break;   
   }

   return(relax_error); 
}
