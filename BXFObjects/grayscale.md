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

<pre><code
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

看看算法:

[id]: http://www.tannerhelland.com/wp-content/uploads/Secret_of_Monkey_Island-600x375.jpg  "This bright, colorful promo art for The Secret of Monkey Island: Special Edition will be used to demonstrate each of our seven unique grayscale algorithms."