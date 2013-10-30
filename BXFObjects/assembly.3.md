原文在这里: http://www.agner.org/optimize/
# 3 The basics of assembly coding
# 3 汇编编程基础

## 3.1 Assemblers available
## 3.1 汇编编译器

There are several assemblers available for the x86 instruction set, but currently none of
them is good enough for universal recommendation. Assembly programmers are in the
unfortunate situation that there is no universally agreed syntax for x86 assembly. Different
assemblers use different syntax variants. The most common assemblers are listed below.

x86指令集有几个汇编编译器, 但是没有一个好到推荐通用的. 汇编程序员面临着x86汇编没有通用语法的窘境. 不同的编译器使用不同的语法. 下面列出了常用的几个. 

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
微软汇编编译器被包含在微软C++编译器里. 免费版本可以通过下载**微软Windows驱动工具包** (WDK) 或者**平台软件开发工具包** (SDK)得到, 也作为免费Visual C++ Express的插件.  MASM事实上充当了好多年的Windows标准, 而且大多数Windows编译器也是用MASM语法输出汇编. MASM具有很多高级语言特性. 由于衍生自8086处理器的最早的汇编编译器, 该语法某种程度上有点混乱和不一致. 微软至今仍在维护MASM以期为Windows提供一套完备的开发工具, 但这显然已经无利可图了, 对MASM的维护也只是勉力为之. 新指令会定期加进来, 但是64位版本有些缺陷. 新版本只能在Windows XP和之后的操作系统上运行, 还得是在安装了编译器的情况下. 第6版之前(含)可以在任何系统上运行, 包括安装了Windows 仿真器的Linux. 这些版本散落在网上. 

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
Gnu汇编编译器是Gnu **Binutils** 包的一部分, 这个包随着大多数Linux, BSD 和 Mac OS X 一起安装.  Gnu编译器使用Gnu 汇编编译器生成汇编输出再做链接. Gnu 汇编编译器通常使用AT&T语法, 这种语法在机器生成代码时表现很好, 但是在人工生成的汇编代码使用上有诸多不便. AT&T语法在操作数的顺序上和其他所有x86的汇编编译器都不一样, 跟Intel与AMD发布的指令文档也不一样. 它使用不一样的前缀例如%和$来指定操作数的类型. Gnu汇编编译器在所有x86平台上都可以用. 
好事是, 新一点的Gnu汇编编译器可以选择用Intel语法. Gnu-Intel语法和MASM语法几乎一样. Gnu-Intel语法值定义了指令码的语法, 没有**指令**, 函数, 宏等. 指令仍然使用老式的Gnu-AT&T语法. 通过指定`.intel_syntax noprefix`来使用Intel语法. 在离开C或C++代码中的内联汇编之前通过`.att_syntax prefix`回到AT&T语法. 

NASM
NASM is a free open source assembler with support for several platforms and object file
formats. The syntax is more clear and consistent than MASM syntax. NASM has fewer highlevel
features than MASM, but it is sufficient for most purposes. NASM would be my best
recommendation for a free multi-platform assembler if you don't need MASM syntax.

#NASM#
NASM是免费的开源汇编编译器, 支持几个平台, 也支持Object文件类型. 它的语法比MASM更清楚和统一. NASM比MASM的高级特性少, 但是多数情况下足够高效. 如果不需要MASM语法的话, NASM是我最推荐的多平台下的汇编编译器. 

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