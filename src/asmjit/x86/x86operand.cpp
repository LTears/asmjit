// [AsmJit]
// Complete x86/x64 JIT and Remote Assembler for C++.
//
// [License]
// ZLIB - See LICENSE.md file in the package.

// [Export]
#define ASMJIT_EXPORTS

// [Guard]
#include "../core/build.h"
#ifdef ASMJIT_BUILD_X86

// [Dependencies]
#include "../core/misc_p.h"
#include "../x86/x86operand.h"

ASMJIT_BEGIN_SUB_NAMESPACE(x86)

// ============================================================================
// [asmjit::x86::OpData]
// ============================================================================

const OpData opData = {
  {
    // RegInfo[]
    # define VALUE(X) { RegTraits<X>::kSignature }
    { ASMJIT_LOOKUP_TABLE_32(VALUE, 0) },
    #undef VALUE

    // RegCount[]
    # define VALUE(X) RegTraits<X>::kCount
    { ASMJIT_LOOKUP_TABLE_32(VALUE, 0) },
    #undef VALUE

    // RegTypeToTypeId[]
    #define VALUE(X) RegTraits<X>::kTypeId
    { ASMJIT_LOOKUP_TABLE_32(VALUE, 0) }
    #undef VALUE
  }
};

// ============================================================================
// [asmjit::x86::Operand - Unit]
// ============================================================================

