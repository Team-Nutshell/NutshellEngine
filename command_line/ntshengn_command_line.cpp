#include "ntshengn_command_line.h"

void NtshEngn::CommandLine::set(int argc, char** argv) {
	m_commandLine[argv[0]] = std::vector<std::string>();
	
	std::string currentCommand;
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			currentCommand = argv[i];
			m_commandLine[currentCommand] = std::vector<std::string>();
		}
		else {
			m_commandLine[currentCommand].push_back(argv[i]);
		}
	}
}

const std::unordered_map<std::string, std::vector<std::string>>& NtshEngn::CommandLine::get() {
	return m_commandLine;
}