// Fill out your copyright notice in the Description page of Project Settings.

#include "Asset/SkywalkerAsset.h"

CSkywalkerAsset::CSkywalkerAsset()
{
	RecyclePattern = ESkywalkerAssetRecyclePattern::ReferenceCount;
	ReferenceCount = 0;
	Status = ESkywalkerAssetStatus::Init;
	AssetObject = nullptr;
}

CSkywalkerAsset::~CSkywalkerAsset()
{
}

void CSkywalkerAsset::SetSoftObjectPath(FSoftObjectPath InSoftObjectPath)
{
	SoftObjectPath = InSoftObjectPath;
}

UObject *CSkywalkerAsset::Get()
{
	ReferenceCount += 1;
	UE_LOG(SkywalkerResourceModule, Log, TEXT("SkywalkerAsset Get {0} ReferenceCount = {1}"), *SoftObjectPath.GetAssetPathString(), ReferenceCount);
	return AssetObject;
}

void CSkywalkerAsset::Set(UObject *InObject)
{
	if (nullptr != AssetObject)
	{
		UE_LOG(SkywalkerResourceModule, Log, TEXT("SkywalkerAsset Set %s Repetition"), *SoftObjectPath.GetAssetPathString());
	}

	AssetObject = InObject;
}

bool CSkywalkerAsset::Recycle()
{
	if (0 >= ReferenceCount)
	{
		UE_LOG(SkywalkerResourceModule, Warning, TEXT("SkywalkerAsset Recycle {0} 0 >= ReferenceCount"), *SoftObjectPath.GetAssetPathString());
		return false;
	}

	ReferenceCount -= 1;
	UE_LOG(SkywalkerResourceModule, Log, TEXT("SkywalkerAsset Recycle {0} ReferenceCount = {1}"), *SoftObjectPath.GetAssetPathString(), ReferenceCount);
	return true;
}

bool CSkywalkerAsset::Release()
{
	ReferenceCount = 0;
	AssetObject = nullptr;
	Status = ESkywalkerAssetStatus::Release;

	return true;
}

bool CSkywalkerAsset::Load()
{
	if (ESkywalkerAssetStatus::Loading == Status || ESkywalkerAssetStatus::Loaded == Status)
	{
		UE_LOG(SkywalkerResourceModule, Log, TEXT("SkywalkerAsset Load {0} Repetition"), *SoftObjectPath.GetAssetPathString());
		return false;
	}

	Status = ESkywalkerAssetStatus::Loading;

	return true;
}

void CSkywalkerAsset::LoadFinish(bool IsSuccessed)
{
	if (IsSuccessed)
	{
		LoadSuccess();
	}
	else
	{
		LoadFailed();
	}

	LoadSuccessEvent.Broadcast(SoftObjectPath);
}

void CSkywalkerAsset::LoadSuccess()
{
	Status = ESkywalkerAssetStatus::Loaded;

#if SKYWALKER_RESOURCE_LOG
	UE_LOG(SkywalkerResourceModule, Log, TEXT("SkywalkerAsset %s LoadSuccess"), *SoftObjectPath.ToString());
#endif
	// TODO
}

void CSkywalkerAsset::LoadFailed()
{
	Status = ESkywalkerAssetStatus::LoadFailed;

	// TODO
}