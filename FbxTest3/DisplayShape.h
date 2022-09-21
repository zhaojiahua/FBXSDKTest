#pragma once

#include <fbxsdk.h>

void DisplayShape(FbxGeometry* pGeometry);

void DisplayString(const char* pHeader, const char* pValue = "", const char* pSuffix = "");

void DisplayDouble(const char* pHeader, double pValue, const char* pSuffix = "");

void DisplayInt(const char* pHeader, int pValue, const char* pSuffix = "");

void Display3DVector(const char* pHeader, FbxVector4 pValue, const char* pSuffix = "");

void InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene);

bool LoadScene(FbxManager* pManager, FbxDocument* pScene, const char* pFilename);

void DisplayMetaData(FbxScene* pScene);