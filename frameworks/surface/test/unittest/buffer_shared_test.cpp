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

#include "buffer_shared_test.h"

#include "buffer_consumer_listener.h"
#include "buffer_manager.h"
#include "buffer_log.h"
#include "test_header.h"

using namespace testing;
using namespace testing::ext;

namespace OHOS {
void BufferSharedTest::SetUpTestCase()
{
    GTEST_LOG_(INFO) << getpid() << std::endl;
    surface = Surface::CreateSurfaceAsConsumer("shared", true);
    sptr<IBufferConsumerListener> listener = new BufferConsumerListener();
    surface->RegisterConsumerListener(listener);
    auto producer1 = surface->GetProducer();
    producerSurface1 = Surface::CreateSurfaceAsProducer(producer1);
    auto producer2 = surface->GetProducer();
    producerSurface2 = Surface::CreateSurfaceAsProducer(producer2);
}

void BufferSharedTest::TearDownTestCase()
{
}

namespace {
/*
* Function: RequestBuffer
* Type: Reliability
* Rank: Important(2)
* EnvConditions: N/A
* CaseDescription: 1. call RequestBufferNoFence with buffer=buffer1, buffer2，the param is same
*                  2. check ret1 and ret2 are SURFACE_ERROR_OK, check buffer1 and buffer2 is not nullptr
*                  3. check the addr of buffer1 EQ buffer2
* */

HWTEST_F(BufferSharedTest, RequestBuffer, testing::ext::TestSize.Level0)
{
    PART("REQUEST BUFFER TWO TIMES") {
        SurfaceError ret1, ret2;
        STEP("1: request buffer") {
            BufferRequestConfig requestConfig = {
                .width = 0x100,
                .height = 0x100,
                .strideAlignment = 0x8,
                .format = PIXEL_FMT_RGBA_8888,
                .usage = HBM_USE_CPU_READ | HBM_USE_CPU_WRITE | HBM_USE_MEM_DMA,
                .timeout = 0,
            };
            ret1 = producerSurface1->RequestBufferNoFence(buffer1, requestConfig);
            ret2 = producerSurface2->RequestBufferNoFence(buffer2, requestConfig);
        }
        STEP("2: check ret1 ret2 buffer1 buffer2") {
            STEP_ASSERT_EQ(ret1, SURFACE_ERROR_OK);
            STEP_ASSERT_NE(buffer1, nullptr);
            STEP_ASSERT_EQ(ret2, SURFACE_ERROR_OK);
            STEP_ASSERT_NE(buffer2, nullptr);
        }
        STEP("3: check buffer addr") {
            STEP_ASSERT_EQ(buffer2, buffer1);
        }
    }
}
/*
* Function: RequestBuffer with different requestconfig
* Type: Reliability
* Rank: Important(2)
* EnvConditions: N/A
* CaseDescription: 1. call RequestBufferNoFence with buffer=bufferDiff,
*                     the requestconfig is not same with buffer1
*                  2. check ret1 is SURFACE_ERROR_INVALID_PARAM
* */

HWTEST_F(BufferSharedTest, RequestBufferDiff, testing::ext::TestSize.Level0)
{
    PART("REQUEST BUFFER with different requestconfig") {
        SurfaceError ret1;
        sptr<SurfaceBuffer> bufferDiff = nullptr;
        STEP("1: request buffer") {
            BufferRequestConfig diffRequestConfig = {
                .width = 0x200,
                .height = 0x100,
                .strideAlignment = 0x8,
                .format = PIXEL_FMT_RGBA_8888,
                .usage = HBM_USE_CPU_READ | HBM_USE_CPU_WRITE | HBM_USE_MEM_DMA,
                .timeout = 0,
            };
            ret1 = producerSurface1->RequestBufferNoFence(bufferDiff, diffRequestConfig);
        }
        STEP("2: check ret1") {
            STEP_ASSERT_EQ(ret1, SURFACE_ERROR_INVALID_PARAM);
        }
    }
}
/*
* Function: FlushBuffer
* Type: Reliability
* Rank: Important(2)
* EnvConditions: N/A
* CaseDescription: 1. call FlushBuffer with buffer=buffer1, buffer2
*                  2. check ret1 and ret2 is SURFACE_ERROR_OK
* */
HWTEST_F(BufferSharedTest, FlushBuffer, testing::ext::TestSize.Level0)
{
    PART("FlushBuffer") {
        SurfaceError ret1, ret2;
        STEP("1: FlushBuffer two times") {
            BufferFlushConfig flushConfig = { .damage = { .w = 0x100, .h = 0x100, }, };
            ret1 = producerSurface1->FlushBuffer(buffer1, -1, flushConfig);
            ret2 = producerSurface2->FlushBuffer(buffer2, -1, flushConfig);
        }
        STEP("2: check ret1 ret2") {
            STEP_ASSERT_EQ(ret1, SURFACE_ERROR_OK);
            STEP_ASSERT_EQ(ret2, SURFACE_ERROR_OK);
        }
    }
}
/*
* Function: AquiredBuffer
* Type: Reliability
* Rank: Important(2)
* EnvConditions: N/A
* CaseDescription: 1. call AcquireBuffer with buffer=sbuffer1, sbuffer2
*                  2. check ret1 and ret2 are WM_ERROR_NULLPTR
* */
HWTEST_F(BufferSharedTest, AquiredBuffer, testing::ext::TestSize.Level0)
{
    PART("AquiredBuffer") {
        SurfaceError ret1, ret2;
        STEP("1: AcquireBuffer two times") {
            int64_t timestamp = 0;
            Rect damage = {};
            int32_t fence = -1;

            ret1 = surface->AcquireBuffer(sbuffer1, fence, timestamp, damage);
            ret2 = surface->AcquireBuffer(sbuffer2, fence, timestamp, damage);
        }
        STEP("2: check ret1 ret2") {
            STEP_ASSERT_EQ(ret1, SURFACE_ERROR_OK);
            STEP_ASSERT_EQ(ret2, SURFACE_ERROR_OK);
        }
        STEP("3: check addr sbuffer1 and sbuffer2") {
            STEP_ASSERT_EQ(sbuffer1, sbuffer2);
        }
    }
}
/*
* Function: CancelBuffer
* Type: Reliability
* Rank: Important(2)
* EnvConditions: N/A
* CaseDescription: 1. call cancelBuffer with buffer=buffer1
*                  2. check ret1 is SURFACE_ERROR_INVALID_OPERATING
*                  3. call cancelBuffer with buffer=buffer2
*                  4. check ret2 is SURFACE_ERROR_INVALID_OPERATING
* */
HWTEST_F(BufferSharedTest, CancelBuffer, testing::ext::TestSize.Level0)
{
    PART("CancelBuffer") {
        SurfaceError ret1, ret2;
        STEP("1: Cancel buffer1") {
            ret1 = producerSurface1->CancelBuffer(buffer1);
        }
        STEP("2: check ret1") {
            STEP_ASSERT_EQ(ret1, SURFACE_ERROR_INVALID_OPERATING);
        }
        STEP("3: Cancel buffer2") {
            ret2 = producerSurface2->CancelBuffer(buffer2);
        }
        STEP("4: check ret2") {
            STEP_ASSERT_EQ(ret2, SURFACE_ERROR_INVALID_OPERATING);
        }
    }
}
/*
* Function: RelaseBuffer
* Type: Reliability
* Rank: Important(2)
* EnvConditions: N/A
* CaseDescription: 1. releaseBuffer two times
*                  2. check ret1 is SURFACE_ERROR_INVALID_OPERATING, check ret1 is SURFACE_ERROR_OK
* */
HWTEST_F(BufferSharedTest, ReleaseBuffer, testing::ext::TestSize.Level0)
{
    PART("ReleaseBuffer") {
        SurfaceError ret1, ret2;
        STEP("1: releaseBuffer two times") {
            ret1 = surface->ReleaseBuffer(sbuffer1, -1);
            ret2 = surface->ReleaseBuffer(sbuffer2, -1);
        }
        STEP("2: check ret1, ret2") {
            STEP_ASSERT_EQ(ret1, SURFACE_ERROR_OK);
            STEP_ASSERT_EQ(ret2, SURFACE_ERROR_OK);
        }
    }
}
}
} // namespace OHOS