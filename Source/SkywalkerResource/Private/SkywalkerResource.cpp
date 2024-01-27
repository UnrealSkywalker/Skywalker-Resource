// Copyright Epic Games, Inc. All Rights Reserved.

#include "SkywalkerResource.h"

#define LOCTEXT_NAMESPACE "FSkywalkerResourceModule"

void FSkywalkerResourceModule::StartupModule()
{
	UE_LOG(SkywalkerResourceModule, Log, TEXT("StartupModule"));

	if (!ObjectLibrary)
	{
		ObjectLibrary = UObjectLibrary::CreateLibrary(UObject::StaticClass(), false, false);
		ObjectLibrary->AddToRoot();
	}
}

void FSkywalkerResourceModule::ShutdownModule()
{
	UE_LOG(SkywalkerResourceModule, Log, TEXT("ShutdownModule"));
}

#pragma region AssetData
bool FSkywalkerResourceModule::AddAssetDataList(const FString &InPath)
{
	if (nullptr != SortedMapPathToArrayAssetData.Find(InPath))
	{
		return true;
	}

	ObjectLibrary->LoadAssetDataFromPath(InPath);

	TArray<FAssetData> ArrayAssetData;
	ObjectLibrary->GetAssetDataList(ArrayAssetData);

	SortedMapPathToArrayAssetData.Add(InPath, ArrayAssetData);

#if SKYWALKER_RESOURCE_LOG
	UE_LOG(SkywalkerResourceModule, Log, TEXT("AddAssetDataList %s"), *InPath);
	for (int32 i = 0; i < ArrayAssetData.Num(); ++i)
	{
		UE_LOG(SkywalkerResourceModule, Log, TEXT("AssetName = %s"), *ArrayAssetData[i].AssetName.ToString());
		UE_LOG(SkywalkerResourceModule, Log, TEXT("AssetClass = %s"), *ArrayAssetData[i].AssetClass.ToString());
		UE_LOG(SkywalkerResourceModule, Log, TEXT("ObjectPath = %s"), *ArrayAssetData[i].ObjectPath.ToString());
		UE_LOG(SkywalkerResourceModule, Log, TEXT("PackageName = %s"), *ArrayAssetData[i].PackageName.ToString());
		UE_LOG(SkywalkerResourceModule, Log, TEXT("PackagePath = %s"), *ArrayAssetData[i].PackagePath.ToString());
	}
	UE_LOG(SkywalkerResourceModule, Log, TEXT("------------------------------------------------------------------"));
#endif

	return true;
}

const TArray<FAssetData> *FSkywalkerResourceModule::GetAssetDataList(const FString &InPath)
{
	return SortedMapPathToArrayAssetData.Find(InPath);
}
#pragma endregion

#pragma region Asset
CSkywalkerAsset *FSkywalkerResourceModule::GetSkywalkerAsset(const FString &IntPath)
{
	return SortedMapPathToAsset.Find(IntPath);
}

CSkywalkerAsset *FSkywalkerResourceModule::GetSkywalkerAsset(const FSoftObjectPath &InSoftObjectPath)
{
	return GetSkywalkerAsset(InSoftObjectPath.ToString());
}

CSkywalkerAsset *FSkywalkerResourceModule::CreateSkywalkerAsset(const FSoftObjectPath &InSoftObjectPath)
{

	if (SortedMapPathToAsset.Contains(InSoftObjectPath.ToString()))
	{
		return GetSkywalkerAsset(InSoftObjectPath);
	}

	CSkywalkerAsset SkywalkerAsset = CSkywalkerAsset();
	SkywalkerAsset.SetSoftObjectPath(InSoftObjectPath);
	SortedMapPathToAsset.Add(InSoftObjectPath.ToString(), SkywalkerAsset);

	return GetSkywalkerAsset(InSoftObjectPath);
}

UObject *FSkywalkerResourceModule::GetAssetObject(const FSoftObjectPath &InSoftObjectPath)
{
	CSkywalkerAsset *SkywalkerAsset = GetSkywalkerAsset(InSoftObjectPath);
	if (nullptr == SkywalkerAsset)
	{
		UE_LOG(SkywalkerResourceModule, Warning, TEXT("GetAsset {0} nullptr == SkywalkerAsset"), *InSoftObjectPath.ToString());
		return nullptr;
	}

	return SkywalkerAsset->Get();
}

bool FSkywalkerResourceModule::HasAsset(const FSoftObjectPath &InSoftObjectPath)
{
	CSkywalkerAsset *SkywalkerAsset = GetSkywalkerAsset(InSoftObjectPath);
	if (nullptr == SkywalkerAsset)
	{
		return false;
	}

	return true;
}
#pragma endregion

#pragma region Streamable Load
bool FSkywalkerResourceModule::StreamableLoadAsync(TArray<FSoftObjectPath> TargetsToStream)
{
	CSkywalkerLoader *SkywalkerLoader = new CSkywalkerLoader(this);

	SkywalkerLoader->Load(TargetsToStream);

	return true;
}

bool FSkywalkerResourceModule::StreamableLoadAsync(const FSoftObjectPath &TargetToStream)
{
	CSkywalkerLoader *SkywalkerLoader = new CSkywalkerLoader(this);

	SkywalkerLoader->Load(TargetToStream);

	return true;
}
#pragma endregion

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSkywalkerResourceModule, SkywalkerResource)