ԭ��������: http://www.agner.org/optimize/

> # 3 The basics of assembly coding
> ## 3.1 Assemblers available

> There are several assemblers available for the x86 instruction set, but currently none of
them is good enough for universal recommendation. Assembly programmers are in the
unfortunate situation that there is no universally agreed syntax for x86 assembly. Different
assemblers use different syntax variants. The most common assemblers are listed below.

# 3 ����̻���

## 3.1 �����

x86ָ��м��������, ����û��һ���õ��Ƽ�ͨ�õ�. ������Ա������x86���û��ͨ���﷨�ľ���. ��ͬ�ı�����ʹ�ò�ͬ���﷨. �����г��˳��õļ���. 

> MASM
> The Microsoft assembler is included with Microsoft C++ compilers. Free versions can
sometimes be obtained by downloading the **Microsoft Windows driver kit** (WDK) or the
**platforms software development kit** (SDK) or as an add-on to the free Visual C++ Express
Edition. MASM has been a de-facto standard in the Windows world for many years, and the
assembly output of most Windows compilers uses MASM syntax. MASM has many
advanced language features. The syntax is somewhat messy and inconsistent due to a
heritage that dates back to the very first assemblers for the 8086 processor. Microsoft is still
maintaining MASM in order to provide a complete set of development tools for Windows, but
it is obviously not profitable and the maintenance of MASM is apparently kept at a minimum.
New instruction sets are still added regularly, but the 64-bit version has several deficiencies.
Newer versions can run only when the compiler is installed and only in Windows XP or later.
Version 6 and earlier can run in any system, including Linux with a Windows emulator. Such
versions are circulating on the web.

<u>MASM</u>
΢��������������΢��C++��������. ��Ѱ汾����ͨ������**΢��Windows�������߰�** (WDK) ����**ƽ̨����������߰�** (SDK)�õ�, Ҳ��Ϊ���Visual C++ Express�Ĳ��.  MASM��ʵ�ϳ䵱�˺ö����Windows��׼, ���Ҵ����Windows������Ҳ����MASM�﷨������. MASM���кܶ�߼���������. ����������8086������������Ļ����, ���﷨ĳ�̶ֳ����е���ҺͲ�һ��. ΢����������ά��MASM����ΪWindows�ṩһ���걸�Ŀ�������, ������Ȼ�Ѿ�������ͼ��, ��MASM��ά��Ҳֻ������Ϊ֮. ��ָ��ᶨ�ڼӽ���, ����64λ�汾��Щȱ��. �°汾ֻ����Windows XP��֮��Ĳ���ϵͳ������, �������ڰ�װ�˱������������. ��6��֮ǰ(��)�������κ�ϵͳ������, ������װ��Windows ��������Linux. ��Щ�汾ɢ��������. 

> GAS

> The Gnu assembler is part of the Gnu **Binutils** package that is included with most
distributions of Linux, BSD and Mac OS X. The Gnu compilers produce assembly output
that goes through the Gnu assembler before it is linked. The Gnu assembler traditionally
uses the AT&T syntax that works well for machine-generated code, but it is very
inconvenient for human-generated assembly code. The AT&T syntax has the operands in
an order that differs from all other x86 assemblers and from the instruction documentation
published by Intel and AMD. It uses various prefixes like % and $ for specifying operand
types. The Gnu assembler is available for all x86 platforms.
Fortunately, newer versions of the Gnu assembler has an option for using Intel syntax
instead. The Gnu-Intel syntax is almost identical to MASM syntax. The Gnu-Intel syntax
defines only the syntax for instruction codes, not for **directives**, functions, macros, etc. The
directives still use the old Gnu-AT&T syntax. Specify .intel_syntax noprefix to use the
Intel syntax. Specify .att_syntax prefix to return to the AT&T syntax before leaving
inline assembly in C or C++ code.

<u>GAS</u>

