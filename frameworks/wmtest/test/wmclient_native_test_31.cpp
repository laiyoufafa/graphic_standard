/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "wmclient_native_test_31.h"

#include <cstdio>

#include <display_type.h>
#include <window_manager.h>

#include "inative_test.h"
#include "native_test_class.h"
#include "egl_native_test_class.h"
#include "util.h"

using namespace OHOS;

namespace {
class WMClientNativeTest31 : public INativeTest {
public:
    std::string GetDescription() const override
    {
        constexpr const char *desc = "egl surface test";
        return desc;
    }

    std::string GetDomain() const override
    {
        constexpr const char *domain = "wmclient";
        return domain;
    }

    int32_t GetID() const override
    {
        constexpr int32_t id = 31;
        return id;
    }

    uint32_t GetLastTime() const override
    {
        constexpr uint32_t lastTime = LAST_TIME_FOREVER;
        return lastTime;
    }

    void Run(int32_t argc, const char **argv) override
    {
        auto initRet = WindowManager::GetInstance()->Init();
        if (initRet) {
            printf("init failed with %s\n", GSErrorStr(initRet).c_str());
            ExitTest();
            return;
        }

        window = NativeTestFactory::CreateWindow(WINDOW_TYPE_NORMAL);
        if (window == nullptr) {
            ExitTest();
            return;
        }

        window->SwitchTop();
        auto producer = window->GetProducer();
        sptr<EglSurface> pEglSurface = EglSurface::CreateEglSurfaceAsProducer(producer);
        windowSync = EGLNativeTestSync::CreateSync(EGLNativeTestDraw::FlushDraw,
            pEglSurface, window->GetWidth(), window->GetHeight());
    }

private:
    sptr<Window> window = nullptr;
    sptr<EGLNativeTestSync> windowSync = nullptr;
} g_autoload;
} // namespace
