#pragma once
#include "../../Common/job_system/ntshengn_job_system_interface.h"
#include "../utils/ntshengn_utils_thread_safe_queue.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>
#include <cstdint>

namespace NtshEngn {

	struct JobSharedData {
		ThreadSafeQueue<std::function<void()>> jobQueue;
		std::condition_variable wakeCondition;
		std::mutex wakeMutex;
		std::atomic<uint32_t> currentJobs;
		bool running;
	};

	class JobSystem : public JobSystemInterface {
	public:
		void init();
		void destroy();

		void execute(const std::function<void()>& job);
		void dispatch(uint32_t jobCount, uint32_t jobsPerWorker, const std::function<void(JobDispatchArguments)>& job);

		bool isBusy();
		void wait();

		uint32_t getNumThreads() const;

	private:
		uint32_t m_numThreads = 0;
		std::vector<std::thread> m_threads;
		JobSharedData m_sharedData;
	};

}