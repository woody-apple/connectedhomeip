/*
 *
 *    Copyright (c) 2020 Project CHIP Authors
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
#include <DeviceInfoProviderImpl.h>
#include <OTAConfig.h>
#include <app/server/Dnssd.h>
#include <app/server/Server.h>
#include <credentials/DeviceAttestationCredsProvider.h>
#include <credentials/examples/DeviceAttestationCredsExample.h>
#include <data-model-providers/codegen/Instance.h>
#include <platform/bouffalolab/common/PlatformManagerImpl.h>
#include <setup_payload/OnboardingCodesUtil.h>
#include <system/SystemClock.h>

#if HEAP_MONITORING
#include <MemMonitoring.h>
#endif

#if CONFIG_ENABLE_CHIP_SHELL
#include <ChipShellCollection.h>
#include <lib/shell/Engine.h>
#endif

#if PW_RPC_ENABLED
#include <PigweedLogger.h>
#include <Rpc.h>
#endif
#if CONFIG_ENABLE_CHIP_SHELL || PW_RPC_ENABLED
#include <uart.h>
#endif

#if CONFIG_BOUFFALOLAB_FACTORY_DATA_ENABLE
#include <platform/bouffalolab/common/FactoryDataProvider.h>
#endif

#if CHIP_DEVICE_CONFIG_ENABLE_WIFI
#include <NetworkCommissioningDriver.h>
#endif

#if CHIP_DEVICE_CONFIG_ENABLE_THREAD
#include <platform/OpenThread/GenericNetworkCommissioningThreadDriver.h>
#include <platform/OpenThread/OpenThreadUtils.h>
#include <platform/ThreadStackManager.h>
#include <platform/bouffalolab/common/ThreadStackManagerImpl.h>
#include <utils_list.h>
#endif

#if CHIP_DEVICE_CONFIG_ENABLE_ETHERNET || CHIP_DEVICE_CONFIG_ENABLE_WIFI
#include <bl_route_hook.h>
#include <lwip/netif.h>
#if CHIP_DEVICE_CONFIG_ENABLE_WIFI && CHIP_DEVICE_LAYER_TARGET_BL602
#include <wifi_mgmr_ext.h>
#endif
#if CHIP_DEVICE_CONFIG_ENABLE_WIFI && CHIP_DEVICE_LAYER_TARGET_BL702
#include <platform/bouffalolab/BL702/wifi_mgmr_portable.h>
#endif
#if CHIP_DEVICE_CONFIG_ENABLE_ETHERNET
#include <platform/bouffalolab/BL702/EthernetInterface.h>
#endif
#endif

#include <app/clusters/network-commissioning/network-commissioning.h>

#include <AppTask.h>
#include <plat.h>

using namespace ::chip;
using namespace ::chip::app;
using namespace ::chip::Credentials;
using namespace ::chip::DeviceLayer;

#if CHIP_DEVICE_CONFIG_ENABLE_WIFI
namespace {
chip::app::Clusters::NetworkCommissioning::Instance
    sWiFiNetworkCommissioningInstance(0 /* Endpoint Id */, &(NetworkCommissioning::BLWiFiDriver::GetInstance()));
}
#endif

#if CHIP_DEVICE_CONFIG_ENABLE_THREAD
Clusters::NetworkCommissioning::InstanceAndDriver<NetworkCommissioning::GenericThreadDriver> sThreadNetworkDriver(0 /*endpointId*/);
#endif // CHIP_DEVICE_CONFIG_ENABLE_THREAD

#if CONFIG_BOUFFALOLAB_FACTORY_DATA_ENABLE
namespace {
FactoryDataProvider sFactoryDataProvider;
}
#endif

static chip::DeviceLayer::DeviceInfoProviderImpl gExampleDeviceInfoProvider;

