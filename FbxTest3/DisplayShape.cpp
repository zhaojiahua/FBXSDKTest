#include "DisplayShape.h"

#ifdef IOS_REF
	#undef  IOS_REF
	#define IOS_REF (*(pManager->GetIOSettings()))
#endif


int main()
{
	FbxManager* fbxManager = NULL;
	FbxScene* fbxScene = NULL;
	bool result;
	InitializeSdkObjects(fbxManager, fbxScene);
	FbxString fbxFilePath("test.fbx");
	if (fbxFilePath.IsEmpty())
	{
		FBXSDK_printf("\n\nUsage: ImportScene <FBX file name>\n\n");
		result = false;
	}
	else
	{
		FBXSDK_printf("\n\nFile: %s\n\n", fbxFilePath.Buffer());
		result = LoadScene(fbxManager, fbxScene, fbxFilePath.Buffer());
	}
	if (result == false) { FBXSDK_printf("\n\nAn error occurred while loading the scene..."); }
	else
	{
		// Display the scene.
		DisplayMetaData(fbxScene);
	}

	return 0;
}

void DisplayShape(FbxGeometry* pGeometry)
{
	int bsCount, bsChCount, targetCount;
	FbxBlendShape* bs;
	FbxBlendShapeChannel* bsCh;
	FbxShape* shape;
	bsCount= pGeometry->GetDeformerCount(FbxDeformer::eBlendShape);
	for (int bsIndex = 0; bsIndex < bsCount; ++bsIndex)
	{
		bs = (FbxBlendShape*)pGeometry->GetDeformer(bsIndex, FbxDeformer::eBlendShape);
		DisplayString("     BlendShape ", (char*)bs->GetName());
		bsChCount = bs->GetBlendShapeChannelCount();
		for (int bsChIndex = 0; bsChIndex < bsChCount; ++bsChIndex)
		{
			bsCh = bs->GetBlendShapeChannel(bsChIndex);
			DisplayString("     BlendShapeChannel ", (char*)bsCh->GetName());
			DisplayDouble("    Default Deform Value: ", bsCh->DeformPercent.Get());
			targetCount = bsCh->GetTargetShapeCount();
			for (int targetIndex = 0; targetIndex < targetCount; ++targetIndex)
			{
				shape = bsCh->GetTargetShape(targetIndex);
				DisplayString("     TargetShape ", (char*)shape->GetName());
				int j, ctrPointCount = shape->GetControlPointsCount();
				FbxVector4* ctrPoints = shape->GetControlPoints();
				FbxLayerElementArrayTemplate<FbxVector4>* lNormals = NULL;
				bool lStatus = shape->GetNormals(&lNormals);
				for (j = 0; j < ctrPointCount; ++j)
				{
					DisplayInt("      Control Point ", j);
					Display3DVector("      Coordinates ", ctrPoints[j]);
					if (lStatus && lNormals && lNormals->GetCount() == ctrPointCount)
					{
						Display3DVector("      Normal Vector:  ", lNormals->GetAt(j));
					}
				}
				DisplayString("");
			}
		}
	}
}

void DisplayString(const char* pHeader, const char* pValue, const char* pSuffix)
{
	FbxString lString;
	lString = pHeader;
	lString += pValue;
	lString += pSuffix;
	lString += "\n";
	FBXSDK_printf(lString);
}

void DisplayDouble(const char* pHeader, double pValue, const char* pSuffix)
{
	FbxString lString;
	FbxString lFloatValue = (float)pValue;
	lFloatValue = pValue <= -HUGE_VAL ? "-INFINITY" : lFloatValue.Buffer();
	lFloatValue = pValue >= HUGE_VAL ? "INFINITY" : lFloatValue.Buffer();
	lString = pHeader;
	lString += lFloatValue;
	lString += pSuffix;
	lString += "\n";
	FBXSDK_printf(lString);
}

