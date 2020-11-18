// Copyright (c) 2020 Shadax-stack <shadax32@gmail.com>
// This file is subject to the terms and conditions defined in
// file 'LICENSE.txt', which is part of this source code package.
#include "Log.h"
#include <time.h>
#include <assert.h> 
#include <sstream>

namespace Gamma {

	uint64_t TimeOffset;

	Message::Message(const std::string& msg, MessageType type) : Content(msg), Type(type) {
		_timespec64 time;
		int time_get = _timespec64_get(&time, TIME_UTC);
		assert(time_get);
		Time = (time.tv_sec + TimeOffset) * 1000000000 + time.tv_nsec;
	}

	Message::Message(void) {
		_timespec64 time;
		int time_get = _timespec64_get(&time, TIME_UTC);
		assert(time_get);
		Time = (time.tv_sec + TimeOffset) * 1000000000 + time.tv_nsec;
	}

	MessageStream::MessageStream(void) : Data(nullptr), Size(0) {}

	//Setting each byte to 0 is quit slow but most likely the only option with such form of rerouting to a stream
	MessageStream::MessageStream(size_t size) : Data(new char[size] {'\0'}), Size(size) {}

	MessageStream::~MessageStream(void) {
		if (Data) {
			delete[] Data;
		}
	}

	MessageOutput::MessageOutput(FILE* out, const char* format) : Output(out), Format(format) {}

	MessageOutput::MessageOutput(const MessageOutput& copy) : Output(copy.Output), Format(copy.Format) {}

	MessageOutput::MessageOutput(const std::string& path, const char* format) : Format(format), Output(nullptr) {
		fopen_s(&Output, path.c_str(), "w");
	}

	MessageOutput::MessageOutput(const MessageStream& stream, const char* format) : Format(format), Output(nullptr) {
		fopen_s(&Output, 
			    #ifdef _WIN32
				"NUL",
			    #else
				"/dev/null"
			    #endif
			    "w");
		assert(Output);
		setvbuf(Output, stream.Data, _IOFBF, stream.Size);
	}

	void MessageOutput::SendMessage(const Message& msg) {
		std::string formattedMsg = FormatMessage(msg);
		fwrite(formattedMsg.c_str(), sizeof(const char), formattedMsg.length(), Output);
	}

	void MessageOutput::SendMessage(const std::string& msg) {
		SendMessage(Message(msg));
	}

	void MessageOutput::SetFormat(const std::string& newFormat) {
		Format = newFormat;
	}

	MessageOutput& MessageOutput::operator=(const MessageOutput& copy) {
		this->Format = copy.Format;
		this->Output - copy.Output;
		return *this;
	}

	MessageOutput& MessageOutput::operator<<(const Message& msg) {
		SendMessage(msg);
		return *this;
	}

	MessageOutput& MessageOutput::operator<<(const std::string& msg) {
		SendMessage(msg);
		return *this;
	}

	//TODO: Find a cross-platform way to colorize text
	std::string MessageOutput::FormatMessage(const Message& msg) {
		std::stringstream FormattedMessageBuilder;
		for (char format : Format) {
			switch (format) {
				case 'H': {
					FormattedMessageBuilder << msg.Time / 1000000000 / 3600 % 24;
					break;
				} 
				case 'M': {
					FormattedMessageBuilder << msg.Time / 1000000000 / 60 % 60;
					break;
				}
				case 'S': {
					FormattedMessageBuilder << msg.Time / 1000000000 % 60;
					break;
				}
				case 'C': {
					FormattedMessageBuilder << msg.Content;
					break;
				}
				case 'T': {
					const char* InfoLogNames[4] = {
						"INFO",
						"WARN",
						"ERROR",
						"FATAL"
					};
					FormattedMessageBuilder << InfoLogNames[(uint32_t)msg.Type];
					break;
				}
				default: {
					FormattedMessageBuilder << format;
					break;
				} 
			}
		}
		return FormattedMessageBuilder.str();
	}

	void Logger::AddOutput(MessageOutput& output) {
		Outputs.push_back(output);
	}

	void Logger::SendMessage(const std::string& msg) {
		Message message(msg);
		for (MessageOutput& output : Outputs) {
			output << message;
		}
	}

	Logger& Logger::operator<<(const std::string& msg) {
		SendMessage(msg);
		return *this;
	}

}