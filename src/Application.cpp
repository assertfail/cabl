/*----------------------------------------------------------------------------------------------------------------------

                 %%%%%%%%%%%%%%%%%
                 %%%%%%%%%%%%%%%%%
                 %%%           %%%
                 %%%           %%%
                 %%%           %%%
%%%%%%%%%%%%%%%%%%%%           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% www.shaduzlabs.com %%%%%

------------------------------------------------------------------------------------------------------------------------

  Copyright (C) 2014 Vincenzo Pacella

  This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
  License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later
  version.

  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along with this program.
  If not, see <http://www.gnu.org/licenses/>.

----------------------------------------------------------------------------------------------------------------------*/

#include "Application.h"
#include <thread> //remove and use a custom sleep function!
#include <iostream>

#include "k-io.h"
#include "devices/DeviceMaschineMK1.h"
#include "devices/DeviceMaschineMikroMK2.h"

namespace
{
unsigned kAppSleepBeforeNextDeviceSearch = 5; // seconds
}

namespace sl
{
namespace kio
{

Application::Application(const Driver::tCollDeviceDescriptor& collSupportedDevices_)
: m_collSupportedDevices(collSupportedDevices_)
{
  std::cout << "k-IO Version " << Lib::getVersion() << std::endl;
  m_collKnownDevices.emplace_back(0x17CC, 0x0808, Device::Type::MaschineMk1); // MK1
  m_collKnownDevices.emplace_back(0x17CC, 0x1140, Device::Type::MaschineMk2); // MK2
  m_collKnownDevices.emplace_back(0x17CC, 0x1110, Device::Type::MaschineMikroMk1); // Mikro MK1
  m_collKnownDevices.emplace_back(0x17CC, 0x1200, Device::Type::MaschineMikroMk2); // Mikro MK2
}

//----------------------------------------------------------------------------------------------------------------------

void Application::run()
{
  m_appStopped = false;

  while (!m_appStopped)
  {
    // get the list of devices
    m_connected = false;
    if (connect(enumerateDevices())) // found known devices
    {
      m_connected = true;
      initHardware();
      while (m_connected)
      {
        tick();
      }
    }
    std::this_thread::sleep_for(std::chrono::seconds(kAppSleepBeforeNextDeviceSearch));
  }
}

//----------------------------------------------------------------------------------------------------------------------

Driver::tCollDeviceDescriptor Application::enumerateDevices()
{
  Driver::tCollDeviceDescriptor devicesList;
#if defined(_WIN32) || defined(__APPLE__) || defined(__linux)
  for (const auto& deviceDescriptor : getDriver(Driver::tDriver::HIDAPI)->enumerate())
  {
    if (!isKnownDevice(deviceDescriptor))
    {
      continue; // not a Native Instruments USB device
    }
    devicesList.push_back(deviceDescriptor);
  }

  Driver::tDriver tMainDriver(Driver::tDriver::LIBUSB);
#endif

  for (const auto& deviceDescriptor : getDriver(tMainDriver)->enumerate())
  {
    if((!isKnownDevice(deviceDescriptor)) ||
       (std::find(devicesList.begin(), devicesList.end(), deviceDescriptor)!=devicesList.end()))
    {
      continue; // not a Native Instruments USB device
    }
    devicesList.push_back(deviceDescriptor);
  }

  return devicesList;
}

//----------------------------------------------------------------------------------------------------------------------

bool Application::connect(Driver::tCollDeviceDescriptor devicesList_)
{
  m_connected = false;
  if (devicesList_.empty())
  {
    return false;
  }
  for (const auto& d : devicesList_)
  {
    if(isSupportedDevice(d))
    {
      tPtr<DeviceHandle> pDeviceHandle = getDriver(Driver::tDriver::HIDAPI)->connect(d);
      if(pDeviceHandle)
      {
        m_collDevices.emplace_back(new DeviceMaschineMikroMK2(std::move(pDeviceHandle)));
        m_connected = true;
      }
    }
    else
    {
      // Device is known but unsupported by the current Application
    }
  }

  return m_connected;
}

//----------------------------------------------------------------------------------------------------------------------

Driver* Application::getDriver(Driver::tDriver tDriver_)
{
  if (m_collDrivers.find(tDriver_) == m_collDrivers.end())
  {
    m_collDrivers.emplace(tDriver_, std::move(tPtr<Driver>(new Driver(tDriver_))));
  }

  return m_collDrivers[tDriver_].get();
}

//----------------------------------------------------------------------------------------------------------------------

bool Application::isKnownDevice(const DeviceDescriptor& device_) const
{
  for (const auto& d : m_collKnownDevices)
  {
    if(device_.isSameProduct(d))
    {
      return true;
    }
  }
  return false;
}

//----------------------------------------------------------------------------------------------------------------------

bool Application::isSupportedDevice(const DeviceDescriptor& device_) const
{
  for (const auto& d : m_collSupportedDevices)
  {
    if(device_.isSameProduct(d))
    {
      return true;
    }
  }
  return false;
}

//----------------------------------------------------------------------------------------------------------------------

} // kio
} // sl