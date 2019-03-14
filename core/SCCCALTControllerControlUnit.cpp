/*
SCCCALTControllerControlUnit.cpp
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
#include "SCCCALTControllerControlUnit.h"
//#include <driver/misc/core/ChaosDatasetAttributeSinchronizer.h>
//#include <driver/misc/core/SCDataSync.h>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <common/debug/core/debug.h>
#include "CmdCCTDefault.h"
#include "CmdCCTPowerOn.h"
#include "CmdCCTSetVoltagesOnGib.h"
#include "CmdCCTSetPulseOnGib.h"
#include "CmdCCTGoToSetPoint.h"
using namespace chaos;
using namespace chaos::common::data;
using namespace chaos::common::batch_command;
using namespace chaos::cu::control_manager::slow_command;
using namespace chaos::cu::driver_manager::driver;
using namespace chaos::cu::control_manager;
#define SCCUAPP INFO_LOG(SCCCALTControllerControlUnit)
#define SCCUDBG DBG_LOG(SCCCALTControllerControlUnit)
#define SCCUERR ERR_LOG(SCCCALTControllerControlUnit)


PUBLISHABLE_CONTROL_UNIT_IMPLEMENTATION(::driver::ccaltcontroller::SCCCALTControllerControlUnit)

/*Construct a new CU with an identifier*/
::driver::ccaltcontroller::SCCCALTControllerControlUnit::SCCCALTControllerControlUnit(const string &_control_unit_id,
			const string &_control_unit_param,const ControlUnitDriverList &_control_unit_drivers)
		/*	: ::driver::misc::SCDataSync*/