Gnu�������Gnu **Binutils** ����һ����, ��������Ŵ����Linux, BSD �� Mac OS X һ��װ.  Gnu������ʹ��Gnu ��������ɻ�������������. Gnu �����ͨ��ʹ��AT&T�﷨, �����﷨�ڻ������ɴ���ʱ���ֺܺ�, �������˹����ɵĻ�����ʹ��������಻��. AT&T�﷨�ڲ�������˳���Ϻ���������x86�Ļ��������һ��, ��Intel��AMD������ָ���ĵ�Ҳ��һ��. ��ʹ�ò�һ����ǰ׺����%��$��ָ��������������. Gnu�����������x86ƽ̨�϶�������. 
������, ��һ���Gnu���������ѡ����Intel�﷨. Gnu-Intel�﷨��MASM�﷨����һ��. Gnu-Intel�﷨ֵ������ָ������﷨, û��**ָ��**, ����, ���. ָ����Ȼʹ����ʽ��Gnu-AT&T�﷨. ͨ��ָ��`.intel_syntax noprefix`��ʹ��Intel�﷨. ���뿪C��C++�����е��������֮ǰͨ��`.att_syntax prefix`�ص�AT&T�﷨. 

> NASM

> NASM is a free open source assembler with support for several platforms and object file
formats. The syntax is more clear and consistent than MASM syntax. NASM has fewer highlevel
features than MASM, but it is sufficient for most purposes. NASM would be my best
recommendation for a free multi-platform assembler if you don't need MASM syntax.

<u>NASM</u>

NASM����ѵĿ�Դ�����, ֧�ּ���ƽ̨, Ҳ֧��Object�ļ�����. �����﷨��MASM�������ͳһ. NASM��MASM�ĸ߼�������, ���Ƕ���������㹻��Ч. �������ҪMASM�﷨�Ļ�, NASM�������Ƽ��Ķ�ƽ̨�µĻ����. 

> YASM

> YASM is very similar to NASM and uses exactly the same syntax. YASM was previously
more reliable than NASM, but is no longer updated regularly.

<u>YASM</u>
YASM��NASM�����Ʋ���ʹ��ͬ�����﷨. ֮ǰYASM��NASM���ɿ���������û���˶��ڸ�������. 


> FASM

> The Flat assembler is another open source assembler for multiple platforms. The syntax is
not compatible with other assemblers. FASM is itself written in assembly language - an
enticing idea, but unfortunately this makes the development and maintenance of it less
efficient.

<u>FASM</u>

Flat���������һ���ƽ̨��Դ�����. �﷨�����������������. FASM�������û������ʵ�ֵ� - ����������, ���ҵ����������Ŀ�����ά����û��ô��ʱ. 

> WASM

> The WASM assembler is included with the Open Watcom C++ compiler. The syntax
resembles MASM but is somewhat different. Not fully up to date.

<u>WASM</u>

WASM�����������Open Watcom C++��������. �﷨����MSASM, ĳ�̶ֳ��ϲ�ͬ. ���Ǻܸ�����ʱ��. 

> JWASM

> JWASM is a further development of WASM. It is fully compatible with MASM syntax,
including advanced macro and high level directives. JWASM is a good choice if MASM
syntax is desired.

<u>JWASM</u>

JWASM��WASM�Ľ��װ�. ����MASM�﷨��ȫ����, �������߼��ĺ�͸߼�ָ��. �����Ҫʹ��MASM�﷨�Ļ�, JWASM����. 

> TASM
> Borland Turbo Assembler is included with CodeGear C++ Builder. It is compatible with
MASM syntax except for some newer syntax additions. TASM is no longer maintained but is
still available. It is obsolete and does not support current instruction sets.

<u>TASM</u>

CodeGear C++ Builder����Borland Turbo �����. ���˼����µ��﷨֮������MASM��ȫ����. TASM���ټ���ά��, �����ǿ��õ�. TASM�Ѿ�����̭��, ���Ҳ�֧�����ڵ�ָ���. 

> GOASM

> GoAsm is a free assembler for 32- and 64-bits Windows including resource compiler, linker
and debugger. The syntax is similar to MASM but not fully compatible. It is currently not up
to date with the latest instruction sets. An integrated development environment (IDE) named
Easy Code is also available.

<u>GOASM</u>

GoAsm��32λ64λWindows�����õ���ѻ����, ������Դ���������, �������͵�����. �﷨��MASM���Ƶ�����ȫ����. ��û�и������µ�ָ�. һ������Easy Code��IDE����ʹ��. 

