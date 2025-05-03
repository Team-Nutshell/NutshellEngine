#pragma once
#include "../utils/ntshengn_core_defines.h"
#include <windows.h>
#include <ImageHlp.h>
#undef far
#undef near
#include <string>
#include <filesystem>

namespace NtshEngn {

	class ExceptionHandler {
	public:
		static std::string exceptionCodeString(DWORD exceptionCode) {
			switch (exceptionCode) {
			case EXCEPTION_ACCESS_VIOLATION:
				return "EXCEPTION_ACCESS_VIOLATION";

			case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
				return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";

			case EXCEPTION_BREAKPOINT:
				return "EXCEPTION_BREAKPOINT";

			case EXCEPTION_DATATYPE_MISALIGNMENT:
				return "EXCEPTION_DATATYPE_MISALIGNMENT";

			case EXCEPTION_FLT_DENORMAL_OPERAND:
				return "EXCEPTION_FLT_DENORMAL_OPERAND";

			case EXCEPTION_FLT_DIVIDE_BY_ZERO:
				return "EXCEPTION_FLT_DIVIDE_BY_ZERO";

			case EXCEPTION_FLT_INEXACT_RESULT:
				return "EXCEPTION_FLT_INEXACT_RESULT";

			case EXCEPTION_FLT_INVALID_OPERATION:
				return "EXCEPTION_FLT_INVALID_OPERATION";

			case EXCEPTION_FLT_OVERFLOW:
				return "EXCEPTION_FLT_OVERFLOW";

			case EXCEPTION_FLT_STACK_CHECK:
				return "EXCEPTION_FLT_STACK_CHECK";

			case EXCEPTION_FLT_UNDERFLOW:
				return "EXCEPTION_FLT_UNDERFLOW";

			case EXCEPTION_ILLEGAL_INSTRUCTION:
				return "EXCEPTION_ILLEGAL_INSTRUCTION";

			case EXCEPTION_IN_PAGE_ERROR:
				return "EXCEPTION_IN_PAGE_ERROR";

			case EXCEPTION_INT_DIVIDE_BY_ZERO:
				return "EXCEPTION_INT_DIVIDE_BY_ZERO";

			case EXCEPTION_INT_OVERFLOW:
				return "EXCEPTION_INT_OVERFLOW";

			case EXCEPTION_INVALID_DISPOSITION:
				return "EXCEPTION_INVALID_DISPOSITION";

			case EXCEPTION_NONCONTINUABLE_EXCEPTION:
				return "EXCEPTION_NONCONTINUABLE_EXCEPTION";

			case EXCEPTION_PRIV_INSTRUCTION:
				return "EXCEPTION_PRIV_INSTRUCTION";

			case EXCEPTION_SINGLE_STEP:
				return "EXCEPTION_SINGLE_STEP";

			case EXCEPTION_STACK_OVERFLOW:
				return "EXCEPTION_STACK_OVERFLOW";

			default:
				return "Unknown Exception";
			}
		}

		static std::string stackTrace(PCONTEXT contextRecord) {
			std::string message;

			SymInitialize(GetCurrentProcess(), 0, true);

			STACKFRAME frame = { 0 };
			frame.AddrPC.Offset = contextRecord->Rip;
			frame.AddrPC.Mode = AddrModeFlat;
			frame.AddrStack.Offset = contextRecord->Rsp;
			frame.AddrStack.Mode = AddrModeFlat;
			frame.AddrFrame.Offset = contextRecord->Rbp;
			frame.AddrFrame.Mode = AddrModeFlat;

			DWORD displacement = 0;
			IMAGEHLP_LINE line;

			while (StackWalk(IMAGE_FILE_MACHINE_AMD64, GetCurrentProcess(), GetCurrentThread(), &frame, contextRecord, 0, SymFunctionTableAccess, SymGetModuleBase, 0)) {
				if (SymGetLineFromAddr(GetCurrentProcess(), frame.AddrPC.Offset, &displacement, &line)) {
					message += std::filesystem::path(line.FileName).filename().string() + " - Line: " + std::to_string(line.LineNumber) + "\n";
				}
			}

			SymCleanup(GetCurrentProcess());

			return message;
		}

		static LONG WINAPI unhandledExceptionFilter(struct _EXCEPTION_POINTERS* lpExceptionInfo) {
			std::string message;
			message += "Exception:\n";
			message += "Code: " + exceptionCodeString(lpExceptionInfo->ExceptionRecord->ExceptionCode);
			if (lpExceptionInfo->ExceptionRecord->ExceptionCode != EXCEPTION_STACK_OVERFLOW) {
				message += "\nStack Trace:\n" + stackTrace(lpExceptionInfo->ContextRecord);
				message.pop_back();
			}

			NTSHENGN_CORE_ERROR(message);

			return EXCEPTION_EXECUTE_HANDLER;
		}
		
	public:
		void setExceptionHandler();
		void unsetExceptionHandler();
	};

}