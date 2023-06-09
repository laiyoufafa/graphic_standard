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

#ifndef FRAMEWORKS_WM_INCLUDE_WINDOW_MANAGER_IMPL_H
#define FRAMEWORKS_WM_INCLUDE_WINDOW_MANAGER_IMPL_H

#include <mutex>
#include <thread>

#include <window_manager.h>

#include <window_manager_service_client.h>

#include "singleton_delegator.h"
#include "wayland_service.h"
#include "window_manager_server.h"
#include "wl_buffer_cache.h"
#include "wl_display.h"
#include "wl_dma_buffer_factory.h"
#include "wl_shm_buffer_factory.h"
#include "wl_subsurface_factory.h"
#include "wl_surface_factory.h"

namespace OHOS {
class WindowManagerImpl : public WindowManager {
public:
    static sptr<WindowManagerImpl> GetInstance();

    virtual GSError Init() override;

    virtual GSError GetDisplays(std::vector<struct WMDisplayInfo> &displays) const override;
    virtual sptr<Window> GetWindowByID(int32_t wid) override;

    virtual GSError CreateWindow(sptr<Window> &window, const sptr<WindowOption> &option) override;
    virtual GSError CreateSubwindow(sptr<Subwindow> &subwindow,
                                    const sptr<Window> &window,
                                    const sptr<SubwindowOption> &option) override;

    virtual GSError ListenNextScreenShot(int32_t id, IScreenShotCallback *cb) override;
    virtual GSError ListenNextWindowShot(const sptr<Window> &window, IWindowShotCallback *cb) override;

    virtual GSError CreateVirtualDisplay(const sptr<VirtualDisplayOption> &option) override;
    virtual GSError DestroyVirtualDisplay(uint32_t did) override;
    virtual GSError SetDisplayMode(WMSDisplayMode mode) override;

private:
    WindowManagerImpl();
    virtual ~WindowManagerImpl() override;
    static inline sptr<WindowManagerImpl> instance = nullptr;
    static inline SingletonDelegator<WindowManagerServiceClient> wmscDelegator;

    void InitSingleton();
    void DeinitSingleton();

    void Deinit();

    std::mutex initMutex;
    bool init = false;
    bool initSingleton = false;
    sptr<IWindowManagerService> wmservice = nullptr;

    using WptrWindow = wptr<Window>;
    std::vector<WptrWindow> windowCache;

    sptr<WlDisplay> display = nullptr;
    sptr<WlBufferCache> wlBufferCache = nullptr;
    sptr<WlDMABufferFactory> wlDMABufferFactory = nullptr;
    sptr<WlSHMBufferFactory> wlSHMBufferFactory = nullptr;
    sptr<WlSubsurfaceFactory> wlSubsurfaceFactory = nullptr;
    sptr<WlSurfaceFactory> wlSurfaceFactory = nullptr;
    sptr<WindowManagerServer> windowManagerServer = nullptr;
    sptr<WaylandService> waylandService = nullptr;
    sptr<WindowManagerServiceClient> wmsc = nullptr;
};
} // namespace OHOS

#endif // FRAMEWORKS_WM_INCLUDE_WINDOW_MANAGER_IMPL_H
