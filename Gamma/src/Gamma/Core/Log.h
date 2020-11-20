// Copyright (c) 2020 Shadax-stack <shadax32@gmail.com>
// This file is subject to the terms and conditions defined in
// file 'LICENSE.txt', which is part of this source code package.

// The logging approach is based on Hazel, however I make a few changes such as there being only the core logger as well as a global logger
#ifndef GAMMA_LOG_H
#define GAMMA_LOG_H

#include "../Gamma.h"
#include <stdint.h>
#include <debug-trap/debug-trap.h>

namespace Gamma {

	class GAMMA_API Logger {
	public:
		void Info(const char* fmt, ...);
		void Warn(const char* fmt, ...);
		void Error(const char* fmt, ...);
		void Critical(const char* fmt, ...);
	};

#ifdef GAMMA_DEBUG
	extern GAMMA_API Logger GlobalLogger;
#endif

}

#ifdef GAMMA_BUILD
#ifdef GAMMA_DEBUG
// Macros for logging in a debug build
#define GAMMA_INFO(fmt, ...) Gamma::GlobalLogger.Info(fmt, __VA_ARGS__)
#define GAMMA_WARN(fmt, ...) Gamma::GlobalLogger.Warn(fmt, __VA_ARGS__)
#define GAMMA_ERROR(fmt, ...) Gamma::GlobalLogger.Error(fmt, __VA_ARGS__)
#define GAMMA_CRITICAL(fmt, ...) Gamma::GlobalLogger.Critical(fmt, __VA_ARGS__)
#define GAMMA_ASSERT_INFO(condition, fmt, ...) if(!(condition)) { GAMMA_INFO(fmt, __VA_ARGS__); psnip_trap(); }
#define GAMMA_ASSERT_WARN(condition, fmt, ...) if(!(condition)) { GAMMA_WARN(fmt, __VA_ARGS__); psnip_trap(); }
#define GAMMA_ASSERT_ERROR(condition, fmt, ...) if(!(condition)) { GAMMA_ERROR(fmt, __VA_ARGS__); psnip_trap(); }
#define GAMMA_ASSERT_CRITICAL(condition, fmt, ...) if(!(condition)) { GAMMA_CRITICAL(fmt, __VA_ARGS__); psnip_trap(); }
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