void ChipEventHandler(const ChipDeviceEvent * event, intptr_t arg)
{
    switch (event->Type)
    {
    case DeviceEventType::kCHIPoBLEAdvertisingChange:
        ChipLogProgress(NotSpecified, "BLE adv changed, connection number: %d", ConnectivityMgr().NumBLEConnections());
        break;
#if CHIP_DEVICE_CONFIG_ENABLE_THREAD
    case DeviceEventType::kThreadStateChange:

        if (ConnectivityMgr().IsThreadAttached())
        {
            chip::DeviceLayer::SystemLayer().StartTimer(chip::System::Clock::Seconds32(OTAConfig::kInitOTARequestorDelaySec),
                                                        OTAConfig::InitOTARequestorHandler, nullptr);
        }
        break;
#endif

#if CHIP_DEVICE_CONFIG_ENABLE_WIFI || CHIP_DEVICE_CONFIG_ENABLE_ETHERNET
    case DeviceEventType::kInterfaceIpAddressChanged:
        if ((event->InterfaceIpAddressChanged.Type == InterfaceIpChangeType::kIpV4_Assigned) ||
            (event->InterfaceIpAddressChanged.Type == InterfaceIpChangeType::kIpV6_Assigned))
        {
            // MDNS server restart on any ip assignment: if link local ipv6 is configured, that
            // will not trigger a 'internet connectivity change' as there is no internet
            // connectivity. MDNS still wants to refresh its listening interfaces to include the
            // newly selected address.

            chip::app::DnssdServer::Instance().StartServer();

            bl_route_hook_init();

            chip::DeviceLayer::SystemLayer().StartTimer(chip::System::Clock::Seconds32(OTAConfig::kInitOTARequestorDelaySec),
                                                        OTAConfig::InitOTARequestorHandler, nullptr);
        }
        break;
#endif
    case DeviceEventType::kInternetConnectivityChange:
        if (event->InternetConnectivityChange.IPv4 == kConnectivity_Established)
        {
            ChipLogProgress(NotSpecified, "IPv4 connectivity ready...");
        }
        else if (event->InternetConnectivityChange.IPv4 == kConnectivity_Lost)
        {
            ChipLogProgress(NotSpecified, "Lost IPv4 connectivity...");
        }
        if (event->InternetConnectivityChange.IPv6 == kConnectivity_Established)
        {
            ChipLogProgress(NotSpecified, "IPv6 connectivity ready...");
        }
        else if (event->InternetConnectivityChange.IPv6 == kConnectivity_Lost)
        {
            ChipLogProgress(NotSpecified, "Lost IPv6 connectivity...");
        }
        break;
    case DeviceEventType::kCHIPoBLEConnectionEstablished:
        ChipLogProgress(NotSpecified, "BLE connection established");
        break;
    case DeviceEventType::kCHIPoBLEConnectionClosed:
        ChipLogProgress(NotSpecified, "BLE disconnected");
        break;
    case DeviceEventType::kCommissioningComplete:
        ChipLogProgress(NotSpecified, "Commissioning complete");
        GetAppTask().PostEvent(AppTask::APP_EVENT_COMMISSION_COMPLETE);
        break;
    default:
        break;
    }
}

#if CHIP_DEVICE_CONFIG_ENABLE_THREAD
void LockOpenThreadTask(void)
{
    chip::DeviceLayer::ThreadStackMgr().LockThreadStack();
}

void UnlockOpenThreadTask(void)
{
    chip::DeviceLayer::ThreadStackMgr().UnlockThreadStack();
}
#endif

#if CONFIG_APP_ADVERTISE_COMMISSIONABLE_ON_LAST_FABRIC_REMOVAL
class AppFabricTableDelegate : public FabricTable::Delegate
{
    void OnFabricRemoved(const FabricTable & fabricTable, FabricIndex fabricIndex)
    {
        if (chip::Server::GetInstance().GetFabricTable().FabricCount() == 0)
        {
            ChipLogProgress(DeviceLayer, "Performing erasing of settings partition");
            PlatformMgr().ScheduleWork([](intptr_t) {
#if CHIP_DEVICE_CONFIG_ENABLE_THREAD
                ConfigurationManagerImpl::GetDefaultInstance().ClearThreadStack();
                ThreadStackMgrImpl().FactoryResetThreadStack();
                ThreadStackMgr().InitThreadStack();
#endif // CHIP_DEVICE_CONFIG_ENABLE_THREAD

#if CHIP_DEVICE_CONFIG_ENABLE_WIFI_STATION
                ChipLogProgress(DeviceLayer, "Clearing WiFi provision");
                chip::DeviceLayer::ConnectivityMgr().ClearWiFiStationProvision();
#endif // CHIP_DEVICE_CONFIG_ENABLE_WIFI_STATION

                CHIP_ERROR err = Server::GetInstance().GetCommissioningWindowManager().OpenBasicCommissioningWindow();
                if (err != CHIP_NO_ERROR)
                {
                    ChipLogError(AppServer, "Failed to open the Basic Commissioning Window");
                }
            });
        }
    }
};
#endif

