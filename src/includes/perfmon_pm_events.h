#define NUM_ARCH_EVENTS_PM 123

static PerfmonHashEntry  pm_arch_events[123] = {
{"DATA_MEM_REFS",
	{0x43,0x00}}
,{"DCU_LINES_IN",
	{0x45,0x00}}
,{"DCU_M_LINES_IN",
	{0x46,0x00}}
,{"DCU_M_LINES_OUT",
	{0x47,0x00}}
,{"DCU_MISS_OUTSTANDING",
	{0x48,0x00}}
,{"IFU_IFETCH",
	{0x80,0x00}}
,{"IFU_IFETCH_MISS",
	{0x81,0x00}}
,{"ITLB_MISS",
	{0x85,0x00}}
,{"IFU_MEM_STALL",
	{0x86,0x00}}
,{"ILD_STALL",
	{0x87,0x00}}
,{"L2_IFETCH",
	{0x28,0x0F}}
,{"L2_LD",
	{0x29,0x0F}}
,{"L2_ST",
	{0x2A,0x0F}}
,{"L2_LINES_IN",
	{0x24,0x00}}
,{"L2_LINES_OUT",
	{0x26,0x00}}
,{"L2_LINES_INM",
	{0x25,0x00}}
,{"L2_LINES_OUTM",
	{0x27,0x00}}
,{"L2_RQSTS",
	{0x2E,0x00}}
,{"L2_ADS",
	{0x21,0x00}}
,{"L2_DBUS_BUSY",
	{0x22,0x00}}
,{"L2_DBUS_BUSY_RD",
	{0x23,0x00}}
,{"BUS_DRDY_CLOCKS_SELF",
	{0x62,0x00}}
,{"BUS_DRDY_CLOCKS_ANY",
	{0x62,0x20}}
,{"BUS_LOCK_CLOCKS_SELF",
	{0x63,0x00}}
,{"BUS_LOCK_CLOCKS_ANY",
	{0x63,0x20}}
,{"BUS_REQ_OUTSTANDING_SELF",
	{0x60,0x00}}
,{"BUS_TRAN_BRD_SELF",
	{0x65,0x00}}
,{"BUS_TRAN_BRD_ANY",
	{0x65,0x20}}
,{"BUS_TRAN_RFO_SELF",
	{0x66,0x00}}
,{"BUS_TRAN_RFO_ANY",
	{0x66,0x20}}
,{"BUS_TRAN_WB_SELF",
	{0x67,0x00}}
,{"BUS_TRAN_WB_ANY",
	{0x67,0x20}}
,{"BUS_TRAN_IFETCH_SELF",
	{0x68,0x00}}
,{"BUS_TRAN_IFETCH_ANY",
	{0x68,0x20}}
,{"BUS_TRAN_INVAL_SELF",
	{0x69,0x00}}
,{"BUS_TRAN_INVAL_ANY",
	{0x69,0x20}}
,{"BUS_TRAN_PWR_SELF",
	{0x6A,0x00}}
,{"BUS_TRAN_PWR_ANY",
	{0x6A,0x20}}
,{"BUS_TRANS_P_SELF",
	{0x6B,0x00}}
,{"BUS_TRANS_P_ANY",
	{0x6B,0x20}}
,{"BUS_TRANS_IO_SELF",
	{0x6C,0x00}}
,{"BUS_TRANS_IO_ANY",
	{0x6C,0x20}}
,{"BUS_TRAN_DEF_SELF",
	{0x6D,0x00}}
,{"BUS_TRAN_DEF_ANY",
	{0x6D,0x20}}
,{"BUS_TRAN_BURST_SELF",
	{0x6E,0x00}}
,{"BUS_TRAN_BURST_ANY",
	{0x6E,0x20}}
,{"BUS_TRAN_ANY_SELF",
	{0x70,0x00}}
,{"BUS_TRAN_ANY_ANY",
	{0x70,0x20}}
,{"BUS_TRAN_MEM_SELF",
	{0x6F,0x00}}
,{"BUS_TRAN_MEM_ANY",
	{0x6F,0x20}}
,{"BUS_DATA_RCV_SELF",
	{0x64,0x00}}
,{"BUS_BNR_DRV_SELF",
	{0x61,0x00}}
,{"BUS_HIT_DRV_SELF",
	{0x7A,0x00}}
,{"BUS_HITM_DRV_SELF",
	{0x7B,0x00}}
,{"BUS_SNOOP_STALL_SELF",
	{0x7E,0x00}}
,{"FLOPS",
	{0xC1,0x00}}
,{"FP_COMP_OPS_EXE",
	{0x10,0x00}}
,{"FP_ASSIST",
	{0x11,0x00}}
,{"MUL",
	{0x12,0x00}}
,{"DIV",
	{0x13,0x00}}
,{"CYCLES_DIV_BUSY",
	{0x14,0x00}}
,{"LD_BLOCKS",
	{0x03,0x00}}
,{"SB_DRAINS",
	{0x04,0x00}}
,{"MISALIGN_MEM_REF",
	{0x05,0x00}}
,{"EMON_KNI_PREF_DISPATCHED_NTA",
	{0x07,0x00}}
,{"EMON_KNI_PREF_DISPATCHED_T1",
	{0x07,0x01}}
,{"EMON_KNI_PREF_DISPATCHED_T2",
	{0x07,0x02}}
,{"EMON_KNI_PREF_DISPATCHED_WEAK",
	{0x07,0x03}}
,{"EMON_KNI_PREF_MISS_NTA",
	{0x4B,0x00}}
,{"EMON_KNI_PREF_MISS_T1",
	{0x4B,0x01}}
,{"EMON_KNI_PREF_MISS_T2",
	{0x4B,0x02}}
,{"EMON_KNI_PREF_MISS_WEAK",
	{0x4B,0x03}}
,{"INST_RETIRED",
	{0xC0,0x00}}
,{"UOPS_RETIRED",
	{0xC2,0x00}}
,{"INST_DECODED",
	{0xD0,0x00}}
,{"EMON_SSE_SSE2_INST_RETIRED_SINGLE_ALL",
	{0xD8,0x00}}
,{"EMON_SSE_SSE2_INST_RETIRED_SCALAR_SINGLE",
	{0xD8,0x01}}
,{"EMON_SSE_SSE2_INST_RETIRED_PACKED_DOUBLE",
	{0xD8,0x02}}
,{"EMON_SSE_SSE2_INST_RETIRED_SCALAR_DOUBLE",
	{0xD8,0x03}}
,{"EMON_SSE_SSE2_COMP_INST_RETIRED_SINGLE_ALL",
	{0xD9,0x00}}
,{"EMON_SSE_SSE2_COMP_INST_RETIRED_SCALAR_SINGLE",
	{0xD9,0x01}}
,{"EMON_SSE_SSE2_COMP_INST_RETIRED_PACKED_DOUBLE",
	{0xD9,0x02}}
,{"EMON_SSE_SSE2_COMP_INST_RETIRED_SCALAR_DOUBLE",
	{0xD9,0x03}}
,{"HW_INT_RX",
	{0xC8,0x00}}
,{"CYCLES_INT_MASKED",
	{0xC6,0x00}}
,{"CYCLES_INT_PENDING_AND_MASKED",
	{0xC7,0x00}}
,{"BR_INST_RETIRED",
	{0xC4,0x00}}
,{"BR_MISS_PRED_RETIRED",
	{0xC5,0x00}}
,{"BR_TAKEN_RETIRED",
	{0xC9,0x00}}
,{"BR_MISS_PRED_TAKEN_RET",
	{0xCA,0x00}}
,{"BR_INST_DECODED",
	{0xE0,0x00}}
,{"BTB_MISSES",
	{0xE2,0x00}}
,{"BR_BOGUS",
	{0xE4,0x00}}
,{"BACLEARS",
	{0xE6,0x00}}
,{"RESOURCE_STALLS",
	{0xA2,0x00}}
,{"PARTIAL_RAT_STALL",
	{0xD2,0x00}}
,{"SEGMENT_REG_LOADS",
	{0x06,0x00}}
,{"CPU_CLK_UNHALTED",
	{0x79,0x00}}
,{"SEG_RENAME_STALLS",
	{0xD4,0x0F}}
,{"SEG_REG_RENAMES",
	{0xD5,0x0F}}
,{"RET_SEG_RENAMES",
	{0xD6,0x0F}}
,{"EMON_EST_TRANS_ALL",
	{0x58,0x00}}
,{"EMON_EST_TRANS_FREQ",
	{0x58,0x02}}
,{"BR_INST_EXEC",
	{0x88,0x00}}
,{"BR_MISSP_EXEC",
	{0x89,0x00}}
,{"BR_BAC_MISSP_EXEC",
	{0x8A,0x00}}
,{"BR_CND_EXEC",
	{0x8B,0x00}}
,{"BR_CND_MISSP_EXEC",
	{0x8C,0x00}}
,{"BR_IND_EXEC",
	{0x8D,0x00}}
,{"BR_IND_MISSP_EXEC",
	{0x8E,0x00}}
,{"BR_RET_EXEC",
	{0x8F,0x00}}
,{"BR_RET_MISSP_EXEC",
	{0x90,0x00}}
,{"BR_RET_BAC_MISSP_EXEC",
	{0x91,0x00}}
,{"BR_CALL_EXEC",
	{0x92,0x00}}
,{"BR_CALL_MISSP_EXEC",
	{0x93,0x00}}
,{"BR_IND_CALL_EXEC",
	{0x94,0x00}}
,{"EMON_SIMD_INSTR_RETIRED",
	{0xCE,0x00}}
,{"EMON_SYNCH_UOPS",
	{0xD3,0x00}}
,{"EMON_ESP_UOPS",
	{0xD7,0x00}}
,{"EMON_FUSED_UOPS_RET",
	{0xDA,0x00}}
,{"EMON_UNFUSION",
	{0xDB,0x00}}
,{"EMON_PREF_RQST_UP",
	{0xF0,0x00}}
,{"EMON_PREF_RQST_DN",
	{0xF8,0x00}}
};