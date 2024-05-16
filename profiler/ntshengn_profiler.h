#pragma once
#include "../../Common/profiler/ntshengn_profiler_interface.h"
#include <vector>
#include <unordered_map>
#include <string>

namespace NtshEngn {

	class Profiler : public ProfilerInterface {
	private:
		struct Node {
			std::string name = "";

			std::vector<double> times;
			double totalTime = 0.0;
			double meanTime = 0.0;
			uint32_t minTimeIndex = 0;
			double minTime = 0.0;
			uint32_t maxTimeIndex = 0;
			double maxTime = 0.0;

			double start;

			Node* parent = nullptr;
			std::vector<Node*> children;

			ProfilerResultNode getResults() const {
				ProfilerResultNode resultNode;
				resultNode.name = name;
				resultNode.times = times;
				resultNode.totalTime = totalTime;
				resultNode.meanTime = meanTime;
				resultNode.minTimeIndex = minTimeIndex;
				resultNode.minTime = times[resultNode.minTimeIndex];
				resultNode.maxTimeIndex = maxTimeIndex;
				resultNode.maxTime = maxTime;

				for (const Node* child : children) {
					resultNode.children.push_back(child->getResults());
				}

				return resultNode;
			}
		};

	public:
		void start(const std::string& sessionName);

		ProfilerResultNode getResults();

		ProfilerResultNode end();

		bool isRunning() const;

		void startBlock(const std::string& blockName);
		void endBlock();

	private:
		std::unordered_map<std::string, Node> m_profilerNodes;
		Node m_rootNode;
		Node* m_currentNode = nullptr;

		bool m_isRunning = false;
	};

}