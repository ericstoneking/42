/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


#include "meshkit.h"

#define CGLTF_IMPLEMENTATION
#include "cgltf.h"

/* #ifdef __cplusplus
** namespace Kit {
** #endif
*/

/**********************************************************************/
struct MatlType *AddMtlLib(const char *PathName, const char *MtlLibName,
   struct MatlType *OldMatl, long *Nmatl)
{
      FILE *MtlLib;
      char line[512];
      long i;
      long AlreadyExists = 0;
      struct MatlType *M, *NewMatl;
      char MatlName[80];

      MtlLib = FileOpen(PathName,MtlLibName,"rt");

      NewMatl = OldMatl;
      while(!feof(MtlLib)) {
         fgets(line,512,MtlLib);
         if (sscanf(line,"newmtl %s",MatlName) == 1) {
            /* Skip if material already exists */
            AlreadyExists = 0;
            for(i=0;i<*Nmatl;i++) {
               if (!strcmp(MatlName,NewMatl[i].Label)) {
                  AlreadyExists = 1;
                  /* printf("Info: Matl %s already exists\n",MatlName); */
               }
            }
            if (!AlreadyExists) {
               (*Nmatl)++;
               NewMatl = (struct MatlType *) realloc(NewMatl,(*Nmatl)*sizeof(struct MatlType));
               if (NewMatl == NULL) {
                  printf("Realloc failed in AddMtlLib\n");
                  exit(1);
               }
               M = &NewMatl[(*Nmatl)-1];
               strcpy(M->Label,MatlName);
               /* Default color values */
               M->Ns = 100.0;
               M->Nu = M->Ns;
               M->Nv = M->Ns;
               M->Refl = 0.0;
               for(i=0;i<3;i++) {
                  M->Ka[i] = 0.8;
                  M->Kd[i] = 0.8;
                  M->Ks[i] = 1.0;
                  M->Ke[i] = 0.0;
                  M->NoiseGain[i] = 0.0;
                  M->NoiseAxis[i] = 0.0;
               }
               M->Ka[3] = 1.0;
               M->Kd[3] = 1.0;
               M->Ks[3] = 1.0;
               M->Ke[3] = 1.0;
               sprintf(M->ColorTexFileName,"NONE");
               sprintf(M->BumpTexFileName,"NONE");
               sprintf(M->SpectrumName,"NONE");
               M->ColorTex = 0;
               M->BumpTex = 0;
               M->SpectrumTex = 0;
               M->NoiseColEnabled = 0;
               M->NoiseBumpEnabled = 0;
               M->NoiseBias = 0.0;
               M->NoiseScale = 1.0;
               M->NoiseType = 0;
               M->SpecFrac = 1.0;
               M->DiffFrac = 0.0;
            }
         }
         if (!AlreadyExists) {
            M = &NewMatl[(*Nmatl)-1];
            sscanf(line," Ns %f",&M->Ns);
            if (sscanf(line," d %f",&M->Ka[3]) == 1) {
               if (M->Ka[3] < 1.0) {
                  M->Kd[3] = 0.0;
                  M->Ks[3] = 0.0;
                  M->Ke[3] = 0.0;
               }
            }
            sscanf(line," Kd %f %f %f",&M->Kd[0],&M->Kd[1],&M->Kd[2]);
            sscanf(line," Ka %f %f %f",&M->Ka[0],&M->Ka[1],&M->Ka[2]);
            sscanf(line," Ks %f %f %f",&M->Ks[0],&M->Ks[1],&M->Ks[2]);
            sscanf(line," Ke %f %f %f",&M->Ke[0],&M->Ke[1],&M->Ke[2]);
            sscanf(line," map_Kd %s",M->ColorTexFileName);
            sscanf(line," map_Bump %s",M->BumpTexFileName);
            sscanf(line," map_Spectrum %s",M->SpectrumName);
            sscanf(line," Refl %f",&M->Refl);
            sscanf(line," Nu %f",&M->Nu);
            sscanf(line," Nv %f",&M->Nv);
            if (!strncmp(line,"NoiseBump",9)) M->NoiseBumpEnabled = 1;
            if (!strncmp(line,"NoiseCol",8)) M->NoiseColEnabled = 1;
            sscanf(line," NoiseGain %f %f %f %f",
               &M->NoiseGain[0],&M->NoiseGain[1],
               &M->NoiseGain[2],&M->NoiseGain[3]);
            sscanf(line," NoiseBias %f",&M->NoiseBias);
            sscanf(line," NoiseScale %f",&M->NoiseScale);
            sscanf(line," NoiseAxis %f %f %f",
               &M->NoiseAxis[0],&M->NoiseAxis[1],&M->NoiseAxis[2]);
            sscanf(line," NoiseType %i",&M->NoiseType);
            sscanf(line," SpecFrac %lf",&M->SpecFrac);
            sscanf(line," DiffFrac %lf",&M->DiffFrac);
         }
         sprintf(line,"Flush");
      }
      fclose(MtlLib);

      if ((*Nmatl) == 0) {
         printf("No materials loaded!\n");
         exit(1);
      }

      return(NewMatl);
}
/**********************************************************************/
void ScaleSpecDiffFrac(struct MatlType *Matl, long Nmatl)
{
      long Im;
      double Tot;
      struct MatlType *M;

      for(Im=0;Im<Nmatl;Im++) {
         M = &Matl[Im];

         Tot = M->SpecFrac + M->DiffFrac;
         if (Tot < 1.0) Tot = 1.0; /* Allow for absorption, transmission */
         M->SpecFrac = M->SpecFrac/Tot;
         M->DiffFrac = M->DiffFrac/Tot;
      }
}
/*********************************************************************/
void SurfaceForceProps(struct MeshType *M)
{
      double **uv;
      double uhat[3],v2[3],nhat[3],vhat[3];
      double C[3][3];
      double uvbar[3];
      double x1,y1,x2,y2,xA,yA;
      long Ip;
      long j;
      struct PolyType *P;

      for(Ip=0;Ip<M->Npoly;Ip++) {
         P = &M->Poly[Ip];

         uv = CreateMatrix(P->Nv+1,3);

         /* Compute Unit Normal Vector */
         for(j=0;j<3;j++) {
            uhat[j] = M->V[P->V[1]][j] - M->V[P->V[0]][j];
            v2[j]   = M->V[P->V[2]][j] - M->V[P->V[1]][j];
         }
         UNITV(uhat);
         VxV(uhat,v2,nhat);
         UNITV(nhat);
         VxV(nhat,uhat,vhat);
         UNITV(vhat);
         for(j=0;j<3;j++) P->Norm[j] = nhat[j];
         if (MAGV(nhat) == 0.0) {
            printf("Zero-length unit vector in SurfaceForceProps.\n");
            printf("Check for zero-area polys or polys with three colinear vertices.\n");
            printf("Tesselating your model to all triangles is highly recommended.\n");
            exit(1);
         }

         /* Compute in-plane basis vectors */
         PerpBasis(P->Norm,P->Uhat,P->Vhat);

         /* Compute Polygon Area and Centroid */
         for(j=0;j<3;j++){
            C[0][j] = uhat[j];
            C[1][j] = vhat[j];
            C[2][j] = nhat[j];
         }
         for(j=0;j<P->Nv;j++) MxV(C,M->V[P->V[j]],uv[j]);
         MxV(C,M->V[P->V[0]],uv[P->Nv]);
         P->Area = 0.0;
         uvbar[0] = 0.0;
         uvbar[1] = 0.0;
         uvbar[2] = 0.0;
         for(j=0;j<P->Nv;j++) {
            x1 = uv[j][0];
            y1 = uv[j][1];
            x2 = uv[j+1][0];
            y2 = uv[j+1][1];
            xA = (x2-x1)*((2.0*x1+x2)*y1+(x1+2.0*x2)*y2)/6.0;
            yA = (x2-x1)*(y1*y1+y1*y2+y2*y2)/6.0;
            P->Area -= 0.5*(x2-x1)*(y1+y2);
            uvbar[0] -= xA;
            uvbar[1] -= yA;
            uvbar[2] += uv[j][2];
         }
         uvbar[0] /= P->Area;
         uvbar[1] /= P->Area;
         uvbar[2] /= (double) P->Nv;
         MTxV(C,uvbar,P->Centroid);
         if (isnan(P->Centroid[0])) {
            printf("NaN Centroid in SurfaceForceProps\n");
            exit(1);
         }
         P->UnshadedArea = P->Area;
         P->UnshadedCtr[0] = P->Centroid[0];
         P->UnshadedCtr[1] = P->Centroid[1];
         P->UnshadedCtr[2] = P->Centroid[2];

         DestroyMatrix(uv,P->Nv+1);
      }
}
/*********************************************************************/
/* Ref Werner and Scheeres, "Exterior Gravitation of a Polyhedron ..." */
void EdgeAndPolyDyads(struct MeshType *M)
{
      struct EdgeType *E;
      struct PolyType *P1,*P2,*P;
      double *V1,*V2;
      double Axis[3],N1[3],N2[3];
      long Ie,Ip,i,j;

      for(Ie=0;Ie<M->Nedge;Ie++) {
         E = &M->Edge[Ie];
         if (E->Poly2 >= 0) {
            P1 = &M->Poly[E->Poly1];
            P2 = &M->Poly[E->Poly2];
            V1 = M->V[E->Vtx1];
            V2 = M->V[E->Vtx2];
            for(i=0;i<3;i++) Axis[i] = V2[i]-V1[i];
            UNITV(Axis);
            /* Unit vectors in plane, pointing outward */
            VxV(Axis,P1->Norm,N1);
            VxV(P2->Norm,Axis,N2);
            UNITV(N1);
            UNITV(N2);
            for(i=0;i<3;i++) {
               for(j=0;j<3;j++) {
                  E->Dyad[i][j] = P1->Norm[i]*N1[j] + P2->Norm[i]*N2[j];
               }
            }
         }
      }
      
      for(Ip=0;Ip<M->Npoly;Ip++) {
         P = &M->Poly[Ip];
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) {
               P->Dyad[i][j] = P->Norm[i]*P->Norm[j];
            }
         }
      }
}
/*********************************************************************/
double PolyhedronVolume(struct MeshType *M)
{
      double Vol;
      struct PolyType *P;
      double *V1,*V2,*V3;
      double V2xV3[3];
      long Ip;
      
      Vol = 0.0;
      for(Ip=0;Ip<M->Npoly;Ip++) {
         P = &M->Poly[Ip];
         V1 = M->V[P->V[0]];
         V2 = M->V[P->V[1]];
         V3 = M->V[P->V[2]];
         VxV(V2,V3,V2xV3);
         Vol += VoV(V1,V2xV3)/6.0;
      }
      return(Vol);
}
/*********************************************************************/
long PolyIsDegenerate(struct PolyType *P, double **V)
{
#define EPS (1.0E-6)
      long ZeroArea;
      long Iv,I1,I2,I3,i;
      double E1[3],E2[3],E1xE2[3];

      ZeroArea = 1;
      for(Iv=0;Iv<P->Nv;Iv++) {
         I1 = P->V[Iv];
         I2 = P->V[(Iv+1)%P->Nv];
         I3 = P->V[(Iv+2)%P->Nv];
         for(i=0;i<3;i++) {
            E1[i] = V[I2][i] - V[I1][i];
            E2[i] = V[I3][i] - V[I2][i];
         }
         VxV(E1,E2,E1xE2);
         if (MAGV(E1xE2) > EPS) ZeroArea = 0;
      }

      return(ZeroArea);
#undef EPS
}
/**********************************************************************/
long RayHitsBBox(double Source[3],double DirVec[3],
   struct BoundingBoxType *BB)
{
      double dx,dy,dz,x,y,z;

