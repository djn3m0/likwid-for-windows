#ifdef WIN32
#include <Windows.h>
#include <tchar.h>
#include <errno.h>
#include <osdep/windowsError_win.h>
#else
#include <types.h>

#include <sched.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>

#define gettid() syscall(SYS_gettid)

#endif

#include <stdio.h>
#include <osdep/affinity.h>
#include <osdep/numOfProcessors.h>

static int
getProcessorID(AffinityMask *affinityMask)
{
    int processorId;

    for (processorId=0;processorId<24;processorId++){
        if (AffinityMask_contains(affinityMask, processorId))
        {
            return processorId;
        }
    }
    fprintf(stderr, "Could not determine processor id from process affinity mask\n");
	return 25;
}

int  affinity_processGetProcessorId()
{
#ifdef WIN32
	DWORD_PTR processAffinityMask;
	DWORD_PTR systemAffinityMask;

	BOOL res = GetProcessAffinityMask(
	  GetCurrentProcess(),
	  &processAffinityMask,
	  &systemAffinityMask
	);
	if (res == 0) {
		perror("GetCurrentProcess");
		exit(1);
	}
	return getProcessorID(&processAffinityMask);
#else
    cpu_set_t processAffinityMask;
    CPU_ZERO(&processAffinityMask);
    sched_getaffinity(getpid(),sizeof(cpu_set_t), &processAffinityMask);

    return getProcessorID(&processAffinityMask);
#endif
}


int  affinity_threadGetProcessorId(ThreadId threadId)
{
#ifdef WIN32
	DWORD_PTR
		tmpThreadAffinityMask = (1 << 0),
		tmpThreadAffinityMask1 = -1;

	// temporarily set the thread affinity mask and retrieve old affinity mask
	DWORD_PTR origThreadAffinityMask = SetThreadAffinityMask(
		threadId,
		tmpThreadAffinityMask
	);
	if (origThreadAffinityMask == 0) {
		perror("SetThreadAffinityMask, used to get the thread affinity mask (1)");
		exit(1);
	}
	// reset the thread affinity mask to its original value
	tmpThreadAffinityMask1 = SetThreadAffinityMask(
		threadId,
		origThreadAffinityMask
	);
	if (tmpThreadAffinityMask1 == 0) {
		perror("SetThreadAffinityMask, used to get the thread affinity mask (2)");
		exit(1);
	}
	// check whether the resetting succeeded
	if (tmpThreadAffinityMask1 != tmpThreadAffinityMask) {
		perror("SetThreadAffinityMask, failed to reset thread affinity mask");
		exit(1);
	}
	return getProcessorID(&origThreadAffinityMask);
#else
	AffinityMask affinityMask;
	if (pthread_getaffinity_np(threadId, sizeof(AffinityMask), &affinityMask))
    {
        perror("pthread_getaffinity_np failed");
        return FALSE;
    }
    return getProcessorID(&affinityMask);
#endif
}

ThreadId affinity_getCurrentThreadId()
{
#ifdef WIN32
	return GetCurrentThread();
#else
	return pthread_self();
#endif
}

int  affinity_pinProcess(int processorId)
{
	AffinityMask mask;
	AffinityMask_clear(&mask);
	AffinityMask_insert(&mask, processorId);
	return affinity_setProcessAffinityMask(mask);
}

int  affinity_pinThread(ThreadId threadId, int processorId)
{
	AffinityMask mask;
	AffinityMask_clear(&mask);
	AffinityMask_insert(&mask, processorId);
	return affinity_setThreadAffinityMask(threadId, mask);
}

int affinity_setProcessAffinityMask(AffinityMask affinityMask)
{
#ifdef WIN32
	BOOL r = SetProcessAffinityMask(
		GetCurrentProcess(),
		affinityMask
	);
	if (r == 0) {
		setErrnoFromLastWindowsError();
		return FALSE;
	}
	return TRUE;
#else
	if (sched_setaffinity(0, sizeof(AffinityMask), &affinityMask) == -1)
	{
		//perror("sched_setaffinity failed");
		/*
		TODO: take this error treatment or the one above?
		if (errno == EFAULT)
        {
            fprintf(stderr, "A supplied memory address was invalid\n");
            exit(EXIT_FAILURE);
        }
        else if (errno == EINVAL)
        {
            fprintf(stderr, "Processor is not available\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            fprintf(stderr, "Unknown error\n");
            exit(EXIT_FAILURE);
        }
		*/
        return FALSE;
	}

    return TRUE;
#endif
}

extern int affinity_setThreadAffinityMask(ThreadId threadId, AffinityMask affinityMask)
{
#ifdef WIN32
	DWORD_PTR origThreadAffinityMask = SetThreadAffinityMask(
		threadId,
		affinityMask
	);
	if (origThreadAffinityMask == 0) {
		setErrnoFromLastWindowsError();
		return FALSE;
	}
	return TRUE;
#else
    if (pthread_setaffinity_np(threadId, sizeof(AffinityMask), &affinityMask))
    {
        perror("pthread_setaffinity_np failed");
        return FALSE;
    }

    return TRUE;
#endif
}

AffinityMask affinity_getLargestProcessAffinityMask()
{
	int numProcs = numOfProcessors();
	int processorId;

	AffinityMask mask;
	AffinityMask_clear(&mask);
	
	for(processorId=0; processorId<numProcs; processorId++) {
		AffinityMask_insert(&mask, processorId);
	}
	return mask;
}
