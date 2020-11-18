// Copyright (c) 2020 Shadax-stack <shadax32@gmail.com>
// This file is subject to the terms and conditions defined in
// file 'LICENSE.txt', which is part of this source code package.
#ifndef GAMMA_LOG_H
#define GAMMA_LOG_H

#include "../Gamma.h"
#include <stdio.h>
#include <string>
#include <vector>

namespace Gamma {

	enum class MessageType {
		INFO,  // Should be used for information
		WARN,  // Should be used for warning
		ERROR, // Should be used for a recoverable error
		FATAL, // Should be used for an unrecoverable error 
	};

	//A message itself
	struct GAMMA_API Message {
	public:
		Message(const std::string& msg, MessageType type = MessageType::INFO);
		Message(void);
		uint64_t Time; //Time in nanoseconds since we are using a 64-bit integer anyway
		std::string Content; //Content of the message
		MessageType Type;
	};

	struct GAMMA_API MessageStream {
	public:
		MessageStream(void);
		MessageStream(size_t size);
		~MessageStream(void);
		char* Data;
		size_t Size;
	};

	//A wrapper for a ostream that outputs to something like the console or a file
	struct GAMMA_API MessageOutput {
	public:
		MessageOutput(FILE* out, const char* format = "C\n");
		MessageOutput(const MessageOutput& copy);
		MessageOutput(const std::string& path, const char* format = "C\n");
		MessageOutput(const MessageStream& stream, const char* format = "C\n");
		void SendMessage(const Message& msg);
		void SendMessage(const std::string& msg);
		void SetFormat(const std::string& newFormat);
		MessageOutput& operator<<(const Message& msg);
		MessageOutput& operator<<(const std::string& msg);
		MessageOutput& operator=(const MessageOutput& copy);
	private:
		FILE* Output; //Handle to the file
		std::string Format; //Format for the log
		std::string FormatMessage(const Message& msg);
	};

	class GAMMA_API Logger {
	public:
		void AddOutput(MessageOutput& output);
		void SendMessage(const std::string& msg);
		Logger& operator<<(const std::string& msg);
	private:
		std::vector<MessageOutput> Outputs;
	};

}

#endif
