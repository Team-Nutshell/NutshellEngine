#pragma once
#include "../../Common/profiler/ntshengn_profiler_interface.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <numeric>

namespace NtshEngn {

	class Profiler : public ProfilerInterface {
	private:
		struct Node {
			std::string name = "";

			std::vector<double> times;

			double start;

			Node* parent = nullptr;
			std::vector<Node*> children;

			ProfilerResultNode getResults() const {
				ProfilerResultNode resultNode;
				resultNode.name = name;
				resultNode.count = static_cast<uint32_t>(times.size());
				resultNode.totalTime = std::accumulate(times.begin(), times.end(), 0.0);
				resultNode.meanTime = resultNode.totalTime / static_cast<double>(resultNode.count);
				resultNode.minTimeIndex = static_cast<uint32_t>(std::distance(times.begin(), std::min_element(times.begin(), times.end())));
				resultNode.minTime = times[resultNode.minTimeIndex];
				resultNode.maxTimeIndex = static_cast<uint32_t>(std::distance(times.begin(), std::max_element(times.begin(), times.end())));
				resultNode.maxTime = times[resultNode.maxTimeIndex];

				std::vector<double> sortedTimes = times;
				std::sort(sortedTimes.begin(), sortedTimes.end());
				resultNode.medianTime = sortedTimes[resultNode.count / 2];

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