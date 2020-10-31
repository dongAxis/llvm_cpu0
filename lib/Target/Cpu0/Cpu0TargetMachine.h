//==-- AArch64TargetMachine.h - Define TargetMachine for AArch64 -*- C++ -*-==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the AArch64 specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_AARCH64_AARCH64TARGETMACHINE_H
#define LLVM_LIB_TARGET_AARCH64_AARCH64TARGETMACHINE_H

#include "AArch64InstrInfo.h"
#include "AArch64Subtarget.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {

class formatted_raw_ostream;
class Cpu0RegisterInfo;

class Cpu0TargetMachine : public LLVMTargetMachine {
    bool isLittle;
    std::unique_ptr<TargetLoweringObjectFile> TLOF;
    Cpu0ABIInfo ABI;
    Cpu0Subtarget DefaultSubtarget;
    mutable StringMap<std::unique_ptr<Cpu0Subtarget>> SubtargetMap;

public:
  Cpu0TargetMachine(const Target &T, StringRef DataLayoutString,
                    const Triple &TargetTriple, StringRef CPU, StringRef FS,
                    const TargetOptions &Options, Optional<Reloc::Model> RM,
                    CodeModel::Model CM, CodeGenOpt::Level OL, bool isLittle);
  ~Cpu0TargetMachine() override;

  const Cpu0Subtarget *getSubtargetImpl() const { return &DefaultSubtarget; }
  const Cpu0Subtarget *getSubtargetImpl(const Function &) const override;

  // Pass Pipeline Configuration
  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;
  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }
  bool isLittleEndian() const { return isLittle; }
  const Cpu0ABIInfo &getABI() const { return ABI; }
};

/// Cpu0ebTargetMachine - Cpu032 big endian target machine
class Cpu0ebTargetMachine : public Cpu0TargetMachine {
  virtual void anchor();
public:
  Cpu0ebTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                      StringRef FS, const TargetOptions &Options,
                      Optional<Reloc::Model> RM, CodeModel::Model CM,
                      CodeGenOpt::Level OL);
};

/// Cpu0ebTargetMachine - Cpu032 little endian target machine
class Cpu0elTargetMachine : public Cpu0TargetMachine {
  virtual void anchor();

public:
  Cpu0elTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                      StringRef FS, const TargetOptions &Options,
                      Optional<Reloc::Model> RM, CodeModel::Model CM,
                      CodeGenOpt::Level OL);
};

} // end namespace llvm

#endif