      /* Check +X face */
      dx = BB->max[0] - Source[0];
      if (dx*DirVec[0] > 0.0) {
         dy = DirVec[1]/DirVec[0]*dx;
         dz = DirVec[2]/DirVec[0]*dx;
         y = Source[1]+dy;
         z = Source[2]+dz;
         if (BB->min[1] < y && y < BB->max[1]
          && BB->min[2] < z && z < BB->max[2]) return(1);
      }
      /* Check +Y face */
      dy = BB->max[1] - Source[1];
      if (dy*DirVec[1] > 0.0) {
         dz = DirVec[2]/DirVec[1]*dy;
         dx = DirVec[0]/DirVec[1]*dy;
         z = Source[2]+dz;
         x = Source[0]+dx;
         if (BB->min[2] < z && z < BB->max[2]
          && BB->min[0] < x && x < BB->max[0]) return(1);
      }
      /* Check +Z face */
      dz = BB->max[2] - Source[2];
      if (dz*DirVec[2] > 0.0) {
         dx = DirVec[0]/DirVec[2]*dz;
         dy = DirVec[1]/DirVec[2]*dz;
         x = Source[0]+dx;
         y = Source[1]+dy;
         if (BB->min[0] < x && x < BB->max[0]
          && BB->min[1] < y && y < BB->max[1]) return(1);
      }
      /* Check -X face */
      dx = BB->min[0] - Source[0];
      if (dx*DirVec[0] > 0.0) {
         dy = DirVec[1]/DirVec[0]*dx;
         dz = DirVec[2]/DirVec[0]*dx;
         y = Source[1]+dy;
         z = Source[2]+dz;
         if (BB->min[1] < y && y < BB->max[1]
          && BB->min[2] < z && z < BB->max[2]) return(1);
      }
      /* Check -Y face */
      dy = BB->min[1] - Source[1];
      if (dy*DirVec[1] > 0.0) {
         dz = DirVec[2]/DirVec[1]*dy;
         dx = DirVec[0]/DirVec[1]*dy;
         z = Source[2]+dz;
         x = Source[0]+dx;
         if (BB->min[2] < z && z < BB->max[2]
          && BB->min[0] < x && x < BB->max[0]) return(1);
      }
      /* Check -Z face */
      dz = BB->max[2] - Source[2];
      if (dz*DirVec[2] > 0.0) {
         dx = DirVec[0]/DirVec[2]*dz;
         dy = DirVec[1]/DirVec[2]*dz;
         x = Source[0]+dx;
         y = Source[1]+dy;
         if (BB->min[0] < x && x < BB->max[0]
          && BB->min[1] < y && y < BB->max[1]) return(1);
      }

      return(0);
}
/**********************************************************************/
long KDRayHitsLeaf(double Source[3], double DirVec[3],
   struct KDNodeType *KD, struct MeshType *M,
   long *HitPoly, double HitPoint[3], double *HitDist)
{
      struct PolyType *P;
      double **Vtx;
      long Hit,Ip,Iv,i;
      double Dist;
      double ProjPoint[3];

      Hit = 0;

      for(Ip=0;Ip<KD->Npoly;Ip++) {
         P = &M->Poly[KD->Poly[Ip]];
         Vtx = (double **) calloc(P->Nv,sizeof(double *));
         if (Vtx==NULL) {
            printf("Allocation failed in %s:%d\n",__FILE__,__LINE__);
            exit(1);
         }
         for(Iv=0;Iv<P->Nv;Iv++) {
            Vtx[Iv] = M->V[P->V[Iv]];
         }
         if (ProjectPointOntoPoly(Source,DirVec,Vtx,P->Nv,
            ProjPoint,&Dist)) {
            Hit = 1;
            if (Dist < *HitDist) {
               *HitDist = Dist;
               *HitPoly = KD->Poly[Ip];
               for(i=0;i<3;i++) HitPoint[i] = ProjPoint[i];
            }
         }
         free(Vtx);
      }

      return(Hit);
}
/**********************************************************************/
long KDRayHitsNode(double Source[3], double DirVec[3], struct KDNodeType *KD,
   struct MeshType *M, long *HitPoly, double HitPoint[3], double *HitDist)
{
      long HitLow,HitHigh;
      long Hit = 0;

      if (RayHitsBBox(Source,DirVec,&KD->BB)) {
         if(KD->IsLeaf) {
            Hit = KDRayHitsLeaf(Source,DirVec,KD,M,
               HitPoly,HitPoint,HitDist);
         }
         else { /* Recursively check child nodes */
            HitLow = KDRayHitsNode(Source,DirVec,KD->LowChild,M,
               HitPoly,HitPoint,HitDist);
            HitHigh = KDRayHitsNode(Source,DirVec,KD->HighChild,M,
               HitPoly,HitPoint,HitDist);
            Hit = HitLow || HitHigh;
         }
      }
      return(Hit);
}
/**********************************************************************/
/* Source and DirVec must be expressed in M's coordinate system       */
long KDProjectRayOntoMesh(double Source[3], double DirVec[3],
   struct MeshType *M, long *HitPoly, double HitPoint[3])
{
      double HitDist = 1.0E12; /* Absurd large value */
      long RayHitsMesh;

      RayHitsMesh = KDRayHitsNode(Source,DirVec,M->KDTree,M,HitPoly,
         HitPoint,&HitDist);

