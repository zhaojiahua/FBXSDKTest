#pragma once

#include <fbxsdk.h>
#include <vector>

using namespace std;

void DisplayShape(FbxGeometry* pGeometry);

void DisplayDouble(const char* pHeader, double pValue, const char* pSuffix = "");

void Display3DVector(const char* pHeader, FbxVector4 pValue, const char* pSuffix = "");

void InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene);

bool LoadScene(FbxManager* pManager, FbxDocument* pScene, const char* pFilename);
bool SaveScene(FbxManager* pManager, FbxDocument* pScene, const char* pFilename, int pFileFormat = -1, bool pEmbedMedia = false);

void DisplayMetaData(FbxScene* pScene);

FbxNode* CreateSkeleton(FbxScene* pScene, const char* pName);

void AddBlendShape(FbxMesh* selfMesh, FbxMesh* targetMesh, vector<vector<double>> infilePos, FbxBlendShape* bs, FbxString inbsname);

void AddBlendShape(FbxScene* infbxScene, FbxMesh* selfMesh, FbxMesh* targetMesh, vector<vector<double>> infilePos, FbxBlendShape* bs, FbxString inbsname);

void LoadObjAsShape(FbxManager* pManager, FbxDocument* pScene, const char* pFilename);

void ReadObjShape(FbxString pFilename, vector<vector<double>>& shapePoints);

void BatchImportBlendShape(FbxString filepath, FbxMesh* selfAndTarfetMesh);

void BatchImportBlendShape(FbxScene* inScene, FbxString filepath, FbxMesh* selfAndTarfetMesh);

void GetFiles(FbxString path, FbxArray<FbxString*>& files);

void AddAnimations(FbxMesh* inmesh, FbxScene* fbxScene);