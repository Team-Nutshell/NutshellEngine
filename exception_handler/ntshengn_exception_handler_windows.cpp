#include "ntshengn_exception_handler_windows.h"

void NtshEngn::ExceptionHandler::setExceptionHandler() {
	SetUnhandledExceptionFilter(unhandledExceptionFilter);
}

void NtshEngn::ExceptionHandler::unsetExceptionHandler() {
	SetUnhandledExceptionFilter(NULL);
}