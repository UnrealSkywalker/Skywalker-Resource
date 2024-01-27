// Fill out your copyright notice in the Description page of Project Settings.

#include "Loader/SkywalkerLoader.h"
#include "Engine/AssetManager.h"

CSkywalkerLoader::CSkywalkerLoader(FSkywalkerResourceModule *InResourceModule) : ResourceModule(InResourceModule)
{
}
CSkywalkerLoader::~CSkywalkerLoader()
{
}

bool CSkywalkerLoader::Load(TArray<FSoftObjectPath> &InTargetsToStream)
{
	ArrayLoadSoftObjectPath = InTargetsToStream;

	TArray<FSoftObjectPath> NeedLoad;
	for (auto &SoftObjectPath : InTargetsToStream)
	{
		CSkywalkerAsset *SkywalkerAsset = ResourceModule->GetSkywalkerAsset(SoftObjectPath);
		if (nullptr == SkywalkerAsset)
		{
			SkywalkerAsset = ResourceModule->CreateSkywalkerAsset(SoftObjectPath);
		}

		if (SkywalkerAsset->IsLoading())
		{
			SkywalkerAsset->OnLoadSuccess().AddRaw(this, &CSkywalkerLoader::AssetLoadSuccess);
		}
		else if (!SkywalkerAsset->IsLoaded())
		{
			NeedLoad.Add(SoftObjectPath);
			SkywalkerAsset->Load();
		}
	}

	if (CheckLoadFinish())
	{
		return true;
	}

	if (NeedLoad.IsEmpty())
	{
		return true;
	}

	FStreamableManager &Streamable = UAssetManager::Get().GetStreamableManager();
	StreamableHandle = Streamable.RequestAsyncLoad(NeedLoad, FStreamableDelegate::CreateRaw(this, &CSkywalkerLoader::AssetLoadFinish));

	return true;
}

bool CSkywalkerLoader::Load(const FSoftObjectPath &TargetToStream)
{
	TArray<FSoftObjectPath> NeedLoad;
	NeedLoad.Add(TargetToStream);

	return Load(NeedLoad);
}

void CSkywalkerLoader::AssetLoadFinish()
{
	TArray<UObject *> OutObjects;
	StreamableHandle->GetLoadedAssets(OutObjects);
	for (int32 i = 0; i < OutObjects.Num(); ++i)
	{
		CSkywalkerAsset *SkywalkerAsset = ResourceModule->GetSkywalkerAsset(OutObjects[i]->GetPathName());
		if (nullptr != SkywalkerAsset)
		{
			SkywalkerAsset->Set(OutObjects[i]);
			SkywalkerAsset->LoadFinish();
		}
	}

	CheckLoadFinish();
}

void CSkywalkerLoader::AssetLoadSuccess(FSoftObjectPath SoftObjectPath)
{
	CheckLoadFinish();
}

bool CSkywalkerLoader::CheckLoadFinish()
{
	for (auto &SoftObjectPath : ArrayLoadSoftObjectPath)
	{
		CSkywalkerAsset *SkywalkerAsset = ResourceModule->GetSkywalkerAsset(SoftObjectPath);
		if (nullptr != SkywalkerAsset && !SkywalkerAsset->IsLoaded())
		{
			return false;
		}
	}

	// TODO ¹ã²¥

	return true;
}

void CSkywalkerLoader::Release()
{
	delete this;
}