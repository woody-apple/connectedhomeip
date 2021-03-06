/*
 *
 *    Copyright (c) 2021 Project CHIP Authors
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

// THIS FILE IS GENERATED BY ZAP

#import <Foundation/Foundation.h>

#import "CHIPDevice.h"
#import "CHIPDevice_Internal.h"
#import "ChipError.h"
#import "gen/CHIPClientCallbacks.h"
#import "gen/CHIPClustersObjc.h"

#include <controller/CHIPClusters.h>
#include <lib/support/Span.h>

using namespace ::chip;

class CHIPDefaultSuccessCallbackBridge : public Callback::Callback<DefaultSuccessCallback> {
public:
    CHIPDefaultSuccessCallbackBridge(ResponseHandler handler, dispatch_queue_t queue)
        : Callback::Callback<DefaultSuccessCallback>(CallbackFn, this)
        , mHandler(handler)
        , mQueue(queue)
    {
    }

    ~CHIPDefaultSuccessCallbackBridge() {};

    static void CallbackFn(void * context)
    {
        CHIPDefaultSuccessCallbackBridge * callback = reinterpret_cast<CHIPDefaultSuccessCallbackBridge *>(context);
        if (callback && callback->mQueue) {
            dispatch_async(callback->mQueue, ^{
                callback->mHandler(nil, nil);
                callback->Cancel();
                delete callback;
            });
        }
    };

private:
    ResponseHandler mHandler;
    dispatch_queue_t mQueue;
};

class CHIPDefaultFailureCallbackBridge : public Callback::Callback<DefaultFailureCallback> {
public:
    CHIPDefaultFailureCallbackBridge(ResponseHandler handler, dispatch_queue_t queue)
        : Callback::Callback<DefaultFailureCallback>(CallbackFn, this)
        , mHandler(handler)
        , mQueue(queue)
    {
    }

    ~CHIPDefaultFailureCallbackBridge() {};

    static void CallbackFn(void * context, uint8_t status)
    {
        CHIPDefaultFailureCallbackBridge * callback = reinterpret_cast<CHIPDefaultFailureCallbackBridge *>(context);
        if (callback && callback->mQueue) {
            dispatch_async(callback->mQueue, ^{
                NSError * error = [NSError errorWithDomain:CHIPErrorDomain
                                                      code:status
                                                  userInfo:@ { NSLocalizedDescriptionKey : @"" }];
                callback->mHandler(error, nil);
                callback->Cancel();
                delete callback;
            });
        }
    };

private:
    ResponseHandler mHandler;
    dispatch_queue_t mQueue;
};

class CHIPUnsupportedAttributeCallbackBridge : public Callback::Callback<DefaultSuccessCallback> {
public:
    CHIPUnsupportedAttributeCallbackBridge(ResponseHandler handler, dispatch_queue_t queue)
        : Callback::Callback<DefaultSuccessCallback>(CallbackFn, this)
    {
    }

    ~CHIPUnsupportedAttributeCallbackBridge() {};

    static void CallbackFn(void * context)
    {
        CHIPUnsupportedAttributeCallbackBridge * callback = reinterpret_cast<CHIPUnsupportedAttributeCallbackBridge *>(context);
        if (callback && callback->mQueue) {
            dispatch_async(callback->mQueue, ^{
                NSError * error = [NSError errorWithDomain:CHIPErrorDomain
                                                      code:CHIPErrorCodeUndefinedError
                                                  userInfo:@ { NSLocalizedDescriptionKey : @"Unsupported attribute type" }];
                callback->mHandler(error, nil);
                callback->Cancel();
                delete callback;
            });
        }
    };

private:
    ResponseHandler mHandler;
    dispatch_queue_t mQueue;
};

class CHIPBooleanAttributeCallbackBridge : public Callback::Callback<BooleanAttributeCallback> {
public:
    CHIPBooleanAttributeCallbackBridge(ResponseHandler handler, dispatch_queue_t queue, bool keepAlive = false)
        : Callback::Callback<BooleanAttributeCallback>(CallbackFn, this)
        , mHandler(handler)
        , mQueue(queue)
        , mKeepAlive(keepAlive)
    {
    }

    ~CHIPBooleanAttributeCallbackBridge() {};

    static void CallbackFn(void * context, bool value)
    {
        CHIPBooleanAttributeCallbackBridge * callback = reinterpret_cast<CHIPBooleanAttributeCallbackBridge *>(context);
        if (callback && callback->mQueue) {
            dispatch_async(callback->mQueue, ^{
                callback->mHandler(nil, @ { @"value" : [NSNumber numberWithBool:value] });
                if (!callback->mKeepAlive) {
                    callback->Cancel();
                    delete callback;
                }
            });
        }
    };

private:
    ResponseHandler mHandler;
    dispatch_queue_t mQueue;
    bool mKeepAlive;
};

class CHIPInt8uAttributeCallbackBridge : public Callback::Callback<Int8uAttributeCallback> {
public:
    CHIPInt8uAttributeCallbackBridge(ResponseHandler handler, dispatch_queue_t queue, bool keepAlive = false)
        : Callback::Callback<Int8uAttributeCallback>(CallbackFn, this)
        , mHandler(handler)
        , mQueue(queue)
        , mKeepAlive(keepAlive)
    {
    }

    ~CHIPInt8uAttributeCallbackBridge() {};

    static void CallbackFn(void * context, uint8_t value)
    {
        CHIPInt8uAttributeCallbackBridge * callback = reinterpret_cast<CHIPInt8uAttributeCallbackBridge *>(context);
        if (callback && callback->mQueue) {
            dispatch_async(callback->mQueue, ^{
                callback->mHandler(nil, @ { @"value" : [NSNumber numberWithUnsignedChar:value] });
                if (!callback->mKeepAlive) {
                    callback->Cancel();
                    delete callback;
                }
            });
        }
    };

private:
    ResponseHandler mHandler;
    dispatch_queue_t mQueue;
    bool mKeepAlive;
};

class CHIPInt8sAttributeCallbackBridge : public Callback::Callback<Int8sAttributeCallback> {
public:
    CHIPInt8sAttributeCallbackBridge(ResponseHandler handler, dispatch_queue_t queue, bool keepAlive = false)
        : Callback::Callback<Int8sAttributeCallback>(CallbackFn, this)
        , mHandler(handler)
        , mQueue(queue)
        , mKeepAlive(keepAlive)
    {
    }

    ~CHIPInt8sAttributeCallbackBridge() {};

    static void CallbackFn(void * context, int8_t value)
    {
        CHIPInt8sAttributeCallbackBridge * callback = reinterpret_cast<CHIPInt8sAttributeCallbackBridge *>(context);
        if (callback && callback->mQueue) {
            dispatch_async(callback->mQueue, ^{
                callback->mHandler(nil, @ { @"value" : [NSNumber numberWithChar:value] });
                if (!callback->mKeepAlive) {
                    callback->Cancel();
                    delete callback;
                }
            });
        }
    };

private:
    ResponseHandler mHandler;
    dispatch_queue_t mQueue;
    bool mKeepAlive;
};

class CHIPInt16uAttributeCallbackBridge : public Callback::Callback<Int16uAttributeCallback> {
public:
    CHIPInt16uAttributeCallbackBridge(ResponseHandler handler, dispatch_queue_t queue, bool keepAlive = false)
        : Callback::Callback<Int16uAttributeCallback>(CallbackFn, this)
        , mHandler(handler)
        , mQueue(queue)
        , mKeepAlive(keepAlive)
    {
    }

    ~CHIPInt16uAttributeCallbackBridge() {};

    static void CallbackFn(void * context, uint16_t value)
    {
        CHIPInt16uAttributeCallbackBridge * callback = reinterpret_cast<CHIPInt16uAttributeCallbackBridge *>(context);
        if (callback && callback->mQueue) {
            dispatch_async(callback->mQueue, ^{
                callback->mHandler(nil, @ { @"value" : [NSNumber numberWithUnsignedShort:value] });
                if (!callback->mKeepAlive) {
                    callback->Cancel();
                    delete callback;
                }
            });
        }
    };

private:
    ResponseHandler mHandler;
    dispatch_queue_t mQueue;
    bool mKeepAlive;
};

class CHIPInt16sAttributeCallbackBridge : public Callback::Callback<Int16sAttributeCallback> {
public:
    CHIPInt16sAttributeCallbackBridge(ResponseHandler handler, dispatch_queue_t queue, bool keepAlive = false)
        : Callback::Callback<Int16sAttributeCallback>(CallbackFn, this)
        , mHandler(handler)
        , mQueue(queue)
        , mKeepAlive(keepAlive)
    {
    }

    ~CHIPInt16sAttributeCallbackBridge() {};

    static void CallbackFn(void * context, int16_t value)
    {
        CHIPInt16sAttributeCallbackBridge * callback = reinterpret_cast<CHIPInt16sAttributeCallbackBridge *>(context);
        if (callback && callback->mQueue) {
            dispatch_async(callback->mQueue, ^{
                callback->mHandler(nil, @ { @"value" : [NSNumber numberWithShort:value] });
                if (!callback->mKeepAlive) {
                    callback->Cancel();
                    delete callback;
                }
            });
        }
    };

private:
    ResponseHandler mHandler;
    dispatch_queue_t mQueue;
    bool mKeepAlive;
};

@interface CHIPCluster ()
@property (readonly, nonatomic) dispatch_queue_t callbackQueue;
- (Controller::ClusterBase *)getCluster;
@end

@implementation CHIPCluster
- (instancetype)initWithDevice:(CHIPDevice *)device endpoint:(EndpointId)endpoint queue:(dispatch_queue_t)queue
{
    if (self = [super init]) {
        Controller::ClusterBase * cppCluster = [self getCluster];
        if (cppCluster == nullptr) {
            return nil;
        }

        CHIP_ERROR err = cppCluster->Associate([device internalDevice], endpoint);
        if (err != CHIP_NO_ERROR) {
            return nil;
        }

        _callbackQueue = queue;
    }
    return self;
}

- (Controller::ClusterBase *)getCluster
{
    return nullptr;
}
@end

@interface CHIPBasic ()
@property (readonly) Controller::BasicCluster cppCluster;
@end

@implementation CHIPBasic

- (Controller::ClusterBase *)getCluster
{
    return &_cppCluster;
}

- (void)resetToFactoryDefaults:(ResponseHandler)completionHandler
{
    CHIPDefaultSuccessCallbackBridge * onSuccess = new CHIPDefaultSuccessCallbackBridge(completionHandler, [self callbackQueue]);
    if (!onSuccess) {
        completionHandler([CHIPError errorForCHIPErrorCode:CHIP_ERROR_INCORRECT_STATE], nil);
        return;
    }

    CHIPDefaultFailureCallbackBridge * onFailure = new CHIPDefaultFailureCallbackBridge(completionHandler, [self callbackQueue]);
    if (!onFailure) {
        delete onSuccess;
        completionHandler([CHIPError errorForCHIPErrorCode:CHIP_ERROR_INCORRECT_STATE], nil);
        return;
    }

    CHIP_ERROR err = self.cppCluster.ResetToFactoryDefaults(onSuccess->Cancel(), onFailure->Cancel());
    if (err != CHIP_NO_ERROR) {
        delete onSuccess;
        delete onFailure;
        completionHandler([CHIPError errorForCHIPErrorCode:err], nil);
    }
}

- (void)readAttributeZclVersion:(ResponseHandler)completionHandler
{
    CHIPInt8uAttributeCallbackBridge * onSuccess = new CHIPInt8uAttributeCallbackBridge(completionHandler, [self callbackQueue]);
    if (!onSuccess) {
        completionHandler([CHIPError errorForCHIPErrorCode:CHIP_ERROR_INCORRECT_STATE], nil);
        return;
    }

    CHIPDefaultFailureCallbackBridge * onFailure = new CHIPDefaultFailureCallbackBridge(completionHandler, [self callbackQueue]);
    if (!onFailure) {
        delete onSuccess;
        completionHandler([CHIPError errorForCHIPErrorCode:CHIP_ERROR_INCORRECT_STATE], nil);
        return;
    }

    CHIP_ERROR err = self.cppCluster.ReadAttributeZclVersion(onSuccess->Cancel(), onFailure->Cancel());
    if (err != CHIP_NO_ERROR) {
        delete onSuccess;
        delete onFailure;
        completionHandler([CHIPError errorForCHIPErrorCode:err], nil);
    }
}

- (void)readAttributePowerSource:(ResponseHandler)completionHandler
{
    CHIPInt8uAttributeCallbackBridge * onSuccess = new CHIPInt8uAttributeCallbackBridge(completionHandler, [self callbackQueue]);
    if (!onSuccess) {
        completionHandler([CHIPError errorForCHIPErrorCode:CHIP_ERROR_INCORRECT_STATE], nil);
        return;
    }

    CHIPDefaultFailureCallbackBridge * onFailure = new CHIPDefaultFailureCallbackBridge(completionHandler, [self callbackQueue]);
    if (!onFailure) {
        delete onSuccess;
        completionHandler([CHIPError errorForCHIPErrorCode:CHIP_ERROR_INCORRECT_STATE], nil);
        return;
    }

    CHIP_ERROR err = self.cppCluster.ReadAttributePowerSource(onSuccess->Cancel(), onFailure->Cancel());
    if (err != CHIP_NO_ERROR) {
        delete onSuccess;
        delete onFailure;
        completionHandler([CHIPError errorForCHIPErrorCode:err], nil);
    }
}

- (void)readAttributeClusterRevision:(ResponseHandler)completionHandler
{
    CHIPInt16uAttributeCallbackBridge * onSuccess = new CHIPInt16uAttributeCallbackBridge(completionHandler, [self callbackQueue]);
    if (!onSuccess) {
        completionHandler([CHIPError errorForCHIPErrorCode:CHIP_ERROR_INCORRECT_STATE], nil);
        return;
    }

    CHIPDefaultFailureCallbackBridge * onFailure = new CHIPDefaultFailureCallbackBridge(completionHandler, [self callbackQueue]);
    if (!onFailure) {
        delete onSuccess;
        completionHandler([CHIPError errorForCHIPErrorCode:CHIP_ERROR_INCORRECT_STATE], nil);
        return;
    }

    CHIP_ERROR err = self.cppCluster.ReadAttributeClusterRevision(onSuccess->Cancel(), onFailure->Cancel());
    if (err != CHIP_NO_ERROR) {
        delete onSuccess;
        delete onFailure;
        completionHandler([CHIPError errorForCHIPErrorCode:err], nil);
    }
}

@end

@interface CHIPTemperatureMeasurement ()
@property (readonly) Controller::TemperatureMeasurementCluster cppCluster;
@end

@implementation CHIPTemperatureMeasurement

- (Controller::ClusterBase *)getCluster
{
    return &_cppCluster;
}

- (void)readAttributeMeasuredValue:(ResponseHandler)completionHandler
{
    CHIPInt16sAttributeCallbackBridge * onSuccess = new CHIPInt16sAttributeCallbackBridge(completionHandler, [self callbackQueue]);
    if (!onSuccess) {
        completionHandler([CHIPError errorForCHIPErrorCode:CHIP_ERROR_INCORRECT_STATE], nil);
        return;
    }

    CHIPDefaultFailureCallbackBridge * onFailure = new CHIPDefaultFailureCallbackBridge(completionHandler, [self callbackQueue]);
    if (!onFailure) {
        delete onSuccess;
        completionHandler([CHIPError errorForCHIPErrorCode:CHIP_ERROR_INCORRECT_STATE], nil);
        return;
    }

    CHIP_ERROR err = self.cppCluster.ReadAttributeMeasuredValue(onSuccess->Cancel(), onFailure->Cancel());
    if (err != CHIP_NO_ERROR) {
        delete onSuccess;
        delete onFailure;
        completionHandler([CHIPError errorForCHIPErrorCode:err], nil);
    }
}

- (void)configureAttributeMeasuredValue:(uint16_t)minInterval
                            maxInterval:(uint16_t)maxInterval
                                 change:(int16_t)change
                      completionHandler:(ResponseHandler)completionHandler
{
    CHIPDefaultSuccessCallbackBridge * onSuccess = new CHIPDefaultSuccessCallbackBridge(completionHandler, [self callbackQueue]);
    if (!onSuccess) {
        completionHandler([CHIPError errorForCHIPErrorCode:CHIP_ERROR_INCORRECT_STATE], nil);
        return;
    }

    CHIPDefaultFailureCallbackBridge * onFailure = new CHIPDefaultFailureCallbackBridge(completionHandler, [self callbackQueue]);
    if (!onFailure) {
        delete onSuccess;
        completionHandler([CHIPError errorForCHIPErrorCode:CHIP_ERROR_INCORRECT_STATE], nil);
        return;
    }

    CHIP_ERROR err = self.cppCluster.ConfigureAttributeMeasuredValue(
        onSuccess->Cancel(), onFailure->Cancel(), minInterval, maxInterval, change);
    if (err != CHIP_NO_ERROR) {
        delete onSuccess;
        delete onFailure;
        completionHandler([CHIPError errorForCHIPErrorCode:err], nil);
    }
}

- (void)reportAttributeMeasuredValue:(ResponseHandler)reportHandler
{
    CHIPInt16sAttributeCallbackBridge * onReport = new CHIPInt16sAttributeCallbackBridge(reportHandler, [self callbackQueue], true);
    if (!onReport) {
        reportHandler([CHIPError errorForCHIPErrorCode:CHIP_ERROR_INCORRECT_STATE], nil);
        return;
    }

    CHIP_ERROR err = self.cppCluster.ReportAttributeMeasuredValue(onReport->Cancel());
    if (err != CHIP_NO_ERROR) {
        delete onReport;
        reportHandler([CHIPError errorForCHIPErrorCode:err], nil);
    }
}

- (void)readAttributeMinMeasuredValue:(ResponseHandler)completionHandler
{
    CHIPInt16sAttributeCallbackBridge * onSuccess = new CHIPInt16sAttributeCallbackBridge(completionHandler, [self callbackQueue]);
    if (!onSuccess) {
        completionHandler([CHIPError errorForCHIPErrorCode:CHIP_ERROR_INCORRECT_STATE], nil);
        return;
    }

    CHIPDefaultFailureCallbackBridge * onFailure = new CHIPDefaultFailureCallbackBridge(completionHandler, [self callbackQueue]);
    if (!onFailure) {
        delete onSuccess;
        completionHandler([CHIPError errorForCHIPErrorCode:CHIP_ERROR_INCORRECT_STATE], nil);
        return;
    }

    CHIP_ERROR err = self.cppCluster.ReadAttributeMinMeasuredValue(onSuccess->Cancel(), onFailure->Cancel());
    if (err != CHIP_NO_ERROR) {
        delete onSuccess;
        delete onFailure;
        completionHandler([CHIPError errorForCHIPErrorCode:err], nil);
    }
}

- (void)readAttributeMaxMeasuredValue:(ResponseHandler)completionHandler
{
    CHIPInt16sAttributeCallbackBridge * onSuccess = new CHIPInt16sAttributeCallbackBridge(completionHandler, [self callbackQueue]);
    if (!onSuccess) {
        completionHandler([CHIPError errorForCHIPErrorCode:CHIP_ERROR_INCORRECT_STATE], nil);
        return;
    }

    CHIPDefaultFailureCallbackBridge * onFailure = new CHIPDefaultFailureCallbackBridge(completionHandler, [self callbackQueue]);
    if (!onFailure) {
        delete onSuccess;
        completionHandler([CHIPError errorForCHIPErrorCode:CHIP_ERROR_INCORRECT_STATE], nil);
        return;
    }

    CHIP_ERROR err = self.cppCluster.ReadAttributeMaxMeasuredValue(onSuccess->Cancel(), onFailure->Cancel());
    if (err != CHIP_NO_ERROR) {
        delete onSuccess;
        delete onFailure;
        completionHandler([CHIPError errorForCHIPErrorCode:err], nil);
    }
}

- (void)readAttributeClusterRevision:(ResponseHandler)completionHandler
{
    CHIPInt16uAttributeCallbackBridge * onSuccess = new CHIPInt16uAttributeCallbackBridge(completionHandler, [self callbackQueue]);
    if (!onSuccess) {
        completionHandler([CHIPError errorForCHIPErrorCode:CHIP_ERROR_INCORRECT_STATE], nil);
        return;
    }

    CHIPDefaultFailureCallbackBridge * onFailure = new CHIPDefaultFailureCallbackBridge(completionHandler, [self callbackQueue]);
    if (!onFailure) {
        delete onSuccess;
        completionHandler([CHIPError errorForCHIPErrorCode:CHIP_ERROR_INCORRECT_STATE], nil);
        return;
    }

    CHIP_ERROR err = self.cppCluster.ReadAttributeClusterRevision(onSuccess->Cancel(), onFailure->Cancel());
    if (err != CHIP_NO_ERROR) {
        delete onSuccess;
        delete onFailure;
        completionHandler([CHIPError errorForCHIPErrorCode:err], nil);
    }
}

@end
