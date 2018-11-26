/*
CmdCCTSetVoltagesOnGib.cpp
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
#include "CmdCCTSetVoltagesOnGib.h"

#include <cmath>
#include  <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <sstream>
#define SCLAPP_ INFO_LOG(CmdCCTSetVoltagesOnGib) << "[" << getDeviceID() << "] "
#define SCLDBG_ DBG_LOG(CmdCCTSetVoltagesOnGib) << "[" << getDeviceID() << "] "
#define SCLERR_ ERR_LOG(CmdCCTSetVoltagesOnGib) << "[" << getDeviceID() << "] "
namespace own = driver::ccaltcontroller;
namespace c_data =  chaos::common::data;
namespace chaos_batch = chaos::common::batch_command;
using namespace chaos::cu::control_manager;
BATCH_COMMAND_OPEN_DESCRIPTION_ALIAS(driver::ccaltcontroller::,CmdCCTSetVoltagesOnGib,CMD_CCT_SETVOLTAGESONGIB_ALIAS,
	"set SIPM voltages",
	"6fe24fa5-e206-470d-80fd-3fb89d64838b")
BATCH_COMMAND_ADD_STRING_PARAM(CMD_CCT_SETVOLTAGESONGIB_GIBUID,"the name of the control unit. ALL for ALL the Control Units",chaos::common::batch_command::BatchCommandAndParameterDescriptionkey::BC_PARAMETER_FLAG_MANDATORY)
BATCH_COMMAND_ADD_INT32_PARAM(CMD_CCT_SETVOLTAGESONGIB_CHANNEL,"the channel to be set at. -1  for all channels",chaos::common::batch_command::BatchCommandAndParameterDescriptionkey::BC_PARAMETER_FLAG_MANDATORY)
BATCH_COMMAND_ADD_DOUBLE_PARAM(CMD_CCT_SETVOLTAGESONGIB_VOLTAGE,"the voltage to be set on the channel ",chaos::common::batch_command::BatchCommandAndParameterDescriptionkey::BC_PARAMETER_FLAG_MANDATORY)
BATCH_COMMAND_CLOSE_DESCRIPTION()


// return the implemented handler
uint8_t own::CmdCCTSetVoltagesOnGib::implementedHandler(){
	return      AbstractCCALTControllerCommand::implementedHandler()|chaos_batch::HandlerType::HT_Acquisition;
}
// empty set handler
void own::CmdCCTSetVoltagesOnGib::setHandler(c_data::CDataWrapper *data) {
	AbstractCCALTControllerCommand::setHandler(data);
	SCLAPP_ << "Set Handler SetVoltagesOnGib "; 
	setStateVariableSeverity(StateVariableTypeAlarmCU,"driver_command_error",chaos::common::alarm::MultiSeverityAlarmLevelClear);
	setStateVariableSeverity(StateVariableTypeAlarmCU,"bad_command_parameter",chaos::common::alarm::MultiSeverityAlarmLevelClear);
	setStateVariableSeverity(StateVariableTypeAlarmCU,"MissingSetPointSnapshot",chaos::common::alarm::MultiSeverityAlarmLevelClear);
	if(!data || !data->hasKey(CMD_CCT_SETVOLTAGESONGIB_GIBUID))
	{
		SCLERR_ << "gibUID not present in CDataWrapper";
		metadataLogging(chaos::common::metadata_logging::StandardLoggingChannel::LogLevelError,"gibUID not present in CDataWrapper" );
		setWorkState(false);
		BC_FAULT_RUNNING_PROPERTY
		return;
	}
	std::string tmp_gibUID=data->getStringValue(CMD_CCT_SETVOLTAGESONGIB_GIBUID);
	if(!data || !data->hasKey(CMD_CCT_SETVOLTAGESONGIB_CHANNEL))
	{
		SCLERR_ << "channel not present in CDataWrapper";
		metadataLogging(chaos::common::metadata_logging::StandardLoggingChannel::LogLevelError,"channel not present in CDataWrapper" );
		setWorkState(false);
		BC_FAULT_RUNNING_PROPERTY
		return;
	}
	int32_t tmp_channel=data->getInt32Value(CMD_CCT_SETVOLTAGESONGIB_CHANNEL);
	if ((tmp_channel < -1) || (tmp_channel > 23))
	{
		metadataLogging(chaos::common::metadata_logging::StandardLoggingChannel::LogLevelError,"bad channel number in SetVoltagesOnGib" );
		setStateVariableSeverity(StateVariableTypeAlarmCU,"bad_command_parameter",chaos::common::alarm::MultiSeverityAlarmLevelHigh);
	}
	if(!data || !data->hasKey(CMD_CCT_SETVOLTAGESONGIB_VOLTAGE))
	{
		SCLERR_ << "voltage not present in CDataWrapper";
		metadataLogging(chaos::common::metadata_logging::StandardLoggingChannel::LogLevelError,"voltage not present in CDataWrapper" );
		setWorkState(false);
		BC_FAULT_RUNNING_PROPERTY
		return;
	}
	double tmp_voltage=data->getDoubleValue(CMD_CCT_SETVOLTAGESONGIB_VOLTAGE);
	
	int err=0;
	driver::misc::ChaosController::command_t cmd;
	std::string lowercaseName=boost::algorithm::to_lower_copy(tmp_gibUID);
	if ((lowercaseName.find("gib1")!= std::string::npos) || (lowercaseName.find("all") != std::string::npos) )
	{
		cmd=GIB1->prepareCommand(gibcontrol::CMD_GIB_SETCHANNELVOLTAGE_ALIAS);
		cmd->addParameter(gibcontrol::CMD_GIB_SETCHANNELVOLTAGE_CHANNEL,tmp_channel);
		cmd->addParameter(gibcontrol::CMD_GIB_SETCHANNELVOLTAGE_VOLTAGE,tmp_voltage);
		err+=GIB1->executeCmd(cmd,false);
	}
	if ((lowercaseName.find("gib2")!= std::string::npos) || (lowercaseName.find("all") != std::string::npos) )
	{
		cmd=GIB2->prepareCommand(gibcontrol::CMD_GIB_SETCHANNELVOLTAGE_ALIAS);
		cmd->addParameter(gibcontrol::CMD_GIB_SETCHANNELVOLTAGE_CHANNEL,tmp_channel);
		cmd->addParameter(gibcontrol::CMD_GIB_SETCHANNELVOLTAGE_VOLTAGE,tmp_voltage);
		err+=GIB2->executeCmd(cmd,false);
	}
	if ((lowercaseName.find("gib3")!= std::string::npos) || (lowercaseName.find("all") != std::string::npos) )
	{
		cmd=GIB3->prepareCommand(gibcontrol::CMD_GIB_SETCHANNELVOLTAGE_ALIAS);
		cmd->addParameter(gibcontrol::CMD_GIB_SETCHANNELVOLTAGE_CHANNEL,tmp_channel);
		cmd->addParameter(gibcontrol::CMD_GIB_SETCHANNELVOLTAGE_VOLTAGE,tmp_voltage);
		err+=GIB3->executeCmd(cmd,false);
	}
	if ((lowercaseName.find("gib4")!= std::string::npos) || (lowercaseName.find("all") != std::string::npos) )
	{
		cmd=GIB4->prepareCommand(gibcontrol::CMD_GIB_SETCHANNELVOLTAGE_ALIAS);
		cmd->addParameter(gibcontrol::CMD_GIB_SETCHANNELVOLTAGE_CHANNEL,tmp_channel);
		cmd->addParameter(gibcontrol::CMD_GIB_SETCHANNELVOLTAGE_VOLTAGE,tmp_voltage);
		err+=GIB4->executeCmd(cmd,false);
	}

	if (err != 0)
	{
		metadataLogging(chaos::common::metadata_logging::StandardLoggingChannel::LogLevelError," command SetVoltagesOnGib not acknowledged");
		setStateVariableSeverity(StateVariableTypeAlarmCU,"driver_command_error",chaos::common::alarm::MultiSeverityAlarmLevelHigh);
		setWorkState(false);
		BC_FAULT_RUNNING_PROPERTY
		return;
	}
	setWorkState(true);
	BC_NORMAL_RUNNING_PROPERTY
}
// empty acquire handler
void own::CmdCCTSetVoltagesOnGib::acquireHandler() {
	SCLDBG_ << "Acquire Handler SetVoltagesOnGib "; 
}
// empty correlation handler
void own::CmdCCTSetVoltagesOnGib::ccHandler() {
	BC_END_RUNNING_PROPERTY;
}
// empty timeout handler
bool own::CmdCCTSetVoltagesOnGib::timeoutHandler() {
	SCLDBG_ << "Timeout Handler SetVoltagesOnGib "; 
	return false;
}
