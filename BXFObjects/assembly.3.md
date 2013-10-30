ԭ��������: http://www.agner.org/optimize/
# 3 The basics of assembly coding
# 3 ����̻���

## 3.1 Assemblers available
## 3.1 ��������

There are several assemblers available for the x86 instruction set, but currently none of
them is good enough for universal recommendation. Assembly programmers are in the
unfortunate situation that there is no universally agreed syntax for x86 assembly. Different
assemblers use different syntax variants. The most common assemblers are listed below.

x86ָ��м�����������, ����û��һ���õ��Ƽ�ͨ�õ�. ������Ա������x86���û��ͨ���﷨�ľ���. ��ͬ�ı�����ʹ�ò�ͬ���﷨. �����г��˳��õļ���. 

MASM
The Microsoft assembler is included with Microsoft C++ compilers. Free versions can
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

+MASM+
΢�����������������΢��C++��������. ��Ѱ汾����ͨ������**΢��Windows�������߰�** (WDK) ����**ƽ̨����������߰�** (SDK)�õ�, Ҳ��Ϊ���Visual C++ Express�Ĳ��.  MASM��ʵ�ϳ䵱�˺ö����Windows��׼, ���Ҵ����Windows������Ҳ����MASM�﷨������. MASM���кܶ�߼���������. ����������8086������������Ļ�������, ���﷨ĳ�̶ֳ����е���ҺͲ�һ��. ΢����������ά��MASM����ΪWindows�ṩһ���걸�Ŀ�������, ������Ȼ�Ѿ�������ͼ��, ��MASM��ά��Ҳֻ������Ϊ֮. ��ָ��ᶨ�ڼӽ���, ����64λ�汾��Щȱ��. �°汾ֻ����Windows XP��֮��Ĳ���ϵͳ������, �������ڰ�װ�˱������������. ��6��֮ǰ(��)�������κ�ϵͳ������, ������װ��Windows ��������Linux. ��Щ�汾ɢ��������. 

GAS
The Gnu assembler is part of the Gnu **Binutils** package that is included with most
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

#GAS#
Gnu����������Gnu **Binutils** ����һ����, ��������Ŵ����Linux, BSD �� Mac OS X һ��װ.  Gnu������ʹ��Gnu �����������ɻ�������������. Gnu ��������ͨ��ʹ��AT&T�﷨, �����﷨�ڻ������ɴ���ʱ���ֺܺ�, �������˹����ɵĻ�����ʹ��������಻��. AT&T�﷨�ڲ�������˳���Ϻ���������x86�Ļ�����������һ��, ��Intel��AMD������ָ���ĵ�Ҳ��һ��. ��ʹ�ò�һ����ǰ׺����%��$��ָ��������������. Gnu��������������x86ƽ̨�϶�������. 
������, ��һ���Gnu������������ѡ����Intel�﷨. Gnu-Intel�﷨��MASM�﷨����һ��. Gnu-Intel�﷨ֵ������ָ������﷨, û��**ָ��**, ����, ���. ָ����Ȼʹ����ʽ��Gnu-AT&T�﷨. ͨ��ָ��`.intel_syntax noprefix`��ʹ��Intel�﷨. ���뿪C��C++�����е��������֮ǰͨ��`.att_syntax prefix`�ص�AT&T�﷨. 

NASM
NASM is a free open source assembler with support for several platforms and object file
formats. The syntax is more clear and consistent than MASM syntax. NASM has fewer highlevel
features than MASM, but it is sufficient for most purposes. NASM would be my best
recommendation for a free multi-platform assembler if you don't need MASM syntax.

#NASM#
NASM����ѵĿ�Դ��������, ֧�ּ���ƽ̨, Ҳ֧��Object�ļ�����. �����﷨��MASM�������ͳһ. NASM��MASM�ĸ߼�������, ���Ƕ���������㹻��Ч. �������ҪMASM�﷨�Ļ�, NASM�������Ƽ��Ķ�ƽ̨�µĻ�������. 

YASM
YASM is very similar to NASM and uses exactly the same syntax. YASM was previously
more reliable than NASM, but is no longer updated regularly.
FASM
The Flat assembler is another open source assembler for multiple platforms. The syntax is
not compatible with other assemblers. FASM is itself written in assembly language - an
enticing idea, but unfortunately this makes the development and maintenance of it less
efficient.
WASM
The WASM assembler is included with the Open Watcom C++ compiler. The syntax
resembles MASM but is somewhat different. Not fully up to date.
JWASM
JWASM is a further development of WASM. It is fully compatible with MASM syntax,
including advanced macro and high level directives. JWASM is a good choice if MASM
syntax is desired.
TASM
Borland Turbo Assembler is included with CodeGear C++ Builder. It is compatible with
MASM syntax except for some newer syntax additions. TASM is no longer maintained but is
still available. It is obsolete and does not support current instruction sets.
GOASM
GoAsm is a free assembler for 32- and 64-bits Windows including resource compiler, linker
and debugger. The syntax is similar to MASM but not fully compatible. It is currently not up
to date with the latest instruction sets. An integrated development environment (IDE) named
Easy Code is also available.
HLA
High Level Assembler is actually a high level language compiler that allows assembly-like
statements and produces assembly output. This was probably a good idea at the time it was
invented, but today where the best C++ compilers support intrinsic functions, I believe that
HLA is no longer needed.