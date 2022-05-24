/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include "dcamera_sink_load_callback.h"

#include <unistd.h>

#include "hisysevent.h"
#include "dcamera_sink_handler.h"
#include "distributed_hardware_log.h"
#include "distributed_camera_constants.h"
#include "distributed_camera_errno.h"

namespace OHOS {
namespace DistributedHardware {
DCameraSinkLoadCallback::DCameraSinkLoadCallback(const std::string& params) : params_(params) {}
void DCameraSinkLoadCallback::OnLoadSystemAbilitySuccess(int32_t systemAbilityId,
    const sptr<IRemoteObject>& remoteObject)
{
    DHLOGI("OnLoadSystemAbilitySuccess systemAbilityId: %d, IRmoteObject result: %s",
        systemAbilityId, (remoteObject != nullptr) ? "true" : "false");
    if (systemAbilityId != DISTRIBUTED_HARDWARE_CAMERA_SINK_SA_ID) {
        DHLOGE("start aystemabilityId is not sinkSAId!");
        return;
    }
    if (remoteObject == nullptr) {
        DHLOGE("remoteObject is null.");
        return;
    }
    DCameraSinkHandler::GetInstance().FinishStartSA(params_);
}

void DCameraSinkLoadCallback::OnLoadSystemAbilityFail(int32_t systemAbilityId)
{
    DHLOGI("OnLoadSystemAbilityFail systemAbilityId: %d.", systemAbilityId);
    if (systemAbilityId != DISTRIBUTED_HARDWARE_CAMERA_SINK_SA_ID) {
        DHLOGE("start aystemabilityId is not sinkSAId!");
        int32_t retVal = OHOS::HiviewDFX::HiSysEvent::Write(
            OHOS::HiviewDFX::HiSysEvent::Domain::DISTRIBUTED_CAMERA,
            "SA_ERROR",
            OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
            "PID", getpid(),
            "UID", getuid(),
            "MSG", "dcamera sink OnLoadSystemAbilityFail.");
        if (retVal != DCAMERA_OK) {
            DHLOGE("Write HiSysEvent error, retVal:%d", retVal);
        }
        return;
    }
    DCameraSinkHandler::GetInstance().FinishStartSAFailed(systemAbilityId);
}
}
}