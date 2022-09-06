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

/*
 * TODO: Rename MTRDevicePairingDelegate => MTRDeviceControllerDelegate
 * MTRPairingStatus => MTRCommissioningStatus
 * Rename header as well
 */

typedef NS_ENUM(NSUInteger, MTRPairingStatus) {
    MTRPairingStatusUnknown = 0,
    MTRPairingStatusSuccess = 1,
    MTRPairingStatusFailed = 2,
    MTRPairingStatusDiscoveringMoreDevices = 3
};
/*
 * TODO: Change signature of all delegates to take object as first argument
 */

/**
 * The protocol definition for the MTRDevicePairingDelegate
 *
 * All delegate methods will be called on the supplied Delegate Queue.
 */
@protocol MTRDevicePairingDelegate <NSObject>
@optional

/**
 * TODO: Suggested delegate callbacks
 * // (PASE established)
 * - (void)deviceController:(MTRDeviceController *)deviceController
 *   establishedCommissioningSessionForNewNodeID:(NSNumber *)newNodeID;
 * - (void)deviceController:(MTRDeviceController *)deviceController
      failedToEstablishCommissioningSessionForNewNodeID:(NSNumber *)newNodeID
       error:(NSError *)error;

     // Device Commissioning
 * - (void)deviceController:(MTRDeviceController *)deviceController
 *    commissionedNodeWithID:(NSNumber *)nodeID;

 * - (void)deviceController:(MTRDeviceController *)deviceController
 *    failedToCommissionNodeWithID:(NSNumber *)nodeID;
 *     error:(NSError * _Nullable)error;
 */


/**
 * Notify the delegate when pairing status gets updated
 *
 */
//- (void)onStatusUpdate:(MTRPairingStatus)status;

/**
 * Notify the delegate when pairing is completed
 *
 */
//- (void)onPairingComplete:(nullable NSError *)error;

/**
 * Notify the delegate when commissioning is completed
 *
 */
//- (void)onCommissioningComplete:(nullable NSError *)error;

/**
 * TODO: Remove this delegate
*/
/**
 * Notify the delegate when pairing is deleted
 *
 */
- (void)onPairingDeleted:(nullable NSError *)error;

@end

NS_ASSUME_NONNULL_END
