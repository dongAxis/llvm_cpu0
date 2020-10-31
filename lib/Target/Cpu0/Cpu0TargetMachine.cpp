//===-- AArch64TargetMachine.cpp - Define TargetMachine for AArch64 -------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//
//===----------------------------------------------------------------------===//
#include "Cpu0TargetMachine.h"
#include "Cpu0TargetObjectFile.h"

using namespace llvm;

#define DEBUG_TYPE "cpu0"

extern "C" void LLVMInitializeCpu0Target() {
  RegisterTargetMachine<Cpu0ebTargetMachine> X(TheCpu0Target);
  RegisterTargetMachine<Cpu0elTargetMachine> Y(TheCpuelTarget);
}

static std::string computeDataLayout(const Triple &TT, StringRef CPU,
                                     const TargetOptions &Options,
                                     bool isLittle) {
  std::string Ret = "";
  if (isLittle) {
      Ret += "e";
  } else {
    Ret += "-m:m";
  }

  // Pointer are 32 bit on some ABIs
  Ret += "-p:32:32";
  Ret += "-i8:8:32-i16:16:32-i64:64";
  Ret += "-n32-S64";
  return Ret;
}

static Reloc::Model getEffectiveRelocModel(CodeModel::Model CM, Optional<Reloc::Mode> RM) {
  if (!RM.hasValue() || CM == CodeModel::JITDefault)
    return Reloc::Static;
  return *RM;
}

// DataLayout --> Big-endian, 32-bit pointer/ABI/alignment
// The stack is always 8 byte aligned
// On function prologue, the stack is created by decrementing
// its pointer. Once decremented, all references are done with positive
// offset from the stack/frame pointer, using StackGrowsUp enables
// an easier handling.
// Using CodeModel::Large enables different CALL behavior.
Cpu0TargetMachine::Cpu0TargetMachine(const Target &T,
                                     StringRef DataLayoutString,
                                     const Triple &TargetTriple, StringRef CPU,
                                     StringRef FS, const TargetOptions &Options,
                                     Optional<Reloc::Model> RM,
                                     CodeModel::Model CM, CodeGenOpt::Level OL,
                                     bool isLittle)
    : LLVMTargetMachine(T, computeDataLayout(TT, CPU, Options, isLittle), TT,
                        CPU, FS, Options, getEffectiveRelocModel(CM, RM), CM,
                        OL),
      isLittle(isLittle), TLOF(std::make_unique<Cpu0TargetObjectFile>()),
      ABI(Cpu0ABIInfo::computeTargetABI()),
      DefaultSubtarget(TT, CPU, FS, isLittle, *this) {
  initAsmInfo();
}

Cpu0TargetMachine::~Cpu0TargetMachine() {}
void Cpu0TargetMachine::anchor() {}

Cpu0ebTargetMachine::Cpu0ebTargetMachine(const Target &T, const Triple &TT,
                                         StringRef CPU, StringRef FS,
                                         const TargetOptions &Options,
                                         Optional<Reloc::Model> RM,
                                         CodeModel::Model CM,
                                         CodeGenOpt::Level OL)
    : Cpu0TargetMachine(T, TT, CPU, FS, Options, RM, CM, OL) {}

const Cpu0Subtarget * Cpu0ebTargetMachine::getSubtargetImpl(const Function &F) {
  Attribute CPUAttr = F.getFnAttribute("target-cpu");
  Attribute FSAttr = F.getFnAttribute("target-features");
  std::string CPU = !CPUAttr.hasAttribute(Attribute::None)
                        ? CPUAttr.getValueAsString().str()
                        : TargetCPU;
  std::string FS = !FSAttr.hasAttribute(Attrbute::None)
                       ? FSAttr.getValueAsString().str()
                       : TargetFS;
  auto &I = SubtargetMap[CPU + FS];
  if (!I) {
    resetTargetOptions(F);
    I = llvm::make_unique<Cpu0Subtarget>(TargetTriple, CPU, FS, isLittle,
                                         *this);
  }
  return I.get();
}

namespace {

class Cpu0PassConfig : public TargetPassConfig {
public:
  Cpu0PassConfig(Cpu0TargetMachine *TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  Cpu0TargetMachine &getCpu0TargetMachine() const {
    return getTM<Cpu0TargetMachine>();
  }

  const Cpu0Subtarget &getCpu0Subtarget() const {
    return *getCpu0TargetMachine().getSubtargetImpl();
  }
};

TargetPassConfig *Cpu0TargetMachine::createPassConfig(PassManagerBase &PM) {
  return new Cpu0PassConfig(this, PM);
}

}
