/**
 *
 *    Copyright (c) 2022 Project CHIP Authors
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
#import <Matter/Matter.h>

NS_ASSUME_NONNULL_BEGIN

// TODO: The prefix names of the delegates need to be consistent

// TODO: Document which queue the callback blocks run on or add a queue to the delegate methods

// TODO: Fix inconsistent OTA vs Ota

// TODO: Re-review with Carol

// TODO: Remove _Nonnull since header assumes so
/**
 * The protocol definition for the MTROTAProviderDelegate
 *
 * All delegate methods will be called on the supplied Delegate Queue.
 */
@protocol MTROTAProviderDelegate <NSObject>
@required
/**
 * Notify the delegate when the query image command is received from some node.
 * The controller identifies the fabric the node is on, and the nodeID
 * identifies the node within that fabric.
 */
- (void)handleQueryImageForNodeID:(NSNumber *)nodeID
                       controller:(MTRDeviceController *)controller
                           params:(MTROtaSoftwareUpdateProviderClusterQueryImageParams *)params
                completionHandler:(void (^)(MTROtaSoftwareUpdateProviderClusterQueryImageResponseParams * _Nullable data,
                                      NSError * _Nullable error))completionHandler;

/**
 * Notify the delegate when the apply update request command is received from
 * some node.  The controller identifies the fabric the node is on, and the
 * nodeID identifies the node within that fabric.
 */
- (void)handleApplyUpdateRequestForNodeID:(NSNumber *)nodeID
                               controller:(MTRDeviceController *)controller
                                   params:(MTROtaSoftwareUpdateProviderClusterApplyUpdateRequestParams *)params
                        completionHandler:(void (^)(MTROtaSoftwareUpdateProviderClusterApplyUpdateResponseParams * _Nullable data,
                                              NSError * _Nullable error))completionHandler;

/**
 * Notify the delegate when the notify update applied command is received from
 * some node.  The controller identifies the fabric the node is on, and the
 * nodeID identifies the node within that fabric.
 */
// TODO: StatusCompletion => MTRStatusCompletion ?
// MTRCluster.h: Fix prefixes in the header that are missing prefixes as well
- (void)handleNotifyUpdateAppliedForNodeID:(NSNumber *)nodeID
                                controller:(MTRDeviceController *)controller
                                    params:(MTROtaSoftwareUpdateProviderClusterNotifyUpdateAppliedParams *)params
                         completionHandler:(StatusCompletion)completionHandler;

/**
 * Notify the delegate when a BDX Session starts for some node.  The controller
 * identifies the fabric the node is on, and the nodeID identifies the node
 * within that fabric.
 */
- (void)handleBDXTransferSessionBeginForNodeID:(NSNumber *)nodeID
                                    controller:(MTRDeviceController *)controller
                                fileDesignator:(NSString *)fileDesignator
                                        offset:(NSNumber *)offset
                             completionHandler:(void (^)(NSError * error))completionHandler;

/**
 * Notify the delegate when a BDX Session ends for some node.  The controller
 * identifies the fabric the node is on, and the nodeID identifies the node
 * within that fabric.
 */
- (void)handleBDXTransferSessionEndForNodeID:(NSNumber *)nodeID
                                  controller:(MTRDeviceController *)controller
                                       error:(NSError * _Nullable)error;

/**
 * Notify the delegate when a BDX Query message has been received for some node.
 * The controller identifies the fabric the node is on, and the nodeID
 * identifies the node within that fabric.
 */
- (void)handleBDXQueryForNodeID:(NSNumber *)nodeID
                     controller:(MTRDeviceController *)controller
                      blockSize:(NSNumber *)blockSize
                     blockIndex:(NSNumber *)blockIndex
                    bytesToSkip:(NSNumber *)bytesToSkip
              completionHandler:(void (^)(NSData * _Nullable data, BOOL isEOF))completionHandler;
@end

NS_ASSUME_NONNULL_END
