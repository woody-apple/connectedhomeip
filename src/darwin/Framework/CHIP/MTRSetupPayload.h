/**
 *
 *    Copyright (c) 2020 Project CHIP Authors
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

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN


typedef NS_ENUM(NSUInteger, MTRRendezvousInformationFlags) {
    // TODO: Rename None to Unknown to mark that the discovery is not known
    MTRRendezvousInformationNone = 0, // Device does not support any method for rendezvous
    MTRRendezvousInformationSoftAP = 1 << 0, // Device supports WiFi softAP
    MTRRendezvousInformationBLE = 1 << 1, // Device supports BLE
    MTRRendezvousInformationOnNetwork = 1 << 2, // Device supports On Network setup

    MTRRendezvousInformationAllMask
    = MTRRendezvousInformationSoftAP | MTRRendezvousInformationBLE | MTRRendezvousInformationOnNetwork,
};

typedef NS_ENUM(NSUInteger, MTRCommissioningFlow) {
    MTRCommissioningFlowStandard = 0, // Device automatically enters pairing mode upon power-up
    MTRCommissioningFlowUserActionRequired = 1, // Device requires a user interaction to enter pairing mode
    MTRCommissioningFlowCustom = 2, // Commissioning steps should be retrieved from the distributed compliance ledger
    MTRCommissioningFlowInvalid = 3,
};

typedef NS_ENUM(NSUInteger, MTROptionalQRCodeInfoType) {
    MTROptionalQRCodeInfoTypeUnknown,
    MTROptionalQRCodeInfoTypeString,
    MTROptionalQRCodeInfoTypeInt32
};

// TODO: Documentation for objects

@interface MTROptionalQRCodeInfo : NSObject
// TODO: Switch to MTROptionalQRCodeInfoType instead of NSNumber for infoType
@property (nonatomic, copy) NSNumber * infoType;
@property (nonatomic, copy) NSNumber * tag;

// TODO: Mark both as nullable and specify that they are mutually exclusive
@property (nonatomic, copy) NSNumber * integerValue;
@property (nonatomic, copy) NSString * stringValue;
@end

@interface MTRSetupPayload : NSObject

@property (nonatomic, copy) NSNumber * version;
@property (nonatomic, copy) NSNumber * vendorID;
@property (nonatomic, copy) NSNumber * productID;
@property (nonatomic, assign) MTRCommissioningFlow commissioningFlow;
// TODO: See TODO in MTRRendezvousInformationFlags
// Make sure to always set this to MTRRendezvousInformationOnNetwork if the value post parsing QR Code evaluates to 0 (in implementation)
@property (nonatomic, assign) MTRRendezvousInformationFlags rendezvousInformation;
@property (nonatomic, copy) NSNumber * discriminator;
@property (nonatomic, assign) BOOL hasShortDiscriminator;
// TODO: Rename to setupPasscode
@property (nonatomic, copy) NSNumber * setUpPINCode;

// TODO: Make this nullable
@property (nonatomic, copy) NSString * serialNumber;
- (nullable NSArray<MTROptionalQRCodeInfo *> *)getAllOptionalVendorData:(NSError * __autoreleasing *)error;

/**
 * Generate a random Matter-valid setup PIN.
 */
// TODO: Make this a NSNumber and rename to generateRandomSetupPasscode
+ (NSUInteger)generateRandomPIN;
@end

NS_ASSUME_NONNULL_END
