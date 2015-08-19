/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "comm/DriverImpl.h"
#include "comm/DeviceHandleImpl.h"

namespace sl
{
namespace kio
{

//--------------------------------------------------------------------------------------------------

class DeviceHandleSAM3XE : public DeviceHandleImpl
{
public:

  using tDeviceHandle = void;

  DeviceHandleSAM3XE(tDeviceHandle*);
  ~DeviceHandleSAM3XE();

  void disconnect() override;

  bool read(Transfer&, uint8_t) override;
  bool write(const Transfer&, uint8_t) const override;

private:

  tRawData                        m_inputBuffer;
};
  
//--------------------------------------------------------------------------------------------------

} // kio
} // sl