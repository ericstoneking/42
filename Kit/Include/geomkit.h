/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


#ifndef __GEOMKIT_H__
#define __GEOMKIT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "iokit.h"
#include "mathkit.h"
#include "dcmkit.h"

/*
** #ifdef __cplusplus
** namespace Kit {
** #endif
*/

struct SilEdgeType {
   long Body;
   long Iv1,Iv2;
   double PosV1B[3];
   double PosV2B[3];
   double PosV1N[3];
   double PosV2N[3];
};

struct SilVtxType {
   long Body;
   double PosB[3];
   double PosN[3];
};

struct PolyType {
   long Nv;
   long HasTex;
   long HasNorm;
   long HasBump;
   long *V;
   long *Vt;
   long *Vn;
   long *E;
   long Matl;
   double Area;
   double Norm[3];
   double Dyad[3][3]; /* For polyhedron gravity */
   double Uhat[3],Vhat[3]; /* In-plane basis vectors */
   double Centroid[3];
   double radius; /* of bounding sphere centered on Centroid */
   double UnshadedArea; /* Variable, accounting for shadowing */
   double UnshadedCtr[3]; /* Variable, accounting for shadowing */
};

struct EdgeType {
   long Vtx1; /* Tail */
   long Vtx2; /* Head */
   long Poly1; /* Left */
   long Poly2; /* Right */
   double Length;
   double Dyad[3][3]; /* For polyhedron gravity */
};

struct BoundingBoxType {
   double max[3];
   double min[3];
   double center[3];
   double radius;
};

struct OctreeCellType {
   long IsEmpty;  /* True if cell and all its children are empty */
   double center[3];
   double radius;
   double min[3],max[3];
   long Npoly;
   long *Poly; /* Polys completely within cell, but not contained in any of its children */
   long Child[8];
   long NextOnMiss;
   long NextOnHit;
};

/* Only add an Octree to Geom struct if it'll be used */
struct OctreeType {
   long Noct;  /* Number of occupied cells in Octree */
   long *OctIdx; /* List of indices pointing into occupied cells */
   /* Octree is four layers deep.  585 = 1 + 8 + 64 + 512 */
   struct OctreeCellType OctCell[585];
};

struct KDNodeType {
   long IsRoot;
   long IsLeaf;
   long Depth;  /* in KDTree */
   long Axis;  /* 0, 1, or 2 */
   long Npoly;
   long *Poly;
   struct BoundingBoxType BB;
   struct KDNodeType *Parent;
   struct KDNodeType *LowChild;
   struct KDNodeType *HighChild;
};

struct GeomType {
   char ObjFileName[40];
   long Nmatl;
   long Nv;
   long Nvt;
   long Nvn;
   long Npoly;
   long Nedge;
   struct BoundingBoxType BBox;
   double **V;
   double **Vt;
   double **Vn;
   struct PolyType *Poly;
   struct EdgeType *Edge;
   long *Matl;
   int DepthListTag;
   int OpaqueListTag;
   int SeeThruListTag;
   struct OctreeType *Octree;
   struct KDNodeType *KDTree;
};

/* Material Definitions used both in graphical output and for         */
/* computation of solar pressure forces.                              */
/* Note that Kd+Ks < 1.0 (K=R,G,B)                                    */
struct MatlType {
   char Label[40];
   float Ka[4]; /* Ambient Color */
   float Kd[4]; /* Diffuse Color */
   float Ks[4]; /* Specular Color */
   float Ke[4]; /* Emissive Color */
   float Ns; /* Shininess */
   float Nu; /* Shininess in U direction, for BRDF */
   float Nv; /* Shininess in V direction, for BRDF */
   float Refl; /* 0.0 = No reflection, 1.0 = Mirror */
   char ColorTexFileName[40];
   char BumpTexFileName[40];
   unsigned int ColorTex;
   unsigned int BumpTex;
   char SpectrumName[40];
   unsigned int SpectrumTex;
   long NoiseColEnabled;
   long NoiseBumpEnabled;
   float NoiseGain[4];
   float NoiseBias;
   float NoiseScale;
   float NoiseAxis[3];
   int NoiseType;
   /* For radiation pressure force computation */
   double SpecFrac;
   double DiffFrac;
};

struct MatlType *AddMtlLib(const char *PathName, const char *MtlLibName,
   struct MatlType *OldMatl, long *Nmatl);
void ScaleSpecDiffFrac(struct MatlType *Matl, long Nmatl);
void SurfaceForceProps(struct GeomType *G);
void LoadKDTree(struct GeomType *G);
long KDProjectRayOntoGeom(double Source[3], double DirVec[3],
   struct GeomType *G, long *HitPoly, double HitPoint[3]);
void LoadOctree(struct GeomType *G);
long OCProjectRayOntoGeom(double Point[3],double DirVec[3],
   struct GeomType *G,double ProjPoint[3],long *ClosestPoly);
struct GeomType *LoadWingsObjFile(const char ModelPath[80], const char ObjFilename[40],
                       struct MatlType **MatlPtr, long *Nmatl,
                       struct GeomType *Geom, long *Ngeom, long *GeomTag,
                       long EdgesEnabled);
void WriteGeomToObjFile(struct MatlType *Matl,struct GeomType *Geom,const char Path[80],
   const char FileName[40]);
double PolyhedronVolume(struct GeomType *G);

/*
** #ifdef __cplusplus
** }
** #endif
*/

#endif /* __GEOMKIT_H__ */
