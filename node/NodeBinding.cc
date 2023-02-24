#include "NodeBinding.h"

Napi::Value Node_SearchProcessKeywords(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 1)
    {
        Napi::TypeError::New(env, "Wrong arguments, expected one string").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsString())
    {
        Napi::TypeError::New(env, "Wrong arguments, position 0 expected one string").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto arrayPush = [](Napi::Array &arrayObj, const Napi::Value &elem) -> decltype(auto)
    {
        auto currentLength = arrayObj.Get("length").As<Napi::Number>().Int32Value();

        auto pushFn = arrayObj.Get("push");

        if (!pushFn.IsFunction())
        {
            return false;
        }

        auto pushFnResult = pushFn.As<Napi::Function>().Call(arrayObj, {elem});

        if (!pushFnResult.IsNumber())
        {
            return false;
        }

        return pushFnResult.As<Napi::Number>().Int32Value() > currentLength;
    };

    auto keywordStr = info[0].As<Napi::String>().Utf8Value();
    ProcessInfoResult *result = nullptr;
    int resultCount = 0;

    auto jsInfoResult = Napi::Array::New(env);

    if (SearchProcessKeywords(keywordStr.data(), &result, &resultCount))
    {
        for (int i = 0; i < resultCount; i++)
        {
            auto jsInfoObject = Napi::Object::New(env);
            auto jsInfoModuleArr = Napi::Array::New(env);
            for (int j = 0; j < result[i].moduleCount; j++)
            {
                arrayPush(jsInfoModuleArr, Napi::String::New(env, result[i].keywords[j]));
            }
            jsInfoObject.Set("pid", Napi::Number::New(env, static_cast<double>(result[i].pid)));
            jsInfoObject.Set("modules", jsInfoModuleArr);
            arrayPush(jsInfoResult, jsInfoObject);
        }
    }
    ReleaseProcessInfoResult(&result, resultCount);
    return jsInfoResult;
}

Napi::Value Node_TerminateProcessByPid(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() < 1)
    {
        Napi::TypeError::New(env, "Wrong arguments, expected one number").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsNumber())
    {
        Napi::TypeError::New(env, "Wrong arguments, position 0 expected one number").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto pid = static_cast<unsigned long>(info[0].As<Napi::Number>().Int32Value());

    auto isTerminated = TerminateProcessByPid(pid);

    return Napi::Boolean::New(env, static_cast<bool>(isTerminated));
}

Napi::Object Initialize(Napi::Env env, Napi::Object exports)
{
    exports.Set(
        Napi::String::New(env, "SearchProcessKeywords"),
        Napi::Function::New(env, Node_SearchProcessKeywords));
    exports.Set(
        Napi::String::New(env, "TerminateProcessByPid"),
        Napi::Function::New(env, Node_TerminateProcessByPid));

    return exports;
}

NODE_API_MODULE(Terminater, Initialize)