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

#include "dcamera_sa_process_state.h"

#include <mutex>

#include "distributed_hardware_log.h"

namespace OHOS {
namespace DistributedHardware {
typedef enum {
    DCAMERA_SA_EXIT_STATE_START = 0,
    DCAMERA_SA_EXIT_STATE_STOP = 1
} DCameraSAState;

DCameraSAState sinkSAState_ = DCAMERA_SA_EXIT_STATE_START;
DCameraSAState sourceSAState_ = DCAMERA_SA_EXIT_STATE_START;
std::mutex saProcessState_;

void SetSinkProcessExit()
{
    DHLOGI("set sink process exit.");
    std::lock_guard<std::mutex> autoLock(saProcessState_);
    sinkSAState_ = DCAMERA_SA_EXIT_STATE_STOP;
    DHLOGI("sourceSAState_ = %d sinkSAState_ = %d", sourceSAState_, sinkSAState_);
    if (sourceSAState_ == DCAMERA_SA_EXIT_STATE_START || sinkSAState_ == DCAMERA_SA_EXIT_STATE_START) {
        return;
    }
    DHLOGI("exit sa process success.");
    exit(0);
}

void SetSourceProcessExit()
{
    DHLOGI("set sources process exit.");
    std::lock_guard<std::mutex> autoLock(saProcessState_);
    sourceSAState_ = DCAMERA_SA_EXIT_STATE_STOP;
    DHLOGI("sourceSAState_ = %d sinkSAState_ = %d", sourceSAState_, sinkSAState_);
    if (sourceSAState_ == DCAMERA_SA_EXIT_STATE_START || sinkSAState_ == DCAMERA_SA_EXIT_STATE_START) {
        return;
    }
    DHLOGI("exit sa process success.");
    exit(0);
}
} // namespace DistributedHardware
} // namespace OHOS