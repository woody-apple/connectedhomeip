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

/**
 * TODO: Remove and include top level header
 */
@class MTRSetupPayload;

NS_ASSUME_NONNULL_BEGIN

/**
 * TODO: Add documentation
 *
 * Switch tp class method on MTRSetupPayload?
 *  + (MTRSetupPayload *)setupPayloadWithDecimalStringRepresentation:(NSString *)decimalStringRepresentation error:(NSError * __autoreleasing *)error;
 * 
 * Make this private and remove from here. Clients use MTROnboardingPayloadParser
 */
@interface MTRManualSetupPayloadParser : NSObject
- (instancetype)initWithDecimalStringRepresentation:(NSString *)decimalStringRepresentation;
- (nullable MTRSetupPayload *)populatePayload:(NSError * __autoreleasing *)error;
@end

NS_ASSUME_NONNULL_END
