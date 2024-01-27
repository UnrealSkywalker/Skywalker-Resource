// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/StreamableManager.h"

#include "SkywalkerResource.h"
#include "Asset/SkywalkerAsset.h"

class FSkywalkerResourceModule;

/**
 * ×ÊÔ´¼ÓÔØÆ÷
 */
class CSkywalkerLoader
{
private:
	TArray<CSkywalkerAsset *> SkywalkerAssetArray;
	TArray<FSoftObjectPath> ArrayLoadSoftObjectPath;
	TSharedPtr<FStreamableHandle> StreamableHandle;
	FSkywalkerResourceModule *ResourceModule;

	void AssetLoadSuccess(FSoftObjectPath SoftObjectPath);

	bool CheckLoadFinish();

	void AssetLoadFinish();

	void Release();

public:
	CSkywalkerLoader(FSkywalkerResourceModule *ResourceModule);
	~CSkywalkerLoader();
	bool Load(TArray<FSoftObjectPath> &TargetsToStream);
	bool Load(const FSoftObjectPath &TargetToStream);
};
