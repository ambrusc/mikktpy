//
// Copyright (c) 2013 Ambrus Csaszar
//

#include <iostream>
#include <vector>
#include "mikktspace.h"

using namespace std;

static const int components_per_vert = 3 + 3 + 2; // xyz, normal-xyz, uv
static const int components_per_tangent = 3 + 3 + 1 + 1; // tangent-xyz, bitangent-xyz, tangent-mag, bitangent-mag
static const int vertices_per_face = 3;

struct TriTangentState
{
  const vector<float> &v;
  vector<float> ret;
};

// Returns the number of faces (triangles/quads) on the mesh to be processed.
int getNumFaces(const SMikkTSpaceContext * pContext);

// Returns the number of vertices on face number iFace
// iFace is a number in the range {0, 1, ..., getNumFaces()-1}
int getNumVerticesOfFace(const SMikkTSpaceContext * pContext, const int iFace);

// returns the position/normal/texcoord of the referenced face of vertex number iVert.
// iVert is in the range {0,1,2} for triangles and {0,1,2,3} for quads.
void getPosition(const SMikkTSpaceContext * pContext, float fvPosOut[], const int iFace, const int iVert);
void getNormal(const SMikkTSpaceContext * pContext, float fvNormOut[], const int iFace, const int iVert);
void getTexCoord(const SMikkTSpaceContext * pContext, float fvTexcOut[], const int iFace, const int iVert);

// either (or both) of the two setTSpace callbacks can be set.
// The call-back m_setTSpaceBasic() is sufficient for basic normal mapping.

// This function is used to return the tangent and fSign to the application.
// fvTangent is a unit length vector.
// For normal maps it is sufficient to use the following simplified version of the bitangent which is generated at pixel/vertex level.
// bitangent = fSign * cross(vN, tangent);
// Note that the results are returned unindexed. It is possible to generate a new index list
// But averaging/overwriting tangent spaces by using an already existing index list WILL produce INCRORRECT results.
// DO NOT! use an already existing index list.
void setTSpaceBasic(const SMikkTSpaceContext * pContext, const float fvTangent[], const float fSign, const int iFace, const int iVert);

// This function is used to return tangent space results to the application.
// fvTangent and fvBiTangent are unit length vectors and fMagS and fMagT are their
// true magnitudes which can be used for relief mapping effects.
// fvBiTangent is the "real" bitangent and thus may not be perpendicular to fvTangent.
// However, both are perpendicular to the vertex normal.
// For normal maps it is sufficient to use the following simplified version of the bitangent which is generated at pixel/vertex level.
// fSign = bIsOrientationPreserving ? 1.0f : (-1.0f);
// bitangent = fSign * cross(vN, tangent);
// Note that the results are returned unindexed. It is possible to generate a new index list
// But averaging/overwriting tangent spaces by using an already existing index list WILL produce INCRORRECT results.
// DO NOT! use an already existing index list.
void setTSpace(const SMikkTSpaceContext * pContext, const float fvTangent[], const float fvBiTangent[], const float fMagS, const float fMagT,
            const tbool bIsOrientationPreserving, const int iFace, const int iVert);


int getNumFaces(const SMikkTSpaceContext * pContext)
{
  TriTangentState *s = (TriTangentState*)pContext->m_pUserData;
  int ret = s->v.size() / (components_per_vert * vertices_per_face);
  // cout << "GetNumFaces: " << ret << endl;
  return ret;
}

int getNumVerticesOfFace(const SMikkTSpaceContext * pContext, const int iFace)
{
  // cout << "GetVerticesPerFace: " << vertices_per_face << endl;
  return vertices_per_face;
}

void getPosition(const SMikkTSpaceContext * pContext, float fvPosOut[], const int iFace, const int iVert)
{
  TriTangentState *s = (TriTangentState*)pContext->m_pUserData;
  int idx = iFace * vertices_per_face * components_per_vert + iVert * components_per_vert;
  // cout << "GetPosition: " << idx << endl;
  fvPosOut[0] = s->v[idx + 0];
  fvPosOut[1] = s->v[idx + 1];
  fvPosOut[2] = s->v[idx + 2];
}

void getNormal(const SMikkTSpaceContext * pContext, float fvNormOut[], const int iFace, const int iVert)
{
  TriTangentState *s = (TriTangentState*)pContext->m_pUserData;
  int idx = iFace * vertices_per_face * components_per_vert + iVert * components_per_vert;
  fvNormOut[0] = s->v[idx + 3];
  fvNormOut[1] = s->v[idx + 4];
  fvNormOut[2] = s->v[idx + 5];
}

void getTexCoord(const SMikkTSpaceContext * pContext, float fvTexcOut[], const int iFace, const int iVert)
{
  TriTangentState *s = (TriTangentState*)pContext->m_pUserData;
  int idx = iFace * vertices_per_face * components_per_vert + iVert * components_per_vert;
  fvTexcOut[0] = s->v[idx + 6];
  fvTexcOut[1] = s->v[idx + 7];
}

void setTSpace(const SMikkTSpaceContext * pContext, const float fvTangent[], const float fvBiTangent[], const float fMagS, const float fMagT,
            const tbool bIsOrientationPreserving, const int iFace, const int iVert)
{
  TriTangentState *s = (TriTangentState*)pContext->m_pUserData;
  int idx = iFace * vertices_per_face * components_per_tangent + iVert * components_per_tangent;

  // cout << "SetTspace (" << iFace << "," << iVert << ") ";
  // for (int i = 0; i < 3; i++)
  //   cout << (i>0 ? "," : "") << fvTangent[i];
  // cout << ";   ";
  // for (int i = 0; i < 3; i++)
  //   cout << (i>0 ? "," : "") << fvBiTangent[i];
  // cout << endl;
  // cout << "idx: " << idx << endl;

  s->ret[idx + 0] = fvTangent[0];
  s->ret[idx + 1] = fvTangent[1];
  s->ret[idx + 2] = fvTangent[2];
  s->ret[idx + 3] = fvBiTangent[0];
  s->ret[idx + 4] = fvBiTangent[1];
  s->ret[idx + 5] = fvBiTangent[2];
  s->ret[idx + 6] = fMagS;
  s->ret[idx + 7] = fMagT;
}

vector<float> compute_tri_tangents(const vector<float> &v)
{
  SMikkTSpaceInterface mikktInterface;
  mikktInterface.m_getNumFaces = getNumFaces;
  mikktInterface.m_getNumVerticesOfFace = getNumVerticesOfFace;
  mikktInterface.m_getPosition = getPosition;
  mikktInterface.m_getNormal = getNormal;
  mikktInterface.m_getTexCoord = getTexCoord;
  mikktInterface.m_setTSpaceBasic = NULL;
  mikktInterface.m_setTSpace = setTSpace;

  TriTangentState s = { v };
  s.ret.resize(components_per_tangent * v.size() / components_per_vert);

  SMikkTSpaceContext mikktContext;
  mikktContext.m_pInterface = &mikktInterface;
  mikktContext.m_pUserData = &s;

  genTangSpaceDefault(&mikktContext);

  return s.ret;
}