      return(RayHitsMesh);
}
/**********************************************************************/
long KDPartition(long *P, long LowEnd, long HighEnd, long Axis, struct MeshType *M)
{
      long PivotIdx,TempIdx;
      long LowIdx;
      double PivotVal;

      PivotIdx = HighEnd;
      PivotVal = M->Poly[P[HighEnd]].Centroid[Axis];
      LowIdx = LowEnd;

      while(LowIdx < PivotIdx) {
         if (M->Poly[P[LowIdx]].Centroid[Axis] > PivotVal) {
            TempIdx = P[PivotIdx];
            P[PivotIdx] = P[LowIdx];
            P[LowIdx] = P[PivotIdx-1];
            P[PivotIdx-1] = TempIdx;
            PivotIdx--;
         }
         else LowIdx++;
      }
      return(PivotIdx);
}
/**********************************************************************/
long KDSelectMedian(long *P, long N, long Axis, struct MeshType *M)
{
      long LowEnd,HighEnd,PivotIdx;

      LowEnd = 0;
      HighEnd = N-1;

      do {
         PivotIdx = KDPartition(P,LowEnd,HighEnd,Axis,M);
         if (PivotIdx > N/2) HighEnd = PivotIdx-1;
         else if (PivotIdx < N/2) LowEnd = PivotIdx+1;
      } while(PivotIdx != N/2);

      return(PivotIdx);
}
/**********************************************************************/
long KDCompare(long P1, long P2,long Axis,struct MeshType *M)
{
      if (M->Poly[P1].Centroid[Axis] < M->Poly[P2].Centroid[Axis])
         return(-1);
      else if (M->Poly[P1].Centroid[Axis] > M->Poly[P2].Centroid[Axis])
         return(1);
      else return(0);
}
/**********************************************************************/
void KDFormHeap(long *P, long L, long Axis, struct MeshType *M)
{
      long Done,k,Temp;

      do {
         Done = 1;
         for(k=0;k<L/2;k++) {
            /* Check against left child (2*k+1) */
            if (KDCompare(P[k],P[2*k+1],Axis,M) < 0) {
               Temp = P[k];
               P[k] = P[2*k+1];
               P[2*k+1] = Temp;
               Done = 0;
            }
            /* Check against right child (2*k+2) */
            if (2*k+2 < L) { /* Right child exists */
               if (KDCompare(P[k],P[2*k+2],Axis,M) < 0) {
                  Temp = P[k];
                  P[k] = P[2*k+2];
                  P[2*k+2] = Temp;
                  Done = 0;
               }
            }
         }
      } while(!Done);
}
/**********************************************************************/
void KDHeapSort(long *Poly, long Npoly, long Axis,struct MeshType *M)
{
      long HeapLength;
      long Temp;

      /* Form heap so each parent is greater than both children */
      KDFormHeap(Poly,Npoly,Axis,M);

      HeapLength = Npoly;
      while (HeapLength > 1) {
         /* Take largest element from top of heap */
         Temp = Poly[0];
         Poly[0] = Poly[HeapLength-1];
         Poly[HeapLength-1] = Temp;

         /* Shorten heap */
         HeapLength--;
         /* Re-form heap */
         KDFormHeap(Poly,HeapLength,Axis,M);
      }
}
/**********************************************************************/
void SplitKDNode(struct KDNodeType *KD,struct MeshType *M)
{
      struct KDNodeType *LC,*HC;
      struct PolyType *P;
      double MedVal;
      long MedIdx;
      long Axis,i,Ip,Iv;
      long AnyVtxBelowMedian;
      long AnyVtxAboveMedian;

      /* printf("KD Node Depth = %ld\n",KD->Depth); */

      /* Create child nodes */
      Axis = (KD->Axis+1)%3;
      KD->LowChild = (struct KDNodeType *) calloc(1,sizeof(struct KDNodeType));
      if (KD->LowChild==NULL) {
         printf("Allocation failed in %s:%d\n",__FILE__,__LINE__);
         exit(1);
      }
      KD->HighChild = (struct KDNodeType *) calloc(1,sizeof(struct KDNodeType));
      if (KD->HighChild==NULL) {
         printf("Allocation failed in %s:%d\n",__FILE__,__LINE__);
         exit(1);
      }
      LC = KD->LowChild;
      HC = KD->HighChild;
      LC->Parent = KD;
      HC->Parent = KD;
      LC->Axis = Axis;
      HC->Axis = Axis;
      LC->IsRoot = 0;
      HC->IsRoot = 0;
      LC->Depth = KD->Depth+1;
      HC->Depth = KD->Depth+1;
      /* Children inherit BBox limits on two axes */
      /* "Axis" axis gets overwritten below */
      for(i=0;i<3;i++) {
         LC->BB.min[i] = KD->BB.min[i];
         LC->BB.max[i] = KD->BB.max[i];
         LC->BB.center[i] = 0.5*(LC->BB.max[i]+LC->BB.min[i]);
         HC->BB.min[i] = KD->BB.min[i];
         HC->BB.max[i] = KD->BB.max[i];
         HC->BB.center[i] = 0.5*(HC->BB.max[i]+HC->BB.min[i]);
      }

      /* Split [Axis] BBox along Median Value */
      MedIdx = KDSelectMedian(KD->Poly,KD->Npoly,Axis,M);
      MedVal = M->Poly[KD->Poly[MedIdx]].Centroid[Axis];
      LC->BB.max[Axis] = MedVal;
      HC->BB.min[Axis] = MedVal;
      LC->BB.center[Axis] = 0.5*(LC->BB.max[Axis]+LC->BB.min[Axis]);
      HC->BB.center[Axis] = 0.5*(HC->BB.max[Axis]+HC->BB.min[Axis]);


      /* Assign Polys to Children */
      for(Ip=0;Ip<KD->Npoly;Ip++) {
         AnyVtxBelowMedian = 0;
         AnyVtxAboveMedian = 0;
         P = &M->Poly[KD->Poly[Ip]];
         for(Iv=0;Iv<P->Nv;Iv++) {
            if (M->V[P->V[Iv]][Axis] < MedVal) AnyVtxBelowMedian = 1;
            else AnyVtxAboveMedian = 1;
         }
         if (AnyVtxBelowMedian) {
            LC->Npoly++;
            LC->Poly = (long *) realloc(LC->Poly,LC->Npoly*sizeof(long));
            LC->Poly[LC->Npoly-1] = KD->Poly[Ip];
         }
         if (AnyVtxAboveMedian) {
            HC->Npoly++;
            HC->Poly = (long *) realloc(HC->Poly,HC->Npoly*sizeof(long));
            HC->Poly[HC->Npoly-1] = KD->Poly[Ip];
         }
      }

      /* Recurse until termination conditions met */
      if (LC->Npoly < 20) {
         LC->IsLeaf = 1;
      }
      else if (LC->Depth > 20) {
         /*printf("Depth exceeds 20.  Npoly = %ld\n",LC->Npoly);*/
         LC->IsLeaf = 1;
      }
      else SplitKDNode(LC,M);

      if (HC->Npoly < 20) {
         HC->IsLeaf = 1;
      }
      else if (HC->Depth > 20) {
         /*printf("Depth exceeds 20.  Npoly = %ld\n",HC->Npoly);*/
         HC->IsLeaf = 1;
      }
      else SplitKDNode(HC,M);

}
/**********************************************************************/
void LoadKDTree(struct MeshType *M)
{

      struct KDNodeType *KD;
      long i,Ip;

/* .. Root Node coincides with Mesh's Bounding Box */
      M->KDTree = (struct KDNodeType *) calloc(1,sizeof(struct KDNodeType));
      if (M->KDTree==NULL) {
         printf("Allocation failed in %s:%d\n",__FILE__,__LINE__);
         exit(1);
      }
      KD = M->KDTree;
      KD->IsRoot = 1;
      KD->IsLeaf = 0;
      KD->Depth = 0;
      KD->Axis = 0;
      for(i=0;i<3;i++) {
         KD->BB.min[i] = M->BBox.min[i];
         KD->BB.max[i] = M->BBox.max[i];
         KD->BB.center[i] = M->BBox.center[i];
      }
      /* Root Node poly list is trivial: */
      KD->Npoly = M->Npoly;
      KD->Poly = (long *) calloc(KD->Npoly,sizeof(long));
      for(Ip=0;Ip<KD->Npoly;Ip++) KD->Poly[Ip] = Ip;

      SplitKDNode(KD,M);
}
/**********************************************************************/
void LoadOctree(struct MeshType *M)
{
      struct OctreeType *O;
      struct OctreeCellType *OC,*C;
      struct BoundingBoxType *BB;
      struct PolyType *P;
      double *V;
      double sign[8][3] = {{-1.0,-1.0,-1.0},{-1.0,-1.0, 1.0},
                           {-1.0, 1.0,-1.0},{-1.0, 1.0, 1.0},
                           { 1.0,-1.0,-1.0},{ 1.0,-1.0, 1.0},
                           { 1.0, 1.0,-1.0},{ 1.0, 1.0, 1.0}};
      double r[3];
      long Ic,Io,i,j,k,Ipoly,Iv;
      long AllPos[3],AllNeg[3];
      long NoChildHasAll;

      BB = &M->BBox;

      M->Octree = (struct OctreeType *) calloc(1,sizeof(struct OctreeType));
      if (M->Octree==NULL) {
         printf("Allocation failed in %s:%d\n",__FILE__,__LINE__);
         exit(1);
      }
      O = M->Octree;

/* .. Assign children */
      Ic=1;
      for(i=0;i<73;i++) {
         for(j=0;j<8;j++) {
            O->OctCell[i].Child[j] = Ic;
            Ic++;
         }
      }

/* .. Assign NextOnHit, NextOnMiss (Credit Matt Heron) */
      for(i=0;i<73;i++)      O->OctCell[i].NextOnHit = 8*i+1;
      for(i=73;i<585;i++)    O->OctCell[i].NextOnHit = i+1;
      for(i=80;i<577;i+=8)   O->OctCell[i].NextOnHit = i/8;
      for(i=136;i<577;i+=64) O->OctCell[i].NextOnHit = (i/8-1)/8;
      for(i=584;i<585;i++)   O->OctCell[i].NextOnHit = 0;
   
      for(i=0;i<585;i++)     O->OctCell[i].NextOnMiss = i+1;
      for(i=16;i<585;i+=8)   O->OctCell[i].NextOnMiss = i/8;
      for(i=136;i<585;i+=64) O->OctCell[i].NextOnMiss = (i/8-1)/8;
      for(i=0;i<585;i=8*i+8) O->OctCell[i].NextOnMiss = 0;

/* .. Find centers, min and max */
      OC = &O->OctCell[0];
      for(j=0;j<3;j++) {
         OC->center[j] = BB->center[j];
         OC->min[j] = BB->min[j];
         OC->max[j] = BB->max[j];
      }
      OC->radius = BB->radius;
      for(i=0;i<73;i++) {
         OC = &O->OctCell[i];
         for(k=0;k<3;k++) {
            r[k] = 0.5*(OC->max[k]-OC->center[k]);
         }
         for(j=0;j<8;j++) {
            C=&O->OctCell[OC->Child[j]];
            for(k=0;k<3;k++) {
               C->center[k] = OC->center[k] + sign[j][k]*r[k];
               C->max[k] = C->center[k] + r[k];
               C->min[k] = C->center[k] - r[k];
            }
            C->radius = MAGV(r);
         }
      }

/* .. Populate with polys */
      for(Ipoly=0;Ipoly<M->Npoly;Ipoly++) {
         P = &M->Poly[Ipoly];
         Io = 0;
         NoChildHasAll = 0;
         while (Io < 73 && !NoChildHasAll) {
            OC = &O->OctCell[Io];
            for(i=0;i<3;i++) {
               AllPos[i] = 1;
               AllNeg[i] = 1;
            }
            NoChildHasAll = 1;
            for(Iv=0;Iv<P->Nv;Iv++) {
               V = M->V[P->V[Iv]];
               for(i=0;i<3;i++) {
                  if (V[i] < OC->center[i]) AllPos[i] = 0;
                  else AllNeg[i] = 0;
               }
            }
            if (AllPos[0]) {
               if (AllPos[1]) {
                  if (AllPos[2]) {
                     Ic = 7;
                     NoChildHasAll = 0;
                  }
                  else if (AllNeg[2]) {
                     Ic = 6;
                     NoChildHasAll = 0;
                  }
               }
               else if (AllNeg[1]) {
                  if (AllPos[2]) {
                     Ic = 5;
                     NoChildHasAll = 0;
                  }
                  else if (AllNeg[2]) {
                     Ic = 4;
                     NoChildHasAll = 0;
                  }
               }
            }
            else if (AllNeg[0]) {
               if (AllPos[1]) {
                  if (AllPos[2]) {
                     Ic = 3;
                     NoChildHasAll = 0;
                  }
                  else if (AllNeg[2]) {
                     Ic = 2;
                     NoChildHasAll = 0;
                  }
               }
               else if (AllNeg[1]) {
                  if (AllPos[2]) {
                     Ic = 1;
                     NoChildHasAll = 0;
                  }
                  else if (AllNeg[2]) {
                     Ic = 0;
                     NoChildHasAll = 0;
                  }
               }
            }
            Io = OC->Child[Ic];
         }
         OC->Npoly++;
         OC->Poly = (long *) realloc(OC->Poly,OC->Npoly*sizeof(long));
         OC->Poly[OC->Npoly-1] = Ipoly;
      }

      for(i=73;i<585;i++) {
         if (O->OctCell[i].Npoly == 0) O->OctCell[i].IsEmpty = 1;
      }
      for(i=72;i>=0;i--) {
         OC = &O->OctCell[i];
         if (OC->Npoly == 0) {
            OC->IsEmpty = 1;
            for(j=0;j<8;j++) {
               C = &O->OctCell[OC->Child[j]];
               if (!C->IsEmpty) OC->IsEmpty = 0;
            }
         }
      }

      O->Noct = 0;
      for(i=0;i<585;i++) {
         if (!O->OctCell[i].IsEmpty) {
            O->Noct++;
            O->OctIdx = (long *) realloc(O->OctIdx,O->Noct*sizeof(long));
            O->OctIdx[O->Noct-1] = i;
         }
      }
}
/*********************************************************************/
/* Point and DirVec have already been transformed into Mesh frame      */
long OCProjectRayOntoMesh(double Point[3],double DirVec[3],
   struct MeshType *M,double ProjPoint[3],long *ClosestPoly)
{
      struct OctreeType *O;
      struct PolyType *P;
      struct OctreeCellType *OC;
      double Point2[3],Dist,Vec[3],dr[3];
      double MinDist,RoD;
      static double **Vtx;
      long Exhausted,Ip,Iv,InPoly,i;
      long FoundPoly;
      static long First = 1;

