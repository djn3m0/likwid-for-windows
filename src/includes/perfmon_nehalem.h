/*
 * ===========================================================================
 *
 *       Filename:  perfmon_nehalem.h
 *
 *    Description:  Header File of perfmon module.
 *                  Configures and reads out performance counters
 *                  on x86 based architectures. Supports multi threading.
 *
 *        Version:  1.0
 *        Created:  07/15/2009
 *       Revision:  none
 *
 *         Author:  Jan Treibig (jt), jan.treibig@gmail.com
 *        Company:  RRZE Erlangen
 *        Project:  none
 *      Copyright:  Copyright (c) 2009, Jan Treibig
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License, v2, as
 *      published by the Free Software Foundation
 *     
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *     
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * ===========================================================================
 */
#include <stdlib.h>
#include <stdio.h>

#include <bstrlib.h>
#include <types.h>
#include <registers.h>
#include <perfmon_nehalem_events.h>

#define NUM_COUNTERS_NEHALEM 14
#define NUM_GROUPS_NEHALEM 8
#define NUM_SETS_NEHALEM 8

static int perfmon_numCountersNehalem = NUM_COUNTERS_NEHALEM;
static int perfmon_numGroupsNehalem = NUM_GROUPS_NEHALEM;
static int perfmon_numArchEventsNehalem = NUM_ARCH_EVENTS_NEHALEM;

static PerfmonCounterMap nehalem_counter_map[NUM_COUNTERS_NEHALEM] = {
    {"PMC0",PMC2},
    {"PMC1",PMC3},
    {"PMC2",PMC4},
    {"PMC3",PMC5},
    {"UPMC0",PMC6},
    {"UPMC1",PMC7},
    {"UPMC2",PMC8},
    {"UPMC3",PMC9},
    {"UPMC4",PMC10},
    {"UPMC5",PMC11},
    {"UPMC6",PMC12},
    {"UPMC7",PMC13},
    {"FIXC0",PMC0},
    {"FIXC1",PMC1}
};

static PerfmonGroupMap nehalem_group_map[NUM_GROUPS_NEHALEM] = {
    {"FLOPS_DP",FLOPS_DP,"Double Precision MFlops/s"},
    {"FLOPS_SP",FLOPS_SP,"Single Precision MFlops/s"},
    {"L2",L2,"L2: L2 cache bandwidth in MBytes/s"},
    {"L3",L3,"L3: L3 cache bandwidth in MBytes/s"},
    {"MEM",MEM,"Main memory bandwidth in MBytes/s"},
    {"DATA",DATA,"Load to store ratio"},
    {"BRANCH",BRANCH,"Branch prediction miss rate"},
    {"TLB",TLB,"Translation lookaside buffer miss rate"}
};

static char* nehalem_group_config[NUM_GROUPS_NEHALEM] = {
    "INSTR_RETIRED_ANY:FIXC0,CPU_CLK_UNHALTED_CORE:FIXC1,FP_COMP_OPS_EXE_SSE_FP_PACKED:PMC0,FP_COMP_OPS_EXE_SSE_FP_SCALAR:PMC1,FP_COMP_OPS_EXE_SSE_SINGLE_PRECISION:PMC2,FP_COMP_OPS_EXE_SSE_DOUBLE_PRECISION:PMC3",
    "INSTR_RETIRED_ANY:FIXC0,CPU_CLK_UNHALTED_CORE:FIXC1,FP_COMP_OPS_EXE_SSE_FP_PACKED:PMC0,FP_COMP_OPS_EXE_SSE_FP_SCALAR:PMC1,FP_COMP_OPS_EXE_SSE_SINGLE_PRECISION:PMC2,FP_COMP_OPS_EXE_SSE_DOUBLE_PRECISION:PMC3",
    "INSTR_RETIRED_ANY:FIXC0,CPU_CLK_UNHALTED_CORE:FIXC1,L1D_REPL:PMC0,L1D_M_EVICT:PMC1",
    "INSTR_RETIRED_ANY:FIXC0,CPU_CLK_UNHALTED_CORE:FIXC1,L2_LINES_IN_ANY:PMC0,L2_LINES_OUT_ANY:PMC1",
    "INSTR_RETIRED_ANY:FIXC0,CPU_CLK_UNHALTED_CORE:FIXC1,UNC_L3_LINES_IN_ANY:UPMC0,UNC_L3_LINES_OUT_ANY:UPMC1",
    "INSTR_RETIRED_ANY:FIXC0,CPU_CLK_UNHALTED_CORE:FIXC1,MEM_INST_RETIRED_LOADS:PMC0,MEM_INST_RETIRED_STORES:PMC1",
    "INSTR_RETIRED_ANY:FIXC0,CPU_CLK_UNHALTED_CORE:FIXC1,BR_INST_RETIRED_ALL_BRANCHES:PMC0,BR_MISP_RETIRED_ALL_BRANCHES:PMC1",
    "INSTR_RETIRED_ANY:FIXC0,CPU_CLK_UNHALTED_CORE:FIXC1,DTLB_MISSES_ANY:PMC0,DTLB_MISSES_MISS_LD:PMC1"
};