> HLA

> High Level Assembler is actually a high level language compiler that allows assembly-like
statements and produces assembly output. This was probably a good idea at the time it was
invented, but today where the best C++ compilers support intrinsic functions, I believe that
HLA is no longer needed.

<u>HLA</u>

�߼������(High Level Assembler)ʵ������һ���߼����Ա�����, ������ʹ���������Ե���䲢�����ƻ�����. �ڷ���HLA��ʱ�������ⲻ��, ����������õ�C++�������Ѿ�֧��αָ�����, �Ҿ���û��Ҫ��HLA��. 

> Inline assembly
Microsoft and Intel C++ compilers support inline assembly using a subset of the MASM
syntax. It is possible to access C++ variables, functions and **labels** simply by inserting their
names in the assembly code. This is easy, but does not support C++ register variables. See
page 36.
The Gnu compiler supports inline assembly with access to the full range of **instructions** and
**directives** of the Gnu assembler in both Intel and AT&T syntax. The access to C++ variables
from assembly uses a quite complicated method.
The Intel compilers for Linux and Mac systems support both the Microsoft style and the Gnu
style of inline assembly.

<u>�������</u>
΢���Ӣ�ض���C++������֧��ʹ��MASM���﷨�Ӽ������������. ���Է���C++����, ������**��ǩ**, ���뵽�������������. �������, ��������֧��C++�Ĵ�������. ��36ҳ. 

Gnu������֧���������, ���Է���Gnu�������Intel��AT&T�﷨�µ�����**ָ��(instructions)**��**����(directives)**. �ڻ���з���C++������ͦ���ӵ�. 

Linux��Macϵͳ�µ�Intel������֧��΢�����Gnu�����������. 

<ע> ����instructions��directives: directive����ָ��������ȥ���������� org .0x000. ���е�org�� directive,ָ�����������������Ĵ�����ڵ�ַΪ0x000�Ŀռ䡣 ������ db 0xff, DW 0x36�ȵ�. ��instruction ��ָ�����������ת���ɶ�Ӧ�Ļ����룬Ȼ��CPU����ʶ��ִ����Щ�����룬�� mov P1,0xff. ADD R3,56�ȵ�ַ0000h~FFFFh����ʮ�����Ʊ�ʾ�ģ�������е�1K=2��10�η���0xffff���� 64��2��10�η���


> **Intrinsic functions** in C++
This is the newest and most convenient way of combining low-level and high-level code.
Intrinsic functions are high-level language representatives of machine instructions. For
example, you can do a vector addition in C++ by calling the intrinsic function that is
equivalent to an assembly instruction for vector addition. Furthermore, it is possible to
define a **vector class** with an overloaded + operator so that a vector addition is obtained
simply by writing +. Intrinsic functions are supported by Microsoft, Intel and Gnu compilers.
See page 34 and manual 1: "Optimizing software in C++".

<u>C++��**αָ���**</u>

�������µ�Ҳ��������Ľ�ϵײ����͸߼����Եķ�ʽ. **αָ���**���Ը߼�������ò���ֵĻ���ָ��. �����������C++�е���αָ������������ӷ�, ���ʹ��������ӵĻ��ָ���ǵȼ۵�. ����һ��, ���Զ���**������** �����������+, ���������ļӷ��Ϳ��Լ򵥵�д��+. ΢��, Ӣ�ض���Gnu�ı�������֧��Ϊαָ���. ��34ҳ���ֲ�1: "�Ż�C++���". 

> Which assembler to choose?
In most cases, the easiest solution is to use intrinsic functions in C++ code. The compiler
can take care of most of the optimization so that the programmer can concentrate on
choosing the best algorithm and organizing the data into vectors. System programmers can
access system instructions by using intrinsic functions without having to use assembly
language.

<u>ѡ����һ�������?</u>

�����������򵥵Ľ����������C++������ʹ��αָ���. �����������󲿷��Ż�, ����Աֻ��ר��ѡ����õ��㷨����֯�����е����ݾͺ���. ϵͳ����Ա����ʹ��αָ�������ϵͳָ��, ����Ҫʹ�û������. 