#if defined(ASMJIT_BUILD_TEST)
UNIT(asmjit_x86_operand) {
  Label L(1000); // Label with some ID.

  INFO("Checking basic properties of built-in X86 registers");
  EXPECT(gpb(Gp::kIdAx) == al);
  EXPECT(gpb(Gp::kIdBx) == bl);
  EXPECT(gpb(Gp::kIdCx) == cl);
  EXPECT(gpb(Gp::kIdDx) == dl);

  EXPECT(gpb_lo(Gp::kIdAx) == al);
  EXPECT(gpb_lo(Gp::kIdBx) == bl);
  EXPECT(gpb_lo(Gp::kIdCx) == cl);
  EXPECT(gpb_lo(Gp::kIdDx) == dl);

  EXPECT(gpb_hi(Gp::kIdAx) == ah);
  EXPECT(gpb_hi(Gp::kIdBx) == bh);
  EXPECT(gpb_hi(Gp::kIdCx) == ch);
  EXPECT(gpb_hi(Gp::kIdDx) == dh);

  EXPECT(gpw(Gp::kIdAx) == ax);
  EXPECT(gpw(Gp::kIdBx) == bx);
  EXPECT(gpw(Gp::kIdCx) == cx);
  EXPECT(gpw(Gp::kIdDx) == dx);

  EXPECT(gpd(Gp::kIdAx) == eax);
  EXPECT(gpd(Gp::kIdBx) == ebx);
  EXPECT(gpd(Gp::kIdCx) == ecx);
  EXPECT(gpd(Gp::kIdDx) == edx);

  EXPECT(gpq(Gp::kIdAx) == rax);
  EXPECT(gpq(Gp::kIdBx) == rbx);
  EXPECT(gpq(Gp::kIdCx) == rcx);
  EXPECT(gpq(Gp::kIdDx) == rdx);

  EXPECT(gpb(Gp::kIdAx) != dl);
  EXPECT(gpw(Gp::kIdBx) != cx);
  EXPECT(gpd(Gp::kIdCx) != ebx);
  EXPECT(gpq(Gp::kIdDx) != rax);

  INFO("Checking if x86::reg(...) matches built-in IDs");
  EXPECT(gpb(5) == bpl);
  EXPECT(gpw(5) == bp);
  EXPECT(gpd(5) == ebp);
  EXPECT(gpq(5) == rbp);
  EXPECT(st(5)  == st5);
  EXPECT(mm(5)  == mm5);
  EXPECT(k(5)   == k5);
  EXPECT(cr(5)  == cr5);
  EXPECT(dr(5)  == dr5);
  EXPECT(xmm(5) == xmm5);
  EXPECT(ymm(5) == ymm5);
  EXPECT(zmm(5) == zmm5);

  INFO("Checking x86::Gp register properties");
  EXPECT(Gp().isReg() == true);
  EXPECT(eax.isReg() == true);
  EXPECT(eax.id() == 0);
  EXPECT(eax.size() == 4);
  EXPECT(eax.type() == Reg::kTypeGpd);
  EXPECT(eax.group() == Reg::kGroupGp);

  INFO("Checking x86::Xmm register properties");
  EXPECT(Xmm().isReg() == true);
  EXPECT(xmm4.isReg() == true);
  EXPECT(xmm4.id() == 4);
  EXPECT(xmm4.size() == 16);
  EXPECT(xmm4.type() == Reg::kTypeXmm);
  EXPECT(xmm4.group() == Reg::kGroupVec);
  EXPECT(xmm4.isVec());

  INFO("Checking x86::Ymm register properties");
  EXPECT(Ymm().isReg() == true);
  EXPECT(ymm5.isReg() == true);
  EXPECT(ymm5.id() == 5);
  EXPECT(ymm5.size() == 32);
  EXPECT(ymm5.type() == Reg::kTypeYmm);
  EXPECT(ymm5.group() == Reg::kGroupVec);
  EXPECT(ymm5.isVec());

  INFO("Checking x86::Zmm register properties");
  EXPECT(Zmm().isReg() == true);
  EXPECT(zmm6.isReg() == true);
  EXPECT(zmm6.id() == 6);
  EXPECT(zmm6.size() == 64);
  EXPECT(zmm6.type() == Reg::kTypeZmm);
  EXPECT(zmm6.group() == Reg::kGroupVec);
  EXPECT(zmm6.isVec());

  INFO("Checking x86::Vec register properties");
  EXPECT(Vec().isReg() == true);
  // Converts a VEC register to a type of the passed register, but keeps the ID.
  EXPECT(xmm4.cloneAs(ymm10) == ymm4);
  EXPECT(xmm4.cloneAs(zmm11) == zmm4);
  EXPECT(ymm5.cloneAs(xmm12) == xmm5);
  EXPECT(ymm5.cloneAs(zmm13) == zmm5);
  EXPECT(zmm6.cloneAs(xmm14) == xmm6);
  EXPECT(zmm6.cloneAs(ymm15) == ymm6);

  INFO("Checking x86::FpMm register properties");
  EXPECT(Mm().isReg() == true);
  EXPECT(mm2.isReg() == true);
  EXPECT(mm2.id() == 2);
  EXPECT(mm2.size() == 8);
  EXPECT(mm2.type() == Reg::kTypeMm);
  EXPECT(mm2.group() == Reg::kGroupMm);

  INFO("Checking x86::KReg register properties");
  EXPECT(KReg().isReg() == true);
  EXPECT(k3.isReg() == true);
  EXPECT(k3.id() == 3);
  EXPECT(k3.size() == 0);
  EXPECT(k3.type() == Reg::kTypeKReg);
  EXPECT(k3.group() == Reg::kGroupKReg);

  INFO("Checking x86::St register properties");
  EXPECT(St().isReg() == true);
  EXPECT(st1.isReg() == true);
  EXPECT(st1.id() == 1);
  EXPECT(st1.size() == 10);
  EXPECT(st1.type() == Reg::kTypeSt);
  EXPECT(st1.group() == Reg::kGroupSt);

  INFO("Checking if default constructed regs behave as expected");
  EXPECT(Reg().isValid() == false);
  EXPECT(Gp().isValid() == false);
  EXPECT(Xmm().isValid() == false);
  EXPECT(Ymm().isValid() == false);
  EXPECT(Zmm().isValid() == false);
  EXPECT(Mm().isValid() == false);
  EXPECT(KReg().isValid() == false);
  EXPECT(SReg().isValid() == false);
  EXPECT(CReg().isValid() == false);
  EXPECT(DReg().isValid() == false);
  EXPECT(St().isValid() == false);
  EXPECT(Bnd().isValid() == false);

  INFO("Checking x86::Mem operand");
  Mem m;
  EXPECT(m == Mem(), "Two default constructed x86::Mem operands must be equal");

  m = ptr(L);
  EXPECT(m.hasBase() == true);
  EXPECT(m.hasBaseReg() == false);
  EXPECT(m.hasBaseLabel() == true);
  EXPECT(m.hasOffset() == false);
  EXPECT(m.isOffset64Bit() == false);
  EXPECT(m.offset() == 0);
  EXPECT(m.offsetLo32() == 0);

  m = ptr(0x0123456789ABCDEFu);
  EXPECT(m.hasBase() == false);
  EXPECT(m.hasBaseReg() == false);
  EXPECT(m.hasIndex() == false);
  EXPECT(m.hasIndexReg() == false);
  EXPECT(m.hasOffset() == true);
  EXPECT(m.isOffset64Bit() == true);
  EXPECT(m.offset() == int64_t(0x0123456789ABCDEFu));
  EXPECT(m.offsetLo32() == int32_t(0x89ABCDEFu));
  m.addOffset(1);
  EXPECT(m.offset() == int64_t(0x0123456789ABCDF0u));

  m = ptr(0x0123456789ABCDEFu, rdi, 4);
  EXPECT(m.hasBase() == false);
  EXPECT(m.hasBaseReg() == false);
  EXPECT(m.hasIndex() == true);
  EXPECT(m.hasIndexReg() == true);
  EXPECT(m.indexType() == rdi.type());
  EXPECT(m.indexId() == rdi.id());
  EXPECT(m.hasOffset() == true);
  EXPECT(m.isOffset64Bit() == true);
  EXPECT(m.offset() == int64_t(0x0123456789ABCDEFu));
  EXPECT(m.offsetLo32() == int32_t(0x89ABCDEFu));
  m.resetIndex();
  EXPECT(m.hasIndex() == false);
  EXPECT(m.hasIndexReg() == false);

  m = ptr(rax);
  EXPECT(m.hasBase() == true);
  EXPECT(m.hasBaseReg() == true);
  EXPECT(m.baseType() == rax.type());
  EXPECT(m.baseId() == rax.id());
  EXPECT(m.hasIndex() == false);
  EXPECT(m.hasIndexReg() == false);
  EXPECT(m.indexType() == 0);
  EXPECT(m.indexId() == 0);
  EXPECT(m.hasOffset() == false);
  EXPECT(m.isOffset64Bit() == false);
  EXPECT(m.offset() == 0);
  EXPECT(m.offsetLo32() == 0);
  m.setIndex(rsi);
  EXPECT(m.hasIndex() == true);
  EXPECT(m.hasIndexReg() == true);
  EXPECT(m.indexType() == rsi.type());
  EXPECT(m.indexId() == rsi.id());
}
#endif

ASMJIT_END_SUB_NAMESPACE

// [Guard]
#endif // ASMJIT_BUILD_X86
