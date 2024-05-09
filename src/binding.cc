#include <napi.h>
#include "libcmt/rollup.h"
#include "rollup.h"

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    return Rollup::Init(env, exports);
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
