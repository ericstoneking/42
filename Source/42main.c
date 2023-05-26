/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


/**********************************************************************/
/*                                                                    */
/*                               42                                   */
/*                                                                    */
/*                  The Mostly Harmless Simulation                    */
/*                                                                    */
/**********************************************************************/
/*  Written by Eric Stoneking, beginning August 6, 2001               */
/*  Based on the VW, Pegasus, TK family of simulations                */
/**********************************************************************/

/* This file contains a minimal "main" function to facilitate         */
/* compiling 42 as a library to be called from other applications.    */
/* This "main" serves as the entry point for the stand-alone          */
/* application.  It calls 42's top-level executive function, "exec".  */

/*
** #ifdef __cplusplus
** namespace _42 {
** #endif
*/

int exec(int argc,char **argv);

int main(int argc,char **argv)
{
      return(exec(argc,argv));
}

/*
** #ifdef __cplusplus
** }
** #endif
*/

