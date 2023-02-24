#ifndef __NODE_BINDING_H__
#define __NODE_BINDING_H__

#include <Napi.h>
#include <Terminater.h>

Napi::Value Node_SearchProcessKeywords(const Napi::CallbackInfo &);
Napi::Value Node_TerminateProcessByPid(const Napi::CallbackInfo &);
Napi::Object Initialize(Napi::Env, Napi::Object);

#endif