void
perfmon_init_nehalem(PerfmonThread *thread)
{
    uint64_t flags = 0x0ULL;
    int cpu_id = thread->processorId;

    /* Fixed Counters: instructions retired, cycles unhalted core */
    thread->counters[PMC0].configRegister = MSR_PERF_FIXED_CTR_CTRL;
    thread->counters[PMC0].counterRegister = MSR_PERF_FIXED_CTR0;
    thread->counters[PMC0].type = FIXED;
    thread->counters[PMC1].configRegister = MSR_PERF_FIXED_CTR_CTRL;
    thread->counters[PMC1].counterRegister = MSR_PERF_FIXED_CTR1;
    thread->counters[PMC1].type = FIXED;

    /* PMC Counters: 4 48bit wide */
    thread->counters[PMC2].configRegister = MSR_PERFEVTSEL0;
    thread->counters[PMC2].counterRegister = MSR_PMC0;
    thread->counters[PMC2].type = PMC;
    thread->counters[PMC3].configRegister = MSR_PERFEVTSEL1;
    thread->counters[PMC3].counterRegister = MSR_PMC1;
    thread->counters[PMC3].type = PMC;
    thread->counters[PMC4].configRegister = MSR_PERFEVTSEL2;
    thread->counters[PMC4].counterRegister = MSR_PMC2;
    thread->counters[PMC4].type = PMC;
    thread->counters[PMC5].configRegister = MSR_PERFEVTSEL3;
    thread->counters[PMC5].counterRegister = MSR_PMC3;
    thread->counters[PMC5].type = PMC;

    /* Uncore PMC Counters: 8 48bit wide */
    thread->counters[PMC6].configRegister = MSR_UNCORE_PERFEVTSEL0;
    thread->counters[PMC6].counterRegister = MSR_UNCORE_PMC0;
    thread->counters[PMC6].type = UNCORE;
    thread->counters[PMC7].configRegister = MSR_UNCORE_PERFEVTSEL1;
    thread->counters[PMC7].counterRegister = MSR_UNCORE_PMC1;
    thread->counters[PMC7].type = UNCORE;
    thread->counters[PMC8].configRegister = MSR_UNCORE_PERFEVTSEL2;
    thread->counters[PMC8].counterRegister = MSR_UNCORE_PMC2;
    thread->counters[PMC8].type = UNCORE;
    thread->counters[PMC9].configRegister = MSR_UNCORE_PERFEVTSEL3;
    thread->counters[PMC9].counterRegister = MSR_UNCORE_PMC3;
    thread->counters[PMC9].type = UNCORE;
    thread->counters[PMC10].configRegister = MSR_UNCORE_PERFEVTSEL4;
    thread->counters[PMC10].counterRegister = MSR_UNCORE_PMC4;
    thread->counters[PMC10].type = UNCORE;
    thread->counters[PMC11].configRegister = MSR_UNCORE_PERFEVTSEL5;
    thread->counters[PMC11].counterRegister = MSR_UNCORE_PMC5;
    thread->counters[PMC11].type = UNCORE;
    thread->counters[PMC12].configRegister = MSR_UNCORE_PERFEVTSEL6;
    thread->counters[PMC12].counterRegister = MSR_UNCORE_PMC6;
    thread->counters[PMC12].type = UNCORE;
    thread->counters[PMC13].configRegister = MSR_UNCORE_PERFEVTSEL7;
    thread->counters[PMC13].counterRegister = MSR_UNCORE_PMC7;
    thread->counters[PMC13].type = UNCORE;

    msr_write(cpu_id, MSR_PERF_FIXED_CTR_CTRL, 0x0ULL);
    msr_write(cpu_id, MSR_PERFEVTSEL0, 0x0ULL);
    msr_write(cpu_id, MSR_PERFEVTSEL1, 0x0ULL);
    msr_write(cpu_id, MSR_PERFEVTSEL2, 0x0ULL);
    msr_write(cpu_id, MSR_PERFEVTSEL3, 0x0ULL);
    msr_write(cpu_id, MSR_PMC0, 0x0ULL);
    msr_write(cpu_id, MSR_PMC1, 0x0ULL);
    msr_write(cpu_id, MSR_PMC2, 0x0ULL);
    msr_write(cpu_id, MSR_PMC3, 0x0ULL);
    msr_write(cpu_id, MSR_PERF_GLOBAL_CTRL, 0x0ULL);
    msr_write(cpu_id, MSR_PERF_GLOBAL_OVF_CTRL, 0x0ULL);
    msr_write(cpu_id, MSR_PEBS_ENABLE, 0x0ULL);
    msr_write(cpu_id, MSR_UNCORE_FIXED_CTR_CTRL, 0x0ULL);
    msr_write(cpu_id, MSR_UNCORE_FIXED_CTR0, 0x0ULL);
    msr_write(cpu_id, MSR_UNCORE_PERFEVTSEL0, 0x0ULL);
    msr_write(cpu_id, MSR_UNCORE_PERFEVTSEL1, 0x0ULL);
    msr_write(cpu_id, MSR_UNCORE_PERFEVTSEL2, 0x0ULL);
    msr_write(cpu_id, MSR_UNCORE_PERFEVTSEL3, 0x0ULL);
    msr_write(cpu_id, MSR_UNCORE_PERFEVTSEL4, 0x0ULL);
    msr_write(cpu_id, MSR_UNCORE_PERFEVTSEL5, 0x0ULL);
    msr_write(cpu_id, MSR_UNCORE_PERFEVTSEL6, 0x0ULL);
    msr_write(cpu_id, MSR_UNCORE_PERFEVTSEL7, 0x0ULL);
    msr_write(cpu_id, MSR_UNCORE_PMC0, 0x0ULL);
    msr_write(cpu_id, MSR_UNCORE_PMC1, 0x0ULL);
    msr_write(cpu_id, MSR_UNCORE_PMC2, 0x0ULL);
    msr_write(cpu_id, MSR_UNCORE_PMC3, 0x0ULL);
    msr_write(cpu_id, MSR_UNCORE_PMC4, 0x0ULL);
    msr_write(cpu_id, MSR_UNCORE_PMC5, 0x0ULL);
    msr_write(cpu_id, MSR_UNCORE_PMC6, 0x0ULL);
    msr_write(cpu_id, MSR_UNCORE_PMC7, 0x0ULL);
    msr_write(cpu_id, MSR_UNCORE_PERF_GLOBAL_CTRL, 0x0ULL);
    msr_write(cpu_id, MSR_UNCORE_PERF_GLOBAL_OVF_CTRL, 0x0ULL);
    msr_write(cpu_id, MSR_UNCORE_ADDR_OPCODE_MATCH, 0x0ULL);
    msr_write(cpu_id, MSR_OFFCORE_RSP0, 0x0ULL);

    /* initialize fixed counters
     * FIXED 0: Instructions retired
     * FIXED 1: Clocks unhalted */
    msr_write(cpu_id, MSR_PERF_FIXED_CTR_CTRL, 0x22ULL);
    /* UNCORE FIXED 0: Uncore cycles */
    msr_write(cpu_id, MSR_UNCORE_FIXED_CTR_CTRL, 0x01ULL);

    /* Preinit of PERFEVSEL registers */
    flags |= (1<<22);  /* enable flag */

    msr_write(cpu_id, MSR_UNCORE_PERFEVTSEL0, flags);
    msr_write(cpu_id, MSR_UNCORE_PERFEVTSEL1, flags);
    msr_write(cpu_id, MSR_UNCORE_PERFEVTSEL2, flags);
    msr_write(cpu_id, MSR_UNCORE_PERFEVTSEL3, flags);
    msr_write(cpu_id, MSR_UNCORE_PERFEVTSEL4, flags);
    msr_write(cpu_id, MSR_UNCORE_PERFEVTSEL5, flags);
    msr_write(cpu_id, MSR_UNCORE_PERFEVTSEL6, flags);
    msr_write(cpu_id, MSR_UNCORE_PERFEVTSEL7, flags);

    flags |= (1<<16);  /* user mode flag */
#if 0
    flags |= (1<<19);  /* pin control flag */
#endif

    msr_write(cpu_id, MSR_PERFEVTSEL0, flags);
    msr_write(cpu_id, MSR_PERFEVTSEL1, flags);
    msr_write(cpu_id, MSR_PERFEVTSEL2, flags);
    msr_write(cpu_id, MSR_PERFEVTSEL3, flags);
}


