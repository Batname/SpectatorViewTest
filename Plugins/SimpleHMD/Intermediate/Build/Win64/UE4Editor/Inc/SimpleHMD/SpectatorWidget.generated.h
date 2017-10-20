// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "ObjectMacros.h"
#include "ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef SIMPLEHMD_SpectatorWidget_generated_h
#error "SpectatorWidget.generated.h already included, missing '#pragma once' in SpectatorWidget.h"
#endif
#define SIMPLEHMD_SpectatorWidget_generated_h

#define SpectatorViewTest_Plugins_SimpleHMD_Source_SimpleHMD_Private_SpectatorUI_SpectatorWidget_h_18_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execUpdateSpectatorUI) \
	{ \
		P_GET_PROPERTY(UFloatProperty,Z_Param_DeltaSeconds); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->UpdateSpectatorUI(Z_Param_DeltaSeconds); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execCreateSpectatorUIWindow) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=this->CreateSpectatorUIWindow(); \
		P_NATIVE_END; \
	}


#define SpectatorViewTest_Plugins_SimpleHMD_Source_SimpleHMD_Private_SpectatorUI_SpectatorWidget_h_18_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execUpdateSpectatorUI) \
	{ \
		P_GET_PROPERTY(UFloatProperty,Z_Param_DeltaSeconds); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->UpdateSpectatorUI(Z_Param_DeltaSeconds); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execCreateSpectatorUIWindow) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=this->CreateSpectatorUIWindow(); \
		P_NATIVE_END; \
	}


#define SpectatorViewTest_Plugins_SimpleHMD_Source_SimpleHMD_Private_SpectatorUI_SpectatorWidget_h_18_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUSpectatorWidget(); \
	friend SIMPLEHMD_API class UClass* Z_Construct_UClass_USpectatorWidget(); \
public: \
	DECLARE_CLASS(USpectatorWidget, UUserWidget, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/SimpleHMD"), NO_API) \
	DECLARE_SERIALIZER(USpectatorWidget) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define SpectatorViewTest_Plugins_SimpleHMD_Source_SimpleHMD_Private_SpectatorUI_SpectatorWidget_h_18_INCLASS \
private: \
	static void StaticRegisterNativesUSpectatorWidget(); \
	friend SIMPLEHMD_API class UClass* Z_Construct_UClass_USpectatorWidget(); \
public: \
	DECLARE_CLASS(USpectatorWidget, UUserWidget, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/SimpleHMD"), NO_API) \
	DECLARE_SERIALIZER(USpectatorWidget) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define SpectatorViewTest_Plugins_SimpleHMD_Source_SimpleHMD_Private_SpectatorUI_SpectatorWidget_h_18_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API USpectatorWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(USpectatorWidget) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, USpectatorWidget); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(USpectatorWidget); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API USpectatorWidget(USpectatorWidget&&); \
	NO_API USpectatorWidget(const USpectatorWidget&); \
public:


#define SpectatorViewTest_Plugins_SimpleHMD_Source_SimpleHMD_Private_SpectatorUI_SpectatorWidget_h_18_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API USpectatorWidget(USpectatorWidget&&); \
	NO_API USpectatorWidget(const USpectatorWidget&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, USpectatorWidget); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(USpectatorWidget); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(USpectatorWidget)


#define SpectatorViewTest_Plugins_SimpleHMD_Source_SimpleHMD_Private_SpectatorUI_SpectatorWidget_h_18_PRIVATE_PROPERTY_OFFSET
#define SpectatorViewTest_Plugins_SimpleHMD_Source_SimpleHMD_Private_SpectatorUI_SpectatorWidget_h_15_PROLOG
#define SpectatorViewTest_Plugins_SimpleHMD_Source_SimpleHMD_Private_SpectatorUI_SpectatorWidget_h_18_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	SpectatorViewTest_Plugins_SimpleHMD_Source_SimpleHMD_Private_SpectatorUI_SpectatorWidget_h_18_PRIVATE_PROPERTY_OFFSET \
	SpectatorViewTest_Plugins_SimpleHMD_Source_SimpleHMD_Private_SpectatorUI_SpectatorWidget_h_18_RPC_WRAPPERS \
	SpectatorViewTest_Plugins_SimpleHMD_Source_SimpleHMD_Private_SpectatorUI_SpectatorWidget_h_18_INCLASS \
	SpectatorViewTest_Plugins_SimpleHMD_Source_SimpleHMD_Private_SpectatorUI_SpectatorWidget_h_18_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define SpectatorViewTest_Plugins_SimpleHMD_Source_SimpleHMD_Private_SpectatorUI_SpectatorWidget_h_18_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	SpectatorViewTest_Plugins_SimpleHMD_Source_SimpleHMD_Private_SpectatorUI_SpectatorWidget_h_18_PRIVATE_PROPERTY_OFFSET \
	SpectatorViewTest_Plugins_SimpleHMD_Source_SimpleHMD_Private_SpectatorUI_SpectatorWidget_h_18_RPC_WRAPPERS_NO_PURE_DECLS \
	SpectatorViewTest_Plugins_SimpleHMD_Source_SimpleHMD_Private_SpectatorUI_SpectatorWidget_h_18_INCLASS_NO_PURE_DECLS \
	SpectatorViewTest_Plugins_SimpleHMD_Source_SimpleHMD_Private_SpectatorUI_SpectatorWidget_h_18_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID SpectatorViewTest_Plugins_SimpleHMD_Source_SimpleHMD_Private_SpectatorUI_SpectatorWidget_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
