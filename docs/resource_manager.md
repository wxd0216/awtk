#AWTK中的资源管理

## 基本架构

这里的资源管理器并非Windows下的文件浏览器，而是负责对各种资源，比如字体、主题、图片、界面数据、字符串和其它数据的进行集中管理的组件。引入资源管理器的目的有以下几个：

* 让上层不需要了解存储的方式。在没有文件系统时或者内存紧缺时，把资源转成常量数组直接编译到代码中。在有文件系统而且内存充足时，资源放在文件系统中。在有网络时，资源也可以存放在服务器上(暂未实现)。资源管理器为上层提供统一的接口，让上层而不用关心底层的存储方式。

* 让上层不需要了解资源的具体格式。比如一个名为earth的图片，没有文件系统或内存紧缺，图片直接用位图数据格式存在ROM中，而有文件系统时，则用PNG格式存放在文件系统中。资源管理器让上层不需要关心图片的格式，访问时指定图片的名称即可(不用指定扩展名)。

* 让上层不需要了解屏幕的密度。不同的屏幕密度下需要加载不同的图片，比如MacPro的Retina屏就需要用双倍解析度的图片，否则就出现界面模糊。AWTK以后会支持PC软件和手机软件的开发，所以资源管理器需要为此提供支持，让上层不需关心屏幕的密度。

* 对资源进行内存缓存。不同类型的资源使用方式是不一样的，比如字体和主题加载之后会一直使用，UI文件在生成界面之后就暂时不需要了，PNG文件解码之后就只需要保留解码的位图数据即可。资源管理器配合图片管理器等其它组件实现资源的自动缓存。

负责资源管理器和资源管理相关的组件如下图所示：

![](images/resource_manager.png)

> Language和网络加载暂未实现。

## 资源的生成

AWTK中的资源需要进行格式转换才能使用：

* 在没有文件系统时或者内存紧缺时，需要把资源转成常量数组直接编译到代码中。
* XML格式的UI文件需要转换成二进制的格式。
* XML格式的主题文件需要转换成二进制的格式。
* TTF可以根据需要转换成位图字体。
* PNG可以根据需要转换成位图图片。

相关工具：

* bin/fontgen 位图字体生成工具
* bin/imagegen 位图图片生成工具
* bin/resgen 二进制文件生成资源常量数组
* bin/themegen XML主题转换成二进制的主题
* bin/xml\_to\_ui XML的界面描述格式转换二进制的界面描述格式
* update\_res.py 批量转换整个项目的资源

## 初始化

将资源生成常量数组直接编译到代码中时，其初始化过程为：

* 1.包含相应的数据文件。

```
#include "res/inc/fonts/ap.data"
#include "res/inc/fonts/default.data"
#include "res/inc/fonts/default_ttf.data"
#include "res/inc/images/bricks.data"
#include "res/inc/images/checked.data"
...
```

* 2.将资源增加到资源管理器中。

```
  resource_manager_add(rm, font_ap);
  resource_manager_add(rm, font_default);
  resource_manager_add(rm, font_default_ttf);
  resource_manager_add(rm, image_bricks);
  resource_manager_add(rm, image_checked);
  ...
```

将资源放在文件系统中时，一般不需要特殊处理，不过可以用resource_manager_load预加载资源。如：

```
  resource_manager_load(rm, RESOURCE_TYPE_THEME, "default");
  resource_manager_load(rm, RESOURCE_TYPE_FONT, "default_ttf");
```

> 参考：demos/resource.c

## 使用方法

* 加载图片图片

使用image_manager_load，指定图片的名称即可。

```
  bitmap_t img;
  image_manager_load(image_manager(), "earth", &img);
```

* 使用UI数据

使用window_open，指定资源的名称即可。如：

```
widget_t* win = window_open(name);
```

* 使用字体

一般在主题文件中指定字体即可。

* 使用主题

一般在界面描述文件中指定style即可。





