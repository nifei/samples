灰度图的几种算法资料

第一篇原文在此, 没有完全翻译, 有些作者的吐槽什么的就略过了, 指甲剪狠了, 打不了几个字. 
http://www.tannerhelland.com/3643/

# Seven grayscale conversion algorithms (with pseudocode and VB6 source code)

## 灰度图 - 介绍

黑白照片(http://en.wikipedia.org/wiki/Monochrome_photography)可以追溯到19世纪中期以前. 即使有了彩色照片黑白照片也还是很流行. 数字革命使黑白照片更流行了因为任何一台数码相机都可以拍摄黑白照片(模拟相机需要特殊胶片). 黑白摄影有时被认为是摄影艺术中的"雕塑". 它抽象主题, 使得拍摄者专注于形式和解释(form and interpretation, 这是艺术术语吗? )而不是简单的复制真实场景. 

因为"黑白"这样的术语并不精准 - 黑白照片其实是有很多不同的灰色构成的 - 本文将这样的图片称为"灰度图"(大家都是这么叫的好么). 

我还要用到几个术语. 一个是"color space". color space是可视化所有颜色的方法. 不一样的颜色表达有不一样的color space. RGB color space(http://en.wikipedia.org/wiki/RGB_color_space)是一个立方体, HSL (http://en.wikipedia.org/wiki/HSL_color_space)是锥形, 或者两个锥形, 或者圆柱形, YIQ和YPbPr有更多的抽象形状. 本文讨论前两个. 

还会用到颜色通道(color chanel, http://en.wikipedia.org/wiki/Color_channel). 多数数字图像使用3个颜色通道: 红绿蓝. 把三个颜色通道的图像叠加在一起就是原图了. 不同的颜色模型有不同的通道(有时是颜色, 有时是亮度, 饱和度之类的), 本文着重于RGB通道. 

## 灰度图算法基本上都是这么算的

所有的灰度图算法都有下面散步: 

1. 取得像素RGB分量
2. 使用奇妙的数学公式把三个颜色分量编程一个灰度值
3. 把RGB三个颜色分量都换成这个灰度值

描述灰度图算法时都是在讲第二步 - 都是数学. 所以你看到这样的公式时:

<pre><code>
Gray = (Red + Green + Blue) / 3
</code></pre>

要知道这个算法实现起来是这样的: 

<pre><code>
For Each Pixel in Image {

   Red = Pixel.Red
   Green = Pixel.Green
   Blue = Pixel.Blue

   Gray = (Red + Green + Blue) / 3

   Pixel.Red = Gray
   Pixel.Green = Gray
   Pixel.Blue = Gray

}
</code></pre>

## 原图

![alt text](http://www.tannerhelland.com/wp-content/uploads/Secret_of_Monkey_Island-600x375.jpg "This bright, colorful promo art for The Secret of Monkey Island: Special Edition will be used to demonstrate each of our seven unique grayscale algorithms.")
This bright, colorful promo art for The Secret of Monkey Island: Special Edition will be used to demonstrate each of our seven unique grayscale algorithms.

## 方法一 - Averaging (平均算法, 又快又差)

![alt text](http://www.tannerhelland.com/wp-content/uploads/grayscale_average-600x375.png "Grayscale image generated from the formula: Average(Red, Green, Blue).")
Grayscale image generated from the formula: Average(Red, Green, Blue)

"Averaging"是用的最多的灰度图算法, 算法这样: 

<pre><code>
Gray = (Red + Green + Blue) / 3
</code></pre>

简单, 快 - 新手的不二之选. 这个公式的灰度图也挺好看的, 实现简单易于优化(快查表效果不错). 缺点呢 - 虽然又快又容易, 它在用灰度对阴影的表现和人眼感知明度(luminosity, brightness)的方式上做的不好. 所以我们需要其他的方法. 

## 方法二 - Correcting for the human eye人眼矫正

(也叫"luma"或者"luminance", 但是这个术语并不精准<http://poynton.com/PDFs/YUV_and_luminance_harmful.pdf>)

![alt text](http://www.tannerhelland.com/3643/grayscale-image-algorithm-vb6/grayscale_humaneye/ "Grayscale generated using a formula similar to (Red * 0.3 + Green * 0.59 + Blue * 0.11)")
Grayscale generated using a formula similar to (Red * 0.3 + Green * 0.59 + Blue * 0.11)

肉眼不太看得出两张图的区别. 看下下面的图, 方法一"Average method"用在上边, 方法二用在下边: 

![alt text](http://www.tannerhelland.com/wp-content/uploads/grayscale_average_and_humaneye-600x375.png "If you look closely, you can see a horizontal line running across the center of the image. The top half (the average method) is more washed-out than the bottom half. This is especially visible in the middle-left segment of the image, beneath the cheekbone of the background skull.")
If you look closely, you can see a horizontal line running across the center of the image. The top half (the average method) is more washed-out than the bottom half. This is especially visible in the middle-left segment of the image, beneath the cheekbone of the background skull.

用文末附带的源码可以看得更清楚. 

方法二利用了人眼锥形体密度对不同颜色不一样的原理. 人眼对绿色的感知强过红色, 红色强过蓝色. 这一点是真的 - 解释了一通. 

因为人眼对颜色的感知不平均, "平均法"就不准确了. 好的灰度算法会根据人眼对每个颜色的感知来权衡颜色, 而不是平均分配. 下面是个常用的公式(Photoshop, GIMP): 

<pre><code>
Gray = (Red * 0.3 + Green * 0.59 + Blue * 0.11)
</code></pre>

很惊讶三种颜色权重差距如此之大吧? 这个公式计算量大一点, 但是生成的灰度图更有动感. 看源代码. 

值得一提的是, 哪个灰度图公式是最好的并无定论. 在源码中我使用原始的ITU-R的推荐(BT.709), 历史的先例. 我用的公式这样子, 叫做"Luma": 

<pre><code>
Gray = (Red * 0.2126 + Green * 0.7152 + Blue * 0.0722)
</code></pre>

有些现代数字图像和视频格式使用BT.601, 在系数上有一点不同:

<pre><code>
Gray = (Red * 0.299 + Green * 0.587 + Blue * 0.114)
</code></pre>

本文不讨论哪种更好. 拓展阅读推荐The work of Charles Poynton (http://poynton.com/). 对99%的程序员来说二者的区别无关紧要. 哪个都比"平均法"强. 

## 方法三 - Desaturation (冲淡颜色, 我更愿意叫成去饱和度)

![alt text](http://www.tannerhelland.com/wp-content/uploads/grayscale_desaturate-600x375.png "A desaturated image. Desaturating an image takes advantage of the ability to treat the (R, G, B) colorspace as a 3-dimensional cube. Desaturation approximates a luminance value for each pixel by choosing a corresponding point on the neutral axis of the cube.")
A desaturated image. Desaturating an image takes advantage of the ability to treat the (R, G, B) colorspace as a 3-dimensional cube. Desaturation approximates a luminance value for each pixel by choosing a corresponding point on the neutral axis of the cube.

描述一个像素颜色的方式有多种. 多数程序员使用RGB颜色模型. 对机器来说这种描述方法很好但是对人类来说很难具象. 一些话引入HSL模型. HSL表示hue(颜色), saturation(饱和度)和lightness(亮度). 见维基百科. 

对图片Desaturation要先把RGB颜色转换成HSL, 然后把饱和度置为0. 简单来讲就是把一个颜色的饱和度减到最小. 推导很复杂, 结论很简单. 要把一个像素去饱和度只需要取Max(R,G,B)和Min(R,G,B)的平均值就好了. 

<pre><code>
Gray = ( Max(Red, Green, Blue) + Min(Red, Green, Blue) ) / 2
</code></pre>

维基百科会告诉你为什么. 

Desaturation会生成一个单调柔和的灰度图. 和方法二的人眼矫正图对比, 你会注意到二者的对比图不同. 方法二更像Ansel Adams的照片<http://www.anseladams.com/category_s/71.htm>, 去饱和度更像便宜的傻瓜相机拍出来的. 目前三种方法Desaturation的图像最单调(低对比度)也最黑. 

## 方法四 - Decomposition (解构)

![alt text](http://www.tannerhelland.com/wp-content/uploads/grayscale_decompose_max-600x375.png "Decomposition using maximum values")
Decomposition using maximum values

![alt text](http://www.tannerhelland.com/3643/grayscale-image-algorithm-vb6/grayscale_decompose_min/ "Decomposition using minimum values")
Decomposition using minimum values

解构图像可以看做去饱和度的简单版. 解构图像, 我们取每个像素的灰度值位三个颜色通道的最大或最小值. 注意是每个像素, 我不知道这里作者有什么好解释的说了好多. 

最大值解构: 

<pre><code>
Gray = Max(Red, Green, Blue)
</code></pre>

最小值解构: 

<pre><code>
Gray = Min(Red, Green, Blue)
</code></pre>



![alt text](link "title")
title
