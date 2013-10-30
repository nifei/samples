原文在这里: http://www.agner.org/optimize/

> # 3 The basics of assembly coding
> ## 3.1 Assemblers available

> There are several assemblers available for the x86 instruction set, but currently none of
them is good enough for universal recommendation. Assembly programmers are in the
unfortunate situation that there is no universally agreed syntax for x86 assembly. Different
assemblers use different syntax variants. The most common assemblers are listed below.

# 3 汇编编程基础

## 3.1 汇编器

x86指令集有几个汇编器, 但是没有一个好到推荐通用的. 汇编程序员面临着x86汇编没有通用语法的窘境. 不同的编译器使用不同的语法. 下面列出了常用的几个. 

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
微软汇编器被包含在微软C++编译器里. 免费版本可以通过下载**微软Windows驱动工具包** (WDK) 或者**平台软件开发工具包** (SDK)得到, 也作为免费Visual C++ Express的插件.  MASM事实上充当了好多年的Windows标准, 而且大多数Windows编译器也是用MASM语法输出汇编. MASM具有很多高级语言特性. 由于衍生自8086处理器的最早的汇编器, 该语法某种程度上有点混乱和不一致. 微软至今仍在维护MASM以期为Windows提供一套完备的开发工具, 但这显然已经无利可图了, 对MASM的维护也只是勉力为之. 新指令会定期加进来, 但是64位版本有些缺陷. 新版本只能在Windows XP和之后的操作系统上运行, 还得是在安装了编译器的情况下. 第6版之前(含)可以在任何系统上运行, 包括安装了Windows 仿真器的Linux. 这些版本散落在网上. 

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

Gnu汇编器是Gnu **Binutils** 包的一部分, 这个包随着大多数Linux, BSD 和 Mac OS X 一起安装.  Gnu编译器使用Gnu 汇编器生成汇编输出再做链接. Gnu 汇编器通常使用AT&T语法, 这种语法在机器生成代码时表现很好, 但是在人工生成的汇编代码使用上有诸多不便. AT&T语法在操作数的顺序上和其他所有x86的汇编器都不一样, 跟Intel与AMD发布的指令文档也不一样. 它使用不一样的前缀例如%和$来指定操作数的类型. Gnu汇编器在所有x86平台上都可以用. 
好事是, 新一点的Gnu汇编器可以选择用Intel语法. Gnu-Intel语法和MASM语法几乎一样. Gnu-Intel语法值定义了指令码的语法, 没有**指令**, 函数, 宏等. 指令仍然使用老式的Gnu-AT&T语法. 通过指定`.intel_syntax noprefix`来使用Intel语法. 在离开C或C++代码中的内联汇编之前通过`.att_syntax prefix`回到AT&T语法. 

> NASM

> NASM is a free open source assembler with support for several platforms and object file
formats. The syntax is more clear and consistent than MASM syntax. NASM has fewer highlevel
features than MASM, but it is sufficient for most purposes. NASM would be my best
recommendation for a free multi-platform assembler if you don't need MASM syntax.

<u>NASM</u>

NASM是免费的开源汇编器, 支持几个平台, 也支持Object文件类型. 它的语法比MASM更清楚和统一. NASM比MASM的高级特性少, 但是多数情况下足够高效. 如果不需要MASM语法的话, NASM是我最推荐的多平台下的汇编器. 

> YASM

> YASM is very similar to NASM and uses exactly the same syntax. YASM was previously
more reliable than NASM, but is no longer updated regularly.

<u>YASM</u>
YASM和NASM很相似并且使用同样的语法. 之前YASM比NASM更可靠但是现在没有人定期更新它了. 


> FASM

> The Flat assembler is another open source assembler for multiple platforms. The syntax is
not compatible with other assemblers. FASM is itself written in assembly language - an
enticing idea, but unfortunately this makes the development and maintenance of it less
efficient.

<u>FASM</u>

Flat汇编器是另一款多平台开源汇编器. 语法和其他汇编器不兼容. FASM本身是用汇编语言实现的 - 听起来不错, 不幸的是这令它的开发和维护都没那么及时. 

> WASM

> The WASM assembler is included with the Open Watcom C++ compiler. The syntax
resembles MASM but is somewhat different. Not fully up to date.

<u>WASM</u>

WASM汇编器包含在Open Watcom C++编译器中. 语法类似MSASM, 某种程度上不同. 不是很跟得上时代. 

> JWASM

> JWASM is a further development of WASM. It is fully compatible with MASM syntax,
including advanced macro and high level directives. JWASM is a good choice if MASM
syntax is desired.

<u>JWASM</u>

