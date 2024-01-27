// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Support/SkywalkerResourceCommon.h"

/**
 * 资源释放模式
 */
enum class ESkywalkerAssetRecyclePattern : uint8
{
	/** 引用计数，计数为0时回收资源 */
	ReferenceCount,

	/** 永不回收 */
	Never,
};

/**
 * 资源状态
 */
enum class ESkywalkerAssetStatus : uint8
{
	/** 初始化 */
	Init,

	/** 加载中 */
	Loading,

	/** 已加载 */
	Loaded,

	/** 加载失败 */
	LoadFailed,

	/** 释放 */
	Release,
};

class CSkywalkerAsset;
DECLARE_EVENT_OneParam(CSkywalkerAsset, FLoadSuccessEvent, FSoftObjectPath);

/**
 *
 */
class CSkywalkerAsset
{
private:
	/** 释放模式 */
	ESkywalkerAssetRecyclePattern RecyclePattern;

	/** 引用计数 */
	uint32 ReferenceCount;

	/** 资源 */
	UObject *AssetObject;

	FSoftObjectPath SoftObjectPath;

	/** 资源状态 */
	ESkywalkerAssetStatus Status;

	FLoadSuccessEvent LoadSuccessEvent;

public:
	CSkywalkerAsset();
	~CSkywalkerAsset();

	void SetSoftObjectPath(FSoftObjectPath InSoftObjectPath);

	FLoadSuccessEvent &OnLoadSuccess() { return LoadSuccessEvent; }

	bool IsLoading() { return Status == ESkywalkerAssetStatus::Loading; }
	bool IsLoaded() { return Status == ESkywalkerAssetStatus::Loaded; }

	/** 获取资源 */
	UObject *Get();

	void Set(UObject *InObject);

	/** 回收资源 */
	bool Recycle();

	/** 释放资源 */
	bool Release();

	bool Load();

	void LoadFinish(bool IsSuccessed = true);

	void LoadSuccess();

	void LoadFailed();
};
