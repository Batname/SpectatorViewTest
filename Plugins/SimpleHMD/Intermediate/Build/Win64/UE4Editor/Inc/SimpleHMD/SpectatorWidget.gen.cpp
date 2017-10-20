// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Private/SimpleHMD.h"
#include "Private/SpectatorUI/SpectatorWidget.h"
PRAGMA_DISABLE_OPTIMIZATION
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeSpectatorWidget() {}
// Cross Module References
	SIMPLEHMD_API UFunction* Z_Construct_UFunction_USpectatorWidget_CreateSpectatorUIWindow();
	SIMPLEHMD_API UClass* Z_Construct_UClass_USpectatorWidget();
	SIMPLEHMD_API UFunction* Z_Construct_UFunction_USpectatorWidget_UpdateSpectatorUI();
	SIMPLEHMD_API UClass* Z_Construct_UClass_USpectatorWidget_NoRegister();
	UMG_API UClass* Z_Construct_UClass_UUserWidget();
	UPackage* Z_Construct_UPackage__Script_SimpleHMD();
// End Cross Module References
	void USpectatorWidget::StaticRegisterNativesUSpectatorWidget()
	{
		UClass* Class = USpectatorWidget::StaticClass();
		static const TNameNativePtrPair<ANSICHAR> AnsiFuncs[] = {
			{ "CreateSpectatorUIWindow", (Native)&USpectatorWidget::execCreateSpectatorUIWindow },
			{ "UpdateSpectatorUI", (Native)&USpectatorWidget::execUpdateSpectatorUI },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, AnsiFuncs, ARRAY_COUNT(AnsiFuncs));
	}
	UFunction* Z_Construct_UFunction_USpectatorWidget_CreateSpectatorUIWindow()
	{
		struct SpectatorWidget_eventCreateSpectatorUIWindow_Parms
		{
			bool ReturnValue;
		};
		UObject* Outer = Z_Construct_UClass_USpectatorWidget();
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			ReturnFunction = new(EC_InternalUseOnlyConstructor, Outer, TEXT("CreateSpectatorUIWindow"), RF_Public|RF_Transient|RF_MarkAsNative) UFunction(FObjectInitializer(), nullptr, (EFunctionFlags)0x04020401, 65535, sizeof(SpectatorWidget_eventCreateSpectatorUIWindow_Parms));
			CPP_BOOL_PROPERTY_BITMASK_STRUCT(ReturnValue, SpectatorWidget_eventCreateSpectatorUIWindow_Parms);
			UProperty* NewProp_ReturnValue = new(EC_InternalUseOnlyConstructor, ReturnFunction, TEXT("ReturnValue"), RF_Public|RF_Transient|RF_MarkAsNative) UBoolProperty(FObjectInitializer(), EC_CppProperty, CPP_BOOL_PROPERTY_OFFSET(ReturnValue, SpectatorWidget_eventCreateSpectatorUIWindow_Parms), 0x0010000000000580, CPP_BOOL_PROPERTY_BITMASK(ReturnValue, SpectatorWidget_eventCreateSpectatorUIWindow_Parms), sizeof(bool), true);
			ReturnFunction->Bind();
			ReturnFunction->StaticLink();
#if WITH_METADATA
			UMetaData* MetaData = ReturnFunction->GetOutermost()->GetMetaData();
			MetaData->SetValue(ReturnFunction, TEXT("Category"), TEXT("SpectatorUI"));
			MetaData->SetValue(ReturnFunction, TEXT("ModuleRelativePath"), TEXT("Private/SpectatorUI/SpectatorWidget.h"));
#endif
		}
		return ReturnFunction;
	}
	UFunction* Z_Construct_UFunction_USpectatorWidget_UpdateSpectatorUI()
	{
		struct SpectatorWidget_eventUpdateSpectatorUI_Parms
		{
			float DeltaSeconds;
		};
		UObject* Outer = Z_Construct_UClass_USpectatorWidget();
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			ReturnFunction = new(EC_InternalUseOnlyConstructor, Outer, TEXT("UpdateSpectatorUI"), RF_Public|RF_Transient|RF_MarkAsNative) UFunction(FObjectInitializer(), nullptr, (EFunctionFlags)0x04020401, 65535, sizeof(SpectatorWidget_eventUpdateSpectatorUI_Parms));
			UProperty* NewProp_DeltaSeconds = new(EC_InternalUseOnlyConstructor, ReturnFunction, TEXT("DeltaSeconds"), RF_Public|RF_Transient|RF_MarkAsNative) UFloatProperty(CPP_PROPERTY_BASE(DeltaSeconds, SpectatorWidget_eventUpdateSpectatorUI_Parms), 0x0010000000000080);
			ReturnFunction->Bind();
			ReturnFunction->StaticLink();
#if WITH_METADATA
			UMetaData* MetaData = ReturnFunction->GetOutermost()->GetMetaData();
			MetaData->SetValue(ReturnFunction, TEXT("Category"), TEXT("SpectatorUI"));
			MetaData->SetValue(ReturnFunction, TEXT("ModuleRelativePath"), TEXT("Private/SpectatorUI/SpectatorWidget.h"));
#endif
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_USpectatorWidget_NoRegister()
	{
		return USpectatorWidget::StaticClass();
	}
	UClass* Z_Construct_UClass_USpectatorWidget()
	{
		static UClass* OuterClass = NULL;
		if (!OuterClass)
		{
			Z_Construct_UClass_UUserWidget();
			Z_Construct_UPackage__Script_SimpleHMD();
			OuterClass = USpectatorWidget::StaticClass();
			if (!(OuterClass->ClassFlags & CLASS_Constructed))
			{
				UObjectForceRegistration(OuterClass);
				OuterClass->ClassFlags |= (EClassFlags)0x20A01080u;

				OuterClass->LinkChild(Z_Construct_UFunction_USpectatorWidget_CreateSpectatorUIWindow());
				OuterClass->LinkChild(Z_Construct_UFunction_USpectatorWidget_UpdateSpectatorUI());

				CPP_BOOL_PROPERTY_BITMASK_STRUCT(bSpectatorUICreated, USpectatorWidget);
				UProperty* NewProp_bSpectatorUICreated = new(EC_InternalUseOnlyConstructor, OuterClass, TEXT("bSpectatorUICreated"), RF_Public|RF_Transient|RF_MarkAsNative) UBoolProperty(FObjectInitializer(), EC_CppProperty, CPP_BOOL_PROPERTY_OFFSET(bSpectatorUICreated, USpectatorWidget), 0x0010000000000005, CPP_BOOL_PROPERTY_BITMASK(bSpectatorUICreated, USpectatorWidget), sizeof(bool), true);
				OuterClass->AddFunctionToFunctionMapWithOverriddenName(Z_Construct_UFunction_USpectatorWidget_CreateSpectatorUIWindow(), "CreateSpectatorUIWindow"); // 3055791848
				OuterClass->AddFunctionToFunctionMapWithOverriddenName(Z_Construct_UFunction_USpectatorWidget_UpdateSpectatorUI(), "UpdateSpectatorUI"); // 2712225925
				static TCppClassTypeInfo<TCppClassTypeTraits<USpectatorWidget> > StaticCppClassTypeInfo;
				OuterClass->SetCppTypeInfo(&StaticCppClassTypeInfo);
				OuterClass->StaticLink();
#if WITH_METADATA
				UMetaData* MetaData = OuterClass->GetOutermost()->GetMetaData();
				MetaData->SetValue(OuterClass, TEXT("IncludePath"), TEXT("SpectatorUI/SpectatorWidget.h"));
				MetaData->SetValue(OuterClass, TEXT("ModuleRelativePath"), TEXT("Private/SpectatorUI/SpectatorWidget.h"));
				MetaData->SetValue(OuterClass, TEXT("ObjectInitializerConstructorDeclared"), TEXT(""));
				MetaData->SetValue(NewProp_bSpectatorUICreated, TEXT("Category"), TEXT("SpectatorUI"));
				MetaData->SetValue(NewProp_bSpectatorUICreated, TEXT("ModuleRelativePath"), TEXT("Private/SpectatorUI/SpectatorWidget.h"));
#endif
			}
		}
		check(OuterClass->GetClass());
		return OuterClass;
	}
	IMPLEMENT_CLASS(USpectatorWidget, 333451593);
	static FCompiledInDefer Z_CompiledInDefer_UClass_USpectatorWidget(Z_Construct_UClass_USpectatorWidget, &USpectatorWidget::StaticClass, TEXT("/Script/SimpleHMD"), TEXT("USpectatorWidget"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(USpectatorWidget);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
PRAGMA_ENABLE_OPTIMIZATION