void
perfmon_setupCounterThread_nehalem(int thread_id,
        uint32_t event, uint32_t umask,
        PerfmonCounterIndex index)
{
    uint64_t flags;
    uint64_t reg = threadData[thread_id].counters[index].configRegister;
    int cpu_id = threadData[thread_id].processorId;

    if ((threadData[thread_id].counters[index].type == PMC) || threadData[thread_id].counters[index].type == UNCORE)
    {

        threadData[thread_id].counters[index].init = TRUE;
        flags = msr_read(cpu_id,reg);
        flags &= ~(0xFFFFU); 

        /* Intel with standard 8 bit event mask: [7:0] */
        flags |= (umask<<8) + event;

        msr_write(cpu_id, reg , flags);

        if (perfmon_verbose)
        {
            printf("[%d] perfmon_setup_counter: Write Register 0x%llX , Flags: 0x%llX \n",
                    cpu_id,
                    LLU_CAST reg,
                    LLU_CAST flags);
        }
    }
    else if (threadData[thread_id].counters[index].type == FIXED)
    {
        threadData[thread_id].counters[index].init = TRUE;
    }
}


void
perfmon_startCountersThread_nehalem(int thread_id)
{
    int i;
    uint64_t flags = 0x0ULL;
    uint64_t uflags = 0x0ULL;
    int cpu_id = threadData[thread_id].processorId;

    msr_write(cpu_id, MSR_PERF_GLOBAL_CTRL, 0x0ULL);
//    msr_write(cpu_id, MSR_PERF_FIXED_CTR0, 0x0ULL);
//    msr_write(cpu_id, MSR_PERF_FIXED_CTR1, 0x0ULL);
    msr_write(cpu_id, MSR_UNCORE_PERF_GLOBAL_CTRL, 0x0ULL);

    /* Enable fixed counters */
//    flags  = 0x300000000ULL;
    uflags = 0x100000000ULL;

    for (i=0;i<NUM_PMC;i++) {
        if (threadData[thread_id].counters[i].init == TRUE) {
            msr_write(cpu_id, threadData[thread_id].counters[i].counterRegister , 0x0ULL);


            if (threadData[thread_id].counters[i].type == PMC)
            {
                flags |= (1<<(i-2));  /* enable counter */
            }
            else if (threadData[thread_id].counters[i].type == FIXED)
            {
                flags |= (1ULL<<(i+32));  /* enable fixed counter */
            }
            else if (threadData[thread_id].counters[i].type == UNCORE)
            {
                uflags |= (1<<(i-6));  /* enable uncore counter */
            }
        }
    }

    if (perfmon_verbose)
    {
        printf("perfmon_start_counters: Write Register 0x%X , Flags: 0x%llX \n",MSR_PERF_GLOBAL_CTRL, LLU_CAST flags);
        printf("perfmon_start_counters: Write Register 0x%X , Flags: 0x%llX \n",MSR_UNCORE_PERF_GLOBAL_CTRL, LLU_CAST uflags);
    }

    msr_write(cpu_id, MSR_PERF_GLOBAL_CTRL, flags);
    msr_write(cpu_id, MSR_UNCORE_PERF_GLOBAL_CTRL, uflags);
    msr_write(cpu_id, MSR_PERF_GLOBAL_OVF_CTRL, 0x30000000FULL);

}

