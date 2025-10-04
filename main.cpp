// #include<iostream>
// #include<string>
// using namespace std;
// int main() {
//     int x=5;
//     int y=6;
//     string name="yk";
//     auto f = [=]() {//隐式捕获，按值捕获Lambda表达式之前的所有变量
//         cout<<x<<endl;
//         cout<<y<<endl;
//         cout<<name<<endl;
//     };
//     f();
//
//     auto g = []() {//什么都不写，Lambda只能用自己内部定义的变量
//         int x=10;
//         cout<<x<<endl;
//     };
//     g();
//
//     auto k = [x]() {//按值捕获（复制一份外部变量的副本到闭包对象里），在Lanmbda表达式里面对x只读
//         cout<<x<<endl;
//     };
//     k();
//
//     auto l = [x,y]() {//选择x，y两个变量按值捕获
//         cout<<x<<endl;
//         cout<<y<<endl;
//     };
//     l();
//
//     auto r = [&x]() {//对x进行按引用捕获，可以在Lambda表达式内部修改x的值了
//         //这里我们没有加mutable关键字，所有底层应该还是operator()(int &x)const，为什么可以修改值呢？
//         //因为对于常函数（成员函数加const），它的限制只针对于成员变量，而这里的x是引用捕获，本质上是外部变量
//         //所以可以修改x的值
//         x+=10;
//     };
//     r();
//     cout<<x<<endl;
//
//     auto s = [x]()mutable {//按值捕获，但是加了mutable关键字，可以修改捕获列表的参数了
//         //但是这里和按引用捕获不一样，因为这里是按值捕获，会拷贝一份副本作为Lambda类的成员变量
//         //所以哪怕这里x+=20，也是对这个成员变量进行操作的，并不会影响到外面的x
//         x+=20;
//     };
//     s();
//     cout<<x<<endl;//x的值不会有变化！
//
//     auto t = [x,&y]()mutable {//可以按照不同的方式捕获多个变量
//         y+=10;
//     };
//     t();
//     cout<<y<<endl;
//
//     auto u = [&]() {//隐式捕获，按引用捕获Lambda表达式之前的所有变量
//         x+=20;
//         y+=20;
//     };
//     u();
//     cout<<x<<endl;
//     cout<<y<<endl;
//
//     auto v = [&,y]() {//混合捕获，可以指定默认捕获方式，再对个别变量例外
//         //除了y是按值捕获，其余的都是按引用捕获
//         x+=30;
//         // y+=10;
//     };
//     v();
//     cout<<x<<endl;
//     return 0;
// }


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


#include <iostream>
#include <string>
using namespace std;
class MyClass {
    public:
        int age;
        string name;

        MyClass() {cout<<"wucan"<<endl;}
        MyClass(int age,string name):age(age),name(move(name)) {
            //这里调用move完全没问题
            //构造函数体内的 name 只是一个局部变量，生命周期只在构造函数内有效，执行完构造函数就会销毁。
            //所以：我们完全可以“ 掏空”它（move 走它的内部资源），不会影响调用者看到的对象
            cout << "youcan" << endl;
        }

        MyClass(const MyClass &obj): age(obj.age), name(obj.name) {
            //我靠，我这个错误以前一直没发现，还以为自己很专业呢md。
            //name(move(name))，本来对于这个string类想调用拷贝构造呢，提高效率
            //但是形参是const MyClass &obj类型！！！
            //也就是说obj.name其实是const string 类型，而move之后变成const string &&类型
            //string 的 移动构造函数 接收的是 string&&，不能接收 const string&&。
            //所以这里调用的不是移动构造，而是拷贝构造（对于name这个成员）！
            //所以这里别加move，现代C++风格，拷贝就是拷贝，老老实实完成复制，同时不会修改原对象（const不能省略）
            cout << "kaobei" << endl;
        }

        MyClass(MyClass &&obj) {
            age = obj.age;
            name = move(obj.name);
            cout<<"yidong"<<endl;
        }
        ~MyClass(){cout<<"xigou"<<endl;}
};
int main() {
    int x=5;
    auto f=[y=x+3.5](){cout<<y<<endl;};//初始化捕获，在C++14可以直接在捕获列表里初始化变量
    f();

    //测试自定义拷贝构造+move会怎么样
    // MyClass myObj(18,"TOM");
    // MyClass myObj2=myObj;
    // cout<<myObj2.name<<endl;
    // cout<<myObj.name<<endl;



    // auto f1 = [&myObj]() {//当然捕获列表里面也可以捕获自定义类对象，按值捕获，按引用捕获
    //     cout<<myObj.age<<endl;
    //     cout<<myObj.name<<endl;
    //     myObj.age+=1;
    // };
    // f1();
    //
    // auto f2 = [temp = myObj]() {//按值捕获，会调用拷贝构造，temp是MyClass类型
    //     cout<<temp.age<<endl;
    //     // temp.age=18;
    // };
    // f2();
    //
    // auto f3 = [temp = &myObj]() {//按引用捕获，不会调用任何构造函数，temp是MyClass*类型
    //     cout<<temp->age<<endl;
    // };
    // f3();
    //
    // auto f4 = [temp = move(myObj)]() {//
    //     cout<<temp.age<<endl;
    // };
    // f4();

    return 0;
}