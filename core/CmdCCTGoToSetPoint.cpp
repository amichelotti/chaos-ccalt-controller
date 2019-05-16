/*
CmdCCTGoToSetPoint.cpp
!CHAOS
Created by CUGenerator

Copyright 2013 INFN, National Institute of Nuclear Physics
Licensed under the Apache License, Version 2.0 (the "License")
you may not use this file except in compliance with the License.
      You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include "CmdCCTGoToSetPoint.h"

#include <cmath>
#include  <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <sstream>
#define SCLAPP_ INFO_LOG(CmdCCTGoToSetPoint) << "[" << getDeviceID() << "] "
#define SCLDBG_ DBG_LOG(CmdCCTGoToSetPoint) << "[" << getDeviceID() << "] "
#define SCLERR_ ERR_LOG(CmdCCTGoToSetPoint) << "[" << getDeviceID() << "] "
namespace own = driver::ccaltcontroller;
namespace c_data =  chaos::common::data;
namespace chaos_batch = chaos::common::batch_command;
using namespace chaos::cu::control_manager;
BATCH_COMMAND_OPEN_DESCRIPTION_ALIAS(driver::ccaltcontroller::,CmdCCTGoToSetPoint,CMD_CCT_GOTOSETPOINT_ALIAS,
	"Move whole  the CCALT to the desired setpoint",
	"7d2b4eea-563a-48ec-b895-80b88d5f160a")
BATCH_COMMAND_CLOSE_DESCRIPTION()


// return the implemented handler
uint8_t own::CmdCCTGoToSetPoint::implementedHandler(){
	return      AbstractCCALTControllerCommand::implementedHandler()|chaos_batch::HandlerType::HT_Acquisition;
}
// empty set handler
void own::CmdCCTGoToSetPoint::setHandler(c_data::CDataWrapper *data) {
	AbstractCCALTControllerCommand::setHandler(data);
	SCLAPP_ << "Set Handler GoToSetPoint "; 
	bool ret= AbstractCCALTControllerCommand::batchGoToSetPoint();
	if (ret)
	{
		BC_NORMAL_RUNNING_PROPERTY;
	}
}
// empty acquire handler
void own::CmdCCTGoToSetPoint::acquireHandler() {
	SCLDBG_ << "Acquire Handler GoToSetPoint "; 
}
// empty correlation handler
void own::CmdCCTGoToSetPoint::ccHandler() {
	BC_END_RUNNING_PROPERTY;
}
// empty timeout handler
bool own::CmdCCTGoToSetPoint::timeoutHandler() {
	SCLDBG_ << "Timeout Handler GoToSetPoint "; 
	return false;
}