      if (First) {
         First = 0;
         Vtx = CreateMatrix(3,3);
      }

      O = M->Octree;

      for(i=0;i<3;i++) Point2[i] = Point[i] + DirVec[i];

      OC = &O->OctCell[0];
      MinDist = 1.0E9;
      *ClosestPoly = 0;
      Exhausted = 0;
      FoundPoly = 0;
      while(!Exhausted) {
         for(i=0;i<3;i++) dr[i] = OC->center[i]-Point[i];
         RoD = VoV(dr,DirVec);
         if (RoD > 0.0) {
            Dist = DistanceToLine(Point,Point2,OC->center,Vec);
            if (Dist < OC->radius) {
               /* Check against polys */
               for(Ip=0;Ip<OC->Npoly;Ip++) {
                  P = &M->Poly[OC->Poly[Ip]];
                  if (P->Nv != 3) {
                     printf("Error.  ProjectPointOntoMesh doesn't handle polygons with %ld vertices.\n",P->Nv);
                     exit(1);
                  }
                  for(Iv=0;Iv<P->Nv;Iv++) {
                     for(i=0;i<3;i++) Vtx[Iv][i] = M->V[P->V[Iv]][i];
                  }
                  InPoly = ProjectPointOntoPoly(Point,DirVec,Vtx,P->Nv,Vec,&Dist);
                  if (InPoly && Dist > 0.0 && Dist < MinDist) {
                     FoundPoly = 1;
                     MinDist = Dist;
                     *ClosestPoly = Ip;
                     for(i=0;i<3;i++) ProjPoint[i] = Vec[i];
                  }
               }
               /* Next Cell on Hit */
               if (OC->NextOnHit == 0) {
                  Exhausted = 1;
               }
               else {
                  OC = &O->OctCell[OC->NextOnHit];
               }
            }
            else if (OC->NextOnMiss == 0) {
               Exhausted = 1;
            }
            else {
               OC = &O->OctCell[OC->NextOnMiss];
            }
         }
         else if (OC->NextOnMiss == 0) {
            Exhausted = 1;
         }
         else {
            OC = &O->OctCell[OC->NextOnMiss];
         }
      }

      return(FoundPoly);
}
/*********************************************************************/
void FinalizeMesh(struct MeshType *M, long EdgesEnabled)
{
      long i,j,Ip,Iv,Ie,Ivtx,Ipoly;
      long V1,V2;
      long BeenHereOnce;
      double r[3],magr;
      double V[3];
      struct PolyType *P;
      void *Ptr;

      /* Find Bounding Box */
      for(j=0;j<3;j++) {
         M->BBox.max[j] = M->V[0][j];
         M->BBox.min[j] = M->V[0][j];
      }
      for(i=1;i<M->Nv;i++) {
         for(j=0;j<3;j++) {
            if (M->V[i][j] < M->BBox.min[j]) M->BBox.min[j] = M->V[i][j];
            if (M->V[i][j] > M->BBox.max[j]) M->BBox.max[j] = M->V[i][j];
         }
      }
      /* Expand BBox slightly to make sure all Vtx's are inside it */
      for(j=0;j<3;j++) {
         M->BBox.max[j] += 0.01;
         M->BBox.min[j] -= 0.01;
      }
      for(j=0;j<3;j++) {
         M->BBox.center[j] = 0.5*(M->BBox.min[j]+M->BBox.max[j]);
      }
      for(j=0;j<3;j++) {
         r[j] = M->V[0][j]-M->BBox.center[j];
      }
      M->BBox.radius = MAGV(r);
      for(i=1;i<M->Nv;i++) {
         for(j=0;j<3;j++) {
            r[j] = M->V[i][j]-M->BBox.center[j];
         }
         if (MAGV(r) > M->BBox.radius) M->BBox.radius = MAGV(r);
      }

      for(i=0;i<M->Nvn;i++) UNITV(M->Vn[i]);

      if (EdgesEnabled) {
         /* Build Edge Tables */
         M->Nedge = 0;
         for(Ip=0;Ip<M->Npoly;Ip++) {
            P = &M->Poly[Ip];
            P->E = (long *) calloc(P->Nv,sizeof(long));
            for(Iv=0;Iv<P->Nv;Iv++) {
               V1 = P->V[Iv];
               V2 = P->V[(Iv+1)%P->Nv];
               BeenHereOnce = 0;
               for(Ie=0;Ie<M->Nedge;Ie++) {
                  if (M->Edge[Ie].Vtx1 == V2) {
                     if (M->Edge[Ie].Vtx2 == V1) {
                        BeenHereOnce = 1;
                        M->Edge[Ie].Poly2 = Ip;
                        P->E[Iv] = Ie;
                        break;
                     }
                  }
               }
               if (!BeenHereOnce) {
                  M->Nedge++;
                  if (M->Nedge == 1) {
                     Ptr = calloc(1,sizeof(struct EdgeType));
                     if (Ptr == NULL) {
                        printf("Realloc failed in FinalizeMesh\n");
                        exit(1);
                     }
                  }
                  else {
                     Ptr = realloc(M->Edge,M->Nedge*sizeof(struct EdgeType));
                     if (Ptr == NULL) {
                        printf("Realloc failed in FinalizeMesh\n");
                        exit(1);
                     }
                  }
                  M->Edge = (struct EdgeType *) Ptr;
                  M->Edge[M->Nedge-1].Vtx1 = V1;
                  M->Edge[M->Nedge-1].Vtx2 = V2;
                  M->Edge[M->Nedge-1].Poly1 = Ip;
                  M->Edge[M->Nedge-1].Poly2 = -1;
                  for(i=0;i<3;i++) V[i] = M->V[V1][i] - M->V[V2][i];
                  M->Edge[M->Nedge-1].Length = MAGV(V);
                  P->E[Iv] = M->Nedge-1;
               }
            }
         }
      }

      /* Find Normals, Areas, Centroids for use in surface force models */
      SurfaceForceProps(M);

      /* For polyhedron gravity */
      if (EdgesEnabled) EdgeAndPolyDyads(M);

      /* Find radius of bounding sphere for each poly */
      for(Ipoly=0;Ipoly<M->Npoly;Ipoly++) {
         P = &M->Poly[Ipoly];
         P->radius = 0.0;
         for(Iv=0;Iv<P->Nv;Iv++) {
            Ivtx = P->V[Iv];
            for(i=0;i<3;i++) r[i] = M->V[Ivtx][i] - P->Centroid[i];
            magr = MAGV(r);
            if (magr > P->radius) P->radius = magr;
         }
      }
}
/*********************************************************************/
static void TransformPoint(const float *m, const float *in, float *out)
{
      out[0] = m[0]*in[0] + m[4]*in[1] + m[8]*in[2]  + m[12];
      out[1] = m[1]*in[0] + m[5]*in[1] + m[9]*in[2]  + m[13];
      out[2] = m[2]*in[0] + m[6]*in[1] + m[10]*in[2] + m[14];
}
/*********************************************************************/
static void TransformNormal(const float *m, const float *in, float *out)
{
      out[0] = m[0]*in[0] + m[4]*in[1] + m[8]*in[2];
      out[1] = m[1]*in[0] + m[5]*in[1] + m[9]*in[2];
      out[2] = m[2]*in[0] + m[6]*in[1] + m[10]*in[2];
}
/*********************************************************************/
static cgltf_accessor *FindAttribute(cgltf_primitive *prim,
   cgltf_attribute_type type)
{
      cgltf_size k;
      for (k = 0; k < prim->attributes_count; k++) {
         if (prim->attributes[k].type == type)
            return(prim->attributes[k].data);
      }
      return(NULL);
}
/*********************************************************************/
static void UriToPpm(const char *uri, char *dst, long dstlen)
{
      const char *dot;
      long baselen;

      dot = strrchr(uri, '.');
      if (dot != NULL) {
         baselen = (long)(dot - uri);
         if (baselen + 4 >= dstlen) baselen = dstlen - 5;
         strncpy(dst, uri, baselen);
         strcpy(dst + baselen, ".ppm");
      }
      else {
         strncpy(dst, uri, dstlen - 1);
         dst[dstlen - 1] = 0;
      }
}
/*********************************************************************/
static long MapGltfMaterial(cgltf_material *gmat,
   struct MatlType **MatlPtr, long *Nmatl)
{
      struct MatlType *Matl;
      long idx, i;
      float kd[4], ks[4], ke[4];
      float metallic, roughness, ns;
      cgltf_pbr_metallic_roughness *pbr;

