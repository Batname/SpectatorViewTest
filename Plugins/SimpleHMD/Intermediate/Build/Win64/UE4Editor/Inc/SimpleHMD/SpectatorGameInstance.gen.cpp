// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Private/SimpleHMD.h"
#include "Private/SpectatorUI/SpectatorGameInstance.h"
PRAGMA_DISABLE_OPTIMIZATION
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeSpectatorGameInstance() {}
// Cross Module References
	SIMPLEHMD_API UClass* Z_Construct_UClass_USpectatorGameInstance_NoRegister();
	SIMPLEHMD_API UClass* Z_Construct_UClass_USpectatorGameInstance();
	ENGINE_API UClass* Z_Construct_UClass_UGameInstance();
	UPackage* Z_Construct_UPackage__Script_SimpleHMD();
	SIMPLEHMD_API UClass* Z_Construct_UClass_ASpectatorManagerActor_NoRegister();
	SIMPLEHMD_API UClass* Z_Construct_UClass_USpectatorWidget_NoRegister();
// End Cross Module References
	void USpectatorGameInstance::StaticRegisterNativesUSpectatorGameInstance()
	{
	}
	UClass* Z_Construct_UClass_USpectatorGameInstance_NoRegister()
	{
		return USpectatorGameInstance::StaticClass();
	}
	UClass* Z_Construct_UClass_USpectatorGameInstance()
	{
		static UClass* OuterClass = NULL;
		if (!OuterClass)
		{
			Z_Construct_UClass_UGameInstance();
			Z_Construct_UPackage__Script_SimpleHMD();
			OuterClass = USpectatorGameInstance::StaticClass();
			if (!(OuterClass->ClassFlags & CLASS_Constructed))
			{
				UObjectForceRegistration(OuterClass);
				OuterClass->ClassFlags |= (EClassFlags)0x20800088u;


				UProperty* NewProp_SpectatorManagerActor = new(EC_InternalUseOnlyConstructor, OuterClass, TEXT("SpectatorManagerActor"), RF_Public|RF_Transient|RF_MarkAsNative) UObjectProperty(CPP_PROPERTY_BASE(SpectatorManagerActor, USpectatorGameInstance), 0x0010000000000005, Z_Construct_UClass_ASpectatorManagerActor_NoRegister());
				UProperty* NewProp_SpectatorWidget = new(EC_InternalUseOnlyConstructor, OuterClass, TEXT("SpectatorWidget"), RF_Public|RF_Transient|RF_MarkAsNative) UObjectProperty(CPP_PROPERTY_BASE(SpectatorWidget, USpectatorGameInstance), 0x001000000008000d, Z_Construct_UClass_USpectatorWidget_NoRegister());
				static TCppClassTypeInfo<TCppClassTypeTraits<USpectatorGameInstance> > StaticCppClassTypeInfo;
				OuterClass->SetCppTypeInfo(&StaticCppClassTypeInfo);
				OuterClass->StaticLink();
#if WITH_METADATA
				UMetaData* MetaData = OuterClass->GetOutermost()->GetMetaData();
				MetaData->SetValue(OuterClass, TEXT("IncludePath"), TEXT("SpectatorUI/SpectatorGameInstance.h"));
				MetaData->SetValue(OuterClass, TEXT("ModuleRelativePath"), TEXT("Private/SpectatorUI/SpectatorGameInstance.h"));
				MetaData->SetValue(NewProp_SpectatorManagerActor, TEXT("Category"), TEXT("SpectatorUI"));
				MetaData->SetValue(NewProp_SpectatorManagerActor, TEXT("ModuleRelativePath"), TEXT("Private/SpectatorUI/SpectatorGameInstance.h"));
				MetaData->SetValue(NewProp_SpectatorWidget, TEXT("Category"), TEXT("SpectatorUI"));
				MetaData->SetValue(NewProp_SpectatorWidget, TEXT("EditInline"), TEXT("true"));
				MetaData->SetValue(NewProp_SpectatorWidget, TEXT("ModuleRelativePath"), TEXT("Private/SpectatorUI/SpectatorGameInstance.h"));
#endif
			}
		}
		check(OuterClass->GetClass());
		return OuterClass;
	}
	IMPLEMENT_CLASS(USpectatorGameInstance, 3656122910);
	static FCompiledInDefer Z_CompiledInDefer_UClass_USpectatorGameInstance(Z_Construct_UClass_USpectatorGameInstance, &USpectatorGameInstance::StaticClass, TEXT("/Script/SimpleHMD"), TEXT("USpectatorGameInstance"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(USpectatorGameInstance);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
PRAGMA_ENABLE_OPTIMIZATION
