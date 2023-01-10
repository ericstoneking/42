/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


#include "42.h"

/* #ifdef __cplusplus
** namespace _42 {
** using namespace Kit;
** #endif
*/

/**********************************************************************/
void PassiveJoint(struct JointType *G, struct SCType *S)
{
      long i;
      double a[3];

      if (G->IsSpherical) {
         Q2AngleVec(G->q,a);
         for(i=0;i<3;i++) {
            G->Trq[i] = -G->RotDampCoef[i]*G->AngRate[i]
                              - G->RotSpringCoef[i]*a[i];
         }
      }
      else {
         for(i=0;i<G->RotDOF;i++) {
            G->Trq[i] = -G->RotDampCoef[i]*G->AngRate[i]
                               -G->RotSpringCoef[i]*G->Ang[i];
         }
      }

      for(i=0;i<G->TrnDOF;i++) {
         G->Frc[i] = -G->TrnDampCoef[i]*G->PosRate[i]
                            -G->TrnSpringCoef[i]*G->Pos[i];
      }
}
/**********************************************************************/
/* Simple actively-controlled joint.                                  */
void ActuatedJoint(struct JointType *G, struct SCType *S)
{
      double RateCmd;
      long i;
      
      for(i=0;i<G->RotDOF;i++) {
         RateCmd = Limit(G->AngRateCmd[i],-G->MaxAngRate[i],G->MaxAngRate[i]);
         G->Trq[i] = Limit(-G->AngRateGain[i]*(G->AngRate[i]-RateCmd),
                        -G->MaxTrq[i],G->MaxTrq[i]);
      }

      for(i=0;i<G->TrnDOF;i++) {
         RateCmd = Limit(G->PosRateCmd[i],-G->MaxPosRate[i],G->MaxPosRate[i]);
         G->Frc[i] = Limit(-G->PosRateGain[i]*(G->PosRate[i]-RateCmd),
                        -G->MaxFrc[i],G->MaxFrc[i]);
      }

}
/**********************************************************************/
void StepperMotorJoint(struct JointType *G, struct SCType *S)
{
}
/**********************************************************************/
void TvcJoint(struct JointType *G, struct SCType *S)
{
}
/**********************************************************************/
void VibrationIsolatorJoint(struct JointType *G, struct SCType *S)
{
}
/**********************************************************************/
void SloshJoint(struct JointType *G, struct SCType *S)
{
}
/**********************************************************************/
void SteeringMirrorJoint(struct JointType *G, struct SCType *S)
{
}
/**********************************************************************/
/* A good place for you to implement a quick-and-dirty model          */
void AdHocJoint(struct JointType *G, struct SCType *S)
{
      long i;

      if (G->IsSpherical) {
         for(i=0;i<3;i++) G->Trq[i] = 0.0;
      }
      else {
         for(i=0;i<G->RotDOF;i++) G->Trq[i] = 0.0;
      }

      for(i=0;i<G->TrnDOF;i++) G->Frc[i] = 0.0;
}
/**********************************************************************/
void JointFrcTrq(struct JointType *G, struct SCType *S)
{

      switch(G->Type) {
         case PASSIVE_JOINT:
            PassiveJoint(G,S);
            break;
         case ACTUATED_JOINT:
            ActuatedJoint(G,S);
            break;
         //case STEPPER_MOTOR_JOINT:
         //   StepperMotorJoint(G,S);
         //   break;
         //case TVC_JOINT:
         //   TvcJoint(G,S);
         //   break;
         //case VIBRATION_ISOLATOR_JOINT:
         //   VibrationIsolatorJoint(G,S);
         //   break;
         //case SLOSH_JOINT:
         //   SloshJoint(G,S);
         //   break;
         //case STEERING_MIRROR_JOINT:
         //   SteeringMirrorJoint(G,S);
         //   break;
         case AD_HOC_JOINT:
            AdHocJoint(G,S);
            break;
         default:
            printf("Unknown joint type %ld in JointFrcTrq.  Bailing out.\n",G->Type);
            exit(1);
     }
}


/* #ifdef __cplusplus
** }
** #endif
*/

