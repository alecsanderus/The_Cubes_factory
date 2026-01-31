#pragma once
// %s - string, %d - int, %f - float                напоминалка :)


#if (UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT)

    #define DEBUG_CHECK(ClassName, ObjectName, Object) if (!Object)\
UE_LOG(LogTemp, Error, TEXT("%s has no %s!    name: %s"), TEXT(ClassName), TEXT (ObjectName), *(this->GetName())) else

#define DEBUG_NOT_OVERRIDE(FunctionName) UE_LOG(LogTemp, Error, TEXT("%s hasnt overrided!     name: %s"), TEXT(FunctionName), *(this->GetName()))

#define DEBUG_CHECK_RETURN(ClassName, ObjectName, Object) if (!Object)\
{UE_LOG(LogTemp, Error, TEXT("%s has no %s!    name: %s"), TEXT(ClassName), TEXT (ObjectName), *(this->GetName())); return;}

#else

#define DEBUG_CHECK(ClassName, ObjectName, Object)

#define DEBUG_NOT_OVERRIDE(FunctionName)

#define DEBUG_CHECK_RETURN (ClassName, ObjectName, Object)

#endif
