/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "DriverProbe.h"
#include "DeviceHandleProbe.h"

namespace sl
{
namespace kio
{

//--------------------------------------------------------------------------------------------------

DriverProbe::DriverProbe()
{

}

//--------------------------------------------------------------------------------------------------

DriverProbe::~DriverProbe()
{

}

//--------------------------------------------------------------------------------------------------

Driver::tCollDeviceDescriptor DriverProbe::enumerate()
{
  return Driver::tCollDeviceDescriptor();
}

//--------------------------------------------------------------------------------------------------

tPtr<DeviceHandleImpl> DriverProbe::connect(const DeviceDescriptor&)
{
  return nullptr;
}

//--------------------------------------------------------------------------------------------------

} // kio
} // sl