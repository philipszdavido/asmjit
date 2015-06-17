// [AsmJit]
// Complete x86/x64 JIT and Remote Assembler for C++.
//
// [License]
// Zlib - See LICENSE.md file in the package.

// This file is used to test opcodes generated by AsmJit. Output can be
// disassembled in your IDE or by your favourite disassembler. Instructions
// are grouped by category and then sorted alphabetically.

// [Dependencies - AsmJit]
#include "../asmjit/asmjit.h"

// [Dependencies - Test]
#include "./asmjit_test_opcode.h"

// [Dependencies - C]
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void (*VoidFunc)(void);

struct OpcodeDumpInfo {
  uint32_t arch;
  bool useRex;
};

static const char* archIdToString(uint32_t archId) {
  switch (archId) {
    case asmjit::kArchNone: return "None";
    case asmjit::kArchX86: return "X86";
    case asmjit::kArchX64: return "X64";
    case asmjit::kArchArm: return "ARM";
    default: return "<unknown>";
  }
}

int main(int argc, char* argv[]) {
  asmjit::FileLogger logger(stdout);
  logger.setOption(asmjit::kLoggerOptionBinaryForm, true);

  OpcodeDumpInfo infoList[] = {
# if defined(ASMJIT_BUILD_X86)
    { asmjit::kArchX86, false },
# endif // ASMJIT_BUILD_X86
# if defined(ASMJIT_BUILD_X64)
    { asmjit::kArchX64, false },
    { asmjit::kArchX64, true  }
# endif // ASMJIT_BUILD_X64
  };

  for (int i = 0; i < ASMJIT_ARRAY_SIZE(infoList); i++) {
    const OpcodeDumpInfo& info = infoList[i];

    printf("Opcodes [ARCH=%s REX=%s]\n",
      archIdToString(info.arch),
      info.useRex ? "true" : "false");

    asmjit::JitRuntime runtime;
    asmjit::X86Assembler a(&runtime, info.arch);
    a.setLogger(&logger);

    asmgen::opcode(a, info.useRex);
    VoidFunc p = asmjit_cast<VoidFunc>(a.make());

    // Only run if disassembly makes sense.
    if (info.arch == asmjit::kArchHost)
      p();

    runtime.release((void*)p);
  }

  return 0;
}
