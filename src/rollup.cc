#include <napi.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdint>
#include "rollup.h"

#define THROW_IF_ERROR_VOID(env, error_code)                   \
    if (error_code != 0)                                       \
    {                                                          \
        Napi::String str = Napi::String::New(env, "Error");    \
        const Napi::Error error = Napi::Error::New(env, str);  \
        error.Set("code", Napi::Number::New(env, error_code)); \
        error.ThrowAsJavaScriptException();                    \
        return;                                                \
    }

#define THROW_IF_ERROR(env, error_code)                        \
    if (error_code != 0)                                       \
    {                                                          \
        Napi::String str = Napi::String::New(env, "Error");    \
        const Napi::Error error = Napi::Error::New(env, str);  \
        error.Set("code", Napi::Number::New(env, error_code)); \
        error.ThrowAsJavaScriptException();                    \
        return Napi::Value();                                  \
    }

// Convert binary data into hex string
// TODO: implement ETH checksum address
static std::string hex(const uint8_t *data, uint64_t length)
{
    static const char t[] = "0123456789abcdef";
    std::stringstream ss;
    ss << "0x";
    for (uint64_t i = 0; i < length; ++i)
    {
        char hi = t[(data[i] >> 4) & 0x0f];
        char lo = t[(data[i] >> 0) & 0x0f];
        ss << std::hex << hi << lo;
    }
    return ss.str();
}

Napi::Object Rollup::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(
        env,
        "Rollup", {
                      InstanceMethod<&Rollup::Finish>("finish"),
                      InstanceMethod<&Rollup::EmitVoucher>("emit_voucher"),
                      InstanceMethod<&Rollup::EmitNotice>("emit_notice"),
                      InstanceMethod<&Rollup::EmitReport>("emit_report"),
                      InstanceMethod<&Rollup::EmitException>("emit_exception"),
                      InstanceMethod<&Rollup::Progress>("progress"),
                  });

    Napi::FunctionReference *constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    env.SetInstanceData(constructor);
    exports.Set("Rollup", func);
    return exports;
}

Rollup::Rollup(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Rollup>(info)
{
    Napi::Env env = info.Env();
    int err = cmt_rollup_init(&this->rollup);
    THROW_IF_ERROR_VOID(env, err);
}

void Rollup::Finalize(Napi::Env env)
{
    cmt_rollup_fini(&this->rollup);
}

Napi::Value Rollup::Finish(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() != 1 || !info[0].IsBoolean())
    {
        Napi::TypeError::New(env, "Wrong argument #0, expecting boolean").ThrowAsJavaScriptException();
        return Napi::Value();
    }

    cmt_rollup_finish_t data = {
        .accept_previous_request = info[0].As<Napi::Boolean>().Value(),
        .next_request_type = 0,
        .next_request_payload_length = 0,
    };
    int err = cmt_rollup_finish(&this->rollup, &data);
    THROW_IF_ERROR(env, err);

    if (data.next_request_type == HTIF_YIELD_REASON_ADVANCE)
    {
        cmt_rollup_advance_t advance;
        cmt_rollup_read_advance_state(&this->rollup, &advance);
        printf("Advance: %d\n", advance.payload_length);
        Napi::Object object = Napi::Object::New(env);
        object.Set("type", "advance");
        object.Set("app_contract", Napi::String::New(env, hex(advance.app_contract, CMT_ADDRESS_LENGTH)));
        object.Set("block_number", Napi::BigInt::New(env, advance.block_number));
        object.Set("block_timestamp", Napi::BigInt::New(env, advance.block_timestamp));
        object.Set("chain_id", Napi::BigInt::New(env, advance.chain_id));
        object.Set("index", Napi::BigInt::New(env, advance.index));
        object.Set("msg_sender", Napi::String::New(env, hex(advance.msg_sender, CMT_ADDRESS_LENGTH)));
        object.Set("payload", Napi::ArrayBuffer::New(env, advance.payload, advance.payload_length));
        return object;
    }
    else if (data.next_request_type == HTIF_YIELD_REASON_INSPECT)
    {
        cmt_rollup_inspect_t inspect;
        cmt_rollup_read_inspect_state(&this->rollup, &inspect);
        Napi::Object object = Napi::Object::New(env);
        object.Set("type", "advance");
        object.Set("payload", Napi::ArrayBuffer::New(env, inspect.payload, inspect.payload_length));
        return object;
    }
    else
    {
        Napi::TypeError::New(env, "Unknown next request type").ThrowAsJavaScriptException();
        return Napi::Value();
    }
}

Napi::Value Rollup::EmitVoucher(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() != 1 || !info[0].IsObject())
    {
        Napi::TypeError::New(env, "Wrong argument #0, expecting Object").ThrowAsJavaScriptException();
        return Napi::Value();
    }

    // TODO
    /*
    uint64_t index;
    cmt_rollup_emit_voucher_t data = {
        .payload = info[0].As<Napi::ArrayBuffer>().Data(),
        .payload_length = info[0].As<Napi::ArrayBuffer>().ByteLength()};
        uint64_t
    int err = cmt_rollup_emit_voucher(this->rollup, CMT_ADDRESS_LENGTH, destination, & data);
    */
    int err = 0;
    int index = 0;
    THROW_IF_ERROR(env, err);
    return Napi::Number::New(env, index);
}

Napi::Value Rollup::EmitNotice(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() != 1 || !info[0].IsObject())
    {
        Napi::TypeError::New(env, "Wrong argument #0, expecting Object").ThrowAsJavaScriptException();
        return Napi::Value();
    }

    int err = 0;
    // TODO
    int index = 0;
    // cmt_rollup_emit_notice(this->rollup, )
    THROW_IF_ERROR(env, err);
    return Napi::Number::New(env, index);
}

void Rollup::EmitReport(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() != 1 || !info[0].IsObject())
    {
        Napi::TypeError::New(env, "Wrong argument #0, expecting ArrayBuffer").ThrowAsJavaScriptException();
        return;
    }

    int err = 0;
    // TODO
    // cmt_rollup_emit_report(this->rollup, )
    THROW_IF_ERROR_VOID(env, err);
}

void Rollup::EmitException(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() != 1 || !info[0].IsObject())
    {
        Napi::TypeError::New(env, "Wrong argument #0, expecting ArrayBuffer").ThrowAsJavaScriptException();
        return;
    }

    int err = 0;
    // TODO
    // cmt_rollup_emit_exception(this->rollup, data);
    THROW_IF_ERROR_VOID(env, err);
}

void Rollup::Progress(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() != 1 || !info[0].IsNumber())
    {
        Napi::TypeError::New(env, "Wrong argument #0, expecting number").ThrowAsJavaScriptException();
        return;
    }
    int err = cmt_rollup_progress(&this->rollup, info[0].As<Napi::Number>().Uint32Value());
    THROW_IF_ERROR_VOID(env, err);
}