void DisplayInt(const char* pHeader, int pValue, const char* pSuffix)
{
	FbxString lString;
	lString = pHeader;
	lString += pValue;
	lString += pSuffix;
	lString += "\n";
	FBXSDK_printf(lString);
}

void Display3DVector(const char* pHeader, FbxVector4 pValue, const char* pSuffix)
{
	FbxString lString;
	FbxString lFloatValue1 = (float)pValue[0];
	FbxString lFloatValue2 = (float)pValue[1];
	FbxString lFloatValue3 = (float)pValue[2];

	lFloatValue1 = pValue[0] <= -HUGE_VAL ? "-INFINITY" : lFloatValue1.Buffer();
	lFloatValue1 = pValue[0] >= HUGE_VAL ? "INFINITY" : lFloatValue1.Buffer();
	lFloatValue2 = pValue[1] <= -HUGE_VAL ? "-INFINITY" : lFloatValue2.Buffer();
	lFloatValue2 = pValue[1] >= HUGE_VAL ? "INFINITY" : lFloatValue2.Buffer();
	lFloatValue3 = pValue[2] <= -HUGE_VAL ? "-INFINITY" : lFloatValue3.Buffer();
	lFloatValue3 = pValue[2] >= HUGE_VAL ? "INFINITY" : lFloatValue3.Buffer();

	lString = pHeader;
	lString += lFloatValue1;
	lString += ", ";
	lString += lFloatValue2;
	lString += ", ";
	lString += lFloatValue3;
	lString += pSuffix;
	lString += "\n";
	FBXSDK_printf(lString);

}

void InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene)
{
	pManager = FbxManager::Create();
	if (!pManager)
	{
		FBXSDK_printf("Error: Unable to create FBX Manager!\n");
		exit(-1);
	}
	else		FBXSDK_printf("Autodesk FBX SDK version %s\n", pManager->GetVersion());
	FbxIOSettings* ios = FbxIOSettings::Create(pManager, IOSROOT);
	pManager->SetIOSettings(ios);
	FbxString path = FbxGetApplicationDirectory();
	pManager->LoadPluginsDirectory(path.Buffer());
	FBXSDK_printf("load plugins directory:%s\n", path.Buffer());
	pScene = FbxScene::Create(pManager, "myScene");
	if (!pScene)
	{
		FBXSDK_printf("Error: Unable to create FBX scene!\n");
		exit(1);
	}
}

