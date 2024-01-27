// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/StreamableManager.h"

#include "SkywalkerResourceSubsystem.h"
#include "Asset/SkywalkerAsset.h"

class USkywalkerResourceSubsystem;

/**
 * ×ÊÔ´¼ÓÔØÆ÷
 */
class CSkywalkerLoader
{
private:
	TArray<CSkywalkerAsset *> SkywalkerAssetArray;
	TArray<FSoftObjectPath> ArrayLoadSoftObjectPath;
	TSharedPtr<FStreamableHandle> StreamableHandle;
	USkywalkerResourceSubsystem *ResourceModule;

	void AssetLoadSuccess(FSoftObjectPath SoftObjectPath);

	bool CheckLoadFinish();

	void AssetLoadFinish();

	void Release();

public:
	CSkywalkerLoader(USkywalkerResourceSubsystem *ResourceModule);
	~CSkywalkerLoader();
	bool Load(TArray<FSoftObjectPath> &TargetsToStream);
	bool Load(const FSoftObjectPath &TargetToStream);
};
