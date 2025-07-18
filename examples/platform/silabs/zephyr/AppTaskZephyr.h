/*
 *
 *    Copyright (c) 2025 Project CHIP Authors
 *    All rights reserved.
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

#pragma once

#include "AppTaskBase.h"

namespace chip::Zephyr::App {

// This class implements the AppTaskBase interface for Zephyr platform.
// In the future a cleanup will be needed to unify Silicon Labs offerings
class AppTaskZephyr : public AppTaskBase
{
public:
    virtual ~AppTaskZephyr() = default;

    /**
     * \brief Entry point of AppTaskBase.
     *
     * This function:
     *              - Init matter stack and other needed components
     *              - Dispatch event coming from the app event queue
     *
     */
    virtual void Start() override;

    /**
     * \brief Send event to the event queue.
     *
     */
    void PostEvent(const AppEvent & event) override;

    /**
     * \brief Return a pointer to the Wifi Driver instance.
     *
     * \return Wifi Driver instance pointer
     */
#if defined(CONFIG_CHIP_WIFI)
    virtual chip::DeviceLayer::NetworkCommissioning::WiFiDriver * GetWifiDriverInstance(void) override;
#endif

    /**
     * \brief This function registers applicative features such as custom CLI commands
     *
     * \return CHIP_ERROR
     *
     */
    virtual CHIP_ERROR AppMatter_Register(void) override;

private:
    void DispatchEvent(const AppEvent & event);
};

} // namespace chip::Zephyr::App
