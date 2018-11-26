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
AbstractCCALTControllerCommand::AbstractCCALTControllerCommand() {
	ccaltcontroller_drv = NULL;
}
AbstractCCALTControllerCommand::~AbstractCCALTControllerCommand() {
	if(ccaltcontroller_drv)
		delete(ccaltcontroller_drv);
	ccaltcontroller_drv = NULL;
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
	GIB1= new ::driver::misc::ChaosController("CCALT/GIB/GIB1");
	GIB2= new ::driver::misc::ChaosController("CCALT/GIB/GIB2");
	GIB3= new ::driver::misc::ChaosController("CCALT/GIB/GIB3");
	GIB4= new ::driver::misc::ChaosController("CCALT/GIB/GIB4");
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