void 
perfmon_stopCountersThread_nehalem(int thread_id)
{
    uint64_t flags;
    int i;
    int cpu_id = threadData[thread_id].processorId;

    msr_write(cpu_id, MSR_PERF_GLOBAL_CTRL, 0x0ULL);
    if (cpuid_info.model == NEHALEM)
    {
        msr_write(cpu_id, MSR_UNCORE_PERF_GLOBAL_CTRL, 0x0ULL);
    }

    for (i=0; i<NUM_COUNTERS_NEHALEM; i++) 
    {
        if (threadData[thread_id].counters[i].init == TRUE) 
        {
            threadData[thread_id].counters[i].counterData = msr_read(cpu_id, threadData[thread_id].counters[i].counterRegister);
        }
    }

    flags = msr_read(cpu_id,MSR_PERF_GLOBAL_STATUS);
    printf ("Status: 0x%llX \n", LLU_CAST flags);
    if((flags & 0x3) || (flags & (0x3ULL<<32)) ) 
    {
        printf ("Overflow occured \n");
    }

}

void
perfmon_printDerivedMetricsNehalem(PerfmonGroup group)
{
    int i;
    int j;
    int threadId;
    double time = 0.0;
    double cpi = 0.0;
    double inverseClock = 1.0 /(double) timer_getCpuClock();
    PerfmonResultTable tableData;
    int numRows;
    int numColumns = perfmon_numThreads;
    bstrList* fc;
    bstring label;

    switch ( group ) 
    {
        case FLOPS_DP:

            numRows = 5;
            INIT_BASIC;
            bstrListAdd(1,Runtime [s]);
            bstrListAdd(2,CPI);
            bstrListAdd(3,DP MFlops/s (DP assumed));
            bstrListAdd(4,Packed MUOPS/s);
            bstrListAdd(5,Scalar MUOPS/s);
            initResultTable(&tableData, fc, numRows, numColumns);

            for(threadId=0; threadId < perfmon_numThreads; threadId++)
            {
                time = (double) threadData[threadId].counters[PMC1].counterData * inverseClock;
                cpi = (double) threadData[threadId].counters[PMC1].counterData/
                    (double) threadData[threadId].counters[PMC0].counterData;
                tableData.rows[0].value[threadId] = time;
                tableData.rows[1].value[threadId] = cpi;
                tableData.rows[2].value[threadId] =
                    (double) 1.0E-06*(threadData[threadId].counters[PMC2].counterData*2+
                            threadData[threadId].counters[PMC3].counterData) / time;
                tableData.rows[3].value[threadId] =
                    (double) 1.0E-06*(threadData[threadId].counters[PMC2].counterData) / time;
                tableData.rows[4].value[threadId] =
                    (double) 1.0E-06*(threadData[threadId].counters[PMC3].counterData) / time;
            }
            break;

        case FLOPS_SP:

            numRows = 3;
            INIT_BASIC;
            bstrListAdd(1,Runtime);
            bstrListAdd(2,CPI);
            bstrListAdd(3,SP/MFlops/s);
            initResultTable(&tableData, fc, numRows, numColumns);

            for(threadId=0; threadId < perfmon_numThreads; threadId++)
            {
                time = (double) threadData[threadId].counters[PMC1].counterData * inverseClock;
                cpi = (double) threadData[threadId].counters[PMC1].counterData/
                    (double) threadData[threadId].counters[PMC0].counterData;
                tableData.rows[0].value[threadId] = time;
                tableData.rows[1].value[threadId] = cpi;
                tableData.rows[2].value[threadId] =
                    (double) 1.0E-06*(threadData[threadId].counters[PMC2].counterData*4+
                            threadData[threadId].counters[PMC3].counterData) / time;
            }
            break;

        case L2:

            numRows = 5;
            INIT_BASIC;
            bstrListAdd(1,Runtime);
            bstrListAdd(2,CPI);
            bstrListAdd(3,L2_Load_MBytes/s);
            bstrListAdd(4,L2_Evict_MBytes/s);
            bstrListAdd(5,L2_bandwidth_MBytes/s);
            initResultTable(&tableData, fc, numRows, numColumns);

            for(threadId=0; threadId < perfmon_numThreads; threadId++)
            {
                time = (double) threadData[threadId].counters[PMC1].counterData * inverseClock;
                cpi = (double) threadData[threadId].counters[PMC1].counterData/
                    (double) threadData[threadId].counters[PMC0].counterData;
                tableData.rows[0].value[threadId] = time;
                tableData.rows[1].value[threadId] = cpi;
                tableData.rows[2].value[threadId] =
                    1.0E-06*((double) threadData[threadId].counters[PMC2].counterData*64)/time;
                tableData.rows[2].value[threadId] =
                1.0E-06*((double) threadData[threadId].counters[PMC3].counterData*64)/time;
                tableData.rows[4].value[threadId] =
                1.0E-06*((double) (threadData[threadId].counters[PMC2].counterData+
                            threadData[threadId].counters[PMC3].counterData)*64)/time;
            }
            break;

        case MEM:

            numRows = 3;
            INIT_BASIC;
            bstrListAdd(1,Runtime [s]);
            bstrListAdd(2,CPI);
            bstrListAdd(3,Memory bandwidth [MBytes/s]);
            initResultTable(&tableData, fc, numRows, numColumns);

            for(threadId=0; threadId < perfmon_numThreads; threadId++)
            {
                time = (double) threadData[threadId].counters[PMC1].counterData * inverseClock;
                cpi = (double) threadData[threadId].counters[PMC1].counterData/
                    (double) threadData[threadId].counters[PMC0].counterData;
                tableData.rows[0].value[threadId] = time;
                tableData.rows[1].value[threadId] = cpi;
                tableData.rows[2].value[threadId] =
                    (double) 1.0E-06*(threadData[threadId].counters[PMC6].counterData+
                            threadData[threadId].counters[PMC7].counterData) * 64 / time;
            }

            //    printf ("[%d] Memory bandwidth MBytes/s: %f \n",
            //    cpu_id,1.0E-06*(float)(thread->pc[0]*64)/time);
            break;

        case DATA:
         //   printf ("[%d] Store to Load ratio: 1:%f \n",cpu_id,(float)thread->pc[0]/(float)thread->pc[1]);
            break;

        case BRANCH:
          //  printf ("[%d] Mispredicted Branches: %f \n",cpu_id,(float) (thread->pc[1]/(float)thread->pc[0]) * 100);
            break;

        case CPI:
           // printf ("[%d] Cycles per uop/s: %f \n",cpu_id,(float)thread->cycles/(float)thread->pc[0]);
            break;

        case FRONTEND:
           // printf ("[%d] Instruction starvation: %f \n",cpu_id,(float)thread->cycles/(float)thread->pc[0]);
            break;

        default:
            break;
    }

    printResultTable(&tableData);
}