> Where real low-level programming is needed, such as in highly optimized function libraries
or device drivers, you may use an assembler.
It may be preferred to use an assembler that is compatible with the C++ compiler you are
using. This allows you to use the compiler for translating C++ to assembly, optimize the
assembly code further, and then assemble it. If the assembler is not compatible with the
syntax generated by the compiler then you may generate an object file with the compiler
and disassemble the object file to the assembly syntax you need. The objconv disassembler
supports several different syntax dialects.

������Ҫ�ײ㿪��ʱ, ����߶��Ż��ĺ���������豸����, �������Ҫ�õ������. 

����ѡ��������õ�C++���������ݵĻ������һЩ. ������Ϳ������ñ�������C++����ɻ�����, ��һ���Ż�������, Ȼ��ʹ�û����. ���������ͱ��������ɵĻ����벻���ݵĻ�, ��͵��ñ��������ɶ����ļ�(*.obj), �ٷ���������Ҫ�Ļ���﷨. objconv�������֧�ּ��в�ͬ���﷨����. 

> The NASM assembler is a good choice for many purposes because it supports many
platforms and object file formats, it is well maintained, and usually up to date with the latest
instruction sets.
The examples in this manual use MASM syntax, unless otherwise noted. The MASM syntax
is described in Microsoft Macro Assembler Reference at msdn.microsoft.com.
See www.agner.org/optimize for links to various syntax manuals, coding manuals and
discussion forums.

�ܶ������NASM�������һ�����ʵ�ѡ��, ��֧�ֶ�ƽ̨�Ͷ����ļ���ʽ, ά���Ĳ���, ͨ�����������µ�ָ�. 

�����ر�˵��, �Ȿ�ֲ������ʹ��MASM�﷨. MASM�﷨�� msdn.microsoft.com ��΢��Macro������ĵ����н�. 

www.agner.org/optimize �и����﷨�ֲ�, �����ֲ����̳������. 

| A | B | C |
|:-----------|:------------|:------------|
| a|b |c     

> ## 3.2 Register set and basic instructions
> Registers in 16 bit mode
> General purpose and integer registers

> | Full register | Partial register |   Partial register |
|:-----------|:------------|:------------|
|bit 0 - 15 |bit 8 - 15 |bit 0 - 7
|AX |AH |AL
|BX |BH |BL
|CX |CH |CL
|DX |DH |DL
|SI
|DI
|BP
|SP
|Flags
|IP

> |Table 3.1. General purpose registers in 16 bit mode.|
|:-----------|

## 3.2 �Ĵ������ͻ���ָ��

<u>16λģʽ�Ĵ���</u>

**ͨ�üĴ���(General Purpose)�����μĴ���**

| ȫ�Ĵ���(Full Register) | ���ּĴ���(Partial register) | ���ּĴ���(Partial register)|
|:-----------|:------------|:------------|
|bit 0 - 15 |bit 8 - 15 |bit 0 - 7
|AX |AH |AL
|BX |BH |BL
|CX |CH |CL
|DX |DH |DL
|SI
|DI
|BP
|SP
|Flags
|IP

|�� 3.1. 16λģʽ�µ�ͨ�üĴ���.|
|:----------:|

> The 32-bit registers are also available in 16-bit mode if supported by the microprocessor
and operating system. The high word of ESP should not be used because it is not saved
during interrupts.

΢�������Ͳ���ϵͳ֧�ֵĻ�, 32λ�Ĵ�����16λģʽ��Ҳ����. `ESP`�ĸ�λ�ֲ�Ҫ��, ����תʱ�����ᱻ����. 

> Floating point registers

> |Full register|
|:-----------|
|bit 0 - 79
|ST(0)
|ST(1)
|ST(2)
|ST(3)
|ST(4)
|ST(5)
|ST(6)
|ST(7)

>|Table 3.2. Floating point stack registers|
|:--------|

> MMX registers may be available if supported by the microprocessor. XMM registers may be
available if supported by microprocessor and operating system.

**����Ĵ���(Floating Point Registers)**

|ȫ�Ĵ���|
|:-----------|
|bit 0 - 79
|ST(0)
|ST(1)
|ST(2)
|ST(3)
|ST(4)
|ST(5)
|ST(6)
|ST(7)

