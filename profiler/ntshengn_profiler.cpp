#include "ntshengn_profiler.h"
#include "../../Common/utils/ntshengn_defines.h"
#include <chrono>
#include <algorithm>
#include <numeric>

void NtshEngn::Profiler::start(const std::string& sessionName) {
	if (!m_isRunning) {
		m_rootNode.name = sessionName;
		m_rootNode.start = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

		m_currentNode = &m_rootNode;

		m_isRunning = true;
	}
}

NtshEngn::ProfilerResultNode NtshEngn::Profiler::getResults() {
	ProfilerResultNode resultNode;

	if (m_isRunning) {
		resultNode.name = m_rootNode.name;
		resultNode.count = 1;
		resultNode.totalTime = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - m_rootNode.start;

		for (const Node* child : m_rootNode.children) {
			resultNode.children.push_back(child->getResults());
		}
	}

	return resultNode;
}

NtshEngn::ProfilerResultNode NtshEngn::Profiler::end() {
	ProfilerResultNode resultNode = getResults();
	m_profilerNodes.clear();
	m_isRunning = false;

	return resultNode;
}

bool NtshEngn::Profiler::isRunning() const {
	return m_isRunning;
}

void NtshEngn::Profiler::startBlock(const std::string& blockName) {
	if (m_isRunning) {
		if (m_profilerNodes.find(blockName) == m_profilerNodes.end()) {
			Node newProfilerNode;
			newProfilerNode.name = blockName;
			newProfilerNode.parent = m_currentNode;
			m_profilerNodes[blockName] = newProfilerNode;
					
			m_currentNode->children.push_back(&m_profilerNodes[blockName]);
		}
		Node* profilerNode = &m_profilerNodes[blockName];

		m_currentNode = profilerNode;

		profilerNode->start = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	}
}

void NtshEngn::Profiler::endBlock() {
	if (m_isRunning) {
		if (m_currentNode != &m_rootNode) {
			const double blockTime = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - m_currentNode->start;

			m_currentNode->times.push_back(blockTime);

			m_currentNode = m_currentNode->parent;
		}
	}
}