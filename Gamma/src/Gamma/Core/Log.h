// Copyright (c) 2020 Shadax-stack <shadax32@gmail.com>
// This file is subject to the terms and conditions defined in
// file 'LICENSE.txt', which is part of this source code package.

// The logging approach is based on Hazel, however I make a few changes such as there being only the core logger as well as a global logger
#ifndef GAMMA_LOG_H
#define GAMMA_LOG_H

#include "../Gamma.h"
#include <stdint.h>
#include <debug-trap/debug-trap.h>
#ifdef GAMMA_MSVC
#include <yvals.h>
#include <crtdbg.h>
#include <corecrt.h>
#endif // GAMMA_MSVC


namespace Gamma {

	class GAMMA_API Logger {
	public:
		void Info(const char* source, const char* fmt, ...);
		void Warn(const char* source, const char* fmt, ...);
		void Error(const char* source, const char* fmt, ...);
		void Critical(const char* source, const char* fmt, ...);
	};

#ifdef GAMMA_DEBUG
	extern GAMMA_API Logger GlobalLogger;
#endif

}

#ifdef GAMMA_MSVC
#include "../Win32/Debug.h"
#define GAMMA_BREAK_POINT_ASSERT(source, msg, ...) {if(Gamma::Win32::DebugReport(__FILE__, __LINE__, source, msg, __VA_ARGS__)){psnip_trap();}}
#else
#define GAMMA_BREAK_POINT_ASSERT(msg, ...) {psnip_trap();}
#endif

#ifdef GAMMA_BUILD
#ifdef GAMMA_DEBUG
// Macros for logging in a debug build
#define GAMMA_INFO(source, fmt, ...) Gamma::GlobalLogger.Info(source, fmt, __VA_ARGS__)
#define GAMMA_WARN(source, fmt, ...) Gamma::GlobalLogger.Warn(source, fmt, __VA_ARGS__)
#define GAMMA_ERROR(source, fmt, ...) Gamma::GlobalLogger.Error(source, fmt, __VA_ARGS__)
#define GAMMA_CRITICAL(source, fmt, ...) Gamma::GlobalLogger.Critical(source, fmt, __VA_ARGS__)
#define GAMMA_ASSERT_INFO(condition, source, fmt, ...) if(!(condition)) { GAMMA_INFO(source, fmt, __VA_ARGS__); GAMMA_BREAK_POINT_ASSERT(source, fmt, __VA_ARGS__); }
#define GAMMA_ASSERT_WARN(condition, source, fmt, ...) if(!(condition)) { GAMMA_WARN(source, fmt, __VA_ARGS__); GAMMA_BREAK_POINT_ASSERT(source, fmt, __VA_ARGS__); }
#define GAMMA_ASSERT_ERROR(condition, source, fmt, ...) if(!(condition)) { GAMMA_ERROR(source, fmt, __VA_ARGS__); GAMMA_BREAK_POINT_ASSERT(source, fmt, __VA_ARGS__); }
#define GAMMA_ASSERT_CRITICAL(condition, source, fmt, ...) if(!(condition)) { GAMMA_CRITICAL(source, fmt, __VA_ARGS__); GAMMA_BREAK_POINT_ASSERT(source, fmt, __VA_ARGS__); }
#define GAMMA_ASSERT GAMMA_ASSERT_CRITICAL
#else
// Generally we don't want any logging in a release or distribution build
#define GAMMA_INFO(fmt, ...) 
#define GAMMA_WARN(fmt, ...) 
#define GAMMA_ERROR(fmt, ...) 
#define GAMMA_CRITICAL(fmt, ...) 
#define GAMMA_ASSERT_INFO(fmt, ...) 
#define GAMMA_ASSERT_WARN(fmt, ...) 
#define GAMMA_ASSERT_ERROR(fmt, ...) 
#define GAMMA_ASSERT_CRITICAL(condition, fmt, ...)
#define GAMMA_ASSERT GAMMA_ASSERT_CRITICAL
#endif
#endif

#endif