|�� 3.2. ����ջ�Ĵ���|
|:--------|

΢������֧�ֵĻ�MMX�Ĵ���Ҳ������. ΢�������Ͳ���ϵͳ֧�ֵĻ�XMM�Ĵ�������ʹ��. 

> **Segment registers**

>|Full register|
|:---------:|
|bit 0 - 15
|CS
|DS
|ES
|SS

>|Table 3.3. Segment registers in 16 bit mode|
|:--------:|

> Register FS and GS may be available.

**�μĴ���**

|ȫ�Ĵ���|
|:---------:|
|0-15λ
|CS
|DS
|ES
|SS

|�� 3.3. 16λģʽ�µĶμĴ���|
|:--------:|

FS��GS�Ĵ������ܿ���. 

> Registers in 32 bit mode
> General purpose and integer registers

> |Full register bit 0 - 31|Partial register bit 0 - 15|Partial register bit 8 - 15|Partial register bit 0 - 7|
|:--------:|:--------:|:--------:|:--------:|
|EAX |AX |AH |AL
|EBX |BX |BH |BL
|ECX |CX |CH |CL
|EDX |DX |DH |DL
|ESI |SI
|EDI |DI
|EBP |BP
|ESP |SP
|EFlags |Flags
|EIP |IP

>|Table 3.4. General purpose registers in 32 bit mode|
|:--------:|

**32λģʽ�µļĴ���**

<u>ͨ�üĴ��������μĴ���</u>

|ȫ�Ĵ��� 0-31λ|���ּĴ��� 0-15λ|���ּĴ��� 8-15λ|���ּĴ��� 0-7λ
|:--------:|:--------:|:--------:|:--------:|
|EAX |AX |AH |AL
|EBX |BX |BH |BL
|ECX |CX |CH |CL
|EDX |DX |DH |DL
|ESI |SI
|EDI |DI
|EBP |BP
|ESP |SP
|EFlags |Flags
|EIP |IP

|�� 3.4. 32λģʽ�µ�ͨ�üĴ���|
|:--------:|

>Floating point and 64-bit vector registers

> |Full register bit 0 - 79 |Partial register bit 0 - 63|
|:--------:|:--------:|
|ST(0) |MM0
|ST(1) |MM1
|ST(2) |MM2
|ST(3) |MM3
|ST(4) |MM4
|ST(5) |MM5
|ST(6) |MM6
|ST(7) |MM7

>|Table 3.5. Floating point and MMX registers|
|:-----:|

>The MMX registers are only available if supported by the microprocessor. The ST and MMX
registers cannot be used in the same part of the code. A section of code using MMX
registers must be separated from any subsequent section using ST registers by executing
an EMMS instruction.

**�����64λ�����Ĵ���**

|ȫ�Ĵ���0-79λ|���ּĴ���0-63λ|
|:--------:|:--------:|
|ST(0) |MM0
|ST(1) |MM1
|ST(2) |MM2
|ST(3) |MM3
|ST(4) |MM4
|ST(5) |MM5
|ST(6) |MM6
|ST(7) |MM7

|�� 3.5. �����MMX�Ĵ���|
|:-----:|

MMX�Ĵ���ֻ��΢������֧�ֵ�����¿���. ST��MMX�Ĵ�����ͬһ������ﲻ�ɻ���. ʹ��MMX�Ĵ����Ĵ���Ҫ����`EMMS`ָ�����ͺ�����ʹ��ST�Ĵ������ֻ������. 

>128- and 256-bit integer and floating point vector registers

> |Full register bit 0 - 255 |Full or partial register bit 0 - 127|
|:----:|:----:|
|YMM0 |XMM0
|YMM1 |XMM1
|YMM2 |XMM2
|YMM3 |XMM3
|YMM4 |XMM4
|YMM5 |XMM5
|YMM6 |XMM6
|YMM7 |XMM7

>|Table 3.6. XMM and YMM registers in 32 bit mode|
|:----:|

>The XMM registers are only available if supported both by the microprocessor and the
operating system. Scalar floating point instructions use only 32 or 64 bits of the XMM
registers for single or double precision, respectively. The YMM registers are available only if
the processor and the operating system supports the AVX instruction set.

