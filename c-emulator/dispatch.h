/* -*- Mode:C -*- */

#ifndef _DISPATCH_H
#define _DISPATCH_H

#include "emulator.h"

extern int InstructionCacheMiss (void);

extern const char *ivory_dispatch_names[];

typedef enum _IvoryDispatch
{
  DispatchCarFP,
  DispatchCarLP,
  DispatchCarSP,
  DispatchCarImmediate,
  DispatchCarPop,
  DispatchCdrFP,
  DispatchCdrLP,
  DispatchCdrSP,
  DispatchCdrImmediate,
  DispatchCdrPop,
  DispatchEndpFP,
  DispatchEndpLP,
  DispatchEndpSP,
  DispatchEndpImmediate,
  DispatchEndpPop,
  DispatchSetup1dArrayFP,
  DispatchSetup1dArrayLP,
  DispatchSetup1dArraySP,
  DispatchSetup1dArrayImmediate,
  DispatchSetup1dArrayPop,
  DispatchSetupForce1dArrayFP,
  DispatchSetupForce1dArrayLP,
  DispatchSetupForce1dArraySP,
  DispatchSetupForce1dArrayImmediate,
  DispatchSetupForce1dArrayPop,
  DispatchBindLocativeFP,
  DispatchBindLocativeLP,
  DispatchBindLocativeSP,
  DispatchBindLocativeImmediate,
  DispatchBindLocativePop,
  DispatchRestoreBindingStackFP,
  DispatchRestoreBindingStackLP,
  DispatchRestoreBindingStackSP,
  DispatchRestoreBindingStackImmediate,
  DispatchRestoreBindingStackPop,
  DispatchEphemeralpFP,
  DispatchEphemeralpLP,
  DispatchEphemeralpSP,
  DispatchEphemeralpImmediate,
  DispatchEphemeralpPop,
  DispatchStartCallFP,
  DispatchStartCallLP,
  DispatchStartCallSP,
  DispatchStartCallImmediate,
  DispatchStartCallPop,
  DispatchJumpFP,
  DispatchJumpLP,
  DispatchJumpSP,
  DispatchJumpImmediate,
  DispatchJumpPop,
  DispatchTagFP,
  DispatchTagLP,
  DispatchTagSP,
  DispatchTagImmediate,
  DispatchTagPop,
  DispatchDereferenceFP,
  DispatchDereferenceLP,
  DispatchDereferenceSP,
  DispatchDereferenceImmediate,
  DispatchDereferencePop,
  DispatchLogicTailTestFP,
  DispatchLogicTailTestLP,
  DispatchLogicTailTestSP,
  DispatchLogicTailTestImmediate,
  DispatchLogicTailTestPop,
  DispatchProcBreakpointFP,
  DispatchProcBreakpointLP,
  DispatchProcBreakpointSP,
  DispatchProcBreakpointImmediate,
  DispatchProcBreakpointPop,
  DispatchPushLexicalVarFP,
  DispatchPushLexicalVarLP,
  DispatchPushLexicalVarSP,
  DispatchPushLexicalVarImmediate,
  DispatchPushLexicalVarPop,
  DispatchBlock0WriteFP,
  DispatchBlock0WriteLP,
  DispatchBlock0WriteSP,
  DispatchBlock0WriteImmediate,
  DispatchBlock0WritePop,
  DispatchBlock1WriteFP,
  DispatchBlock1WriteLP,
  DispatchBlock1WriteSP,
  DispatchBlock1WriteImmediate,
  DispatchBlock1WritePop,
  DispatchBlock2WriteFP,
  DispatchBlock2WriteLP,
  DispatchBlock2WriteSP,
  DispatchBlock2WriteImmediate,
  DispatchBlock2WritePop,
  DispatchBlock3WriteFP,
  DispatchBlock3WriteLP,
  DispatchBlock3WriteSP,
  DispatchBlock3WriteImmediate,
  DispatchBlock3WritePop,
  DispatchZeropFP,
  DispatchZeropLP,
  DispatchZeropSP,
  DispatchZeropImmediate,
  DispatchZeropPop,
  DispatchMinuspFP,
  DispatchMinuspLP,
  DispatchMinuspSP,
  DispatchMinuspImmediate,
  DispatchMinuspPop,
  DispatchPluspFP,
  DispatchPluspLP,
  DispatchPluspSP,
  DispatchPluspImmediate,
  DispatchPluspPop,
  DispatchTypeMember,
  DispatchTypeMemberNoPop,
  DispatchLocateLocals,
  DispatchCatchClose,
  DispatchGenericDispatch,
  DispatchMessageDispatch,
  DispatchCheckPreemptRequest,
  DispatchPushGlobalLogicVariable,
  DispatchNoOp,
  DispatchHalt,
  DispatchBranchTrue,
  DispatchBranchTrueElseExtraPop,
  DispatchBranchTrueAndExtraPop,
  DispatchBranchTrueExtraPop,
  DispatchBranchTrueNoPop,
  DispatchBranchTrueAndNoPop,
  DispatchBranchTrueElseNoPop,
  DispatchBranchTrueAndNoPopElseNoPopExtraPop,
  DispatchBranchFalse,
  DispatchBranchFalseElseExtraPop,
  DispatchBranchFalseAndExtraPop,
  DispatchBranchFalseExtraPop,
  DispatchBranchFalseNoPop,
  DispatchBranchFalseAndNoPop,
  DispatchBranchFalseElseNoPop,
  DispatchBranchFalseAndNoPopElseNoPopExtraPop,
  DispatchPushFP,
  DispatchPushLP,
  DispatchPushSP,
  DispatchPushImmediate,
  DispatchPushPop,
  DispatchPushNNils,
  DispatchPushAddressSpRelativeFP,
  DispatchPushAddressSpRelativeLP,
  DispatchPushAddressSpRelativeSP,
  DispatchPushAddressSpRelativeImmediate,
  DispatchPushAddressSpRelativePop,
  DispatchPushLocalLogicVariablesFP,
  DispatchPushLocalLogicVariablesLP,
  DispatchPushLocalLogicVariablesSP,
  DispatchPushLocalLogicVariablesImmediate,
  DispatchPushLocalLogicVariablesPop,
  DispatchReturnMultipleFP,
  DispatchReturnMultipleLP,
  DispatchReturnMultipleSP,
  DispatchReturnMultipleImmediate,
  DispatchReturnMultiplePop,
  DispatchReturnKludgeFP,
  DispatchReturnKludgeLP,
  DispatchReturnKludgeSP,
  DispatchReturnKludgeImmediate,
  DispatchReturnKludgePop,
  DispatchTakeValues,
  DispatchUnbindNImmediate,
  DispatchUnbindNPop,
  DispatchPushInstanceVariable,
  DispatchPushAddressInstanceVariable,
  DispatchPushInstanceVariableOrdered,
  DispatchPushAddressInstanceVariableOrdered,
  DispatchUnaryMinusFP,
  DispatchUnaryMinusLP,
  DispatchUnaryMinusSP,
  DispatchUnaryMinusImmediate,
  DispatchUnaryMinusPop,
  DispatchReturnSingleNIL,
  DispatchReturnSingleT,
  DispatchReturnSingleTOS,
  DispatchMemoryRead,
  DispatchMemoryReadAddress,
  DispatchBlock0Read,
  DispatchBlock1Read,
  DispatchBlock2Read,
  DispatchBlock3Read,
  DispatchBlock0ReadShift,
  DispatchBlock1ReadShift,
  DispatchBlock2ReadShift,
  DispatchBlock3ReadShift,
  DispatchBlock0ReadTest,
  DispatchBlock1ReadTest,
  DispatchBlock2ReadTest,
  DispatchBlock3ReadTest,
  DispatchFinishCallN,
  DispatchFinishCallNApply,
  DispatchFinishCallTos,
  DispatchFinishCallTosApply,
  DispatchSetToCarFP,
  DispatchSetToCarLP,
  DispatchSetToCarSP,
  DispatchSetToCarImmediate,
  DispatchSetToCarPop,
  DispatchSetToCdrFP,
  DispatchSetToCdrLP,
  DispatchSetToCdrSP,
  DispatchSetToCdrImmediate,
  DispatchSetToCdrPop,
  DispatchSetToCdrPushCarFP,
  DispatchSetToCdrPushCarLP,
  DispatchSetToCdrPushCarSP,
  DispatchSetToCdrPushCarImmediate,
  DispatchSetToCdrPushCarPop,
  DispatchIncrementFP,
  DispatchIncrementLP,
  DispatchIncrementSP,
  DispatchIncrementImmediate,
  DispatchIncrementPop,
  DispatchDecrementFP,
  DispatchDecrementLP,
  DispatchDecrementSP,
  DispatchDecrementImmediate,
  DispatchDecrementPop,
  DispatchPointerIncrementFP,
  DispatchPointerIncrementLP,
  DispatchPointerIncrementSP,
  DispatchPointerIncrementImmediate,
  DispatchPointerIncrementPop,
  DispatchSetCdrCode1FP,
  DispatchSetCdrCode1LP,
  DispatchSetCdrCode1SP,
  DispatchSetCdrCode1Immediate,
  DispatchSetCdrCode1Pop,
  DispatchSetCdrCode2FP,
  DispatchSetCdrCode2LP,
  DispatchSetCdrCode2SP,
  DispatchSetCdrCode2Immediate,
  DispatchSetCdrCode2Pop,
  DispatchPushAddressFP,
  DispatchPushAddressLP,
  DispatchPushAddressSP,
  DispatchPushAddressImmediate,
  DispatchPushAddressPop,
  DispatchSetSpToAddressFP,
  DispatchSetSpToAddressLP,
  DispatchSetSpToAddressSP,
  DispatchSetSpToAddressImmediate,
  DispatchSetSpToAddressPop,
  DispatchSetSpToAddressSaveTosFP,
  DispatchSetSpToAddressSaveTosLP,
  DispatchSetSpToAddressSaveTosSP,
  DispatchSetSpToAddressSaveTosImmediate,
  DispatchSetSpToAddressSaveTosPop,
  DispatchReadInternalRegister,
  DispatchWriteInternalRegister,
  DispatchCoprocessorRead,
  DispatchCoprocessorWrite,
  DispatchBlock0ReadAluFP,
  DispatchBlock0ReadAluLP,
  DispatchBlock0ReadAluSP,
  DispatchBlock0ReadAluImmediate,
  DispatchBlock0ReadAluPop,
  DispatchBlock1ReadAluFP,
  DispatchBlock1ReadAluLP,
  DispatchBlock1ReadAluSP,
  DispatchBlock1ReadAluImmediate,
  DispatchBlock1ReadAluPop,
  DispatchBlock2ReadAluFP,
  DispatchBlock2ReadAluLP,
  DispatchBlock2ReadAluSP,
  DispatchBlock2ReadAluImmediate,
  DispatchBlock2ReadAluPop,
  DispatchBlock3ReadAluFP,
  DispatchBlock3ReadAluLP,
  DispatchBlock3ReadAluSP,
  DispatchBlock3ReadAluImmediate,
  DispatchBlock3ReadAluPop,
  DispatchLdb,
  DispatchCharLdb,
  DispatchPLdb,
  DispatchPTagLdb,
  DispatchBranch,
  DispatchLoopDecrementTos,
  DispatchEntryRestAccepted,
  DispatchEntryRestNotAccepted,
  DispatchRplacaFP,
  DispatchRplacaLP,
  DispatchRplacaSP,
  DispatchRplacaImmediate,
  DispatchRplacaPop,
  DispatchRplacdFP,
  DispatchRplacdLP,
  DispatchRplacdSP,
  DispatchRplacdImmediate,
  DispatchRplacdPop,
  DispatchMultiplyFP,
  DispatchMultiplyLP,
  DispatchMultiplySP,
  DispatchMultiplyImmediate,
  DispatchMultiplyPop,
  DispatchQuotientFP,
  DispatchQuotientLP,
  DispatchQuotientSP,
  DispatchQuotientImmediate,
  DispatchQuotientPop,
  DispatchCeilingFP,
  DispatchCeilingLP,
  DispatchCeilingSP,
  DispatchCeilingImmediate,
  DispatchCeilingPop,
  DispatchFloorFP,
  DispatchFloorLP,
  DispatchFloorSP,
  DispatchFloorImmediate,
  DispatchFloorPop,
  DispatchTruncateFP,
  DispatchTruncateLP,
  DispatchTruncateSP,
  DispatchTruncateImmediate,
  DispatchTruncatePop,
  DispatchRoundFP,
  DispatchRoundLP,
  DispatchRoundSP,
  DispatchRoundImmediate,
  DispatchRoundPop,
  DispatchRationalQuotientFP,
  DispatchRationalQuotientLP,
  DispatchRationalQuotientSP,
  DispatchRationalQuotientImmediate,
  DispatchRationalQuotientPop,
  DispatchMinFP,
  DispatchMinLP,
  DispatchMinSP,
  DispatchMinImmediate,
  DispatchMinPop,
  DispatchMaxFP,
  DispatchMaxLP,
  DispatchMaxSP,
  DispatchMaxImmediate,
  DispatchMaxPop,
  DispatchAluFP,
  DispatchAluLP,
  DispatchAluSP,
  DispatchAluImmediate,
  DispatchAluPop,
  DispatchLogandFP,
  DispatchLogandLP,
  DispatchLogandSP,
  DispatchLogandImmediate,
  DispatchLogandPop,
  DispatchLogxorFP,
  DispatchLogxorLP,
  DispatchLogxorSP,
  DispatchLogxorImmediate,
  DispatchLogxorPop,
  DispatchLogiorFP,
  DispatchLogiorLP,
  DispatchLogiorSP,
  DispatchLogiorImmediate,
  DispatchLogiorPop,
  DispatchRotFP,
  DispatchRotLP,
  DispatchRotSP,
  DispatchRotImmediate,
  DispatchRotPop,
  DispatchLshFP,
  DispatchLshLP,
  DispatchLshSP,
  DispatchLshImmediate,
  DispatchLshPop,
  DispatchMultiplyDoubleFP,
  DispatchMultiplyDoubleLP,
  DispatchMultiplyDoubleSP,
  DispatchMultiplyDoubleImmediate,
  DispatchMultiplyDoublePop,
  DispatchLshcBignumStepFP,
  DispatchLshcBignumStepLP,
  DispatchLshcBignumStepSP,
  DispatchLshcBignumStepImmediate,
  DispatchLshcBignumStepPop,
  DispatchStackBltFP,
  DispatchStackBltLP,
  DispatchStackBltSP,
  DispatchStackBltImmediate,
  DispatchStackBltPop,
  DispatchRgetfFP,
  DispatchRgetfLP,
  DispatchRgetfSP,
  DispatchRgetfImmediate,
  DispatchRgetfPop,
  DispatchMemberFP,
  DispatchMemberLP,
  DispatchMemberSP,
  DispatchMemberImmediate,
  DispatchMemberPop,
  DispatchAssocFP,
  DispatchAssocLP,
  DispatchAssocSP,
  DispatchAssocImmediate,
  DispatchAssocPop,
  DispatchPointerPlusFP,
  DispatchPointerPlusLP,
  DispatchPointerPlusSP,
  DispatchPointerPlusImmediate,
  DispatchPointerPlusPop,
  DispatchPointerDifferenceFP,
  DispatchPointerDifferenceLP,
  DispatchPointerDifferenceSP,
  DispatchPointerDifferenceImmediate,
  DispatchPointerDifferencePop,
  DispatchAshFP,
  DispatchAshLP,
  DispatchAshSP,
  DispatchAshImmediate,
  DispatchAshPop,
  DispatchStoreConditionalFP,
  DispatchStoreConditionalLP,
  DispatchStoreConditionalSP,
  DispatchStoreConditionalImmediate,
  DispatchStoreConditionalPop,
  DispatchMemoryWriteFP,
  DispatchMemoryWriteLP,
  DispatchMemoryWriteSP,
  DispatchMemoryWriteImmediate,
  DispatchMemoryWritePop,
  DispatchPStoreContentsFP,
  DispatchPStoreContentsLP,
  DispatchPStoreContentsSP,
  DispatchPStoreContentsImmediate,
  DispatchPStoreContentsPop,
  DispatchBindLocativeToValueFP,
  DispatchBindLocativeToValueLP,
  DispatchBindLocativeToValueSP,
  DispatchBindLocativeToValueImmediate,
  DispatchBindLocativeToValuePop,
  DispatchUnifyFP,
  DispatchUnifyLP,
  DispatchUnifySP,
  DispatchUnifyImmediate,
  DispatchUnifyPop,
  DispatchPopLexicalVarFP,
  DispatchPopLexicalVarLP,
  DispatchPopLexicalVarSP,
  DispatchPopLexicalVarImmediate,
  DispatchPopLexicalVarPop,
  DispatchMovemLexicalVarFP,
  DispatchMovemLexicalVarLP,
  DispatchMovemLexicalVarSP,
  DispatchMovemLexicalVarImmediate,
  DispatchMovemLexicalVarPop,
  DispatchEqualNumberFP,
  DispatchEqualNumberLP,
  DispatchEqualNumberSP,
  DispatchEqualNumberImmediate,
  DispatchEqualNumberPop,
  DispatchLesspFP,
  DispatchLesspLP,
  DispatchLesspSP,
  DispatchLesspImmediate,
  DispatchLesspPop,
  DispatchGreaterpFP,
  DispatchGreaterpLP,
  DispatchGreaterpSP,
  DispatchGreaterpImmediate,
  DispatchGreaterpPop,
  DispatchEqlFP,
  DispatchEqlLP,
  DispatchEqlSP,
  DispatchEqlImmediate,
  DispatchEqlPop,
  DispatchEqualNumberNoPopFP,
  DispatchEqualNumberNoPopLP,
  DispatchEqualNumberNoPopSP,
  DispatchEqualNumberNoPopImmediate,
  DispatchEqualNumberNoPopPop,
  DispatchLesspNoPopFP,
  DispatchLesspNoPopLP,
  DispatchLesspNoPopSP,
  DispatchLesspNoPopImmediate,
  DispatchLesspNoPopPop,
  DispatchGreaterpNoPopFP,
  DispatchGreaterpNoPopLP,
  DispatchGreaterpNoPopSP,
  DispatchGreaterpNoPopImmediate,
  DispatchGreaterpNoPopPop,
  DispatchEqlNoPopFP,
  DispatchEqlNoPopLP,
  DispatchEqlNoPopSP,
  DispatchEqlNoPopImmediate,
  DispatchEqlNoPopPop,
  DispatchEqFP,
  DispatchEqLP,
  DispatchEqSP,
  DispatchEqImmediate,
  DispatchEqPop,
  DispatchLogtestFP,
  DispatchLogtestLP,
  DispatchLogtestSP,
  DispatchLogtestImmediate,
  DispatchLogtestPop,
  DispatchEqNoPopFP,
  DispatchEqNoPopLP,
  DispatchEqNoPopSP,
  DispatchEqNoPopImmediate,
  DispatchEqNoPopPop,
  DispatchLogtestNoPopFP,
  DispatchLogtestNoPopLP,
  DispatchLogtestNoPopSP,
  DispatchLogtestNoPopImmediate,
  DispatchLogtestNoPopPop,
  DispatchAddFP,
  DispatchAddLP,
  DispatchAddSP,
  DispatchAddImmediate,
  DispatchAddPop,
  DispatchSubFP,
  DispatchSubLP,
  DispatchSubSP,
  DispatchSubImmediate,
  DispatchSubPop,
  Dispatch32BitPlusFP,
  Dispatch32BitPlusLP,
  Dispatch32BitPlusSP,
  Dispatch32BitPlusImmediate,
  Dispatch32BitPlusPop,
  Dispatch32BitDifferenceFP,
  Dispatch32BitDifferenceLP,
  Dispatch32BitDifferenceSP,
  Dispatch32BitDifferenceImmediate,
  Dispatch32BitDifferencePop,
  DispatchAddBignumStepFP,
  DispatchAddBignumStepLP,
  DispatchAddBignumStepSP,
  DispatchAddBignumStepImmediate,
  DispatchAddBignumStepPop,
  DispatchSubBignumStepFP,
  DispatchSubBignumStepLP,
  DispatchSubBignumStepSP,
  DispatchSubBignumStepImmediate,
  DispatchSubBignumStepPop,
  DispatchMultiplyBignumStepFP,
  DispatchMultiplyBignumStepLP,
  DispatchMultiplyBignumStepSP,
  DispatchMultiplyBignumStepImmediate,
  DispatchMultiplyBignumStepPop,
  DispatchDivideBignumStepFP,
  DispatchDivideBignumStepLP,
  DispatchDivideBignumStepSP,
  DispatchDivideBignumStepImmediate,
  DispatchDivideBignumStepPop,
  DispatchAset1FP,
  DispatchAset1LP,
  DispatchAset1SP,
  DispatchAset1Immediate,
  DispatchAset1Pop,
  DispatchAllocateListBlockFP,
  DispatchAllocateListBlockLP,
  DispatchAllocateListBlockSP,
  DispatchAllocateListBlockImmediate,
  DispatchAllocateListBlockPop,
  DispatchAref1FP,
  DispatchAref1LP,
  DispatchAref1SP,
  DispatchAref1Immediate,
  DispatchAref1Pop,
  DispatchAloc1FP,
  DispatchAloc1LP,
  DispatchAloc1SP,
  DispatchAloc1Immediate,
  DispatchAloc1Pop,
  DispatchStoreArrayLeaderFP,
  DispatchStoreArrayLeaderLP,
  DispatchStoreArrayLeaderSP,
  DispatchStoreArrayLeaderImmediate,
  DispatchStoreArrayLeaderPop,
  DispatchAllocateStructureBlockFP,
  DispatchAllocateStructureBlockLP,
  DispatchAllocateStructureBlockSP,
  DispatchAllocateStructureBlockImmediate,
  DispatchAllocateStructureBlockPop,
  DispatchArrayLeaderFP,
  DispatchArrayLeaderLP,
  DispatchArrayLeaderSP,
  DispatchArrayLeaderImmediate,
  DispatchArrayLeaderPop,
  DispatchAlocLeaderFP,
  DispatchAlocLeaderLP,
  DispatchAlocLeaderSP,
  DispatchAlocLeaderImmediate,
  DispatchAlocLeaderPop,
  DispatchPopInstanceVariable,
  DispatchMovemInstanceVariable,
  DispatchPopInstanceVariableOrdered,
  DispatchMovemInstanceVariableOrdered,
  DispatchInstanceRefFP,
  DispatchInstanceRefLP,
  DispatchInstanceRefSP,
  DispatchInstanceRefImmediate,
  DispatchInstanceRefPop,
  DispatchInstanceSetFP,
  DispatchInstanceSetLP,
  DispatchInstanceSetSP,
  DispatchInstanceSetImmediate,
  DispatchInstanceSetPop,
  DispatchInstanceLocFP,
  DispatchInstanceLocLP,
  DispatchInstanceLocSP,
  DispatchInstanceLocImmediate,
  DispatchInstanceLocPop,
  DispatchSetTagFP,
  DispatchSetTagLP,
  DispatchSetTagSP,
  DispatchSetTagImmediate,
  DispatchSetTagPop,
  DispatchUnsignedLesspFP,
  DispatchUnsignedLesspLP,
  DispatchUnsignedLesspSP,
  DispatchUnsignedLesspImmediate,
  DispatchUnsignedLesspPop,
  DispatchUnsignedLesspNoPopFP,
  DispatchUnsignedLesspNoPopLP,
  DispatchUnsignedLesspNoPopSP,
  DispatchUnsignedLesspNoPopImmediate,
  DispatchUnsignedLesspNoPopPop,
  DispatchPopFP,
  DispatchPopLP,
  DispatchPopSP,
  DispatchPopImmediate,
  DispatchPopPop,
  DispatchMovemFP,
  DispatchMovemLP,
  DispatchMovemSP,
  DispatchMovemImmediate,
  DispatchMovemPop,
  DispatchMergeCdrNoPopFP,
  DispatchMergeCdrNoPopLP,
  DispatchMergeCdrNoPopSP,
  DispatchMergeCdrNoPopImmediate,
  DispatchMergeCdrNoPopPop,
  DispatchFastAref1FP,
  DispatchFastAref1LP,
  DispatchFastAref1SP,
  DispatchFastAref1Immediate,
  DispatchFastAref1Pop,
  DispatchFastAset1FP,
  DispatchFastAset1LP,
  DispatchFastAset1SP,
  DispatchFastAset1Immediate,
  DispatchFastAset1Pop,
  DispatchStackBltAddressFP,
  DispatchStackBltAddressLP,
  DispatchStackBltAddressSP,
  DispatchStackBltAddressImmediate,
  DispatchStackBltAddressPop,
  DispatchDpb,
  DispatchCharDpb,
  DispatchPDpb,
  DispatchPTagDpb,
  DispatchLoopIncrementTosLessThan,
  DispatchCatchOpen,
  DispatchHack,
  DispatchPushNull,
  DispatchPushMonitorForward,
  DispatchPushHeaderP,
  DispatchPushHeaderI,
  DispatchPushExternalValueCellPointer,
  DispatchPushOneQForward,
  DispatchPushHeaderForward,
  DispatchPushElementForward,
  DispatchPushFixnum,
  DispatchPushSmallRatio,
  DispatchPushSingleFloat,
  DispatchPushDoubleFloat,
  DispatchPushBignum,
  DispatchPushBigRatio,
  DispatchPushComplex,
  DispatchPushSpareNumber,
  DispatchPushInstance,
  DispatchPushListInstance,
  DispatchPushArrayInstance,
  DispatchPushStringInstance,
  DispatchPushNil,
  DispatchPushList,
  DispatchPushArray,
  DispatchPushString,
  DispatchPushSymbol,
  DispatchPushLocative,
  DispatchPushLexicalClosure,
  DispatchPushDynamicClosure,
  DispatchPushCompiledFunction,
  DispatchPushGenericFunction,
  DispatchPushSparePointer1,
  DispatchPushSparePointer2,
  DispatchPushPhysicalAddress,
  DispatchPushSpareImmediate1,
  DispatchPushBoundLocation,
  DispatchPushCharacter,
  DispatchPushLogicVariable,
  DispatchPushGcForward,
  DispatchPushEvenPc,
  DispatchPushOddPc,
  DispatchCallCompiledEven,
  DispatchCallCompiledOdd,
  DispatchCallIndirect,
  DispatchCallGeneric,
  DispatchCallCompiledEvenPrefetch,
  DispatchCallCompiledOddPrefetch,
  DispatchCallIndirectPrefetch,
  DispatchCallGenericPrefetch,
  DispatchPushPackedInstruction60,
  DispatchPushPackedInstruction61,
  DispatchPushPackedInstruction62,
  DispatchPushPackedInstruction63,
  DispatchPushPackedInstruction64,
  DispatchPushPackedInstruction65,
  DispatchPushPackedInstruction66,
  DispatchPushPackedInstruction67,
  DispatchPushPackedInstruction70,
  DispatchPushPackedInstruction71,
  DispatchPushPackedInstruction72,
  DispatchPushPackedInstruction73,
  DispatchPushPackedInstruction74,
  DispatchPushPackedInstruction75,
  DispatchPushPackedInstruction76,
  DispatchPushPackedInstruction77,
  DispatchInstructionCacheLookup,
  DispatchIllegalInstruction
} IvoryDispatch;

#endif