CHIP_ERROR PlatformManagerImpl::PlatformInit(void)
{
    chip::RendezvousInformationFlags rendezvousMode(chip::RendezvousInformationFlag::kOnNetwork);
#if CONFIG_BOUFFALOLAB_FACTORY_DATA_ENABLE
    CHIP_ERROR retFactoryData = sFactoryDataProvider.Init();
#endif

#if PW_RPC_ENABLED
    PigweedLogger::pw_init();
#elif CONFIG_ENABLE_CHIP_SHELL
    AppTask::StartAppShellTask();
#endif

#if HEAP_MONITORING
    MemMonitoring::startHeapMonitoring();
#endif

    ChipLogProgress(NotSpecified, "Initializing CHIP stack");
    ReturnLogErrorOnFailure(PlatformMgr().InitChipStack());

    chip::DeviceLayer::ConnectivityMgr().SetBLEDeviceName(CHIP_BLE_DEVICE_NAME);

#if CHIP_DEVICE_CONFIG_ENABLE_THREAD

#if CONFIG_ENABLE_CHIP_SHELL
    cmd_otcli_init();
#endif

    ChipLogProgress(NotSpecified, "Initializing OpenThread stack");
    ReturnLogErrorOnFailure(ThreadStackMgr().InitThreadStack());

#if CHIP_DEVICE_CONFIG_THREAD_FTD
    ReturnLogErrorOnFailure(ConnectivityMgr().SetThreadDeviceType(ConnectivityManager::kThreadDeviceType_Router));
#else
#if CHIP_CONFIG_ENABLE_ICD_SERVER
    ReturnErrorOnFailure(ConnectivityMgr().SetThreadDeviceType(ConnectivityManager::kThreadDeviceType_SleepyEndDevice));
#else
    ReturnErrorOnFailure(ConnectivityMgr().SetThreadDeviceType(ConnectivityManager::kThreadDeviceType_MinimalEndDevice));
#endif
#endif
    sThreadNetworkDriver.Init();
#endif

#if CHIP_DEVICE_CONFIG_ENABLE_WIFI
    ReturnLogErrorOnFailure(sWiFiNetworkCommissioningInstance.Init());
#endif

    // Initialize device attestation config
#if CONFIG_BOUFFALOLAB_FACTORY_DATA_ENABLE
    if (CHIP_NO_ERROR == retFactoryData)
    {
        SetDeviceInstanceInfoProvider(&sFactoryDataProvider);
        SetDeviceAttestationCredentialsProvider(&sFactoryDataProvider);
        SetCommissionableDataProvider(&sFactoryDataProvider);
    }
    else
    {
        ChipLogError(NotSpecified, "factory data provider is failed to initialize, use example DAC provider.");
        SetDeviceAttestationCredentialsProvider(Examples::GetExampleDACProvider());
    }
#else
    SetDeviceAttestationCredentialsProvider(Examples::GetExampleDACProvider());
#endif

    chip::DeviceLayer::PlatformMgr().LockChipStack();

#if CHIP_DEVICE_CONFIG_ENABLE_EXTENDED_DISCOVERY
    chip::app::DnssdServer::Instance().SetExtendedDiscoveryTimeoutSecs(EXT_DISCOVERY_TIMEOUT_SECS);
#endif

    chip::DeviceLayer::SetDeviceInfoProvider(&gExampleDeviceInfoProvider);

    static CommonCaseDeviceServerInitParams initParams;
    (void) initParams.InitializeStaticResourcesBeforeServerInit();
    initParams.dataModelProvider = CodegenDataModelProviderInstance(initParams.persistentStorageDelegate);

#if CHIP_DEVICE_CONFIG_ENABLE_THREAD
    chip::Inet::EndPointStateOpenThread::OpenThreadEndpointInitParam nativeParams;
    nativeParams.lockCb                = LockOpenThreadTask;
    nativeParams.unlockCb              = UnlockOpenThreadTask;
    nativeParams.openThreadInstancePtr = chip::DeviceLayer::ThreadStackMgrImpl().OTInstance();
    initParams.endpointNativeParams    = static_cast<void *>(&nativeParams);
#endif

    ReturnLogErrorOnFailure(chip::Server::GetInstance().Init(initParams));

    gExampleDeviceInfoProvider.SetStorageDelegate(&chip::Server::GetInstance().GetPersistentStorage());

#if CONFIG_APP_ADVERTISE_COMMISSIONABLE_ON_LAST_FABRIC_REMOVAL
    static AppFabricTableDelegate sAppFabricDelegate;
    chip::Server::GetInstance().GetFabricTable().AddFabricDelegate(&sAppFabricDelegate);
#endif

    chip::DeviceLayer::PlatformMgr().UnlockChipStack();

#if CHIP_DEVICE_CONFIG_ENABLE_THREAD
    ChipLogProgress(NotSpecified, "Starting OpenThread task");
    // Start OpenThread task
    ReturnLogErrorOnFailure(ThreadStackMgrImpl().StartThreadTask());
#endif

    ConfigurationMgr().LogDeviceConfig();

#if CHIP_DEVICE_CONFIG_ENABLE_CHIPOBLE
    rendezvousMode.Set(chip::RendezvousInformationFlag::kBLE);
#endif
    PrintOnboardingCodes(rendezvousMode);

    PlatformMgr().AddEventHandler(ChipEventHandler, 0);

#if PW_RPC_ENABLED
    chip::rpc::Init();
#endif

    vTaskResume(GetAppTask().sAppTaskHandle);

    return CHIP_NO_ERROR;
}
