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

#ifndef FRAMEWORKS_WM_INCLUDE_CLIENT_WINDOW_MANAGER_CONTROLLER_CLIENT_H
#define FRAMEWORKS_WM_INCLUDE_CLIENT_WINDOW_MANAGER_CONTROLLER_CLIENT_H

#include <list>
#include <pthread.h>
#include <vector>

#include <refbase.h>
#include <surface.h>
#include <window_manager_service_client.h>

#include "wayland_service.h"
#include "window_manager_server.h"
#include "wl_buffer_cache.h"
#include "wl_buffer.h"
#include "wl_display.h"
#include "wl_dma_buffer_factory.h"
#include "wl_shm_buffer_factory.h"
#include "wl_subsurface_factory.h"
#include "wl_surface_factory.h"

namespace OHOS {
struct InnerWindowInfo {
    sptr<WlSurface> wlSurface;
    sptr<WlSubsurface> wlSubsurf;
    sptr<WlBuffer> wlBuffer;
    WindowConfig windowconfig;
    uint32_t windowid;
    uint32_t layerid;
    uint32_t parentid;
    uint32_t voLayerId;
    sptr<Surface> surf;
    sptr<IBufferConsumerListener> listener;
    std::list<uint32_t> childIDList;
    bool subwidow;
    int32_t width;
    int32_t height;
    int32_t pos_x;
    int32_t pos_y;
    funcWindowInfoChange windowInfoChangeCb;
    void (* onWindowCreateCb)(uint32_t pid);

    bool operator ==(const InnerWindowInfo &other) const
    {
        return windowid == other.windowid;
    }
};

class LayerControllerClient : public RefBase, public IWlBufferReleaseClazz {
public:
    static sptr<LayerControllerClient> GetInstance();

    bool init(sptr<IWindowManagerService> &service);

    InnerWindowInfo *CreateWindow(int32_t id, WindowConfig &config);
    InnerWindowInfo *CreateSubWindow(int32_t subid, int32_t parentid, WindowConfig &config);
    void CreateWlBuffer(sptr<Surface>& surf, uint32_t id);
    void DestroyWindow(int32_t id);
    void Move(int32_t id, int32_t x, int32_t y);
    void Show(int32_t id);
    void Hide(int32_t id);
    void ReSize(int32_t id, int32_t width, int32_t height);
    void Rotate(int32_t id, int32_t type);
    int32_t GetMaxWidth();
    int32_t GetMaxHeight();
    void ChangeWindowType(int32_t id, WindowType type);
    void RegistWindowInfoChangeCb(int id, funcWindowInfoChange cb);
    void RegistOnWindowCreateCb(int32_t id, void(* cb)(uint32_t pid));
    void SendWindowCreate(uint32_t pid);

    InnerWindowInfo *GetInnerWindowInfoFromId(uint32_t windowid);
    void RemoveInnerWindowInfo(uint32_t id);
    static void *thread_display_dispatch(void *param);

protected:
    bool CreateSurface(int32_t id);
    bool initScreen();

private:
    void SetSubSurfaceSize(int32_t id, int32_t width, int32_t height);

    static inline sptr<LayerControllerClient> instance = nullptr;
    LayerControllerClient();
    virtual ~LayerControllerClient();
    virtual void OnWlBufferRelease(struct wl_buffer *wbuffer, int32_t fence) override;

    sptr<IWindowManagerService> wms = nullptr;
    bool isInit = false;
    std::mutex mutex;
    std::mutex windowListMutex;
    std::list<InnerWindowInfo> m_windowList;
    uint32_t m_screenId = 0;
    int32_t m_screenWidth = 0;
    int32_t m_screenHeight = 0;
};

class SurfaceListener : public IBufferConsumerListener {
public:
    SurfaceListener(sptr<Surface>& surf, uint32_t windowid);
    virtual ~SurfaceListener();

    virtual void OnBufferAvailable() override;

private:
    wptr<Surface> surface_;
    uint32_t windowid_;
};
}

#endif // FRAMEWORKS_WM_INCLUDE_CLIENT_WINDOW_MANAGER_CONTROLLER_CLIENT_H
