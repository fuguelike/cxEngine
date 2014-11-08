cxEngine V2.0
------
support json object

2D game engine,base openGL ES 2.0

support android ios

Email: cxuhua@gmail.com

QQ:4575407

使用简介
------

定义一个类型
------
```
CX_DEF(typeName, baseType)
    //定义字段
    int filed;
CX_END(typeName, baseType)
```
实现一个类型
------
```
CX_TYPE(typeName, baseType)
{
    //当类型被注册时调用
}
CX_INIT(typeName, baseType)
{
    //当对象被初始化
}
CX_FREE(typeName, baseType)
{
    //当对象被销毁时
}
CX_TERM(typeName, baseType)
```
创建对象,对象使用引用计数内存管理
------
```
CX_ADD (append event)添加一个事件
CX_SET (method set)重写一个方法
CX_CON (link signal)连接到信号

cxAny object1 = CX_CREATE(typeName); //对象将放入自动释放池

cxAny object2 = CX_ALLOC(typeName);//对象不放入自动释放池，需要 CX_RELEASE释放

CX_RETAIN(object1); //引用计数＋1

CX_RELEASE(object2); //引用计数-1,当引用计数＝0时对象降被释放

CX_AUTO(object2);//加入自动释放池

CX_TYPE_NAME(obj);//获取对象的字符串类型

CX_TYPE_OF(type,obj);强制转换对象类型，这里不检测类型

CX_TYPE_REG(type);//注册一个类型到系统中，如果没有注册系统将不承认这个类型

CX_RETAIN_SWAP(old,new);//释放旧的变量值，设置新的变量并retain对象

CX_RETAIN_SET(f,v);//将字段f设置为v，并retain对象

CX_INSTANCE_OF(obj,type);//判断对象类型

//断言只在DEBUG版本有效,如果需要判断对象类型，请试用CX_INSTANCE_OF,但会带来性能上的损失

CX_ASSERT_VALUE(obj,type,name) 断言obj类型，并生成type类型的name本地变量

CX_ASSERT_TYPE(obj,type);//断言一个对象的类型

CX_ASSERT_THIS(obj,type);//断言一个对象的类型,并生成本地 this变量
//
```

```

$从语言文件中获取数据
例如:
$ui.json?title 从语言文件strings/zh-CN/ui.json中获取key=title的数据

$.获取当前对象json属性，当前对象不存在从全局获取
$.WinSize

@对纹理缓存进行分组处理
例如:
@group:texture.png(?xxx) 将会把texture.png纹理缓存到group中
```


