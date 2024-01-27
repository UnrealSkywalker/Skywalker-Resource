// Fill out your copyright notice in the Description page of Project Settings.

#include "SkywalkerResourceSubsystem.h"

#pragma region USubsystem interface

void USkywalkerResourceSubsystem::Initialize(FSubsystemCollectionBase &Collection)
{
	if (!ObjectLibrary)
	{
		ObjectLibrary = UObjectLibrary::CreateLibrary(UObject::StaticClass(), false, false);
		ObjectLibrary->AddToRoot();
	}
}

void USkywalkerResourceSubsystem::Deinitialize()
{
}

bool USkywalkerResourceSubsystem::ShouldCreateSubsystem(UObject *Outer) const
{
	return true;
}

#pragma endregion USubsystem interface

#pragma region AssetData

bool USkywalkerResourceSubsystem::AddAssetDataList(const FString &InPath)
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

const TArray<FAssetData> *USkywalkerResourceSubsystem::GetAssetDataList(const FString &InPath)
{
	return SortedMapPathToArrayAssetData.Find(InPath);
}

#pragma endregion

#pragma region Asset

CSkywalkerAsset *USkywalkerResourceSubsystem::GetSkywalkerAsset(const FString &IntPath)
{
	return SortedMapPathToAsset.Find(IntPath);
}

CSkywalkerAsset *USkywalkerResourceSubsystem::GetSkywalkerAsset(const FSoftObjectPath &InSoftObjectPath)
{
	return GetSkywalkerAsset(InSoftObjectPath.ToString());
}

CSkywalkerAsset *USkywalkerResourceSubsystem::CreateSkywalkerAsset(const FSoftObjectPath &InSoftObjectPath)
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

UObject *USkywalkerResourceSubsystem::GetAssetObject(const FSoftObjectPath &InSoftObjectPath)
{
	CSkywalkerAsset *SkywalkerAsset = GetSkywalkerAsset(InSoftObjectPath);
	if (nullptr == SkywalkerAsset)
	{
		UE_LOG(SkywalkerResourceModule, Warning, TEXT("GetAsset {0} nullptr == SkywalkerAsset"), *InSoftObjectPath.ToString());
		return nullptr;
	}

	return SkywalkerAsset->Get();
}

bool USkywalkerResourceSubsystem::HasAsset(const FSoftObjectPath &InSoftObjectPath)
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

bool USkywalkerResourceSubsystem::StreamableLoadAsync(TArray<FSoftObjectPath> TargetsToStream)
{
	CSkywalkerLoader *SkywalkerLoader = new CSkywalkerLoader(this);

	SkywalkerLoader->Load(TargetsToStream);

	return true;
}

bool USkywalkerResourceSubsystem::StreamableLoadAsync(const FSoftObjectPath &TargetToStream)
{
	CSkywalkerLoader *SkywalkerLoader = new CSkywalkerLoader(this);

	SkywalkerLoader->Load(TargetToStream);

	return true;
}

#pragma endregion
