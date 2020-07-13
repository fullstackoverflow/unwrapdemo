#include <napi.h>

class A : public Napi::ObjectWrap<A>
{
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    A(const Napi::CallbackInfo &info);

public:
    double value;
};

Napi::Object A::Init(Napi::Env env, Napi::Object exports)
{
    // This method is used to hook the accessor and method callbacks
    Napi::Function func = DefineClass(env, "A", {

                                                });

    Napi::FunctionReference *constructor = new Napi::FunctionReference();

    // Create a peristent reference to the class constructor. This will allow
    // a function called on a class prototype and a function
    // called on instance of a class to be distinguished from each other.
    *constructor = Napi::Persistent(func);
    exports.Set("A", func);

    // Store the constructor as the add-on instance data. This will allow this
    // add-on to support multiple instances of itself running on multiple worker
    // threads, as well as multiple instances of itself running in different
    // contexts on the same thread.
    //
    // By default, the value set on the environment here will be destroyed when
    // the add-on is unloaded using the `delete` operator, but it is also
    // possible to supply a custom deleter.
    env.SetInstanceData<Napi::FunctionReference>(constructor);

    return exports;
}

A::A(const Napi::CallbackInfo &info) : Napi::ObjectWrap<A>(info)
{
    Napi::Env env = info.Env();
    // ...
    Napi::Number value = info[0].As<Napi::Number>();
    this->value = value.DoubleValue();
}

class B : public Napi::ObjectWrap<B>
{
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    Napi::Value GetValue(const Napi::CallbackInfo& info);
    B(const Napi::CallbackInfo &info);

private:
    double value;
};

Napi::Object B::Init(Napi::Env env, Napi::Object exports)
{
    // This method is used to hook the accessor and method callbacks
    Napi::Function func = DefineClass(env, "B", {InstanceMethod<&B::GetValue>("GetValue")});

    Napi::FunctionReference *constructor = new Napi::FunctionReference();

    // Create a peristent reference to the class constructor. This will allow
    // a function called on a class prototype and a function
    // called on instance of a class to be distinguished from each other.
    *constructor = Napi::Persistent(func);
    exports.Set("B", func);

    // Store the constructor as the add-on instance data. This will allow this
    // add-on to support multiple instances of itself running on multiple worker
    // threads, as well as multiple instances of itself running in different
    // contexts on the same thread.
    //
    // By default, the value set on the environment here will be destroyed when
    // the add-on is unloaded using the `delete` operator, but it is also
    // possible to supply a custom deleter.
    env.SetInstanceData<Napi::FunctionReference>(constructor);

    return exports;
}

B::B(const Napi::CallbackInfo &info) : Napi::ObjectWrap<B>(info)
{
    Napi::Env env = info.Env();
    // ...
    Napi::Object a = info[0].As<Napi::Object>();
    A *a1 = Napi::ObjectWrap<A>::Unwrap(a);
    this->value = a1->value;
}

Napi::Value B::GetValue(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    return Napi::Number::New(env, this->value);
}

// Initialize native add-on
Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    A::Init(env, exports);
    B::Init(env, exports);
    return exports;
}

// Register and initialize native add-on
NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)