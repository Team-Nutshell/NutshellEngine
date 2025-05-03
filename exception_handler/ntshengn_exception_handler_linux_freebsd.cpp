#include "ntshengn_exception_handler_linux_freebsd.h"

void NtshEngn::ExceptionHandler::setExceptionHandler() {
	m_signalStack.ss_size = SIGSTKSZ;
	m_signalStack.ss_flags = 0;
	m_signalStack.ss_sp = mmap(NULL, SIGSTKSZ, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
	if (m_signalStack.ss_sp != MAP_FAILED) {
		if (sigaltstack(&m_signalStack, 0) < 0) {
			NTSHENGN_CORE_ERROR("Could not sigaltstack.");
		}
	}
	else {
		NTSHENGN_CORE_ERROR("Could not mmap the signal stack.");
	}

	struct sigaction sigAction = {};
	sigAction.sa_sigaction = signalHandler;
	sigemptyset(&sigAction.sa_mask);
	sigAction.sa_flags = SA_SIGINFO | SA_ONSTACK;

	if (sigaction(SIGILL, &sigAction, NULL) != 0) {
		NTSHENGN_CORE_ERROR("Could not the set the action for signal SIGILL.");
	}
	if (sigaction(SIGSEGV, &sigAction, NULL) != 0) {
		NTSHENGN_CORE_ERROR("Could not the set the action for signal SIGSEGV.");
	}
	if (sigaction(SIGBUS, &sigAction, NULL) != 0) {
		NTSHENGN_CORE_ERROR("Could not the set the action for signal SIGBUS.");
	}
	if (sigaction(SIGABRT, &sigAction, NULL) != 0) {
		NTSHENGN_CORE_ERROR("Could not the set the action for signal SIGABRT.");
	}
	if (sigaction(SIGTRAP, &sigAction, NULL) != 0) {
		NTSHENGN_CORE_ERROR("Could not the set the action for signal SIGTRAP.");
	}
	if (sigaction(SIGFPE, &sigAction, NULL) != 0) {
		NTSHENGN_CORE_ERROR("Could not the set the action for signal SIGFPE.");
	}
}

void NtshEngn::ExceptionHandler::unsetExceptionHandler() {
	munmap(m_signalStack.ss_sp, SIGSTKSZ);
}