**128��256λ���������͸��������Ĵ���**

|ȫ�Ĵ���0-255λ |ȫ�Ĵ����򲿷ּĴ���0-127λ|
|:----:|:----:|
|YMM0 |XMM0
|YMM1 |XMM1
|YMM2 |XMM2
|YMM3 |XMM3
|YMM4 |XMM4
|YMM5 |XMM5
|YMM6 |XMM6
|YMM7 |XMM7

|�� 3.6. 32λģʽ�µ�XMM�Ĵ�����YMM�Ĵ���|
|:----:|

ֻ����΢�������Ͳ���ϵͳ��֧�ֵ������XMM�Ĵ�������. ���������ָ���ڵ����Ⱥ�˫�����·ֱ�ֻʹ��32λ��64λ��XMM�Ĵ���. **(Scalar floating point instructions use only 32 or 64 bits of the XMM registers for single or double precision, respectively) ��仰��˵XMM�Ĵ�������32λ��64λ��, ����˵ֻ�мĴ����е�һ���ֱ�ʹ����?** YMM�Ĵ���ֻ�ڴ������Ͳ���ϵͳ��֧��AVXָ�������¿���. 

>Segment registers

> |Full register bit 0 - 15|
|:----:|
|CS
|DS
|ES
|FS
|GS
|SS

> |Table 3.7. Segment registers in 32 bit mode|
|:----:|

**�μĴ���**

|ȫ�Ĵ���0-15λ|
|:----:|
|CS
|DS
|ES
|FS
|GS
|SS

|�� 3.7. 32λģʽ�µĶμĴ���|
|:----:|

> Registers in 64 bit mode
> General purpose and integer registers

> |Full register bit 0 - 63|Partial register bit 0 - 31|Partial register bit 0 - 15|Partial register bit 8 - 15|Partial register bit 0 - 7|
|:----:|----:|----:|----:|----:|
|RAX |EAX |AX |AH AL
|RBX |EBX |BX |BH BL
|RCX |ECX |CX |CH CL
|RDX |EDX |DX |DH DL
|RSI |ESI |SI |    |SIL
|RDI |EDI |DI |    |DIL
|RBP |EBP |BP |    |BPL
|RSP |ESP |SP |    |SPL
|R8 |R8D |R8W |    |R8B
|R9 |R9D |R9W |    |R9B
|R10 |R10D |R10W |    |R10B
|R11 |R11D |R11W |    |R11B
|R12 |R12D |R12W |    |R12B
|R13 |R13D |R13W |    |R13B
|R14 |R14D |R14W |    |R14B
|R15 |R15D |R15W |    |R15B
|RFlags |    |Flags
|RIP

>|Table 3.8. Registers in 64 bit mode|
|:----:|

<u>64λģʽ�µļĴ���</u>

**ͨ�ü�Ⱥ�������μĴ���**

|ȫ�Ĵ���0-63λ|Partial register bit 0 - 31|Partial register bit 0 - 15|Partial register bit 8 - 15|Partial register bit 0 - 7|
|:----:|----:|----:|----:|----:|
|RAX |EAX |AX |AH AL
|RBX |EBX |BX |BH BL
|RCX |ECX |CX |CH CL
|RDX |EDX |DX |DH DL
|RSI |ESI |SI |    |SIL
|RDI |EDI |DI |    |DIL
|RBP |EBP |BP |    |BPL
|RSP |ESP |SP |    |SPL
|R8 |R8D |R8W |    |R8B
|R9 |R9D |R9W |    |R9B
|R10 |R10D |R10W |    |R10B
|R11 |R11D |R11W |    |R11B
|R12 |R12D |R12W |    |R12B
|R13 |R13D |R13W |    |R13B
|R14 |R14D |R14W |    |R14B
|R15 |R15D |R15W |    |R15B
|RFlags |    |Flags
|RIP

|�� 3.8. 64λģʽ�µļĴ���|
|:----:|

>The high 8-bit registers AH, BH, CH, DH can only be used in instructions that have no REX
prefix.

