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
	 * ·����Ӧ��SkywalkerAsset
	 *
	 * @param	InPath		��Դ·��
	 * @param	InName		��Դ����
	 * @return	CSkywalkerAsset*
	 */
	CSkywalkerAsset* GetSkywalkerAsset(const FSoftObjectPath& InSoftObjectPath);


	CSkywalkerAsset* CreateSkywalkerAsset(const FSoftObjectPath& InSoftObjectPath);

	/**
	 * ·����Ӧ��Դ
	 *
	 * @param	InPath		��Դ·��
	 * @param	InName		��Դ����
	 * @return	UObject*
	 */
	UObject* GetAssetObject(const FSoftObjectPath& InSoftObjectPath);

	/**
	 * ·����Ӧ��Դ
	 *
	 * @param	InPath		��Դ·��
	 * @param	InName		��Դ����
	 * @return	True��ʾ�Ѽ��������Դ
	 */
	bool		HasAsset(const FSoftObjectPath& InSoftObjectPath);
#pragma endregion

#pragma region AssetData
private:
	/** ·����Ӧ��Դ */
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
