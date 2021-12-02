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

#include "wmclient_native_test_2.h"

#include <cstdio>
#include <iostream>
#include <securec.h>
#include <sstream>

#include <display_type.h>
#include <window_manager.h>

#include "inative_test.h"
#include "native_test_class.h"
#include "util.h"

using namespace OHOS;

namespace {
class WMClientNativeTest2 : public INativeTest {
public:
    std::string GetDescription() const override
    {
        constexpr const char *desc = "window type test";
        return desc;
    }

    std::string GetDomain() const override
    {
        constexpr const char *domain = "wmclient";
        return domain;
    }

    int32_t GetID() const override
    {
        constexpr int32_t id = 2;
        return id;
    }

    uint32_t GetLastTime() const override
    {
        constexpr uint32_t lastTime = LAST_TIME_FOREVER;
        return lastTime;
    }

    void Usage()
    {
        printf("Usage: wmtest wmclient 2 <WindowTypeID>\n");
        struct WindowTypeItem {
            std::string s;
            WindowType t;
        };

        std::vector<struct WindowTypeItem> v;
        for (const auto &[str, type] : windowTypeStrs) {
            struct WindowTypeItem t = { .s = str, .t = type };
            v.push_back(t);
        }

        std::sort(v.begin(), v.end(), [](auto i, auto j) { return i.t < j.t;});
        for (const auto &it : v) {
            std::cout << it.t << ":" << it.s << std::endl;
        }
    }

    bool CheckArguments(const char *argv1, int &typeId)
    {
        std::stringstream ss(argv1);
        ss >> typeId;
        if (!ss.eof() || !ss) {
            printf("input error\n");
            return false;
        }

        if (typeId < 0 || typeId > WINDOW_TYPE_MAX) {
            printf ("input id is %d, not with rules!!!\n", typeId);
            return false;
        }

        return true;
    }

    void Run(int32_t argc, const char **argv) override
    {
        int type = -1;
        if (argc == 1 || (!CheckArguments(argv[1], type))) {
            Usage();
            ExitTest();
            return;
        }

        auto initRet = WindowManager::GetInstance()->Init();
        if (initRet) {
            printf("init failed with %s\n", WMErrorStr(initRet).c_str());
            ExitTest();
            return;
        }

        window = NativeTestFactory::CreateWindow(static_cast<WindowType>(type));
        if (window == nullptr) {
            printf("NativeTestFactory::CreateWindow return nullptr\n");
            return;
        }

        window->SwitchTop();
        auto surface = window->GetSurface();
        windowSync = NativeTestSync::CreateSync(NativeTestDraw::FlushDraw, surface);
        }

private:
    sptr<Window> window = nullptr;
    sptr<NativeTestSync> windowSync = nullptr;
} g_autoload;
} // namespace