bool LoadScene(FbxManager* pManager, FbxDocument* pScene, const char* pFilename)
{
	int lFileMajor, lFileMinor, lFileRevision;
	int lSDKMajor, lSDKMinor, lSDKRevision;
	int i, lAnimStackCount;
	bool lStatus;
	char lPassword[1024];
	FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);
	FbxImporter* importer = FbxImporter::Create(pManager, "");
	const bool importStatus = importer->Initialize(pFilename, -1, pManager->GetIOSettings());
	importer->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);
	if (!importStatus)
	{
		FbxString error = importer->GetStatus().GetErrorString();
		FBXSDK_printf("Call to FbxImporter::Initialize() failed.\n");
		FBXSDK_printf("Error returned: %s\n\n", error.Buffer());
		if (importer->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
		{
			FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
			FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);
		}
		return false;
	}
	FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
	if (importer->IsFBX())
	{
		FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);
		FBXSDK_printf("Animation Stack Information\n");
		lAnimStackCount = importer->GetAnimStackCount();
		FBXSDK_printf("    Number of Animation Stacks: %d\n", lAnimStackCount);
		FBXSDK_printf("    Current Animation Stack: \"%s\"\n", importer->GetActiveAnimStackName().Buffer());
		FBXSDK_printf("\n");
		for (i = 0; i < lAnimStackCount; i++)
		{
			FbxTakeInfo* lTakeInfo = importer->GetTakeInfo(i);
			FBXSDK_printf("    Animation Stack %d\n", i);
			FBXSDK_printf("         Name: \"%s\"\n", lTakeInfo->mName.Buffer());
			FBXSDK_printf("         Description: \"%s\"\n", lTakeInfo->mDescription.Buffer());
			FBXSDK_printf("         Import Name: \"%s\"\n", lTakeInfo->mImportName.Buffer());
			FBXSDK_printf("         Import State: %s\n", lTakeInfo->mSelect ? "true" : "false");
			FBXSDK_printf("\n");
		}
		IOS_REF.SetBoolProp(IMP_FBX_MATERIAL, true);
		IOS_REF.SetBoolProp(IMP_FBX_TEXTURE, true);
		IOS_REF.SetBoolProp(IMP_FBX_LINK, true);
		IOS_REF.SetBoolProp(IMP_FBX_SHAPE, true);
		IOS_REF.SetBoolProp(IMP_FBX_GOBO, true);
		IOS_REF.SetBoolProp(IMP_FBX_ANIMATION, true);
		IOS_REF.SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
	}
	lStatus = importer->Import(pScene);
	if (!lStatus && importer->GetStatus().GetCode() == FbxStatus::ePasswordError)
	{
		FBXSDK_printf("Please enter password: ");
		lPassword[0] = '\0';
		FBXSDK_CRT_SECURE_NO_WARNING_BEGIN
			scanf("%s", lPassword);
		FBXSDK_CRT_SECURE_NO_WARNING_END
		FbxString lString(lPassword);
		IOS_REF.SetStringProp(IMP_FBX_PASSWORD, lString);
		IOS_REF.SetBoolProp(IMP_FBX_PASSWORD_ENABLE, true);
		lStatus = importer->Import(pScene);
		if (lStatus == false && importer->GetStatus().GetCode() == FbxStatus::ePasswordError)
		{
			FBXSDK_printf("\nPassword is wrong, import aborted.\n");
		}
	}
	importer->Destroy();
	return lStatus;
}

void DisplayMetaData(FbxScene* pScene)
{
	FbxDocumentInfo* sceneInfo = pScene->GetSceneInfo();
	if (sceneInfo)
	{
		FBXSDK_printf("\n\n--------------------\nMeta-Data\n--------------------\n\n");
		FBXSDK_printf("    Title: %s\n", sceneInfo->mTitle.Buffer());
		FBXSDK_printf("    Subject: %s\n", sceneInfo->mSubject.Buffer());
		FBXSDK_printf("    Author: %s\n", sceneInfo->mAuthor.Buffer());
		FBXSDK_printf("    Keywords: %s\n", sceneInfo->mKeywords.Buffer());
		FBXSDK_printf("    Revision: %s\n", sceneInfo->mRevision.Buffer());
		FBXSDK_printf("    Comment: %s\n", sceneInfo->mComment.Buffer());
		FbxThumbnail* thumbnail = sceneInfo->GetSceneThumbnail();
		if (thumbnail)
		{
			FBXSDK_printf("    Thumbnail:\n");
			switch (thumbnail->GetDataFormat())
			{
			case FbxThumbnail::eRGB_24:
				FBXSDK_printf("        Format: RGB\n");
				break;
			case FbxThumbnail::eRGBA_32:
				FBXSDK_printf("        Format: RGBA\n");
				break;
			}
			switch (thumbnail->GetSize())
			{
			default:
				break;
			case FbxThumbnail::eNotSet:
				FBXSDK_printf("        Size: no dimensions specified (%ld bytes)\n", thumbnail->GetSizeInBytes());
				break;
			case FbxThumbnail::e64x64:
				FBXSDK_printf("        Size: 64 x 64 pixels (%ld bytes)\n", thumbnail->GetSizeInBytes());
				break;
			case FbxThumbnail::e128x128:
				FBXSDK_printf("        Size: 128 x 128 pixels (%ld bytes)\n", thumbnail->GetSizeInBytes());
			}
		}
	}
}