      Matl = *MatlPtr;

      /* Check if this material was already mapped (by label) */
      if (gmat->name != NULL) {
         for (idx = 0; idx < *Nmatl; idx++) {
            if (!strcmp(gmat->name, Matl[idx].Label))
               return(idx);
         }
      }

      /* Add new material */
      idx = *Nmatl;
      (*Nmatl)++;
      Matl = (struct MatlType *) realloc(Matl, (*Nmatl) * sizeof(struct MatlType));
      *MatlPtr = Matl;
      memset(&Matl[idx], 0, sizeof(struct MatlType));

      if (gmat->name != NULL) {
         strncpy(Matl[idx].Label, gmat->name, 39);
         Matl[idx].Label[39] = 0;
      }
      else {
         sprintf(Matl[idx].Label, "gltf_mat_%ld", idx);
      }

      /* Default diffuse white */
      for (i = 0; i < 4; i++) kd[i] = 1.0f;
      for (i = 0; i < 4; i++) ks[i] = 0.0f;
      for (i = 0; i < 4; i++) ke[i] = 0.0f;
      metallic = 0.0f;
      roughness = 1.0f;

      if (gmat->has_pbr_metallic_roughness) {
         pbr = &gmat->pbr_metallic_roughness;
         for (i = 0; i < 4; i++) kd[i] = pbr->base_color_factor[i];
         metallic = pbr->metallic_factor;
         roughness = pbr->roughness_factor;

         /* Color texture */
         if (pbr->base_color_texture.texture != NULL &&
             pbr->base_color_texture.texture->image != NULL &&
             pbr->base_color_texture.texture->image->uri != NULL) {
            UriToPpm(pbr->base_color_texture.texture->image->uri,
               Matl[idx].ColorTexFileName, 40);
         }
         else {
            strcpy(Matl[idx].ColorTexFileName, "NONE");
         }
      }
      else {
         strcpy(Matl[idx].ColorTexFileName, "NONE");
      }

      /* Normal (bump) texture */
      if (gmat->normal_texture.texture != NULL &&
          gmat->normal_texture.texture->image != NULL &&
          gmat->normal_texture.texture->image->uri != NULL) {
         UriToPpm(gmat->normal_texture.texture->image->uri,
            Matl[idx].BumpTexFileName, 40);
      }
      else {
         strcpy(Matl[idx].BumpTexFileName, "NONE");
      }

      /* Emissive */
      for (i = 0; i < 3; i++) ke[i] = gmat->emissive_factor[i];
      ke[3] = 1.0f;

      /* PBR -> Phong approximation */
      ns = (1.0f - roughness) * 128.0f;
      if (ns < 1.0f) ns = 1.0f;

      for (i = 0; i < 4; i++) Matl[idx].Kd[i] = kd[i];
      for (i = 0; i < 4; i++) Matl[idx].Ka[i] = 0.2f * kd[i];
      Matl[idx].Ka[3] = kd[3];
      for (i = 0; i < 3; i++) ks[i] = metallic;
      ks[3] = 1.0f;
      for (i = 0; i < 4; i++) Matl[idx].Ks[i] = ks[i];
      for (i = 0; i < 4; i++) Matl[idx].Ke[i] = ke[i];
      Matl[idx].Ns = ns;
      Matl[idx].Refl = metallic * (1.0f - roughness);

      /* Spectrum defaults */
      strcpy(Matl[idx].SpectrumName, "NONE");

      /* Solar pressure defaults */
      Matl[idx].SpecFrac = (double)metallic;
      Matl[idx].DiffFrac = 1.0 - Matl[idx].SpecFrac;
      if (Matl[idx].SpecFrac + Matl[idx].DiffFrac > 1.0) {
         Matl[idx].DiffFrac = 1.0 - Matl[idx].SpecFrac;
      }

