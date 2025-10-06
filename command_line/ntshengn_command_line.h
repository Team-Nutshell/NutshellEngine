#pragma once
#include "../Common/command_line/ntshengn_command_line_interface.h"

namespace NtshEngn {

	class CommandLine : public CommandLineInterface {
	public:
		void set(int argc, char** argv);
		const std::unordered_map<std::string, std::vector<std::string>>& get();

	private:
		std::unordered_map<std::string, std::vector<std::string>> m_commandLine;
	};

}