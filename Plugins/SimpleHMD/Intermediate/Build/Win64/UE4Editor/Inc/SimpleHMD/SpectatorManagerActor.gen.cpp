// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Private/SimpleHMD.h"
#include "Private/SpectatorUI/SpectatorManagerActor.h"
PRAGMA_DISABLE_OPTIMIZATION
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeSpectatorManagerActor() {}
// Cross Module References
	SIMPLEHMD_API UClass* Z_Construct_UClass_ASpectatorManagerActor_NoRegister();
	SIMPLEHMD_API UClass* Z_Construct_UClass_ASpectatorManagerActor();
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	UPackage* Z_Construct_UPackage__Script_SimpleHMD();
// End Cross Module References
	void ASpectatorManagerActor::StaticRegisterNativesASpectatorManagerActor()
	{
	}
	UClass* Z_Construct_UClass_ASpectatorManagerActor_NoRegister()
	{
		return ASpectatorManagerActor::StaticClass();
	}
	UClass* Z_Construct_UClass_ASpectatorManagerActor()
	{
		static UClass* OuterClass = NULL;
		if (!OuterClass)
		{
			Z_Construct_UClass_AActor();
			Z_Construct_UPackage__Script_SimpleHMD();
			OuterClass = ASpectatorManagerActor::StaticClass();
			if (!(OuterClass->ClassFlags & CLASS_Constructed))
			{
				UObjectForceRegistration(OuterClass);
				OuterClass->ClassFlags |= (EClassFlags)0x20800080u;


				static TCppClassTypeInfo<TCppClassTypeTraits<ASpectatorManagerActor> > StaticCppClassTypeInfo;
				OuterClass->SetCppTypeInfo(&StaticCppClassTypeInfo);
				OuterClass->StaticLink();
#if WITH_METADATA
				UMetaData* MetaData = OuterClass->GetOutermost()->GetMetaData();
				MetaData->SetValue(OuterClass, TEXT("IncludePath"), TEXT("SpectatorUI/SpectatorManagerActor.h"));
				MetaData->SetValue(OuterClass, TEXT("ModuleRelativePath"), TEXT("Private/SpectatorUI/SpectatorManagerActor.h"));
#endif
			}
		}
		check(OuterClass->GetClass());
		return OuterClass;
	}
	IMPLEMENT_CLASS(ASpectatorManagerActor, 3423597659);
	static FCompiledInDefer Z_CompiledInDefer_UClass_ASpectatorManagerActor(Z_Construct_UClass_ASpectatorManagerActor, &ASpectatorManagerActor::StaticClass, TEXT("/Script/SimpleHMD"), TEXT("ASpectatorManagerActor"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(ASpectatorManagerActor);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
PRAGMA_ENABLE_OPTIMIZATION
