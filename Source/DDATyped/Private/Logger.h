#pragma once

#include "CoreMinimal.h"
#include "Misc/DateTime.h"
#include "HAL/PlatformProcess.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDDA, Log, All);

class Logger
{

public:

    static void WriteLog(ELogVerbosity::Type Verbosity, const FString& ClassFunction, const FString& Message)
    {
        FString Timestamp = FDateTime::Now().ToString(TEXT("%Y-%m-%d %H:%M:%S.%s"));
        FString FormattedMessage = FString::Printf(
            TEXT("%s | %s << %s >>"),
            *Timestamp,
            *ClassFunction,
            *Message
        );

        FMsg::Logf(__FILE__, __LINE__, LogDDA.GetCategoryName(), Verbosity,
            TEXT("%s"), *FormattedMessage);
    }
};

#define DDA_VERBOSE(Format, ...) \
    Logger::WriteLog(ELogVerbosity::Verbose, __FUNCSIG__, FString::Printf(Format, ##__VA_ARGS__))

#define DDA_LOG(Format, ...) \
    Logger::WriteLog(ELogVerbosity::Log, __FUNCSIG__, FString::Printf(Format, ##__VA_ARGS__))

#define DDA_WARNING(Format, ...) \
    Logger::WriteLog(ELogVerbosity::Warning, __FUNCSIG__, FString::Printf(Format, ##__VA_ARGS__))

#define DDA_ERROR(Format, ...) \
    Logger::WriteLog(ELogVerbosity::Error, __FUNCSIG__, FString::Printf(Format, ##__VA_ARGS__))
