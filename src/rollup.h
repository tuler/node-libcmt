
#ifndef NODE_LIBCMT_ROLLUP_H
#define NODE_LIBCMT_ROLLUP_H

#include <napi.h>
extern "C"
{
#include "libcmt/rollup.h"
}

class Rollup : public Napi::ObjectWrap<Rollup>
{
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    Rollup(const Napi::CallbackInfo &info);
    void Finalize(Napi::Env env);

private:
    Napi::Value Finish(const Napi::CallbackInfo &info);
    Napi::Value EmitVoucher(const Napi::CallbackInfo &info);
    Napi::Value EmitNotice(const Napi::CallbackInfo &info);
    void EmitReport(const Napi::CallbackInfo &info);
    void EmitException(const Napi::CallbackInfo &info);
    void Progress(const Napi::CallbackInfo &info);
    cmt_rollup_t rollup;
};

#endif
