#pragma once
#include "../utils/ntshengn_core_defines.h"
#include "../Common/utils/ntshengn_defines.h"
#include <execinfo.h>
#include <signal.h>
#include <sys/mman.h>
#include <string>
#include <vector>
#include <utility>
#include <filesystem>

namespace NtshEngn {

	class ExceptionHandler {
	public:
		static std::string signalString(int signo, int si_code) {
			switch (signo) {
			case SIGILL:
				switch (si_code) {
				case ILL_ILLOPC:
					return "SIGILL - ILLOPC";

				case ILL_ILLOPN:
					return "SIGILL - ILLOPN";

				case ILL_ILLADR:
					return "SIGILL - ILLADR";

				case ILL_ILLTRP:
					return "SIGILL - ILLTRP";

				case ILL_PRVOPC:
					return "SIGILL - PRVOPC";

				case ILL_PRVREG:
					return "SIGILL - PRVREG";

				case ILL_COPROC:
					return "SIGILL - COPROC";

				case ILL_BADSTK:
					return "SIGILL - BADSTK";

				default:
					return "SIGILL";
				}

			case SIGSEGV:
				return "SIGSEGV";

			case SIGBUS:
				return "SIGBUS";

			case SIGABRT:
				return "SIGABRT";

			case SIGTRAP:
				return "SIGTRAP";

			case SIGFPE:
				switch (si_code) {
				case FPE_INTOVF:
					return "SIGFPE - INTOVF";

				case FPE_INTDIV:
					return "SIGFPE - INTDIV";

				case FPE_FLTOVF:
					return "SIGFPE - FLTOVF";

				case FPE_FLTDIV:
					return "SIGFPE - FLTDIV";

				case FPE_FLTUND:
					return "SIGFPE - FLTUND";

				default:
					return "SIGFPE";
				}

			default:
				return "Unknown Signal";
			}
		}

		static std::string stackTrace() {
			std::string message;

			const size_t MAX_TRACE_SIZE = 64;
			void* stackTraces[MAX_TRACE_SIZE];
			int traceSize = backtrace(stackTraces, MAX_TRACE_SIZE);
			char** symbols = backtrace_symbols(stackTraces, traceSize);

			std::vector<std::pair<std::string, std::string>> addresses;
			for (int i = 3; i < (traceSize - 1); i++) {
				std::string symbolsString = std::string(symbols[i]);
				size_t openParenthesisPosition = symbolsString.find("(");
				size_t closedParenthesisPosition = symbolsString.find(')', openParenthesisPosition);
				addresses.push_back({ std::filesystem::canonical(symbolsString.substr(0, openParenthesisPosition)), symbolsString.substr(openParenthesisPosition + 1, closedParenthesisPosition - (openParenthesisPosition + 1)) });
			}

			for (size_t i = 0; i < addresses.size(); i++) {
				std::string command = "addr2line -f -p -C -e" + addresses[i].first + " " + addresses[i].second;
				FILE* fp = popen(command.c_str(), "r");
				char stdOutBuffer[4096];
				std::string addr2Line;
				while (fgets(stdOutBuffer, 4096, fp) != NULL) {
					addr2Line += std::string(stdOutBuffer);
				}

				pclose(fp);

				size_t messageCursor = 0;
				size_t previousMessageCursor = 0;
				size_t cppPosition = addr2Line.find(".cpp", messageCursor);
				size_t hPosition = addr2Line.find(".h", messageCursor);
				while ((cppPosition != std::string::npos) || (hPosition != std::string::npos)) {
					previousMessageCursor = messageCursor;

					if (cppPosition < hPosition) {
						messageCursor = cppPosition + 4;
					}
					else {
						messageCursor = hPosition + 2;
					}

					std::string untilExtension = addr2Line.substr(previousMessageCursor, messageCursor - previousMessageCursor);

					size_t lastSpacePosition = untilExtension.find_last_of(' ', messageCursor);

					message += untilExtension.substr(0, lastSpacePosition + 1);
					message += std::filesystem::path(untilExtension.substr(lastSpacePosition)).filename();

					cppPosition = addr2Line.find(".cpp", messageCursor);
					hPosition = addr2Line.find(".h", messageCursor);
				}
				message += addr2Line.substr(messageCursor);
			}

			if (symbols) {
				free(symbols);
			}

			return message;
		}

		static void signalHandler(int signo, siginfo_t* info, void* other) {
			NTSHENGN_UNUSED(other);

			std::string message;
			message += "Signal:\n";
			message += "Code: " + signalString(signo, info->si_code);
			message += "\nStack Trace:\n" + stackTrace();
			message.pop_back();

			NTSHENGN_CORE_ERROR(message);
		}

	public:
		void setExceptionHandler();
		void unsetExceptionHandler();

	private:
		stack_t m_signalStack;
	};

}
