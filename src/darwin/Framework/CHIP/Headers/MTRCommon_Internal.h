//
/**
 *    Copyright (c) 2023 Project CHIP Authors
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#ifndef MTRCommon_h
#define MTRCommon_h

#import <Foundation/Foundation.h>

#import <Matter/MTRBaseDevice.h>
#import <Matter/MTRDevice.h>
#import <Matter/MTRDefines.h>

#import "MTRAsyncWorkQueue.h"
#import "MTRAttributeSpecifiedCheck.h"
#import "MTRBaseClusters.h"
#import "MTRBaseDevice_Internal.h"
#import "MTRBaseSubscriptionCallback.h"
#import "MTRCluster.h"
#import "MTRClusterConstants.h"
#import "MTRCommandTimedCheck.h"
#import "MTRConversion.h"
#import "MTRDefines_Internal.h"
#import "MTRDevice_Internal.h"
#import "MTRDeviceConnectivityMonitor.h"
#import "MTRDeviceController_Internal.h"
#import "MTRDeviceControllerOverXPC.h"
#import "MTRDeviceStorageBehaviorConfiguration_Internal.h"
#import "MTRError_Internal.h"
#import "MTREventTLVValueDecoder_Internal.h"
#import "MTRLogging_Internal.h"
#import "MTRTimeUtils.h"
#import "MTRUnfairLock.h"
#import "zap-generated/MTRCommandPayloads_Internal.h"

#import "lib/core/CHIPError.h"
#import "lib/core/DataModelTypes.h"

#import <app/AttributePathParams.h>
#import <app/BufferedReadCallback.h>
#import <app/ClusterStateCache.h>
#import <app/ConcreteAttributePath.h>
#import <app/InteractionModelEngine.h>
#import <lib/support/FibonacciUtils.h>
#import <os/lock.h>
#import <platform/LockTracker.h>
#import <platform/PlatformManager.h>

#endif /* MTRCommon_h */
