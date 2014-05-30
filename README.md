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
CX_OBJECT_DEF(typeName, baseType)
    //定义字段
    int filed;
CX_OBJECT_END(typeName, baseType)
```
实现一个类型
------
```
CX_OBJECT_TYPE(typeName, baseType)
{
    //当类型被注册时调用
}
CX_OBJECT_INIT(typeName, baseType)
{
    //当对象被初始化
}
CX_OBJECT_FREE(typeName, baseType)
{
    //当对象被销毁时
}
CX_OBJECT_TERM(typeName, baseType)
```
创建对象,对象使用引用计数内存管理
------
```
cxAny object1 = CX_CREATE(typeName); //对象将放入自动释放池

cxAny object2 = CX_ALLOC(typeName);//对象不放入自动释放池，需要 CX_RELEASE释放

CX_RETAIN(object1); //引用计数＋1

CX_RELEASE(object2); //引用计数-1,当引用计数＝0时对象降被释放
//
```
