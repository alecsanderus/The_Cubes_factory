#pragma once
//#if (UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT) 
//	#define DEBUG_LOG(ClassName, ObjectName, Object)	if (!Object) \
//		UE_LOG(LogTemp, Warning, TEXT(ClassName +" hasnt " + ObjectName +"    name: %s"), *GetName());\
//		else
//	
//#else
//	#define DEBUG_LOG(ClassName, ObjectName, Object)
//#endif

#if (UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT)
    #define DEBUG_CHECK(ClassName, ObjectName, Object) if (!Object)\
UE_LOG(LogTemp, Warning, TEXT("%s has no %s!    name: %s"), TEXT(ClassName), TEXT (ObjectName), *(this->GetName())) else

#else
#define DEBUG_CHECK(ClassName, ObjectName, Object)
#endif