>Note that modifying a 32-bit partial register will set the rest of the register (bit 32-63) to zero,
but modifying an 8-bit or 16-bit partial register does not affect the rest of the register. This
can be illustrated by the following sequence:

> <pre><code>
; Example 3.1. 8, 16, 32 and 64 bit registers
mov rax, 1111111111111111H ; rax = 1111111111111111H
mov eax, 22222222H ; rax = 0000000022222222H
mov ax, 3333H ; rax = 0000000022223333H
mov al, 44H ; rax = 0000000022223344H
</code></pre>

> There is a good reason for this inconsistency. Setting the unused part of a register to zero is
more efficient than leaving it unchanged because this removes a false dependence on
previous values. But the principle of resetting the unused part of a register cannot be
extended to 16 bit and 8 bit partial registers because this would break the backwards
compatibility with 32-bit and 16-bit modes.

> The only instruction that can have a 64-bit immediate data operand is MOV. Other integer
instructions can only have a 32-bit sign extended operand. Examples:

> <pre><code>
; Example 3.2. Immediate operands, full and sign extended
mov rax, 1111111111111111H ; Full 64 bit immediate operand
mov rax, -1 ; 32 bit sign-extended operand
mov eax, 0ffffffffH ; 32 bit zero-extended operand
add rax, 1 ; 8 bit sign-extended operand
add rax, 100H ; 32 bit sign-extended operand
add eax, 100H ; 32 bit operand. result is zero-extended
mov rbx, 100000000H ; 64 bit immediate operand
add rax, rbx ; Use an extra register if big operand
</code></pre>

> It is not possible to use a 16-bit sign-extended operand. If you need to add an immediate
value to a 64 bit register then it is necessary to first move the value into another register if
the value is too big for fitting into a 32 bit sign-extended operand.

��8λ�Ĵ���`AH`, `BH`, `CH`, `DH`ֻ����û��REXǰ׺��ָ����ʹ��. 

ע���޸�32λ���ּĴ�����ѼĴ���������32λ(32-64λ)��Ϊ0, �����޸�8λ��16λ���ּĴ���������λû��Ӱ��. ��������Կ���: 

<pre><code>
; �� 3.1. 8, 16, 32��64λ�Ĵ���
mov rax, 1111111111111111H ; rax = 1111111111111111H (64λ)
mov eax, 22222222H ; rax = 0000000022222222H	(32λ)
mov ax, 3333H ; rax = 0000000022223333H	(16λ)
mov al, 44H ; rax = 0000000022223344H	(8λ)
</code></pre>

���ֲ�һ��ʱ�����ɵ�. �ѼĴ����в��õ�λ��Ϊ0, �Ͳ�������֮ǰ��ֵ, ��������Ч��. ������ʹ��8λ��16λ���ּĴ���ʱ���������ƻ���32λ��16λģʽ�ļ�����(**How?**), ���Բ���������. 

Ψһ�ܲ���64λֱ�Ӳ�������ָ����`MOV`. ��������ָ��ֻ�ܲ���32λ������չ������. ����: 
<pre><code>
; �� 3.2. ֱ�Ӳ�����, ȫ64λ, �ͷ�����չ��64λ������
mov rax, 1111111111111111H ;					ȫ64λ��ֱ�Ӳ�����
mov rax, -1 ;											32λ������չ������
mov eax, 0ffffffffH ;							32λ0��չ������
add rax, 1 ;											8λ������չ������
add rax, 100H ;									32λ������չ������
add eax, 100H ;									32λ������. �����0��չ	(ע: ��һ�к���һ��, ������Ϊʲôһ���Ƿ�����չһ����0��չ)
mov rbx, 100000000H ;						64λֱ�Ӳ�����, (ע, 100000000H���ֱ������32λ�Ĵ�����˵̫����, ����Ҫ�ȷ���һ��64λ�Ĵ�����)
add rax, rbx ; ����������ܴ�, ����һ���Ĵ���. 
</code></pre>

16λ������չ������������. �������Ҫ��һ��64λ�Ĵ����ϼ�һ����32λ������չ�����������ֱ�����Ļ�, �͵��Ȱ������ֵ������һ���Ĵ�����. 

> Floating point and 64-bit vector registers