      return(idx);
}
/*********************************************************************/
static struct MeshType *LoadGltfFile(const char *ModelPath,
   const char *GltfFilename,
   struct MatlType **MatlPtr, long *Nmatl,
   struct MeshType *Mesh, long *Nmesh, long *MeshTag,
   long EdgesEnabled)
{
      long Ng, Ig;
      struct MeshType *M;
      cgltf_options options;
      cgltf_data *data = NULL;
      cgltf_result result;
      char filepath[512];
      cgltf_size in, ip;
      long TotalVtx, TotalNorm, TotalTex, TotalTri;
      long Ivtx, Ivn, Ivt, Ipoly;
      cgltf_accessor *pos_acc, *norm_acc, *tex_acc;
      float worldmat[16];
      long VtxOffset, NormOffset, TexOffset;
      long MatlIdx;

      Ng = *Nmesh;

      /* Check for prior definition */
      for (Ig = 0; Ig < Ng; Ig++) {
         if (!strcmp(GltfFilename, Mesh[Ig].ObjFileName)) {
            *MeshTag = Ig;
            return(Mesh);
         }
      }

      /* Add to list */
      Ng++;
      Mesh = (struct MeshType *) realloc(Mesh, Ng * sizeof(struct MeshType));
      M = &Mesh[Ng - 1];
      memset(M, 0, sizeof(struct MeshType));

      strncpy(M->ObjFileName, GltfFilename, 39);
      M->ObjFileName[39] = 0;

      /* Allow a Null Mesh entry */
      if (!strcmp(M->ObjFileName, "NONE")) {
         *Nmesh = Ng;
         *MeshTag = Ng - 1;
         return(Mesh);
      }

      M->Matl = (long *) calloc(2, sizeof(long));
      if (M->Matl == NULL) {
         printf("M->Matl calloc returned null pointer.  Bailing out!\n");
         exit(1);
      }

      /* Build full path */
      sprintf(filepath, "%s%s", ModelPath, GltfFilename);
      printf("glTF: Loading %s\n", filepath);

      /* Parse glTF file */
      memset(&options, 0, sizeof(options));
      result = cgltf_parse_file(&options, filepath, &data);
      if (result != cgltf_result_success) {
         printf("cgltf_parse_file failed for %s (error %d)\n",
            filepath, (int)result);
         *Nmesh = Ng;
         *MeshTag = Ng - 1;
         return(Mesh);
      }

      result = cgltf_load_buffers(&options, data, filepath);
      if (result != cgltf_result_success) {
         printf("cgltf_load_buffers failed for %s (error %d)\n",
            filepath, (int)result);
         cgltf_free(data);
         *Nmesh = Ng;
         *MeshTag = Ng - 1;
         return(Mesh);
      }

      /* First pass: count vertices, normals, texcoords, triangles */
      TotalVtx = 0;
      TotalNorm = 0;
      TotalTex = 0;
      TotalTri = 0;

      for (in = 0; in < data->nodes_count; in++) {
         cgltf_node *node = &data->nodes[in];
         if (node->mesh == NULL) continue;
         for (ip = 0; ip < node->mesh->primitives_count; ip++) {
            cgltf_primitive *prim = &node->mesh->primitives[ip];
            if (prim->type != cgltf_primitive_type_triangles) {
               printf("Warning: glTF primitive type %d not supported, "
                  "skipping (file %s)\n", (int)prim->type, GltfFilename);
               continue;
            }

            pos_acc = FindAttribute(prim, cgltf_attribute_type_position);
            norm_acc = FindAttribute(prim, cgltf_attribute_type_normal);
            tex_acc = FindAttribute(prim, cgltf_attribute_type_texcoord);

            if (pos_acc != NULL) TotalVtx += (long)pos_acc->count;
            if (norm_acc != NULL) TotalNorm += (long)norm_acc->count;
            if (tex_acc != NULL) TotalTex += (long)tex_acc->count;

            if (prim->indices != NULL) {
               TotalTri += (long)(prim->indices->count / 3);
            }
            else if (pos_acc != NULL) {
               TotalTri += (long)(pos_acc->count / 3);
            }
         }
      }

      printf("glTF: Counted %ld vertices, %ld normals, %ld texcoords, %ld triangles\n",
         TotalVtx, TotalNorm, TotalTex, TotalTri);

      if (TotalVtx == 0) {
         printf("Warning: No triangle vertices found in %s\n", GltfFilename);
         cgltf_free(data);
         *Nmesh = Ng;
         *MeshTag = Ng - 1;
         return(Mesh);
      }

      /* Allocate arrays */
      M->Nv = TotalVtx;
      M->Nvn = TotalNorm;
      M->Nvt = TotalTex;
      M->Npoly = TotalTri;
      M->Nedge = 0;

      M->V = CreateMatrix(M->Nv, 3);
      if (M->Nvt > 0) M->Vt = CreateMatrix(M->Nvt, 2);
      if (M->Nvn > 0) M->Vn = CreateMatrix(M->Nvn, 3);
      M->Poly = (struct PolyType *) calloc(M->Npoly, sizeof(struct PolyType));
      if (M->Poly == NULL) {
         printf("M->Poly calloc returned null pointer.  Bailing out!\n");
         exit(1);
      }

      /* Second pass: extract data */
      Ivtx = 0;
      Ivn = 0;
      Ivt = 0;
      Ipoly = 0;

      for (in = 0; in < data->nodes_count; in++) {
         cgltf_node *node = &data->nodes[in];
         if (node->mesh == NULL) continue;

         cgltf_node_transform_world(node, worldmat);

         for (ip = 0; ip < node->mesh->primitives_count; ip++) {
            cgltf_primitive *prim = &node->mesh->primitives[ip];
            if (prim->type != cgltf_primitive_type_triangles) continue;

            pos_acc = FindAttribute(prim, cgltf_attribute_type_position);
            norm_acc = FindAttribute(prim, cgltf_attribute_type_normal);
            tex_acc = FindAttribute(prim, cgltf_attribute_type_texcoord);

            VtxOffset = Ivtx;
            NormOffset = Ivn;
            TexOffset = Ivt;

            /* Extract positions */
            if (pos_acc != NULL) {
               float *posbuf;
               long v;

               posbuf = (float *)malloc(pos_acc->count * 3 * sizeof(float));
               cgltf_accessor_unpack_floats(pos_acc, posbuf, pos_acc->count * 3);
               for (v = 0; v < (long)pos_acc->count; v++) {
                  float xf[3];
                  TransformPoint(worldmat, &posbuf[v * 3], xf);
                  M->V[Ivtx][0] = (double)xf[0];
                  M->V[Ivtx][1] = (double)xf[1];
                  M->V[Ivtx][2] = (double)xf[2];
                  Ivtx++;
               }
               free(posbuf);
            }

            /* Extract normals */
            if (norm_acc != NULL) {
               float *normbuf;
               cgltf_size count;
               long v;

               count = norm_acc->count;
               normbuf = (float *)malloc(count * 3 * sizeof(float));
               cgltf_accessor_unpack_floats(norm_acc, normbuf, count * 3);
               for (v = 0; v < (long)count; v++) {
                  float xf[3];
                  TransformNormal(worldmat, &normbuf[v * 3], xf);
                  M->Vn[Ivn][0] = (double)xf[0];
                  M->Vn[Ivn][1] = (double)xf[1];
                  M->Vn[Ivn][2] = (double)xf[2];
                  Ivn++;
               }
               free(normbuf);
            }

            /* Extract texcoords */
            if (tex_acc != NULL) {
               float *texbuf;
               cgltf_size count;
               long v;

               count = tex_acc->count;
               texbuf = (float *)malloc(count * 2 * sizeof(float));
               cgltf_accessor_unpack_floats(tex_acc, texbuf, count * 2);
               for (v = 0; v < (long)count; v++) {
                  M->Vt[Ivt][0] = (double)texbuf[v * 2];
                  M->Vt[Ivt][1] = (double)texbuf[v * 2 + 1];
                  Ivt++;
               }
               free(texbuf);
            }

            /* Map material */
            MatlIdx = 0;
            if (prim->material != NULL) {
               MatlIdx = MapGltfMaterial(prim->material, MatlPtr, Nmatl);
               /* Track materials used by this mesh */
               {
                  long Im, FirstUse = 1;
                  for (Im = 0; Im < M->Nmatl; Im++) {
                     if (MatlIdx == M->Matl[Im]) FirstUse = 0;
                  }
                  if (FirstUse) {
                     M->Nmatl++;
                     if (M->Nmatl > 2)
                        M->Matl = (long *) realloc(M->Matl, M->Nmatl * sizeof(long));
                     M->Matl[M->Nmatl - 1] = MatlIdx;
                  }
               }
            }

            /* Build triangles */
            if (prim->indices != NULL) {
               long tri, ntri;
               ntri = (long)(prim->indices->count / 3);
               for (tri = 0; tri < ntri; tri++) {
                  struct PolyType *P = &M->Poly[Ipoly];
                  long i0, i1, i2;

                  i0 = (long)cgltf_accessor_read_index(prim->indices, tri * 3);
                  i1 = (long)cgltf_accessor_read_index(prim->indices, tri * 3 + 1);
                  i2 = (long)cgltf_accessor_read_index(prim->indices, tri * 3 + 2);

                  P->Nv = 3;
                  P->Matl = MatlIdx;
                  P->ContactPoly = 0;
                  P->V = (long *) calloc(3, sizeof(long));
                  P->Vt = (long *) calloc(3, sizeof(long));
                  P->Vn = (long *) calloc(3, sizeof(long));

                  P->V[0] = VtxOffset + i0;
                  P->V[1] = VtxOffset + i1;
                  P->V[2] = VtxOffset + i2;

                  if (norm_acc != NULL) {
                     P->Vn[0] = NormOffset + i0;
                     P->Vn[1] = NormOffset + i1;
                     P->Vn[2] = NormOffset + i2;
                     P->HasNorm = 1;
                  }

                  if (tex_acc != NULL) {
                     P->Vt[0] = TexOffset + i0;
                     P->Vt[1] = TexOffset + i1;
                     P->Vt[2] = TexOffset + i2;
                     P->HasTex = 1;
                  }

                  if (PolyIsDegenerate(P, M->V)) {
                     free(P->V);
                     free(P->Vt);
                     free(P->Vn);
                     M->Npoly--;
                  }
                  else {
                     Ipoly++;
                  }
               }
            }
            else if (pos_acc != NULL) {
               /* Non-indexed: every 3 vertices form a triangle */
               long tri, ntri;
               ntri = (long)(pos_acc->count / 3);
               for (tri = 0; tri < ntri; tri++) {
                  struct PolyType *P = &M->Poly[Ipoly];

                  P->Nv = 3;
                  P->Matl = MatlIdx;
                  P->ContactPoly = 0;
                  P->V = (long *) calloc(3, sizeof(long));
                  P->Vt = (long *) calloc(3, sizeof(long));
                  P->Vn = (long *) calloc(3, sizeof(long));

                  P->V[0] = VtxOffset + tri * 3;
                  P->V[1] = VtxOffset + tri * 3 + 1;
                  P->V[2] = VtxOffset + tri * 3 + 2;

                  if (norm_acc != NULL) {
                     P->Vn[0] = NormOffset + tri * 3;
                     P->Vn[1] = NormOffset + tri * 3 + 1;
                     P->Vn[2] = NormOffset + tri * 3 + 2;
                     P->HasNorm = 1;
                  }

                  if (tex_acc != NULL) {
                     P->Vt[0] = TexOffset + tri * 3;
                     P->Vt[1] = TexOffset + tri * 3 + 1;
                     P->Vt[2] = TexOffset + tri * 3 + 2;
                     P->HasTex = 1;
                  }

                  if (PolyIsDegenerate(P, M->V)) {
                     free(P->V);
                     free(P->Vt);
                     free(P->Vn);
                     M->Npoly--;
                  }
                  else {
                     Ipoly++;
                  }
               }
            }
         }
      }

      printf("glTF: Extracted %ld vertices, %ld normals, %ld texcoords, %ld polys\n",
         Ivtx, Ivn, Ivt, Ipoly);
      printf("glTF: M->Nv=%ld M->Nvn=%ld M->Nvt=%ld M->Npoly=%ld\n",
         M->Nv, M->Nvn, M->Nvt, M->Npoly);
      fflush(stdout);

      FinalizeMesh(M, EdgesEnabled);

      printf("glTF: FinalizeMesh complete\n");
      fflush(stdout);

      cgltf_free(data);

      *Nmesh = Ng;
      *MeshTag = Ng - 1;
      return(Mesh);
}
/*********************************************************************/
struct MeshType *LoadWingsObjFile(const char *ModelPath,const char *ObjFilename,
                       struct MatlType **MatlPtr, long *Nmatl,
                       struct MeshType *Mesh, long *Nmesh, long *MeshTag,
                       long EdgesEnabled)
{
#define D2R (0.0174532925199433)
      FILE *infile,*outfile;
      FILE *TmpFile;
      char *txtptr;
      double V[3];
      long Ng,Ig,Im;
      long I,It,In,i,j,MatlIdx;
      long Ivtx,Ivt,Ivn,Ipoly;
      struct MeshType *M;
      struct PolyType *P;
      struct MatlType *Matl;
      long NoArraySizesFound;
      double Value,Scale = 1.0;
      double Val1,Val2,Val3;
      char response[40];
      long Seq;
      double RotM[3][3] = {{1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0}};
      double TransVec[3] = {0.0,0.0,0.0};
      double Vr[3];
      long FirstUse;

      char line[512],vtxstring[512],*vtxtoken,MatlName[40];
      char MtlLibName[40];

      Ng = *Nmesh;
      Matl = *MatlPtr;

      /* Check for prior definition */
      for(Ig=0;Ig<Ng;Ig++) {
         if (!strcmp(ObjFilename,Mesh[Ig].ObjFileName)) {
            *MeshTag = Ig;
            return(Mesh);
         }
      }
      /* If not defined yet, add to list */
      Ng++;
      Mesh = (struct MeshType *) realloc(Mesh,Ng*sizeof(struct MeshType));
      M = &Mesh[Ng-1];

      strncpy(M->ObjFileName,ObjFilename,39);
      M->ObjFileName[39] = 0; /* Null-terminated string */
      M->Nmatl = 0;
      M->Nv = 0;
      M->Nvt = 0;
      M->Nvn = 0;
      M->Npoly = 0;
      M->Nedge = 0;
      MatlIdx = 0;

      /* Allow a Null Mesh entry */
      if (!strcmp(M->ObjFileName,"NONE")) {
         *Nmesh = Ng;
         *MeshTag = Ng-1;
         return(Mesh);
      }

      /* These will be expanded as needed */
      M->Matl = (long *) calloc(2,sizeof(long));
      if (M->Matl == NULL) {
         printf("M->Matl calloc returned null pointer.  Bailing out!\n");
         exit(1);
      }

      infile=FileOpen(ModelPath,ObjFilename,"rt");

/* .. First pass just to count things up */
      NoArraySizesFound = 1;
      while(!feof(infile) && NoArraySizesFound) {
         fgets(line,512,infile);
         if (sscanf(line,"# Nv = %ld  Nvt = %ld  Nvn = %ld  Npoly = %ld",
            &M->Nv,&M->Nvt,&M->Nvn,&M->Npoly) == 4) {
            NoArraySizesFound = 0;
         }
         else if (sscanf(line,"v  %lf %lf %lf",&V[0],&V[1],&V[2]) == 3) {
            M->Nv++;
         }
         else if (sscanf(line,"vt %lf %lf",&V[0],&V[1]) == 2 ||
                  sscanf(line,"vt %lf %lf %lf",&V[0],&V[1],&V[2]) == 3) {
            M->Nvt++;
         }
         else if (sscanf(line,"vn  %lf %lf %lf",&V[0],&V[1],&V[2]) == 3) {
            M->Nvn++;
         }
         else if(line[0] == 'f') {
            M->Npoly++;
         }
         sprintf(line,"Flush");
      }
      rewind(infile);

/* .. Insert comment to save future work */
      if (NoArraySizesFound) {
         TmpFile = tmpfile();
         if (TmpFile != NULL) { /* Windows seems to object.  This is a workaround. */
            while((txtptr = fgets(line,512,infile)) != NULL) {
               fputs(line,TmpFile);
            }
            rewind(infile);
            rewind(TmpFile);
            outfile = FileOpen(ModelPath,ObjFilename,"w");
            fprintf(outfile,"# Nv = %ld  Nvt = %ld  Nvn = %ld  Npoly = %ld\n",
               M->Nv,M->Nvt,M->Nvn,M->Npoly);
            while((txtptr = fgets(line,512,TmpFile)) != NULL) {
               fputs(line,outfile);
            }
            fclose(outfile);
            fclose(TmpFile);
         }
      }

/* .. Allocate arrays */
      M->V = CreateMatrix(M->Nv,3);
      M->Vt = CreateMatrix(M->Nvt,2);
      M->Vn = CreateMatrix(M->Nvn,3);
      M->Poly = (struct PolyType *) calloc(M->Npoly,sizeof(struct PolyType));
      if (M->Poly == NULL) {
         printf("M->Poly calloc returned null pointer.  Bailing out!\n");
         exit(1);
      }

/* .. Second pass to read things in */
      Ivtx = 0;
      Ivt = 0;
      Ivn = 0;
      Ipoly = 0;
      while(!feof(infile)) {
         fgets(line,512,infile);
         if (sscanf(line,"# Scale up by %lf to actual size",&Value) == 1) {
            Scale = Value;
         }
         else if (sscanf(line,"# Scale down by %lf to actual size",&Value) == 1) {
            Scale = 1.0/Value;
         }
         else if (sscanf(line,"# Units = %s",response) == 1) {
            if (!strncmp(response,"mm",2)) Scale = 0.001;
            else if (!strncmp(response,"in",2)) Scale = 0.0254;
            else if (!strncmp(response,"ft",2)) Scale = 0.3048;
         }
         else if (sscanf(line,"# Translate by [%lf %lf %lf]",
            &Val1,&Val2,&Val3) == 3) {
            TransVec[0] = Val1;
            TransVec[1] = Val2;
            TransVec[2] = Val3;
         }
         else if (sscanf(line,"# Rotate via Seq = %ld by [%lf %lf %lf] deg",
            &Seq,&Val1,&Val2,&Val3) == 4) {
            A2C(Seq,Val1*D2R,Val2*D2R,Val3*D2R,RotM);
         }
         else if (sscanf(line,"v  %lf %lf %lf",&V[0],&V[1],&V[2]) == 3) {
            MTxV(RotM,V,Vr);
            for(i=0;i<3;i++) M->V[Ivtx][i] = Scale*Vr[i]+TransVec[i];
            Ivtx++;
         }
         else if (sscanf(line,"vt %lf %lf",&V[0],&V[1]) == 2 ||
                  sscanf(line,"vt %lf %lf %lf",&V[0],&V[1],&V[2]) == 3) {
            M->Vt[Ivt][0] = V[0];
            M->Vt[Ivt][1] = (1.0-V[1]); /* Flip about horizontal axis */
            Ivt++;
         }
         else if (sscanf(line,"vn  %lf %lf %lf",&V[0],&V[1],&V[2]) == 3) {
            if (V[0] == 0 && V[1] == 0 && V[2] == 0) {
               for(i=0;i<3;i++) {
                  V[i] = 1.0;  /* Kludge.  Who defines a zero-length normal?? */
               }
               /* printf("Zero-length normal in LoadWingsObjFile %s\n",ObjFilename); */
            }
            for(i=0;i<3;i++) M->Vn[Ivn][i] = V[i];
            Ivn++;
         }
         else if(line[0] == 'f') {
            P = &M->Poly[Ipoly];
            P->Nv = 0;
            P->Matl = MatlIdx;
            P->ContactPoly = 0;
            P->V = (long *) calloc(3,sizeof(long));
            if (P->V == NULL) {
               printf("P->V calloc returned null pointer.  Bailing out!\n");
               exit(1);
            }
            P->Vt = (long *) calloc(3,sizeof(long));
            if (P->Vt == NULL) {
               printf("P->Vt calloc returned null pointer.  Bailing out!\n");
               exit(1);
            }
            P->Vn = (long *) calloc(3,sizeof(long));
            if (P->Vn == NULL) {
               printf("P->Vn calloc returned null pointer.  Bailing out!\n");
               exit(1);
            }
            strcpy(vtxstring,line);
            vtxtoken = strtok(vtxstring," "); /* Discards "f" */
            while((vtxtoken = strtok(NULL," ")) != NULL) {
               P->Nv++;
               if (P->Nv > 3) {
                  P->V = (long *) realloc(P->V,P->Nv*sizeof(long));
                  P->Vt = (long *) realloc(P->Vt,P->Nv*sizeof(long));
                  P->Vn = (long *) realloc(P->Vn,P->Nv*sizeof(long));
               }
               if (sscanf(vtxtoken,"%ld/%ld/%ld",&I,&It,&In) == 3) {
                  P->V[P->Nv-1] = I-1;
                  P->Vt[P->Nv-1] = It-1;
                  P->Vn[P->Nv-1] = In-1;
                  P->HasTex = 1;
                  P->HasNorm = 1;
               }
               else if (sscanf(vtxtoken,"%ld/%ld",&I,&It) == 2) {
                  P->V[P->Nv-1] = I-1;
                  P->Vt[P->Nv-1] = It-1;
                  P->Vn[P->Nv-1] = 0;
                  P->HasTex = 1;
                  P->HasNorm = 0;
               }
               else if (sscanf(vtxtoken,"%ld//%ld",&I,&In) == 2) {
                  P->V[P->Nv-1] = I-1;
                  P->Vt[P->Nv-1] = 0;
                  P->Vn[P->Nv-1] = In-1;
                  P->HasTex = 0;
                  P->HasNorm = 1;
               }
               else if (sscanf(vtxtoken,"%ld",&I) == 1) {
                  P->V[P->Nv-1] = I-1;
                  P->Vt[P->Nv-1] = 0;
                  P->Vn[P->Nv-1] = 0;
                  P->HasTex = 0;
                  P->HasNorm = 0;
               }
            }
            /* If poly is degenerate, remove it from Mesh */
            if (PolyIsDegenerate(P,M->V)) {
               free(P->V);
               free(P->Vt);
               free(P->Vn);
               M->Npoly--;
            }
            else {
               Ipoly++;
            }
         }
         else if (sscanf(line,"mtllib %s",MtlLibName) == 1) {
            Matl = AddMtlLib(ModelPath,MtlLibName,Matl,Nmatl);
            *MatlPtr = Matl;
            ScaleSpecDiffFrac(Matl,*Nmatl);
         }
         else if (sscanf(line,"usemtl %s",MatlName) == 1) {
            MatlIdx = 0;
            while (MatlIdx < *Nmatl &&
                   strcmp(MatlName,Matl[MatlIdx].Label))
                      MatlIdx++;
            if (MatlIdx >= *Nmatl) {
               printf("Material %s is not in Matl structure\n",MatlName);
               printf("Setting material to default\n");
               sprintf(MatlName,"default");
               MatlIdx = 0;
               while (MatlIdx < *Nmatl &&
                      strcmp(MatlName,Matl[MatlIdx].Label))
                         MatlIdx++;
               if (MatlIdx >= *Nmatl) {
                  printf("default material not found either\n");
                  exit(1);
               }
            }
            FirstUse = 1;
            for(Im=0;Im<M->Nmatl;Im++) {
               if(MatlIdx == M->Matl[Im]) FirstUse = 0;
            }
            if (FirstUse) {
               M->Nmatl++;
               if (M->Nmatl > 2)
                  M->Matl = (long *) realloc(M->Matl,M->Nmatl*sizeof(long));
               M->Matl[M->Nmatl-1] = MatlIdx;
            }
         }
         sprintf(line,"Flush");
      }
      fclose(infile);

      FinalizeMesh(M, EdgesEnabled);

      *Nmesh = Ng;
      *MeshTag = Ng-1;
      return(Mesh);
#undef D2R
}
/*********************************************************************/
struct MeshType *LoadMeshFile(const char *ModelPath, const char *MeshFilename,
                       struct MatlType **MatlPtr, long *Nmatl,
                       struct MeshType *Mesh, long *Nmesh, long *MeshTag,
                       long EdgesEnabled)
{
      const char *ext;

      ext = strrchr(MeshFilename, '.');
      if (ext != NULL &&
          (!strcmp(ext, ".gltf") || !strcmp(ext, ".glb"))) {
         return(LoadGltfFile(ModelPath, MeshFilename,
            MatlPtr, Nmatl, Mesh, Nmesh, MeshTag, EdgesEnabled));
      }
      return(LoadWingsObjFile(ModelPath, MeshFilename,
         MatlPtr, Nmatl, Mesh, Nmesh, MeshTag, EdgesEnabled));
}
/*********************************************************************/
void WriteMeshToObjFile(struct MatlType *Matl,struct MeshType *Mesh,const char *Path,
   const char *FileName)
{
      char MtlFileName[80],ObjFileName[80];
      FILE *MtlFile,*ObjFile;
      long Im,Iv,Ip;
      struct MatlType *M;
      double *V;
      struct PolyType *P;

      strcpy(MtlFileName,FileName);
      strcat(MtlFileName,".mtl");
      MtlFile = FileOpen(Path,MtlFileName,"wt");

      for(Im=0;Im<Mesh->Nmatl;Im++) {
         M = &Matl[Im];
         fprintf(MtlFile,"newmtl %s\n",M->Label);
         fprintf(MtlFile,"d %f\n",M->Kd[3]);
         fprintf(MtlFile,"Ns %f\n",M->Ns);
         fprintf(MtlFile,"Ka %f %f %f\n",M->Ka[0],M->Ka[1],M->Ka[2]);
         fprintf(MtlFile,"Kd %f %f %f\n",M->Kd[0],M->Kd[1],M->Kd[2]);
         fprintf(MtlFile,"Ks %f %f %f\n",M->Ks[0],M->Ks[1],M->Ks[2]);
         fprintf(MtlFile,"Ke %f %f %f\n",M->Ke[0],M->Ke[1],M->Ke[2]);
         if (strcmp(M->ColorTexFileName,"NONE")) {
            fprintf(MtlFile,"map_Kd %s\n",M->ColorTexFileName);
         }
         if (strcmp(M->BumpTexFileName,"NONE")) {
            fprintf(MtlFile,"map_Bump %s\n",M->BumpTexFileName);
         }
         if (M->Nu != M->Nv) {
            fprintf(MtlFile,"Nu %f\n",M->Nu);
            fprintf(MtlFile,"Nv %f\n",M->Nv);
         }
         if (M->Refl > 0.0) {
            fprintf(MtlFile,"Refl %f\n",M->Refl);
         }
         if (M->NoiseColEnabled) {
            fprintf(MtlFile,"NoiseCol\n");
         }
         if (M->NoiseBumpEnabled) {
            fprintf(MtlFile,"NoiseBump\n");
         }
         if (strcmp(M->SpectrumName,"NONE")) {
            fprintf(MtlFile,"map_Spectrum %s\n",M->SpectrumName);
         }
         if (M->NoiseColEnabled || M->NoiseBumpEnabled) {
            fprintf(MtlFile,"NoiseGain %f %f %f %f\n",
               M->NoiseGain[0],M->NoiseGain[1],M->NoiseGain[2],M->NoiseGain[3]);
            fprintf(MtlFile,"NoiseBias %f\n",M->NoiseBias);
            fprintf(MtlFile,"NoiseScale %f\n",M->NoiseScale);
            fprintf(MtlFile,"NoiseAxis %f %f %f\n",
               M->NoiseAxis[0],M->NoiseAxis[1],M->NoiseAxis[2]);
            fprintf(MtlFile,"NoiseType %d\n",M->NoiseType);
         }
         fprintf(MtlFile,"\n");
      }
      fclose(MtlFile);

      strcpy(ObjFileName,FileName);
      strcat(ObjFileName,".obj");
      ObjFile = FileOpen(Path,ObjFileName,"wt");

      fprintf(ObjFile,"# Nv = %ld  Nvt = %ld  Nvn = %ld  Npoly = %ld\n\n",
               Mesh->Nv,Mesh->Nvt,Mesh->Nvn,Mesh->Npoly);

      fprintf(ObjFile,"mtllib %s\n\n",MtlFileName);

      /* Vertices */
      for(Iv=0;Iv<Mesh->Nv;Iv++) {
         V = Mesh->V[Iv];
         fprintf(ObjFile,"v %lf %lf %lf\n",V[0],V[1],V[2]);
      }
      /* Texture Vertices */
      for(Iv=0;Iv<Mesh->Nvt;Iv++) {
         V = Mesh->Vt[Iv];
         fprintf(ObjFile,"vt %lf %lf %lf\n",V[0],V[1],V[2]);
      }
      /* Normals */
      for(Iv=0;Iv<Mesh->Nvn;Iv++) {
         V = Mesh->Vn[Iv];
         fprintf(ObjFile,"vn %lf %lf %lf\n",V[0],V[1],V[2]);
      }
      fprintf(ObjFile,"\n");

      for(Im=0;Im<Mesh->Nmatl;Im++) {
         M = &Matl[Im];
         fprintf(ObjFile,"usemtl %s\n",M->Label);
         for(Ip=0;Ip<Mesh->Npoly;Ip++) {
            P = &Mesh->Poly[Ip];
            if (P->Matl == Im) {
               fprintf(ObjFile,"f ");
               for(Iv=0;Iv<P->Nv;Iv++) {
                  fprintf(ObjFile,"%ld",P->V[Iv]+1);
                  fprintf(ObjFile,"/");
                  if (P->HasTex) fprintf(ObjFile,"%ld",P->Vt[Iv]+1);
                  fprintf(ObjFile,"/");
                  if (P->HasNorm) fprintf(ObjFile,"%ld",P->Vn[Iv]+1);
                  fprintf(ObjFile," ");
               }
               fprintf(ObjFile,"\n");
            }
         }
         fprintf(ObjFile,"\n");
      }
      fclose(ObjFile);

}

/* #ifdef __cplusplus
** }
** #endif
*/
