// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/ObjectLibrary.h"
#include "Containers/SortedMap.h"

#include "Support/SkywalkerResourceCommon.h"
#include "Asset/SkywalkerAsset.h"
#include "Loader/SkywalkerLoader.h"

#include "SkywalkerResourceSubsystem.generated.h"

/**
 *
 */
UCLASS()
class SKYWALKERRESOURCE_API USkywalkerResourceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

#pragma region USubsystem interface

public:
	virtual void Initialize(FSubsystemCollectionBase &Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject *Outer) const override;

#pragma endregion USubsystem interface

#pragma region Asset

private:
	TSortedMap<FString, CSkywalkerAsset> SortedMapPathToAsset;

public:
	CSkywalkerAsset *GetSkywalkerAsset(const FString &IntPath);

	/**
	 * 路径对应的SkywalkerAsset
	 *
	 * @param	InPath		资源路径
	 * @param	InName		资源名称
	 * @return	CSkywalkerAsset*
	 */
	CSkywalkerAsset *GetSkywalkerAsset(const FSoftObjectPath &InSoftObjectPath);

	CSkywalkerAsset *CreateSkywalkerAsset(const FSoftObjectPath &InSoftObjectPath);

	/**
	 * 路径对应资源
	 *
	 * @param	InPath		资源路径
	 * @param	InName		资源名称
	 * @return	UObject*
	 */
	UObject *GetAssetObject(const FSoftObjectPath &InSoftObjectPath);

	/**
	 * 路径对应资源
	 *
	 * @param	InPath		资源路径
	 * @param	InName		资源名称
	 * @return	True表示已加载这个资源
	 */
	bool HasAsset(const FSoftObjectPath &InSoftObjectPath);

#pragma endregion

#pragma region AssetData

private:
	/** 路径对应资源 */
	TSortedMap<FString, TArray<FAssetData>> SortedMapPathToArrayAssetData;

public:
	bool AddAssetDataList(const FString &InPath);
	const TArray<FAssetData> *GetAssetDataList(const FString &InPath);

#pragma endregion

#pragma region Streamable Load

public:
	bool StreamableLoadAsync(TArray<FSoftObjectPath> TargetsToStream);
	bool StreamableLoadAsync(const FSoftObjectPath &TargetToStream);

#pragma endregion

private:
	UObjectLibrary *ObjectLibrary;
};
