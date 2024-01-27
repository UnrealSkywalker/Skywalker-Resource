// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "Engine/ObjectLibrary.h"
#include "Containers/SortedMap.h"

#include "Support/SkywalkerResourceCommon.h"
#include "Asset/SkywalkerAsset.h"
#include "Loader/SkywalkerLoader.h"

class ISkywalkerResourceModule : public IModuleInterface
{
public:
	/** Loads the module so it can be turned on */
	static inline ISkywalkerResourceModule& Get()
	{
		return FModuleManager::LoadModuleChecked<ISkywalkerResourceModule>("SkywalkerResource");
	}

	virtual bool StreamableLoadAsync(TArray<FSoftObjectPath> TargetsToStream) = 0;
	virtual bool StreamableLoadAsync(const FSoftObjectPath& TargetToStream) = 0;

	virtual bool AddAssetDataList(const FString& InPath) = 0;
	virtual const TArray<FAssetData>* GetAssetDataList(const FString& InPath) = 0;
};

class FSkywalkerResourceModule : public ISkywalkerResourceModule
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
#pragma region Asset
private:
	TSortedMap<FString, CSkywalkerAsset> SortedMapPathToAsset;

public:
	CSkywalkerAsset* GetSkywalkerAsset(const FString& IntPath);

	/**
	 * 路径对应的SkywalkerAsset
	 *
	 * @param	InPath		资源路径
	 * @param	InName		资源名称
	 * @return	CSkywalkerAsset*
	 */
	CSkywalkerAsset* GetSkywalkerAsset(const FSoftObjectPath& InSoftObjectPath);


	CSkywalkerAsset* CreateSkywalkerAsset(const FSoftObjectPath& InSoftObjectPath);

	/**
	 * 路径对应资源
	 *
	 * @param	InPath		资源路径
	 * @param	InName		资源名称
	 * @return	UObject*
	 */
	UObject* GetAssetObject(const FSoftObjectPath& InSoftObjectPath);

	/**
	 * 路径对应资源
	 *
	 * @param	InPath		资源路径
	 * @param	InName		资源名称
	 * @return	True表示已加载这个资源
	 */
	bool		HasAsset(const FSoftObjectPath& InSoftObjectPath);
#pragma endregion

#pragma region AssetData
private:
	/** 路径对应资源 */
	TSortedMap<FString, TArray<FAssetData>> SortedMapPathToArrayAssetData;

public:
	bool AddAssetDataList(const FString& InPath);
	const TArray<FAssetData>* GetAssetDataList(const FString& InPath);
#pragma endregion

#pragma region Streamable Load
public:
	bool StreamableLoadAsync(TArray<FSoftObjectPath> TargetsToStream);
	bool StreamableLoadAsync(const FSoftObjectPath& TargetToStream);
#pragma endregion

private:
	UObjectLibrary* ObjectLibrary;
};