:  chaos::cu::control_manager::SCAbstractControlUnit(_control_unit_id,
			 _control_unit_param, _control_unit_drivers){
	ccaltcontroller_drv = NULL;

	/*driver=new ::driver::misc::remoteGroupAccessInterface(getAccessoInstanceByIndex(0));
     if((driver == NULL) || (driver->connect()!=0)){
         throw chaos::CException(-1,"cannot connect with driver",__PRETTY_FUNCTION__);
     }
    std::vector<::driver::misc::ChaosDatasetAttribute*> ret= driver->getRemoteVariables();
	*/
}
/*Base Destructor*/
::driver::ccaltcontroller::SCCCALTControllerControlUnit::~SCCCALTControllerControlUnit() {
	if (ccaltcontroller_drv) {
		delete (ccaltcontroller_drv);
	}
}
//handlers
bool ::driver::ccaltcontroller::SCCCALTControllerControlUnit::setBehaviour(const std::string &name, int32_t value, uint32_t size)
{
  if (value != 0)
  {
	int32_t* i_setPointBehaviour=getAttributeCache()->getRWPtr<int32_t>(DOMAIN_INPUT,"registered_setpoint_behaviour");
	char		*gib1SetPoint = getAttributeCache()->getRWPtr<char>(DOMAIN_INPUT, "setpointNameGIB1");
	char		*gib2SetPoint = getAttributeCache()->getRWPtr<char>(DOMAIN_INPUT, "setpointNameGIB2");
	char		*gib3SetPoint = getAttributeCache()->getRWPtr<char>(DOMAIN_INPUT, "setpointNameGIB3");
	char		*gib4SetPoint = getAttributeCache()->getRWPtr<char>(DOMAIN_INPUT, "setpointNameGIB4");
	if ((gib1SetPoint==NULL)||(gib2SetPoint==NULL)||(gib3SetPoint==NULL)||(gib4SetPoint==NULL) )
	{
		//setStateVariableSeverity(StateVariableTypeAlarmCU,"MissingSetPointSnapshot",chaos::common::alarm::MultiSeverityAlarmLevelWarning);
		metadataLogging(chaos::common::metadata_logging::StandardLoggingChannel::LogLevelError,"cannot activate control over setPoint. No enough setpoints in configuration" );
		*i_setPointBehaviour=0;
		return false;
	}
	
  }
  return true;
}
//end handlers
void ::driver::ccaltcontroller::SCCCALTControllerControlUnit::unitDefineActionAndDataset()  {
	installCommand(BATCH_COMMAND_GET_DESCRIPTION(CmdCCTDefault),true);
	installCommand(BATCH_COMMAND_GET_DESCRIPTION(CmdCCTPowerOn));
	installCommand(BATCH_COMMAND_GET_DESCRIPTION(CmdCCTSetVoltagesOnGib));
	installCommand(BATCH_COMMAND_GET_DESCRIPTION(CmdCCTSetPulseOnGib));
	installCommand(BATCH_COMMAND_GET_DESCRIPTION(CmdCCTGoToSetPoint));
	/* //Uncomment when you want to connect the driver
	chaos::cu::driver_manager::driver::DriverAccessor *ccaltcontroller_accessor = getAccessoInstanceByIndex(0);
	if (ccaltcontroller_accessor == NULL ) {
		throw chaos::CException(-1, "Cannot retrieve the requested driver", __FUNCTION__);
	}
	ccaltcontroller_drv = new chaos::driver::ccaltcontroller::ChaosCCALTControllerInterface(ccaltcontroller_accessor);
	if (ccaltcontroller_drv == NULL) {
		throw chaos::CException(-2, "Cannot allocate driver resources", __FUNCTION__);
	}
	*/ //Uncomment when you want to connect the driver
	addAttributeToDataSet("status_id",
							"default status attribute",
							DataType::TYPE_INT32,
							DataType::Output);
	addAttributeToDataSet("status_description",
							"Description of the status of CCALT",
							DataType::TYPE_STRING,
							DataType::Output, 256);
	addAttributeToDataSet("alarms",
							"default alarms attribute",
							DataType::TYPE_INT64,
							DataType::Output);
	addBinaryAttributeAsSubtypeToDataSet("GIB1_Voltages",
							"SIPM Voltage of GIB1",
							chaos::DataType::SUB_TYPE_DOUBLE,
							24*sizeof(double),
							chaos::DataType::Output);
	addBinaryAttributeAsSubtypeToDataSet("GIB2_Voltages",
							"SIPM Voltage of GIB2",
							chaos::DataType::SUB_TYPE_DOUBLE,
							24*sizeof(double),
							chaos::DataType::Output);
	addBinaryAttributeAsSubtypeToDataSet("GIB3_Voltages",
							"SIPM Voltage of GIB3",
							chaos::DataType::SUB_TYPE_DOUBLE,
							24*sizeof(double),
							chaos::DataType::Output);
	addBinaryAttributeAsSubtypeToDataSet("GIB4_Voltages",
							"SIPM Voltage of GIB4",
							chaos::DataType::SUB_TYPE_DOUBLE,
							24*sizeof(double),
							chaos::DataType::Output);
	addAttributeToDataSet("setpointNameGIB1",
							"name of the snapshot for the working position of the GIB1",
							DataType::TYPE_STRING,
							DataType::Input, 256);
	addAttributeToDataSet("setpointNameGIB2",
							"name of the snapshot for the working position of the GIB2",
							DataType::TYPE_STRING,
							DataType::Input, 256);
	addAttributeToDataSet("setpointNameGIB3",
							"name of the snapshot for the working position of the GIB3",
							DataType::TYPE_STRING,
							DataType::Input, 256);
	addAttributeToDataSet("setpointNameGIB4",
							"name of the snapshot for the working position of the GIB4",
							DataType::TYPE_STRING,
							DataType::Input, 256);
	addAttributeToDataSet("registered_setpoint_behaviour",
							"0: No Actions, 1: Signal if voltages out of setpoint, 2: restore automatically setpoint when found out of set",
							DataType::TYPE_INT32,
							DataType::Input);
	
	addHandlerOnInputAttributeName<::driver::ccaltcontroller::SCCCALTControllerControlUnit, int32_t>(this,
                                                                                           &::driver::ccaltcontroller::SCCCALTControllerControlUnit::setBehaviour, "registered_setpoint_behaviour");



	addStateVariable(StateVariableTypeAlarmCU,"gib_data_retrieving_error",
		"Raised when the controller fails to receive data from MDS");

	addStateVariable(StateVariableTypeAlarmCU,"driver_command_error",
		"default driver communication error");
	addStateVariable(StateVariableTypeAlarmCU,"bad_command_parameter",
		"raised when a command is issued with wrong parameters");
	addStateVariable(StateVariableTypeAlarmCU,"MissingSetPointSnapshot",
		"Raised when issued command for going to setpoint, but there's no registered setpoint");
	addStateVariable(StateVariableTypeAlarmDEV,"gib_unreachable",
		"raised when at least one gib is unreachable");
	addStateVariable(StateVariableTypeAlarmDEV,"GIBCU_DEAD",
		"raised when at least one CU is not alive");
	addStateVariable(StateVariableTypeAlarmDEV,"GIB_CU_NOT_STARTED",
		"raised when at least one CU is not in start state");
	addStateVariable(StateVariableTypeAlarmDEV,"out_of_set",
		"raised when at least one channel of one gib moved from the setpoint");
	addStateVariable(StateVariableTypeAlarmDEV,"setpoint_not_reached",
		"raised when at least one gib failed to go to setpoint");
   addStateVariable(StateVariableTypeAlarmDEV,"driver_in_wrong_status",
		"raised when at least one gib have a driver in wrong and uncoherent status");

		
}
void ::driver::ccaltcontroller::SCCCALTControllerControlUnit::unitDefineCustomAttribute() {
}
// Abstract method for the initialization of the control unit
void ::driver::ccaltcontroller::SCCCALTControllerControlUnit::unitInit() {
}
// Abstract method for the start of the control unit
void ::driver::ccaltcontroller::SCCCALTControllerControlUnit::unitStart() {
}
// Abstract method for the stop of the control unit
void ::driver::ccaltcontroller::SCCCALTControllerControlUnit::unitStop()  {
}
// Abstract method for deinit the control unit
void ::driver::ccaltcontroller::SCCCALTControllerControlUnit::unitDeinit() {
	SCCUAPP << "deinitializing ";
}
	//! restore the control unit to snapshot
