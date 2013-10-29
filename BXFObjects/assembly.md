原文在这里: http://www.agner.org/optimize/

# 1 介绍
这部手册是如下系列的第二部:

1. 优化c++软件: Windows, Linux 和 Mac 平台上的优化指南. 

2. 优化汇编语言的子程序: X86 平台的优化指南. 

3. Intel, AMD 和 VIA CPU 的微结构(*microarchicture*): 汇编程序员和编译器作者的指南. 

4. 指令表: Intel, AMD 和 VIA CPU 的指令背后的潜在操作, 数据吞吐量和指令操作分解. 

5. 不同 C++ 编译器和操作系统下的调用惯例. 

这些手册的最新版本在这里: http://www.agner.org/optimize/. 
版权说明在164页. 

这本手册解释了怎样把汇编代码和高级编程语言结合起来以及怎样使用汇编代码来优化 CPU 密集型代码的速度. 

这本手册面向高级汇编程序员和编译器作者. 读者最好已经了解汇编语言并且有一些汇编编程的经验. 建议初学者使用指南里介绍的优化技巧之前先搜索相关知识并获取一些编程经验. 我可以推荐一大堆互联网上的介绍, 手册, 论坛和讨论组 (在 http://www.agner.org/optimize/ 有列) 以及 R. C. Detmer 在2006年完成的这本书 "Introduction to 80x86 Assembly Language and Computer Archicture", 第二版. 

这本手册囊括了使用 x86  和 x86-64 指令集的所有平台. 大多数 Intel, AMD 和 VIA 的微处理器都使用这套指令集. 可以使用这套指令集的操作系统包括 DOS, Windows, Linux, FreeBSD/OpenBSD 和 Intel-Based Mac OS. 手册包含了最新的微处理器和指令集. 手册3和4介绍了个别微处理器模型的细节. 

比汇编语言更普适的优化技巧在手册1: "C++软件优化" 中有介绍. 个别微处理器的细节在手册3: "Intel, AMD 和 VIA CPU 的微结构" 有介绍. 指令表耗时(*tables of instruction timings*)等问题在手册4: "指令表: Intel, AMD 和 VIA CPU 的指令背后的潜在操作, 数据吞吐量和指令操作分解" 有介绍. 不同操作系统和编译器下的调用惯例在手册5: "不同 C++ 编译器和操作系统下的调用惯例" 中有讲. 

汇编语言变成比高级语言编程难多了. 犯错误很容易, 调试很难. 警告! 不要把你的程序问题发给我. 我不回复这类邮件. 如果在相关书籍和手册中找不到你要的答案的话, 互联网上多的是能解答你编程问题的论坛. 

纳秒征途好运!

## 1.1 使用汇编的理由
汇编语言如今不像过去用的那么多了. 但是还是有理由学习和使用它的. 主要是: 

1. 为了学习. 知道微处理器和编译器在指令层如何工作是很重要的, 这样我们就可以推测什么样的编程技巧最高效, 理解各种结构在高级语言中怎么工作的, 还可以追踪奇怪的错误. 

2. 为了调试和定位(verifying). 在查找错误和查看编译器能把一段特定代码优化成什么样的时候, 查看编译器生成的汇编代码或者调试器中的反汇编窗口很有帮助. 

3. 写编译器. 在开发编译器, 调试器和其他开发工具的时候, 理解汇编编程技巧是必需的.

4. 嵌入式系统. 小型嵌入式系统的资源和主机都比PC少. 为追求速度或体积在小型嵌入式系统上有必要使用汇编编程. 

5. 硬件驱动和系统编码. 使用高级编程语言访问硬件, 系统控制寄存器等有时很困难或者做不到. 

6. 获取使用高级语言无法获取的指令. 有些汇编指令没有对等的高级语言语句. 

7. 自修正代码. 自修正代码有时候因为干扰了高效率的代码缓存而显得不那么划算. 但它也可以变得有用, 例如在一个必需多次计算自定义方法的数学程序中包含一个小型编译器. 

8. 为空间优化代码. 如今的存储空间和内存如此廉价, 为减少代码量使用汇编语言已不再值当. 但缓存尺寸仍然是一块关键的资源, 我们要让它适合代码缓存的大小, 在某些情况下, 为此优化一段关键代码的尺寸还是有用的. 

9. 为速度优化代码. 多数情况下当代的C++编译器把代码优化得很好. 但有些情况下编译器仍然表现很差, 这时候小心使用汇编编程可以在速度上获得惊人的优化. 

10. 函数库. 优化很多程序员都在用的函数库获利更多. 

11. 让函数库和多编译器, 多操作系统兼容. 让有多个输入的函数库和不同的编译器和操作系统兼容是可能的. 这需要使用汇编编程. 

这本手册主要讨论优化代码速度, 其他几个方面也会讲下. 

## 1.2 不使用汇编代码的理由

汇编编程有许多缺陷和问题, 建议决定使用汇编代码来完成特定需求之前考虑其他方案. 最重要的几条不适用汇编编程理由有: 

1. 开发时间. 使用汇编语言写代码比使用高级语言花时间多了. 

2. 可靠性和安全性. 使用汇编代码很容易犯错误. 汇编编译器不会检查你有没有遵守调用惯例和寄存器保存惯例. 没人管你在所有分支和路径中加起来的push和pop指令数目是不是相同. 汇编代码搞出潜在错误的方法很多, 除非你有一套系统的方法测试和验证, 项目的可靠性和安全性都会受到影响. 

3. 调试和验证. 汇编代码更难调试和验证因为出错的可能性比高级语言多. 

4. 可维护性. 汇编代码也更难修改和维护, 因为这门语言允许没有结构的乱糟糟的代码存在, 还允许你使用别人都看不懂的卑鄙的技巧. (你造吗, 我自己第二天都看不懂了, o(╯□╰)o) 你需要完善的文档和可持续编程风格. (啥? o(>﹏<)o)

5. 系统代码可以使用伪指令函数(intrinsic functions)代替指令集. 最好的当代C++编译器提供伪指令函数来访问系统控制寄存器和其他系统指令. 伪指令函数可用时, 设备驱动和其他系统代码不再需要汇编代码了. 

6. 应用代码可以使用伪指令函数或者向量类代替汇编. 最好的当代C++编译器提供伪指令代码来实现向量操作和其他之前需要汇编编程的特殊指令. 为了利用SIMD的优越性, 我们不再非使用老式汇编代码不可了. 见34页. 

7. 可移植性. 汇编代码的平台相关性强. 移植到不同的平台上有困难. 而使用伪指令函数来代替汇编的代码可以移植到所有x86和x86-64的平台上. 

8. 编译器已经进步很多了. 现在最好的编译器在大多数情况下已经比汇编程序员的平均水平干得还要好. 

9. 编译代码有可能比汇编代码还要快, 因为编译器可以做过程间(inter-procedural)优化和全局优化. 为了让代码可以被测试和验证, 汇编程序员必须遵循函数与调用之间的调用规则. 很多编译器使用的优化方法因此不能使用, 比如内联函数, 寄存器分布, 常量传递(constant propagation), 从函数中剔除通用子表达式, 函数之间的调度等等. 这些优点可以通过使用伪指令代码加上c++代码来代替汇编代码加以利用. 

## 1.3 Operating systems covered by this manual
The following operating systems can use x86 family microprocessors:
16 bit: DOS, Windows 3.x.
32 bit: Windows, Linux, FreeBSD, OpenBSD, NetBSD, Intel-based Mac OS X.
64 bit: Windows, Linux, FreeBSD, OpenBSD, NetBSD, Intel-based Mac OS X.

All the UNIX-like operating systems (Linux, BSD, Mac OS) use the same calling
conventions, with very few exceptions. Everything that is said in this manual about Linux
also applies to other UNIX-like systems, possibly including systems not mentioned here.

## 1.3 这本手册谈论的操作系统

以下操作系统可以使用x86家族的微处理器:
16位: Dos, Windows 3.x.
32位: Windows, Linux, FreeBSD, OpenBSD, NetBSD, Intel-based Mac OS X.
64位: Windows, Linux, FreeBSD, OpenBSD, NetBSD, Intel-based Mac OS X.

所有类Unix操作系统(Linux, BSD, Mac OS)使用一样的调用惯例, 鲜有例外. 这本手册里提到的Linux相关的信息也适用于其他类Unix系统, 有些没被提到的系统也适用. 

# 2 Before you start
## 2.1 Things to decide before you start programming
Before you start to program in assembly, you have to think about why you want to use
assembly language, which part of your program you need to make in assembly, and what
programming method to use. If you haven't made your development strategy clear, then you
will soon find yourself wasting time optimizing the wrong parts of the program, doing things
in assembly that could have been done in C++, attempting to optimize things that cannot be
optimized further, making spaghetti code that is difficult to maintain, and making code that is
full or errors and difficult to debug.
Here is a checklist of things to consider before you start programming:

# 2 开始之前

## 2.1 开始编程之前要决定的事情

使用汇编开始编程之前, 你得好好想想为什么你要使用汇编语言, 你的程序的哪部分需要使用汇编来完成, 以及使用什么样的编程方法. 要是开发策略还不明了, 很快你就会发现自己在浪费时间, 优化程序中错误的部分, 使用汇编来干本来可以用c++干的事儿, 尝试优化不可能再优化的东西, 写出一堆一堆难以维护的代码, 或者充斥着错误而且无法调试的代码. 

下面列出了开始编程之前需要考虑的几件事: 

* Never make the whole program in assembly. That is a waste of time. Assembly code
should be used only where speed is critical and where a significant improvement in
speed can be obtained. Most of the program should be made in C or C++. These are
the programming languages that are most easily combined with assembly code.

+ 不要用汇编写整个程序. 那会浪费时间. 只应该在追求速度而且可以取得大幅速度提升的地方使用汇编代码. 要用c或者c++实现大部分的程序. 它们是最容易和汇编代码混合使用的语言了. 

* If the purpose of using assembly is to make system code or use special instructions
that are not available in standard C++ then you should isolate the part of the
program that needs these instructions in a separate function or class with a well
defined functionality. Use intrinsic functions (see p. 34) if possible.

+ 如果使用汇编的目的是完成系统代码或者使用某些标准c++不提供的指令, 应使用函数或者功能完备的类把汇编代码和程序的其他部分隔离开来. 可能的话使用伪指令函数(见34页)

* If the purpose of using assembly is to optimize for speed then you have to identify
the part of the program that consumes the most CPU time, possibly with the use of a
**profiler**. Check if the bottleneck is file access, memory access, CPU instructions, or
something else, as described in manual 1: "Optimizing software in C++". Isolate the
critical part of the program into a function or class with a well-defined functionality.

+ 如果使用汇编的目的是优化速度就得先定位出程序的哪一部分最消耗cpu时间, 可能得用**profiler** .看看瓶颈是访问文件, 访问内存, CPU指令还是别的什么, 就像手册1: "优化c++软件" 中讲的那样. 把程序的关键部分用函数或者功能完备的类隔离出来.  

* If the purpose of using assembly is to make a function library then you should clearly
define the functionality of the library. Decide whether to make a function library or a
class library. Decide whether to use static linking (.lib in Windows, .a in Linux) or
dynamic linking (.dll in Windows, .so in Linux). Static linking is usually more
efficient, but dynamic linking may be necessary if the library is called from languages
such as C# and Visual Basic. You may possibly make both a static and a dynamic
link version of the library.

+ 如果使用汇编是为了完成函数库, 要清楚地定义库的功能. 先定下是要完成函数库还是类库. 再定下是要使用静态链接(Windows的.lib, Linux的.a)还是动态链接(Windows的.dll, Linux的.so). 静态链接更高效, 但是从C#或者VB调用的话只能用动态链接库了. 可能动态和静态的版本都要事先. 

* If the purpose of using assembly is to optimize an embedded application for size or
speed then find a development tool that supports both C/C++ and assembly and
make as much as possible in C or C++.

+ 如果使用汇编是为了优化嵌入式应用的空间和速度, 找个支持C/C++和汇编的开发工具, 并且尽可能用C/C++实现. 

* Decide if the code is reusable or application-specific. Spending time on careful
optimization is more justified if the code is reusable. A reusable code is most
appropriately implemented as a function library or class library.

+ 代码是可重用的还是应用独有的. 用心优化可重用代码更合理. 可重用代码最合适的实现方式是函数库或者类库. 

* Decide if the code should support multithreading. A multithreading application can
take advantage of microprocessors with multiple cores. **Any data that must be
preserved from one function call to the next on a per-thread basis should be stored
in a C++ class or a per-thread buffer supplied by the calling program**.

+ 代码是否支持多线程. 多线程应用可以使用多核微处理器. **在线程之内需要保存以函数调用之间传递的数据应该由调用程序保存在C++类或者线程缓冲区内**

* Decide if portability is important for your application. Should the application work in
both Windows, Linux and Intel-based Mac OS? Should it work in both 32 bit and 64
bit mode? Should it work on non-x86 platforms? This is important for the choice of
compiler, assembler and programming method.

+ 可移植性对应用软件是否重要. 软件要在Windows, Linux和Intel-based Mac OS下都工作吗? 32位和64位模式都要支持吗? 非x86平台呢? 这些事情对编译器的选择, 汇编编译器的选择和编程方法都很重要. 

* Decide if your application should work on old microprocessors. If so, then you may
make one version for microprocessors with, for example, the SSE2 instruction set,
and another version which is compatible with old microprocessors. You may even
make several versions, each optimized for a particular CPU. It is recommended to
make **automatic CPU dispatching** (see page 137).

+ 软件是否要支持旧的微处理器? 如果是的话, 你可能要实现一个SSE2指令集之类的版本给微处理器用, 另一个版本用来和旧微处理器兼容. 甚至可以为每个特殊的CPU都实现一个优化版本. 建议使用**自动CPU分配**(见137页). 

* There are three assembly programming methods to choose between: (1) Use
intrinsic functions and vector classes in a C++ compiler. (2) Use inline assembly in a
C++ compiler. (3) Use an assembler. These three methods and their relative
advantages and disadvantages are described in chapter 5, 6 and 7 respectively
(page 34, 36 and 45 respectively).

+ 有三种汇编编程方法可选: (1) 在C++编译器中使用伪指令函数和向量类. (2) 在C++编译器中使用内联汇编. (3)使用汇编编译器. 这三种方法和它们的优缺点在第5, 6, 7章讲述(见34, 36, 45页). 

* If you are using an assembler then you have to choose between different syntax
dialects. It may be preferred to use an assembler that is compatible with the
assembly code that your C++ compiler can generate.

+ 使用汇编编译器的话还要在不同的语法中选择. (作者?)倾向使用和你的C++编译器生成的汇编代码兼容的汇编编译器. 

* Make your code in C++ first and optimize it as much as you can, using the methods
described in manual 1: "Optimizing software in C++". Make the compiler translate
the code to assembly. Look at the compiler-generated code and see if there are any
possibilities for improvement in the code.

+ 先使用C++实现并且尽可能用手册1: "优化C++软件"中介绍的方法优化它. 让编译器把代码翻译成汇编代码. 看看编译器生成的汇编代码, 看看你的(C++)代码是否还有改进余地. 

* Highly optimized code tends to be very difficult to read and understand for others
and even for yourself when you get back to it after some time. In order to make it
possible to maintain the code, it is important that you organize it into small logical
units (procedures or macros) with a well-defined interface and calling convention and
appropriate comments. Decide on a consistent strategy for code comments and
documentation.

+ 高度优化的代码对其他人来说是很难阅读和理解的,即使你自己过段时间回来看也这样. 为了让代码可维护, 把汇编代码组织成小一些的逻辑单位(过程或者宏), 遵循良好的接口定义和调用规范, 适当的注释都是很重要的. 制定一个可持续化的注释和文档策略. 

* Save the compiler, assembler and all other development tools together with the
source code and project files for later maintenance. Compatible tools may not be
available in a few years when updates and modifications in the code are needed.

+ 保存编译器, 汇编编译器和所有开发工具, 连同源代码和项目文件, 以备来日维护可用. 几年之后需要更新和修改代码的时候, 兼容的工具可能就没有了. (纳尼?)

## 2.2 Make a test strategy
Assembly code is error prone, difficult to debug, difficult to make in a clearly structured way,
difficult to read, and difficult to maintain, as I have already mentioned. A consistent test
strategy can ameliorate some of these problems and save you a lot of time.

## 2.2 制定测试策略

正如前文所述, 汇编代码爱出错, 难调试, 难写得条理清晰, 难读, 还难维护. 持续的测试策略可以改善这些问题, 节省很多时间. 

My recommendation is to make the assembly code as an isolated module, function, class or
library with a well-defined interface to the calling program. Make it all in C++ first. Then
make a test program which can test all aspects of the code you want to optimize. It is easier
and safer to use a test program than to test the module in the final application.

我的建议是让汇编代码自成一体, 模块, 方法, 类或者接口对调用者易用的库. 先全用C++实现. 然后写一个覆盖所有待优化代码的测试程序. 使用测试程序和在最终产品中测试模块相比更安全也更容易. 

The test program has two purposes. The first purpose is to verify that the assembly code
works correctly in all situations. And the second purpose is to test the speed of the
assembly code without invoking the user interface, file access and other parts of the final
application program that may make the speed measurements less accurate and less
reproducible.

测试程序有两个目的. 其一是验证汇编代码在所有分支下都正常工作. 其二是测试汇编代码的速度时, 可以不启动界面, 不访问文件, 也不调用应用程序的其他部分, 这样测试结果更准确并且可重现. 

You should use the test program repeatedly after each step in the development process and
after each modification of the code.

开发过程中的每一步, 每一次对代码的修改之后, 都应该使用测试程序. 

Make sure the test program works correctly. It is quite common to spend a lot of time
looking for an error in the code under test when in fact the error is in the test program.
There are different test methods that can be used for verifying that the code works correctly.
A white box test supplies a carefully chosen series of different sets of input data to make
sure that all branches, paths and special cases in the code are tested. A black box test
supplies a series of random input data and verifies that the output is correct. A very long
series of random data from a good random number generator can sometimes find rarely
occurring errors that the white box test hasn't found.

保证测试程序正确地工作. 花很多时间根据测试结果在代码里面找错误, 结果实际上是测试程序出错了这样的事情也不少. 

有各种测试方法可以用来验证代码是否工作正常. 白盒测试提供一系列精挑细选的输入数据集合保证代码中的各个分支路径和特例都会被测试到. 黑盒测试提供一系列随机输入数据并验证输出结果是否正确. 优秀并且足够长的随机生成机制有时可以发现白盒测试发现不了的极小概率错误.

The test program may compare the output of the assembly code with the output of a C++
implementation to verify that it is correct. The test should cover all boundary cases and
preferably also illegal input data to see if the code generates the correct error responses.
The speed test should supply a realistic set of input data. A significant part of the CPU time
may be spent on branch mispredictions in code that contains a lot of branches. The amount
of branch mispredictions depends on the degree of randomness in the input data. You may
experiment with the degree of randomness in the input data to see how much it influences
the computation time, and then decide on a realistic degree of randomness that matches a
typical real application.

测试程序可以通过对比汇编代码输出和C++代码输出来验证是否正确. 测试应覆盖所有边界条件并且最好有一些非法输入来验证代码生成的错误反馈是否正确. 

速度测试应提供真实输入数据. 在包含很多分支的代码中, CPU的时间很大一部分可能消耗在错误的分支预测上. 错误的分支预测数量取决于输入数据的随机程度. 你可以先试验一下输入数据的随机程度对计算时间的影响, 然后定下一个符合典型真实应用(场景)的随机度. 

An automatic test program that supplies a long stream of test data will typically find more
errors and find them much faster than testing the code in the final application. A good test
program will find most errors, but you cannot be sure that it finds all errors. It is possible that
some errors show up only in combination with the final application.

提供长测试数据流的自动化测试程序通常比在最终软件中测试更多更快地找到错误. 好的测试程序可以找到大部分错误, 但是你不能保证它找到所有错误. 有可能有些错误只有在最终产品中(模块)结合起来时才会出现. 

2.3 Common coding pitfalls
The following list points out some of the most common programming errors in assembly
code.
1. Forgetting to save registers. Some registers have callee-save status, for example
 'EBX '. These registers must be saved in the prolog of a function and restored in the
epilog if they are modified inside the function. Remember that the order of `POP`
instructions must be the opposite of the order of 'PUSH' instructions. See page 28 for a
list of callee-save registers.

## 2.3 常见编程陷阱
下面列出了一些汇编编程时最经常犯的错误.
1. 忘记保存寄存器. 有些寄存器保存调用者状态, 例如 `EBX` . 