JWASM是WASM的进阶版. 它和MASM语法完全兼容, 包括更高级的宏和高级指令. 如果需要使用MASM语法的话, JWASM不错. 

> TASM
> Borland Turbo Assembler is included with CodeGear C++ Builder. It is compatible with
MASM syntax except for some newer syntax additions. TASM is no longer maintained but is
still available. It is obsolete and does not support current instruction sets.

<u>TASM</u>

CodeGear C++ Builder带有Borland Turbo 汇编器. 除了几处新的语法之外它和MASM完全兼容. TASM不再继续维护, 但还是可用的. TASM已经被淘汰了, 而且不支持现在的指令集了. 

> GOASM

> GoAsm is a free assembler for 32- and 64-bits Windows including resource compiler, linker
and debugger. The syntax is similar to MASM but not fully compatible. It is currently not up
to date with the latest instruction sets. An integrated development environment (IDE) named
Easy Code is also available.

<u>GOASM</u>

GoAsm是32位64位Windows都可用的免费汇编器, 还包括源代码编译器, 链接器和调试器. 语法和MASM类似但不完全兼容. 它没有跟上最新的指令集. 一个叫做Easy Code的IDE可以使用. 

> HLA

> High Level Assembler is actually a high level language compiler that allows assembly-like
statements and produces assembly output. This was probably a good idea at the time it was
invented, but today where the best C++ compilers support intrinsic functions, I believe that
HLA is no longer needed.

<u>HLA</u>

高级汇编器(High Level Assembler)实际上是一个高级语言编译器, 它允许使用类汇编语言的语句并且声称汇编输出. 在发明HLA的时候这主意不错, 但是现在最好的C++编译器已经支持伪指令函数了, 我觉得没必要用HLA了. 

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

<u>内联汇编</u>
微软和英特尔的C++编译器支持使用MASM的语法子集进行内联汇编. 可以访问C++变量, 方法和**标签**, 插入到汇编代码里就行了. 这很容易, 但是它不支持C++寄存器变量. 见36页. 

Gnu编译器支持内联汇编, 可以访问Gnu汇编器在Intel和AT&T语法下的所有**指令(instructions)**和**命令(directives)**. 在汇编中访问C++变量则挺复杂的. 

Linux和Mac系统下的Intel编译器支持微软风格和Gnu风格的内联汇编. 

<注> 关于instructions和directives: directive用来指导编译器去工作，如像 org .0x000. 其中的org是 directive,指导编译器将接下来的代码放在地址为0x000的空间。 还有如 db 0xff, DW 0x36等等. 而instruction 是指令，编译器将其转化成对应的机器码，然后CPU可以识别并执行这些机器码，如 mov P1,0xff. ADD R3,56等地址0000h~FFFFh是用十六进制表示的，计算机中的1K=2的10次方。0xffff行于 64个2的10次方。


> **Intrinsic functions** in C++
This is the newest and most convenient way of combining low-level and high-level code.
Intrinsic functions are high-level language representatives of machine instructions. For
example, you can do a vector addition in C++ by calling the intrinsic function that is
equivalent to an assembly instruction for vector addition. Furthermore, it is possible to
define a **vector class** with an overloaded + operator so that a vector addition is obtained
simply by writing +. Intrinsic functions are supported by Microsoft, Intel and Gnu compilers.
See page 34 and manual 1: "Optimizing software in C++".

<u>C++的**伪指令函数**</u>

这是最新的也是最便利的结合底层代码和高级语言的方式. **伪指令函数**是以高级语言面貌出现的机器指令. 比如你可以在C++中调用伪指令函数来做向量加法, 这和使用向量相加的汇编指令是等价的. 更进一步, 可以定义**向量类** 的重载运算符+, 这样向量的加法就可以简单第写成+. 微软, 英特尔和Gnu的编译器都支持为伪指令函数. 见34页和手册1: "优化C++软件". 

> Which assembler to choose?
In most cases, the easiest solution is to use intrinsic functions in C++ code. The compiler
can take care of most of the optimization so that the programmer can concentrate on
choosing the best algorithm and organizing the data into vectors. System programmers can
access system instructions by using intrinsic functions without having to use assembly
language.

<u>选择哪一个汇编器?</u>

多数情况下最简单的解决方案是在C++代码里使用伪指令函数. 编译器会做大部分优化, 程序员只需专心选择最好的算法并组织向量中的数据就好了. 系统程序员可以使用伪指令函数访问系统指令, 不需要使用汇编语言. 

> Where real low-level programming is needed, such as in highly optimized function libraries
or device drivers, you may use an assembler.
It may be preferred to use an assembler that is compatible with the C++ compiler you are
using. This allows you to use the compiler for translating C++ to assembly, optimize the
assembly code further, and then assemble it. If the assembler is not compatible with the
syntax generated by the compiler then you may generate an object file with the compiler
and disassemble the object file to the assembly syntax you need. The objconv disassembler
supports several different syntax dialects.