#define RESTORE_LAPP SCCUAPP << "[RESTORE-" <<getCUID() << "] "
#define RESTORE_LERR SCCUERR << "[RESTORE-" <<getCUID() << "] "
bool ::driver::ccaltcontroller::SCCCALTControllerControlUnit::unitRestoreToSnapshot(chaos::cu::control_manager::AbstractSharedDomainCache *const snapshot_cache)  {
	return false;
}
bool ::driver::ccaltcontroller::SCCCALTControllerControlUnit::waitOnCommandID(uint64_t cmd_id) {
	 ChaosUniquePtr<chaos::common::batch_command::CommandState> cmd_state;
do { 
cmd_state = getStateForCommandID(cmd_id);
if (!cmd_state.get()) break;
switch (cmd_state->last_event) {
case BatchCommandEventType::EVT_QUEUED:
SCCUAPP << cmd_id << " -> QUEUED";
break;
case BatchCommandEventType::EVT_RUNNING:
SCCUAPP << cmd_id << " -> RUNNING"; 
break;
case BatchCommandEventType::EVT_WAITING:
SCCUAPP << cmd_id << " -> WAITING";
break;
case BatchCommandEventType::EVT_PAUSED:
SCCUAPP << cmd_id << " -> PAUSED";
break;
case BatchCommandEventType::EVT_KILLED:
SCCUAPP << cmd_id << " -> KILLED";
break;
case BatchCommandEventType::EVT_COMPLETED:
SCCUAPP << cmd_id << " -> COMPLETED";
break;
case BatchCommandEventType::EVT_FAULT:
    SCCUAPP << cmd_id << " -> FAULT";
break;
}
usleep(500000);
} while (cmd_state->last_event != BatchCommandEventType::EVT_COMPLETED &&
        cmd_state->last_event != BatchCommandEventType::EVT_FAULT &&
    cmd_state->last_event != BatchCommandEventType::EVT_KILLED);
return (cmd_state.get() &&
cmd_state->last_event == BatchCommandEventType::EVT_COMPLETED);

}