#if 0

    switch ( group) {
        case FLOPS_DP:
            if (time < 1.0E-12)
            {
                printf ("[%d] Double Precision MFlops/s (DP assumed): %f \n",
                        cpu_id,0.0);
                printf ("[%d] Packed MUOPS/s: %f \n",cpu_id,0.0F);
                printf ("[%d] Scalar MUOPS/s: %f \n",cpu_id,0.0F);
            }
            else
            {
                printf ("[%d] Double Precision MFlops/s (DP assumed): %f \n",
                        cpu_id,1.0E-06*(float)((thread->pc[0]*2)+thread->pc[1])/time);
                printf ("[%d] Packed MUOPS/s: %f \n",cpu_id,1.0E-06*(float)((thread->pc[0]))/time);
                printf ("[%d] Scalar MUOPS/s: %f \n",cpu_id,1.0E-06*(float)((thread->pc[1]))/time);
            }
            break;

        case FLOPS_SP:
            if (time < 1.0E-12)
            {
                printf ("[%d] Single Precision MFlops/s (SP assumed): %f \n", cpu_id,0.0F);
                printf ("[%d] Packed MUOPS/s: %f \n",cpu_id,0.0F);
                printf ("[%d] Scalar MUOPS/s: %f \n",cpu_id,0.0F);
            }
            else
            {
                printf ("[%d] Single Precision MFlops/s (SP assumed): %f \n",
                        cpu_id,1.0E-06*(float)((thread->pc[0]*4)+thread->pc[1])/time);
                printf ("[%d] Packed MUOPS/s: %f \n",cpu_id,1.0E-06*(float)((thread->pc[0]))/time);
                printf ("[%d] Scalar MUOPS/s: %f \n",cpu_id,1.0E-06*(float)((thread->pc[1]))/time);
            }
            break;

        case L2:
            if (time < 1.0E-12)
            {
                printf ("[%d] L2 Bandwidth MBytes/s: %f \n",cpu_id,0.0F);
                printf ("[%d] L2 Load Bandwidth MBytes/s: %f \n",cpu_id,0.0F);
                printf ("[%d] L2 Evict Bandwidth MBytes/s: %f \n",cpu_id,0.0F);
            }
            else
            {
                printf ("[%d] L2 Bandwidth MBytes/s: %f \n",cpu_id,1.0E-06*(float)((thread->pc[0]+thread->pc[1])*64)/time);
                printf ("[%d] L2 Load Bandwidth MBytes/s: %f \n",cpu_id,1.0E-06*(float)((thread->pc[0])*64)/time);
                printf ("[%d] L2 Evict Bandwidth MBytes/s: %f \n",cpu_id,1.0E-06*(float)((thread->pc[1])*64)/time);
            }
            break;

        case L3:
            if (time < 1.0E-12)
            {
                printf ("[%d] L3 Bandwidth MBytes/s: %f \n",cpu_id,0.0F);
                printf ("[%d] L3 Load Bandwidth MBytes/s: %f \n",cpu_id,0.0F);
                printf ("[%d] L3 Evict Bandwidth MBytes/s: %f \n",cpu_id,0.0F);
            }
            else
            {
                printf ("[%d] L3 Bandwidth MBytes/s: %f \n",cpu_id,1.0E-06*(float)((thread->pc[0]+thread->pc[1])*64)/time);
                printf ("[%d] L3 Load Bandwidth MBytes/s: %f \n",cpu_id,1.0E-06*(float)((thread->pc[0])*64)/time);
                printf ("[%d] L3 Evict Bandwidth MBytes/s: %f \n",cpu_id,1.0E-06*(float)((thread->pc[1])*64)/time);
            }
            break;

        case MEM:
            if (time < 1.0E-12)
            {
                printf ("[%d] Memory bandwidth MBytes/s: %f \n",cpu_id,0.0F);
            }
            else
            {
                printf ("[%d] Memory bandwidth MBytes/s: %f \n",cpu_id,1.0E-06*(float)((thread->pc[4]+thread->pc[5])*64)/time);
            }
            break;

        case DATA:
            printf ("[%d] Store to Load ratio: 1:%f \n",cpu_id,(float)thread->pc[0]/(float)thread->pc[1]);
            break;

        case BRANCH:
            printf ("[%d] Ratio Mispredicted Branches: %f \n",cpu_id,(float)thread->pc[1]/(float)thread->pc[0]);
            break;

        case TLB:
            break;

        case CPI:
            printf ("[%d] Cycles per uop/s: %f \n",cpu_id,(float)thread->cycles/(float)thread->pc[0]);
            break;

        case CLUSTER:
            if (time < 1.0E-12)
            {
                printf ("[%d] X87 Mops/s: %f \n",cpu_id,0.0F);
                printf ("[%d] SSE Mops/s: %f \n",cpu_id,0.0F);
                printf ("[%d] L2 MMiss/s: %f \n",cpu_id,0.0F);
            }
            else
            {
                printf ("[%d] X87 Mops/s: %f \n",cpu_id,1.0E-06*(float)((thread->pc[0]))/time);
                printf ("[%d] SSE Mops/s: %f \n",cpu_id,1.0E-06*(float)((thread->pc[1]))/time);
                printf ("[%d] L2 MMiss/s: %f \n",cpu_id,1.0E-06*(float)((thread->pc[2]))/time);
            }
            break;

        case CLUSTER_FLOPS:
            if (time < 1.0E-12)
            {
                printf ("[%d] Packed MUOPS/s: %f \n",cpu_id,0.0F);
                printf ("[%d] Scalar MUOPS/s: %f \n",cpu_id,0.0F);
                printf ("[%d] SP MUOPS/s: %f \n",cpu_id,0.0F);
                printf ("[%d] DP MUOPS/s: %f \n",cpu_id,0.0F);
            }
            else
            {
                printf ("[%d] X87 Mops/s: %f \n",cpu_id,1.0E-06*(float)((thread->pc[0]))/time);
                printf ("[%d] SSE Mops/s: %f \n",cpu_id,1.0E-06*(float)((thread->pc[1]))/time);
                printf ("[%d] L2 MMiss/s: %f \n",cpu_id,1.0E-06*(float)((thread->pc[2]))/time);
            }
            break;

        default:
            break;
    }

}
#endif



