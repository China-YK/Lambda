#include<iostream>
#include<string>
using namespace std;
int main() {
    int x=5;
    int y=6;
    string name="yk";
    auto f = [=]() {//隐式捕获，按值捕获Lambda表达式之前的所有变量
        cout<<x<<endl;
        cout<<y<<endl;
        cout<<name<<endl;
    };
    f();

    auto g = []() {//什么都不写，Lambda只能用自己内部定义的变量
        int x=10;
        cout<<x<<endl;
    };
    g();

    auto k = [x]() {//按值捕获（复制一份外部变量的副本到闭包对象里），在Lanmbda表达式里面对x只读
        cout<<x<<endl;
    };
    k();

    auto l = [x,y]() {//选择x，y两个变量按值捕获
        cout<<x<<endl;
        cout<<y<<endl;
    };
    l();

    auto r = [&x]() {//对x进行按引用捕获，可以在Lambda表达式内部修改x的值了
        //这里我们没有加mutable关键字，所有底层应该还是operator()(int &x)const，为什么可以修改值呢？
        //因为对于常函数（成员函数加const），它的限制只针对于成员变量，而这里的x是引用捕获，本质上是外部变量
        //所以可以修改x的值
        x+=10;
    };
    r();
    cout<<x<<endl;

    auto s = [x]()mutable {//按值捕获，但是加了mutable关键字，可以修改捕获列表的参数了
        //但是这里和按引用捕获不一样，因为这里是按值捕获，会拷贝一份副本作为Lambda类的成员变量
        //所以哪怕这里x+=20，也是对这个成员变量进行操作的，并不会影响到外面的x
        x+=20;
    };
    s();
    cout<<x<<endl;//x的值不会有变化！

    auto t = [x,&y]()mutable {//可以按照不同的方式捕获多个变量
        y+=10;
    };
    t();
    cout<<y<<endl;

    auto u = [&]() {//隐式捕获，按引用捕获Lambda表达式之前的所有变量
        x+=20;
        y+=20;
    };
    u();
    cout<<x<<endl;
    cout<<y<<endl;

    auto v = [&,y]() {//混合捕获，可以指定默认捕获方式，再对个别变量例外
        //除了y是按值捕获，其余的都是按引用捕获
        x+=30;
        // y+=10;
    };
    v();
    cout<<x<<endl;
    return 0;
}


// class Lambda {
//     int x;
//     Lambda(int x):x(x){}
//     void operator() () const {
//         cout<<x<<endl;
//     }
// };
//
// Lambda f(5);
// f();