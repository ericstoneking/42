/************************************************************************/
/*  Make template SC                                                    */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/************************************************************************/
int main(int argc, char **argv)
{
      FILE *file;
      char s[80];
      short Nb,Ng,Ib,Ig,Nf,i;
      double ang;
      double TwoPi = 8.0*atan(1.0);
      char Axis[3][12] = {"X_AXIS","Y_AXIS","Z_AXIS"};

      if (argc == 0) {
         printf("Usage: MakeScTemplate n, where n is the number of bodies desired.\n");
         exit(0);
      }

      Nb = atoi(argv[1]);
      Ng = Nb - 1;

      sprintf(s,"SCTemplate%02i.inp",Nb);
      file = fopen(s,"w");
      fprintf(file,"<<<<<<<<<<<<<<<<<  42: Spacecraft Description File   >>>>>>>>>>>>>>>>>\n");
      fprintf(file,"%hd-body Template             !  Description\n",Nb);
      fprintf(file,"GenScSpriteAlpha.ppm          !  Sprite File Name\n");
      fprintf(file,"************************* Orbit Parameters ****************************\n");
      fprintf(file,"FIXED                         !  Orbit Prop FIXED, EULER_HILL, or ENCKE\n");
      fprintf(file,"0                             !  Reference Orbit\n");
      fprintf(file,"CM                            !  Pos of CM or ORIGIN, wrt F\n");
      fprintf(file,"0.0  0.0  0.0                 !  Pos wrt Formation (m), expressed in F\n");
      fprintf(file,"0.0  0.0  0.0                 !  Vel wrt Formation (m/s), expressed in F\n");
      fprintf(file,"*************************** Initial Attitude ***************************\n");
      fprintf(file,"NAN                           ! Ang Vel wrt [NL], Att [QA] wrt [NLF]\n");
      fprintf(file,"0.0    0.0    0.0             ! Ang Vel (deg/sec)\n");
      fprintf(file,"0.0    0.0    0.0    1.0      ! Quaternion\n");
      fprintf(file,"0.0    0.0    0.0    123      ! Angles (deg) & Euler Sequence\n");
      fprintf(file,"************************************************************************\n");
      fprintf(file,"************************* Body Parameters ******************************\n");
      fprintf(file,"************************************************************************\n");
      fprintf(file,"DYN_JOINT                     ! Rotation STEADY, KIN_JOINT, or DYN_JOINT\n");
      fprintf(file,"TRUE                          ! Assume constant mass properties\n");
      fprintf(file,"%hd                           ! Number of Bodies\n",Nb);
      fprintf(file,"2.0                           ! Drag Coefficient\n");

      for(Ib=0;Ib<Nb;Ib++) {
         fprintf(file,"================================ Body %hd ================================\n",Ib);
         fprintf(file,"100.0                         ! Mass\n");
         fprintf(file,"100.0  200.0  300.0           ! Moments of Inertia (kg-m^2)\n");
         fprintf(file,"0.0  0.0  0.0                 ! Products of Inertia (xy,xz,yz)\n");
         fprintf(file,"0.0  0.0  0.0                 ! Location of mass center, m\n");
         fprintf(file,"Sphere.obj                    ! Geometry Input File Name\n");
      }

      fprintf(file,"************************************************************************\n");
      fprintf(file,"*************************** Joint Parameters ***************************\n");
      fprintf(file,"************************************************************************\n");

      if (Ng == 0) {
         fprintf(file,"NO JOINTS\n");
      }
      else {
         for(Ig=0;Ig<Ng;Ig++) {
            ang = ((double) Ig)/((double) Ng)*TwoPi;
            fprintf(file,"============================== Joint %hd ================================\n",Ig);
            fprintf(file,"3DOF                          ! Type (SPHERICAL,1DOF,2DOF,3DOF)\n");
            fprintf(file,"0 %hd                         ! Inner, outer body indices\n",Ig+1);
            fprintf(file,"0.0   0.0  0.0  123           ! Initial Angles [deg] & Seq\n");
            fprintf(file,"0.0   0.0  0.0                ! Initial Rates, deg/sec\n");
            fprintf(file,"%4.1lf   0.0  0.0  312        ! Bi to Gi Static Angles [deg] & Seq\n",ang*360.0/TwoPi);
            fprintf(file,"0.0   0.0   0.0   312         ! Go to Bo Static Angles [deg] & Seq\n",ang*360.0/TwoPi);
            fprintf(file,"%4.1lf  %4.1lf  0.0           ! Position wrt inner body origin, m\n",1.1*cos(ang),1.1*sin(ang));
            fprintf(file,"-1.1  0.0  0.0                ! Position wrt outer body origin, m\n");
         }
      }

      fprintf(file,"************************************************************************\n");
      fprintf(file,"*********************** Flexible Mode Parameters ***********************\n");
      fprintf(file,"************************************************************************\n");
      if (Nb > 1) {
         fprintf(file,"0                               ! Number of Flexible Modes\n");
      }
      else {
         printf("\n   How many flexible modes would you like? ");
         scanf("%hd",&Nf);
         fprintf(file,"%hd                               ! Number of Flexible Modes\n",Nf);
         for(i=0;i<Nf;i++) {
            fprintf(file,"================================ Mode %hd ================================\n",i);
            fprintf(file,"%s                          ! Axis (X_AXIS, Y_AXIS, Z_AXIS)\n",Axis[i%3]);
            fprintf(file,"%4.2lf                            ! Natural Frequency, Hz\n",0.25+0.05*i);
            fprintf(file,"0.01                            ! Damping Ratio\n");
            fprintf(file,"0.05                            ! Mass Participation\n");
            fprintf(file,"0.0                             ! Initial Deflection, rad/sqrt(kg-m^2)\n");
            fprintf(file,"0.0                             ! Initial Rate, rad/sec/sqrt(kg-m^2)       \n");
         }
      }
      
      fprintf(file,"*************************** Wheel Parameters ***************************\n");
      fprintf(file,"3                             ! Number of wheels (max 4)\n");
      fprintf(file,"0.0    0.0    0.0    0.0      ! Initial Momenta, N-m-sec\n");
      fprintf(file,"0.012  0.012  0.012  0.012    ! Wheel Inertias, kg-m^2\n");
      fprintf(file,"1.0    0.0    0.0    1.0      ! Axis X components\n");
      fprintf(file,"0.0    1.0    0.0    1.0      ! Axis Y components\n");
      fprintf(file,"0.0    0.0    1.0    1.0      ! Axis Z components\n");
      fprintf(file,"0.14   0.14   0.14   0.14     ! Max Torque, N-m\n");
      fprintf(file,"50.0  50.0   50.0   50.0      ! Max Momentum, N-m-sec\n");
      fprintf(file,"**************************** MTB Parameters ****************************\n");
      fprintf(file,"3                                   ! Number of MTB's (max 4)\n");
      fprintf(file,"180.0  180.0    180.0    180.0      ! Saturation (A-m^2)\n");
      fprintf(file,"1.0      0.0      0.0      1.0      ! Axis X components\n");
      fprintf(file,"0.0      1.0      0.0      1.0      ! Axis Y components\n");
      fprintf(file,"0.0      0.0      1.0      1.0      ! Axis Z components\n");
      fprintf(file,"************************* Thruster Parameters **************************\n");
      fprintf(file,"0                             ! Number of Thrusters (max 12)\n");
      fprintf(file," 1.0                          ! Thrust Force (N)\n");
      fprintf(file,"-1.0  0.0  0.0                ! Thrust Axis \n");
      fprintf(file," 1.0  1.0  1.0                ! Location in B0, m\n");
      fprintf(file," 1.0                          ! Thrust Force (N)\n");
      fprintf(file," 0.0 -1.0  0.0                ! Thrust Axis \n");
      fprintf(file," 1.0  1.0  1.0                ! Location in B0, m\n");
      fprintf(file," 1.0                          ! Thrust Force (N)\n");
      fprintf(file," 0.0  0.0 -1.0                ! Thrust Axis \n");
      fprintf(file," 1.0  1.0  1.0                ! Location in B0, m\n");
      fprintf(file," 1.0                          ! Thrust Force (N)\n");
      fprintf(file," 1.0  0.0  0.0                ! Thrust Axis \n");
      fprintf(file,"-1.0 -1.0  1.0                ! Location in B0, m\n");
      fprintf(file," 1.0                          ! Thrust Force (N)\n");
      fprintf(file," 0.0  1.0  0.0                ! Thrust Axis \n");
      fprintf(file,"-1.0 -1.0  1.0                ! Location in B0, m\n");
      fprintf(file," 1.0                          ! Thrust Force (N)\n");
      fprintf(file," 0.0  0.0 -1.0                ! Thrust Axis \n");
      fprintf(file,"-1.0 -1.0  1.0                ! Location in B0, m\n");
      fprintf(file," 1.0                          ! Thrust Force (N)\n");
      fprintf(file,"-1.0  0.0  0.0                ! Thrust Axis \n");
      fprintf(file," 1.0 -1.0 -1.0                ! Location in B0, m\n");
      fprintf(file," 1.0                          ! Thrust Force (N)\n");
      fprintf(file," 0.0  1.0  0.0                ! Thrust Axis \n");
      fprintf(file," 1.0 -1.0 -1.0                ! Location in B0, m\n");
      fprintf(file," 1.0                          ! Thrust Force (N)\n");
      fprintf(file," 0.0  0.0  1.0                ! Thrust Axis \n");
      fprintf(file," 1.0 -1.0 -1.0                ! Location in B0, m\n");
      fprintf(file," 1.0                          ! Thrust Force (N)\n");
      fprintf(file," 1.0  0.0  0.0                ! Thrust Axis \n");
      fprintf(file,"-1.0  1.0 -1.0                ! Location in B0, m\n");
      fprintf(file," 1.0                          ! Thrust Force (N)\n");
      fprintf(file," 0.0 -1.0  0.0                ! Thrust Axis \n");
      fprintf(file,"-1.0  1.0 -1.0                ! Location in B0, m\n");
      fprintf(file," 1.0                          ! Thrust Force (N)\n");
      fprintf(file," 0.0  0.0  1.0                ! Thrust Axis \n");
      fprintf(file,"-1.0  1.0 -1.0                ! Location in B0, m\n");

      fclose(file);
      return(0);
}