真正需要底层开发时, 例如高度优化的函数库或者设备驱动, 你可能需要用到汇编器. 

可能选择跟你在用的C++编译器兼容的汇编器好一些. 这样你就可以先用编译器把C++翻译成汇编代码, 进一步优化汇编代码, 然后使用汇编器. 如果汇编器和编译器生成的汇编代码不兼容的话, 你就得用编译器生成对象文件(*.obj), 再反汇编成你需要的汇编语法. objconv反汇编器支持集中不同的语法方言. 

> The NASM assembler is a good choice for many purposes because it supports many
platforms and object file formats, it is well maintained, and usually up to date with the latest
instruction sets.
The examples in this manual use MASM syntax, unless otherwise noted. The MASM syntax
is described in Microsoft Macro Assembler Reference at msdn.microsoft.com.
See www.agner.org/optimize for links to various syntax manuals, coding manuals and
discussion forums.

很多情况下NASM汇编器是一个合适的选择, 它支持多平台和对象文件格式, 维护的不错, 通常跟的上最新的指令集. 

除非特别说明, 这本手册的例子使用MASM语法. MASM语法在 msdn.microsoft.com 的微软Macro汇编器文档中有讲. 

www.agner.org/optimize 有各种语法手册, 编码手册和论坛的链接. 

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

## 3.2 寄存器集和基本指令

<u>16位模式寄存器</u>

**通用寄存器(General Purpose)和整形寄存器**

| 全寄存器(Full Register) | 部分寄存器(Partial register) | 部分寄存器(Partial register)|
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

|表 3.1. 16位模式下的通用寄存器.|
|:----------:|

> The 32-bit registers are also available in 16-bit mode if supported by the microprocessor
and operating system. The high word of ESP should not be used because it is not saved
during interrupts.

微处理器和操作系统支持的话, 32位寄存器在16位模式下也可用. `ESP`的高位字不要用, 在跳转时它不会被保存. 

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

**浮点寄存器(Floating Point Registers)**

|全寄存器|
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

|表 3.2. 浮点栈寄存器|
|:--------|

微处理器支持的话MMX寄存器也可以用. 微处理器和操作系统支持的话XMM寄存器可以使用. 

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

**段寄存器**

|全寄存器|
|:---------:|
|0-15位
|CS
|DS
|ES
|SS

|表 3.3. 16位模式下的段寄存器|
|:--------:|

FS和GS寄存器可能可用. 

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

**32位模式下的寄存器**

<u>通用寄存器和整形寄存器</u>

|全寄存器 0-31位|部分寄存器 0-15位|部分寄存器 8-15位|部分寄存器 0-7位
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

|表 3.4. 32位模式下的通用寄存器|
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

**浮点和64位向量寄存器**

|全寄存器0-79位|部分寄存器0-63位|
|:--------:|:--------:|
|ST(0) |MM0
|ST(1) |MM1
|ST(2) |MM2
|ST(3) |MM3
|ST(4) |MM4
|ST(5) |MM5
|ST(6) |MM6
|ST(7) |MM7

|表 3.5. 浮点和MMX寄存器|
|:-----:|

MMX寄存器只在微处理器支持的情况下可用. ST和MMX寄存器在同一块代码里不可混用. 使用MMX寄存器的代码要调用`EMMS`指令来和后续的使用ST寄存器部分划清界限. 

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

**128和256位整形向量和浮点向量寄存器**

|全寄存器0-255位 |全寄存器或部分寄存器0-127位|
|:----:|:----:|
|YMM0 |XMM0
|YMM1 |XMM1
|YMM2 |XMM2
|YMM3 |XMM3
|YMM4 |XMM4
|YMM5 |XMM5
|YMM6 |XMM6
|YMM7 |XMM7

|表 3.6. 32位模式下的XMM寄存器和YMM寄存器|
|:----:|

只有在微处理器和操作系统都支持的情况下XMM寄存器可用. 浮点向量的指令在单精度和双精度下分别只使用32位或64位的XMM寄存器. **(Scalar floating point instructions use only 32 or 64 bits of the XMM registers for single or double precision, respectively) 这句话是说XMM寄存器还分32位和64位的, 还是说只有寄存器中的一部分被使用了?** YMM寄存器只在处理器和操作系统都支持AVX指令集的情况下可用. 

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

**段寄存器**

|全寄存器0-15位|
|:----:|
|CS
|DS
|ES
|FS
|GS
|SS

|表 3.7. 32位模式下的段寄存器|
|:----:|