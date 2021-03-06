/*
AbstractCCALTControllerCommand.cpp
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
#include "AbstractCCALTControllerCommand.h"
#include <boost/format.hpp>
#define CMDCUINFO_ INFO_LOG(AbstractCCALTControllerCommand)
#define CMDCUDBG_ DBG_LOG(AbstractCCALTControllerCommand)
#define CMDCUERR_ ERR_LOG(AbstractCCALTControllerCommand)
using namespace driver::ccaltcontroller;
namespace chaos_batch = chaos::common::batch_command;
using namespace chaos::cu::control_manager;
AbstractCCALTControllerCommand::AbstractCCALTControllerCommand():GIB1(NULL),GIB2(NULL),GIB3(NULL),GIB4(NULL) {
	ccaltcontroller_drv = NULL;
	GIB1= new ::driver::misc::ChaosController("CCALT/GIB/GIB1");
	GIB2= new ::driver::misc::ChaosController("CCALT/GIB/GIB2");
	GIB3= new ::driver::misc::ChaosController("CCALT/GIB/GIB3");
	GIB4= new ::driver::misc::ChaosController("CCALT/GIB/GIB4");
}
AbstractCCALTControllerCommand::~AbstractCCALTControllerCommand() {
	if(ccaltcontroller_drv)
		delete(ccaltcontroller_drv);
	ccaltcontroller_drv = NULL;
	if (GIB1)
		delete(GIB1);
	if (GIB2)
		delete(GIB2);
	if (GIB3)
		delete(GIB3);
	if (GIB4)
		delete(GIB4);
	
	GIB1=GIB2=GIB3=GIB4=NULL;
	
}
void AbstractCCALTControllerCommand::setHandler(c_data::CDataWrapper *data) {
	CMDCUDBG_ << "loading pointer for output channel"; 
	//get pointer to the output dataset variable
	o_status_id = getAttributeCache()->getRWPtr<int32_t>(DOMAIN_OUTPUT, "status_id");
	o_status_desc = getAttributeCache()->getRWPtr<char>(DOMAIN_OUTPUT, "status_description");
	o_alarms = getAttributeCache()->getRWPtr<uint64_t>(DOMAIN_OUTPUT, "alarms"); 
	//setting default timeout (usec) 
	setFeatures(chaos_batch::features::FeaturesFlagTypes::FF_SET_COMMAND_TIMEOUT,(uint32_t) 10000000);
	chaos::cu::driver_manager::driver::DriverAccessor *ccaltcontroller_accessor = driverAccessorsErogator->getAccessoInstanceByIndex(0);
	if(ccaltcontroller_accessor != NULL) {
		if(ccaltcontroller_drv == NULL) {
			ccaltcontroller_drv = new chaos::driver::ccaltcontroller::ChaosCCALTControllerInterface(ccaltcontroller_accessor);
		}
	}
	
}
// return the implemented handler
uint8_t AbstractCCALTControllerCommand::implementedHandler() {
	return  chaos_batch::HandlerType::HT_Set | chaos_batch::HandlerType::HT_Correlation;
}
void AbstractCCALTControllerCommand::ccHandler() {

}
void AbstractCCALTControllerCommand::setWorkState(bool working_flag) {
	setBusyFlag(working_flag);
}

bool AbstractCCALTControllerCommand::CalculateState(chaos::common::data::CDWShrdPtr Gib1,chaos::common::data::CDWShrdPtr Gib2,
				chaos::common::data::CDWShrdPtr Gib3,chaos::common::data::CDWShrdPtr Gib4)   
{
	int32_t status1=Gib1->getInt32Value("status_id");
	int32_t status2=Gib2->getInt32Value("status_id");
	int32_t status3=Gib3->getInt32Value("status_id");
	int32_t status4=Gib4->getInt32Value("status_id");
	int32_t globalState=0;
	bool pulsing=false;
	*o_status_id=0;
	if (CHECKMASK(status1,::common::gibcontrol::GIBCONTROL_SUPPLIED)) {globalState++;}
	if (CHECKMASK(status2,::common::gibcontrol::GIBCONTROL_SUPPLIED)) {globalState++;}
	if (CHECKMASK(status3,::common::gibcontrol::GIBCONTROL_SUPPLIED)) {globalState++;}
	if (CHECKMASK(status4,::common::gibcontrol::GIBCONTROL_SUPPLIED)) {globalState++;}

	if (CHECKMASK(status1,::common::gibcontrol::GIBCONTROL_PULSING)) {pulsing=true;}
	if (CHECKMASK(status2,::common::gibcontrol::GIBCONTROL_PULSING)) {pulsing=true;}
	if (CHECKMASK(status3,::common::gibcontrol::GIBCONTROL_PULSING)) {pulsing=true;}
	if (CHECKMASK(status4,::common::gibcontrol::GIBCONTROL_PULSING)) {pulsing=true;}
	std::string description="";
	if (globalState==4) 
	{
	    UPMASK(*o_status_id,::common::ccaltcontroller::CCALTCONTROLLER_STATE_ON);
		description+="ON. ";
	}
	else if (globalState > 0) 
	{
		UPMASK(*o_status_id,::common::ccaltcontroller::CCALTCONTROLLER_STATE_PARTIALLY_ON);
		description+="Partially ON. ";
	}
	else
	{
		description="OFF. ";
	}

	CMDCUDBG_ << "pulsing now is " << pulsing ;
	if (pulsing)
	{	
		UPMASK(*o_status_id,::common::ccaltcontroller::CCALTCONTROLLER_STATE_PULSING);
		description+="Pulsing. ";
	}
	
	strcpy(o_status_desc,description.c_str());
	return true;
}

std::pair<std::vector<int32_t>,std::vector<std::string>> AbstractCCALTControllerCommand::checkHealthState() {
	std::pair<std::vector<int32_t>,std::vector<std::string>> ReturnObject;
	std::vector<int32_t> differenceTimeVector;
	std::vector<std::string> CUStateVector;
	differenceTimeVector.resize(4);
	CUStateVector.resize(4);
	chaos::common::data::CDWShrdPtr  GIB1HDS, GIB2HDS, GIB3HDS, GIB4HDS;
	GIB1HDS=GIB1->getLiveChannel(GIB1Name,4);
	GIB2HDS=GIB2->getLiveChannel(GIB2Name,4);
	GIB3HDS=GIB3->getLiveChannel(GIB3Name,4);
	GIB4HDS=GIB4->getLiveChannel(GIB4Name,4);
	u_int64_t atsTime;
	time_t now;
	u_int64_t differenceTime;
	std::string custate;
	now=time(NULL);
	
	atsTime=GIB1HDS->getUInt64Value("dpck_ats");
	custate=GIB1HDS->getStringValue("nh_status");
	differenceTime=now-(u_int64_t)(atsTime/1000);
	differenceTimeVector[0]=differenceTime;
	CUStateVector[0]=custate;

	atsTime=GIB2HDS->getUInt64Value("dpck_ats");
	custate=GIB2HDS->getStringValue("nh_status");
	differenceTime=now-(u_int64_t)(atsTime/1000);
	differenceTimeVector[1]=differenceTime;
	CUStateVector[1]=custate;

	atsTime=GIB3HDS->getUInt64Value("dpck_ats");
	custate=GIB3HDS->getStringValue("nh_status");
	differenceTime=now-(u_int64_t)(atsTime/1000);
	differenceTimeVector[2]=differenceTime;
	CUStateVector[2]=custate;

	atsTime=GIB4HDS->getUInt64Value("dpck_ats");
	custate=GIB4HDS->getStringValue("nh_status");
	differenceTime=now-(u_int64_t)(atsTime/1000);
	differenceTimeVector[3]=differenceTime;
	CUStateVector[3]=custate;

	ReturnObject.first=differenceTimeVector;
	ReturnObject.second=CUStateVector;

	if ((differenceTime > DEFAULT_ELAPSED_TIME_HEALTH ) || (custate != "Start") )
	{
		CMDCUDBG_ << "GIB1 "<< custate << "time difference " << differenceTime;
	}
	//Usare 10 secondi dovrebbe andare bene


	return ReturnObject;
}

bool AbstractCCALTControllerCommand::batchGoToSetPoint() 
{
	setStateVariableSeverity(StateVariableTypeAlarmCU,"driver_command_error",chaos::common::alarm::MultiSeverityAlarmLevelClear);
	setStateVariableSeverity(StateVariableTypeAlarmCU,"bad_command_parameter",chaos::common::alarm::MultiSeverityAlarmLevelClear);
	setStateVariableSeverity(StateVariableTypeAlarmCU,"MissingSetPointSnapshot",chaos::common::alarm::MultiSeverityAlarmLevelClear);
	int err=0;
	gib1SetPoint = getAttributeCache()->getRWPtr<char>(DOMAIN_INPUT, "setpointNameGIB1");
	gib2SetPoint = getAttributeCache()->getRWPtr<char>(DOMAIN_INPUT, "setpointNameGIB2");
	gib3SetPoint = getAttributeCache()->getRWPtr<char>(DOMAIN_INPUT, "setpointNameGIB3");
	gib4SetPoint = getAttributeCache()->getRWPtr<char>(DOMAIN_INPUT, "setpointNameGIB4");
	if ((gib1SetPoint==NULL)||(gib2SetPoint==NULL)||(gib3SetPoint==NULL)||(gib4SetPoint==NULL) )
	{
		setStateVariableSeverity(StateVariableTypeAlarmCU,"MissingSetPointSnapshot",chaos::common::alarm::MultiSeverityAlarmLevelWarning);
		metadataLogging(chaos::common::metadata_logging::StandardLoggingChannel::LogLevelError,"no name for snapshot in configuration" );
		setWorkState(false);
		BC_FAULT_RUNNING_PROPERTY
		return false;
	}
	std::map<u_int64_t,std::string> retMap;
	GIB1->getSnapshotsofCU(GIB1Name,retMap);
	bool found=false;
	for (std::map<u_int64_t,std::string>::iterator Iter=retMap.begin(); Iter != retMap.end(); Iter++)
	{
		if ((*Iter).second == gib1SetPoint)
		{
			GIB1->restoreDeviceToTag(gib1SetPoint);
			CMDCUINFO_ << "Should restore snapshot "<< (*Iter).second;
			found=true;
		}
	}
	if (!found) {err++;}
	retMap.clear();
	GIB2->getSnapshotsofCU(GIB2Name,retMap);
	found=false;
	for (std::map<u_int64_t,std::string>::iterator Iter=retMap.begin(); Iter != retMap.end(); Iter++)
	{
		if ((*Iter).second == gib2SetPoint)
		{
			GIB2->restoreDeviceToTag(gib2SetPoint);
			CMDCUINFO_ << "Should restore snapshot "<< (*Iter).second;
			found=true;
		}
	}
	if (!found) {err++;}
	retMap.clear();
	GIB3->getSnapshotsofCU(GIB3Name,retMap);
	found=false;
	for (std::map<u_int64_t,std::string>::iterator Iter=retMap.begin(); Iter != retMap.end(); Iter++)
	{
		if ((*Iter).second == gib3SetPoint)
		{
			GIB3->restoreDeviceToTag(gib3SetPoint);
			CMDCUINFO_ << "Should restore snapshot "<< (*Iter).second;
			found=true;
		}
	}
	if (!found) {err++;}
	retMap.clear();
	GIB4->getSnapshotsofCU(GIB4Name,retMap);
	found=false;
	for (std::map<u_int64_t,std::string>::iterator Iter=retMap.begin(); Iter != retMap.end(); Iter++)
	{
		if ((*Iter).second == gib4SetPoint)
		{
			GIB1->restoreDeviceToTag(gib4SetPoint);
			CMDCUINFO_ << "Should restore snapshot "<< (*Iter).second;
			found=true;
		}
	}
	if (!found) {err++;}

	if (err != 0)
	{
		std::string errorString = " command GoToSetPoint not acknowledged " ;
		metadataLogging(chaos::common::metadata_logging::StandardLoggingChannel::LogLevelError,errorString);
		setStateVariableSeverity(StateVariableTypeAlarmCU,"MissingSetPointSnapshot",chaos::common::alarm::MultiSeverityAlarmLevelHigh);
		setWorkState(false);
		BC_FAULT_RUNNING_PROPERTY
		return false;
	}
	return true;
}