> |Full register bit 0 - 79|Partial register bit 0 - 63|
|:----:|:----:|
|ST(0) |MM0
|ST(1) |MM1
|ST(2) |MM2
|ST(3) |MM3
|ST(4) |MM4
|ST(5) |MM5
|ST(6) |MM6
|ST(7) |MM7

>|Table 3.9. Floating point and MMX registers|
|:----:|:----:|

>The ST and MMX registers cannot be used in the same part of the code. A section of code
using MMX registers must be separated from any subsequent section using ST registers by
executing an EMMS instruction. The ST and MMX registers cannot be used in device drivers
for 64-bit Windows.

**�����64λ�����Ĵ���**

|ȫ�Ĵ���0-79λ|���ּĴ���0-63λ|
|:----:|:----:|
|ST(0) |MM0
|ST(1) |MM1
|ST(2) |MM2
|ST(3) |MM3
|ST(4) |MM4
|ST(5) |MM5
|ST(6) |MM6
|ST(7) |MM7

|�� 3.9. �����MMX�Ĵ���|
|:----:|:----:|

ST��MMX�Ĵ��������ڴ����е�ͬһ����ʹ��. ʹ��MMX�Ĵ����Ĵ���Ҫ����`EMMS`ָ������Ĵ������ͺ�����ʹ��ST�Ĵ����Ĳ��ֻ������. ST��MMX�Ĵ�����������64λWindows��ϵͳ��������. 

>128- and 256-bit integer and floating point vector registers

> |Full register bit 0 - 255|Full or partial register bit 0 - 127|
|:----:|:----:|
|YMM0 |XMM0
|YMM1 |XMM1
|YMM2 |XMM2
|YMM3 |XMM3
|YMM4 |XMM4
|YMM5 |XMM5
|YMM6 |XMM6
|YMM7 |XMM7
|YMM8 |XMM8
|YMM9 |XMM9
|YMM10 |XMM10
|YMM11 |XMM11
|YMM12 |XMM12
|YMM13 |XMM13
|YMM14 |XMM14
|YMM15 |XMM15

>|Table 3.10. XMM and YMM registers in 64 bit mode|
|:----:|

> Scalar floating point instructions use only 32 or 64 bits of the XMM registers for single or
double precision, respectively. The YMM registers are available only if the processor and
the operating system supports the AVX instruction set.

**128λ��256λ���������͸��������Ĵ���**

|ȫ�Ĵ���0-255λ|ȫ�Ĵ����򲿷ּĴ���0-127λ|
|:----:|:----:|
|YMM0 |XMM0
|YMM1 |XMM1
|YMM2 |XMM2
|YMM3 |XMM3
|YMM4 |XMM4
|YMM5 |XMM5
|YMM6 |XMM6
|YMM7 |XMM7
|YMM8 |XMM8
|YMM9 |XMM9
|YMM10 |XMM10
|YMM11 |XMM11
|YMM12 |XMM12
|YMM13 |XMM13
|YMM14 |XMM14
|YMM15 |XMM15

|�� 3.10. 64λģʽ�µ�XMM��YMM�Ĵ���|
|:----:|

���������ָ���ڵ����Ⱥ�˫�����·ֱ�ֻʹ��XMM�Ĵ�����32λ��64λ. YMM�Ĵ���ֻ�ڴ������Ͳ���ϵͳ֧��AVXָ�ʱ����. 

> Segment registers

> |Full register bit 0 - 15|
|:----:|
|CS
|FS
|GS

> |Table 3.11. Segment registers in 64 bit mode|
|:----:|

> Segment registers are only used for special purposes.

**�μĴ���**

|ȫ�Ĵ���0-15λ|
|:----:|
|CS
|FS
|GS

|�� 3.11. 64λģʽ�µĶμĴ���|
|:----:|

�μĴ���ֻ��������Ŀ��. 

> ## 3.3 Addressing modes

> Addressing in 16-bit mode

> 16-bit code uses a segmented memory model. A memory operand can have any of these
components:

## 3.3 ȡַģʽ

<u>16λȡַģʽ</u>

16λ����ʹ�÷ֶ��ڴ�ģʽ. �ڴ�������ɺ�������ģ��: 

