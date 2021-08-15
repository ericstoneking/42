/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


#include "geomkit.h"

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
void SurfaceForceProps(struct GeomType *G)
{
      double **uv;
      double uhat[3],v2[3],nhat[3],vhat[3];
      double C[3][3];
      double uvbar[3];
      double x1,y1,x2,y2,xA,yA;
      long Ip;
      long j;
      struct PolyType *P;

      for(Ip=0;Ip<G->Npoly;Ip++) {
         P = &G->Poly[Ip];

         uv = CreateMatrix(P->Nv+1,3);

         /* Compute Unit Normal Vector */
         for(j=0;j<3;j++) {
            uhat[j] = G->V[P->V[1]][j] - G->V[P->V[0]][j];
            v2[j]   = G->V[P->V[2]][j] - G->V[P->V[1]][j];
         }
         UNITV(uhat);
         VxV(uhat,v2,nhat);
         UNITV(nhat);
         VxV(nhat,uhat,vhat);
         UNITV(vhat);
         for(j=0;j<3;j++) P->Norm[j] = nhat[j];

         /* Compute in-plane basis vectors */
         PerpBasis(P->Norm,P->Uhat,P->Vhat);

         /* Compute Polygon Area and Centroid */
         for(j=0;j<3;j++){
            C[0][j] = uhat[j];
            C[1][j] = vhat[j];
            C[2][j] = nhat[j];
         }
         for(j=0;j<P->Nv;j++) MxV(C,G->V[P->V[j]],uv[j]);
         MxV(C,G->V[P->V[0]],uv[P->Nv]);
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
void EdgeAndPolyDyads(struct GeomType *G)
{
      struct EdgeType *E;
      struct PolyType *P1,*P2,*P;
      double *V1,*V2;
      double Axis[3],N1[3],N2[3];
      long Ie,Ip,i,j;

      for(Ie=0;Ie<G->Nedge;Ie++) {
         E = &G->Edge[Ie];
         if (E->Poly2 >= 0) {
            P1 = &G->Poly[E->Poly1];
            P2 = &G->Poly[E->Poly2];
            V1 = G->V[E->Vtx1];
            V2 = G->V[E->Vtx2];
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
      
      for(Ip=0;Ip<G->Npoly;Ip++) {
         P = &G->Poly[Ip];
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) {
               P->Dyad[i][j] = P->Norm[i]*P->Norm[j];
            }
         }
      }
}
/*********************************************************************/
double PolyhedronVolume(struct GeomType *G)
{
      double Vol;
      struct PolyType *P;
      double *V1,*V2,*V3;
      double V2xV3[3];
      long Ip;
      
      Vol = 0.0;
      for(Ip=0;Ip<G->Npoly;Ip++) {
         P = &G->Poly[Ip];
         V1 = G->V[P->V[0]];
         V2 = G->V[P->V[1]];
         V3 = G->V[P->V[2]];
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
   struct KDNodeType *KD, struct GeomType *G,
   long *HitPoly, double HitPoint[3], double *HitDist)
{
      struct PolyType *P;
      double **Vtx;
      long Hit,Ip,Iv,i;
      double Dist;
      double ProjPoint[3];

      Hit = 0;

      for(Ip=0;Ip<KD->Npoly;Ip++) {
         P = &G->Poly[KD->Poly[Ip]];
         Vtx = (double **) calloc(P->Nv,sizeof(double *));
         for(Iv=0;Iv<P->Nv;Iv++) {
            Vtx[Iv] = G->V[P->V[Iv]];
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
   struct GeomType *G, long *HitPoly, double HitPoint[3], double *HitDist)
{
      long HitLow,HitHigh;
      long Hit = 0;

      if (RayHitsBBox(Source,DirVec,&KD->BB)) {
         if(KD->IsLeaf) {
            Hit = KDRayHitsLeaf(Source,DirVec,KD,G,
               HitPoly,HitPoint,HitDist);
         }
         else { /* Recursively check child nodes */
            HitLow = KDRayHitsNode(Source,DirVec,KD->LowChild,G,
               HitPoly,HitPoint,HitDist);
            HitHigh = KDRayHitsNode(Source,DirVec,KD->HighChild,G,
               HitPoly,HitPoint,HitDist);
            Hit = HitLow || HitHigh;
         }
      }
      return(Hit);
}
/**********************************************************************/
/* Source and DirVec must be expressed in G's coordinate system       */
long KDProjectRayOntoGeom(double Source[3], double DirVec[3],
   struct GeomType *G, long *HitPoly, double HitPoint[3])
{
      double HitDist = 1.0E12; /* Absurd large value */
      long RayHitsGeom;

      RayHitsGeom = KDRayHitsNode(Source,DirVec,G->KDTree,G,HitPoly,
         HitPoint,&HitDist);

      return(RayHitsGeom);
}
/**********************************************************************/
long KDPartition(long *P, long LowEnd, long HighEnd, long Axis, struct GeomType *G)
{
      long PivotIdx,TempIdx;
      long LowIdx;
      double PivotVal;

      PivotIdx = HighEnd;
      PivotVal = G->Poly[P[HighEnd]].Centroid[Axis];
      LowIdx = LowEnd;

      while(LowIdx < PivotIdx) {
         if (G->Poly[P[LowIdx]].Centroid[Axis] > PivotVal) {
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
long KDSelectMedian(long *P, long N, long Axis, struct GeomType *G)
{
      long LowEnd,HighEnd,PivotIdx;

      LowEnd = 0;
      HighEnd = N-1;

      do {
         PivotIdx = KDPartition(P,LowEnd,HighEnd,Axis,G);
         if (PivotIdx > N/2) HighEnd = PivotIdx-1;
         else if (PivotIdx < N/2) LowEnd = PivotIdx+1;
      } while(PivotIdx != N/2);

      return(PivotIdx);
}
/**********************************************************************/
long KDCompare(long P1, long P2,long Axis,struct GeomType *G)
{
      if (G->Poly[P1].Centroid[Axis] < G->Poly[P2].Centroid[Axis])
         return(-1);
      else if (G->Poly[P1].Centroid[Axis] > G->Poly[P2].Centroid[Axis])
         return(1);
      else return(0);
}
/**********************************************************************/
void KDFormHeap(long *P, long L, long Axis, struct GeomType *G)
{
      long Done,k,Temp;

      do {
         Done = 1;
         for(k=0;k<L/2;k++) {
            /* Check against left child (2*k+1) */
            if (KDCompare(P[k],P[2*k+1],Axis,G) < 0) {
               Temp = P[k];
               P[k] = P[2*k+1];
               P[2*k+1] = Temp;
               Done = 0;
            }
            /* Check against right child (2*k+2) */
            if (2*k+2 < L) { /* Right child exists */
               if (KDCompare(P[k],P[2*k+2],Axis,G) < 0) {
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
void KDHeapSort(long *Poly, long Npoly, long Axis,struct GeomType *G)
{
      long HeapLength;
      long Temp;

      /* Form heap so each parent is greater than both children */
      KDFormHeap(Poly,Npoly,Axis,G);

      HeapLength = Npoly;
      while (HeapLength > 1) {
         /* Take largest element from top of heap */
         Temp = Poly[0];
         Poly[0] = Poly[HeapLength-1];
         Poly[HeapLength-1] = Temp;

         /* Shorten heap */
         HeapLength--;
         /* Re-form heap */
         KDFormHeap(Poly,HeapLength,Axis,G);
      }
}
/**********************************************************************/
void SplitKDNode(struct KDNodeType *KD,struct GeomType *G)
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
      KD->HighChild = (struct KDNodeType *) calloc(1,sizeof(struct KDNodeType));
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
      MedIdx = KDSelectMedian(KD->Poly,KD->Npoly,Axis,G);
      MedVal = G->Poly[KD->Poly[MedIdx]].Centroid[Axis];
      LC->BB.max[Axis] = MedVal;
      HC->BB.min[Axis] = MedVal;
      LC->BB.center[Axis] = 0.5*(LC->BB.max[Axis]+LC->BB.min[Axis]);
      HC->BB.center[Axis] = 0.5*(HC->BB.max[Axis]+HC->BB.min[Axis]);


      /* Assign Polys to Children */
      for(Ip=0;Ip<KD->Npoly;Ip++) {
         AnyVtxBelowMedian = 0;
         AnyVtxAboveMedian = 0;
         P = &G->Poly[KD->Poly[Ip]];
         for(Iv=0;Iv<P->Nv;Iv++) {
            if (G->V[P->V[Iv]][Axis] < MedVal) AnyVtxBelowMedian = 1;
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
      else SplitKDNode(LC,G);

      if (HC->Npoly < 20) {
         HC->IsLeaf = 1;
      }
      else if (HC->Depth > 20) {
         /*printf("Depth exceeds 20.  Npoly = %ld\n",HC->Npoly);*/
         HC->IsLeaf = 1;
      }
      else SplitKDNode(HC,G);

}
/**********************************************************************/
void LoadKDTree(struct GeomType *G)
{

      struct KDNodeType *KD;
      long i,Ip;

/* .. Root Node coincides with Geom's Bounding Box */
      G->KDTree = (struct KDNodeType *) calloc(1,sizeof(struct KDNodeType));
      KD = G->KDTree;
      KD->IsRoot = 1;
      KD->IsLeaf = 0;
      KD->Depth = 0;
      KD->Axis = 0;
      for(i=0;i<3;i++) {
         KD->BB.min[i] = G->BBox.min[i];
         KD->BB.max[i] = G->BBox.max[i];
         KD->BB.center[i] = G->BBox.center[i];
      }
      /* Root Node poly list is trivial: */
      KD->Npoly = G->Npoly;
      KD->Poly = (long *) calloc(KD->Npoly,sizeof(long));
      for(Ip=0;Ip<KD->Npoly;Ip++) KD->Poly[Ip] = Ip;

      SplitKDNode(KD,G);
}
/**********************************************************************/
void LoadOctree(struct GeomType *G)
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

      BB = &G->BBox;

      G->Octree = (struct OctreeType *) calloc(1,sizeof(struct OctreeType));
      O = G->Octree;

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
      for(Ipoly=0;Ipoly<G->Npoly;Ipoly++) {
         P = &G->Poly[Ipoly];
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
               V = G->V[P->V[Iv]];
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
/* Point and DirVec have already been transformed into Geom frame      */
long OCProjectRayOntoGeom(double Point[3],double DirVec[3],
   struct GeomType *G,double ProjPoint[3],long *ClosestPoly)
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

      O = G->Octree;

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
                  P = &G->Poly[OC->Poly[Ip]];
                  if (P->Nv != 3) {
                     printf("Error.  ProjectPointOntoGeom doesn't handle polygons with %ld vertices.\n",P->Nv);
                     exit(1);
                  }
                  for(Iv=0;Iv<P->Nv;Iv++) {
                     for(i=0;i<3;i++) Vtx[Iv][i] = G->V[P->V[Iv]][i];
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
struct GeomType *LoadWingsObjFile(const char ModelPath[80],const char ObjFilename[40],
                       struct MatlType **MatlPtr, long *Nmatl,
                       struct GeomType *Geom, long *Ngeom, long *GeomTag,
                       long EdgesEnabled)
{
#define D2R (0.0174532925199433)
      FILE *infile,*outfile;
      FILE *TmpFile;
      char *txtptr;
      double V[3];
      double r[3],magr;
      long V1,V2;
      long Ng,Ig,Iv,Im;
      long I,It,In,i,j,MatlIdx;
      long Ivtx,Ivt,Ivn,Ipoly;
      long Ip,Ie;
      struct GeomType *G;
      struct PolyType *P;
      void *Ptr;
      struct MatlType *Matl;
      long BeenHereOnce;
      long NoArraySizesFound;
      double Value,Scale = 1.0;
      double Val1,Val2,Val3;
      long Seq;
      double RotM[3][3] = {{1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0}};
      double TransVec[3] = {0.0,0.0,0.0};
      double Vr[3];
      long FirstUse;

      char line[512],vtxstring[512],*vtxtoken,MatlName[40];
      char MtlLibName[40];

      Ng = *Ngeom;
      Matl = *MatlPtr;

      /* Check for prior definition */
      for(Ig=0;Ig<Ng;Ig++) {
         if (!strcmp(ObjFilename,Geom[Ig].ObjFileName)) {
            *GeomTag = Ig;
            return(Geom);
         }
      }
      /* If not defined yet, add to list */
      Ng++;
      Geom = (struct GeomType *) realloc(Geom,Ng*sizeof(struct GeomType));
      G = &Geom[Ng-1];

      strncpy(G->ObjFileName,ObjFilename,39);
      G->ObjFileName[39] = 0; /* Null-terminated string */
      G->Nmatl = 0;
      G->Nv = 0;
      G->Nvt = 0;
      G->Nvn = 0;
      G->Npoly = 0;
      G->Nedge = 0;
      MatlIdx = 0;

      /* Allow a Null Geom entry */
      if (!strcmp(G->ObjFileName,"NONE")) {
         *Ngeom = Ng;
         *GeomTag = Ng-1;
         return(Geom);
      }

      /* These will be expanded as needed */
      G->Matl = (long *) calloc(2,sizeof(long));
      if (G->Matl == NULL) {
         printf("G->Matl calloc returned null pointer.  Bailing out!\n");
         exit(1);
      }

      infile=FileOpen(ModelPath,ObjFilename,"rt");

/* .. First pass just to count things up */
      NoArraySizesFound = 1;
      while(!feof(infile) && NoArraySizesFound) {
         fgets(line,512,infile);
         if (sscanf(line,"# Nv = %ld  Nvt = %ld  Nvn = %ld  Npoly = %ld",
            &G->Nv,&G->Nvt,&G->Nvn,&G->Npoly) == 4) {
            NoArraySizesFound = 0;
         }
         else if (sscanf(line,"v  %lf %lf %lf",&V[0],&V[1],&V[2]) == 3) {
            G->Nv++;
         }
         else if (sscanf(line,"vt %lf %lf",&V[0],&V[1]) == 2 ||
                  sscanf(line,"vt %lf %lf %lf",&V[0],&V[1],&V[2]) == 3) {
            G->Nvt++;
         }
         else if (sscanf(line,"vn  %lf %lf %lf",&V[0],&V[1],&V[2]) == 3) {
            G->Nvn++;
         }
         else if(line[0] == 'f') {
            G->Npoly++;
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
               G->Nv,G->Nvt,G->Nvn,G->Npoly);
            while((txtptr = fgets(line,512,TmpFile)) != NULL) {
               fputs(line,outfile);
            }
            fclose(outfile);
            fclose(TmpFile);
         }
      }

/* .. Allocate arrays */
      G->V = CreateMatrix(G->Nv,3);
      G->Vt = CreateMatrix(G->Nvt,2);
      G->Vn = CreateMatrix(G->Nvn,3);
      G->Poly = (struct PolyType *) calloc(G->Npoly,sizeof(struct PolyType));
      if (G->Poly == NULL) {
         printf("G->Poly calloc returned null pointer.  Bailing out!\n");
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
            for(i=0;i<3;i++) G->V[Ivtx][i] = Scale*Vr[i]+TransVec[i];
            Ivtx++;
         }
         else if (sscanf(line,"vt %lf %lf",&V[0],&V[1]) == 2 ||
                  sscanf(line,"vt %lf %lf %lf",&V[0],&V[1],&V[2]) == 3) {
            G->Vt[Ivt][0] = V[0];
            G->Vt[Ivt][1] = (1.0-V[1]); /* Flip about horizontal axis */
            Ivt++;
         }
         else if (sscanf(line,"vn  %lf %lf %lf",&V[0],&V[1],&V[2]) == 3) {
            if (V[0] == 0 && V[1] == 0 && V[2] == 0) {
               for(i=0;i<3;i++) {
                  V[i] = 1.0;  /* Kludge.  Who defines a zero-length normal?? */
               }
               /* printf("Zero-length normal in LoadWingsObjFile %s\n",ObjFilename); */
            }
            for(i=0;i<3;i++) G->Vn[Ivn][i] = V[i];
            Ivn++;
         }
         else if(line[0] == 'f') {
            P = &G->Poly[Ipoly];
            P->Nv = 0;
            P->Matl = MatlIdx;
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
            /* If poly is degenerate, remove it from Geom */
            if (PolyIsDegenerate(P,G->V)) {
               free(P->V);
               free(P->Vt);
               free(P->Vn);
               G->Npoly--;
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
            for(Im=0;Im<G->Nmatl;Im++) {
               if(MatlIdx == G->Matl[Im]) FirstUse = 0;
            }
            if (FirstUse) {
               G->Nmatl++;
               if (G->Nmatl > 2)
                  G->Matl = (long *) realloc(G->Matl,G->Nmatl*sizeof(long));
               G->Matl[G->Nmatl-1] = MatlIdx;
            }
         }
         sprintf(line,"Flush");
      }
      fclose(infile);

      /* Find Bounding Box */
      for(j=0;j<3;j++) {
         G->BBox.max[j] = G->V[0][j];
         G->BBox.min[j] = G->V[0][j];
      }
      for(i=1;i<G->Nv;i++) {
         for(j=0;j<3;j++) {
            if (G->V[i][j] < G->BBox.min[j]) G->BBox.min[j] = G->V[i][j];
            if (G->V[i][j] > G->BBox.max[j]) G->BBox.max[j] = G->V[i][j];
         }
      }
      /* Expand BBox slightly to make sure all Vtx's are inside it */
      for(j=0;j<3;j++) {
         G->BBox.max[j] += 0.01;
         G->BBox.min[j] -= 0.01;
      }
      for(j=0;j<3;j++) {
         G->BBox.center[j] = 0.5*(G->BBox.min[j]+G->BBox.max[j]);
      }
      for(j=0;j<3;j++) {
         r[j] = G->V[0][j]-G->BBox.center[j];
      }
      G->BBox.radius = MAGV(r);
      for(i=1;i<G->Nv;i++) {
         for(j=0;j<3;j++) {
            r[j] = G->V[i][j]-G->BBox.center[j];
         }
         if (MAGV(r) > G->BBox.radius) G->BBox.radius = MAGV(r);
      }

      for(i=0;i<G->Nvn;i++) UNITV(G->Vn[i]);

      if (EdgesEnabled) {
         /* Build Edge Tables */
         G->Nedge = 0;
         for(Ip=0;Ip<G->Npoly;Ip++) {
            P = &G->Poly[Ip];
            P->E = (long *) calloc(P->Nv,sizeof(long));
            for(Iv=0;Iv<P->Nv;Iv++) {
               V1 = P->V[Iv];
               V2 = P->V[(Iv+1)%P->Nv];
               BeenHereOnce = 0;
               for(Ie=0;Ie<G->Nedge;Ie++) {
                  if (G->Edge[Ie].Vtx1 == V2) {
                     if (G->Edge[Ie].Vtx2 == V1) {
                        BeenHereOnce = 1;
                        G->Edge[Ie].Poly2 = Ip;
                        P->E[Iv] = Ie;
                        break;
                     }
                  }
               }
               if (!BeenHereOnce) {
                  G->Nedge++;
                  if (G->Nedge == 1) {
                     Ptr = calloc(1,sizeof(struct EdgeType));
                     if (Ptr == NULL) {
                        printf("Realloc failed in LoadWingsObjFile\n");
                        exit(1);
                     }
                  }
                  else {
                     Ptr = realloc(G->Edge,G->Nedge*sizeof(struct EdgeType));
                     if (Ptr == NULL) {
                        printf("Realloc failed in LoadWingsObjFile\n");
                        exit(1);
                     }
                  }
                  G->Edge = (struct EdgeType *) Ptr;
                  G->Edge[G->Nedge-1].Vtx1 = V1;
                  G->Edge[G->Nedge-1].Vtx2 = V2;
                  G->Edge[G->Nedge-1].Poly1 = Ip;
                  G->Edge[G->Nedge-1].Poly2 = -1;
                  for(i=0;i<3;i++) V[i] = G->V[V1][i] - G->V[V2][i];
                  G->Edge[G->Nedge-1].Length = MAGV(V);
                  P->E[Iv] = G->Nedge-1;
               }
            }
         }
      }

      /* Find Normals, Areas, Centroids for use in surface force models */
      SurfaceForceProps(G);
      
      /* For polyhedron gravity */
      if (EdgesEnabled) EdgeAndPolyDyads(G);

      /* Find radius of bounding sphere for each poly */
      for(Ipoly=0;Ipoly<G->Npoly;Ipoly++) {
         P = &G->Poly[Ipoly];
         P->radius = 0.0;
         for(Iv=0;Iv<P->Nv;Iv++) {
            Ivtx = P->V[Iv];
            for(i=0;i<3;i++) r[i] = G->V[Ivtx][i] - P->Centroid[i];
            magr = MAGV(r);
            if (magr > P->radius) P->radius = magr;
         }
      }

      *Ngeom = Ng;
      *GeomTag = Ng-1;
      return(Geom);
#undef D2R
}
/*********************************************************************/
void WriteGeomToObjFile(struct MatlType *Matl,struct GeomType *Geom,const char Path[80],
   const char FileName[40])
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

      for(Im=0;Im<Geom->Nmatl;Im++) {
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
               Geom->Nv,Geom->Nvt,Geom->Nvn,Geom->Npoly);

      fprintf(ObjFile,"mtllib %s\n\n",MtlFileName);

      /* Vertices */
      for(Iv=0;Iv<Geom->Nv;Iv++) {
         V = Geom->V[Iv];
         fprintf(ObjFile,"v %lf %lf %lf\n",V[0],V[1],V[2]);
      }
      /* Texture Vertices */
      for(Iv=0;Iv<Geom->Nvt;Iv++) {
         V = Geom->Vt[Iv];
         fprintf(ObjFile,"vt %lf %lf %lf\n",V[0],V[1],V[2]);
      }
      /* Normals */
      for(Iv=0;Iv<Geom->Nvn;Iv++) {
         V = Geom->Vn[Iv];
         fprintf(ObjFile,"vn %lf %lf %lf\n",V[0],V[1],V[2]);
      }
      fprintf(ObjFile,"\n");

      for(Im=0;Im<Geom->Nmatl;Im++) {
         M = &Matl[Im];
         fprintf(ObjFile,"usemtl %s\n",M->Label);
         for(Ip=0;Ip<Geom->Npoly;Ip++) {
            P = &Geom->Poly[Ip];
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
