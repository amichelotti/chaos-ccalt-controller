/*
ChaosCCALTControllerInterface.cpp
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
#include "ChaosCCALTControllerInterface.h"
using namespace chaos::driver::ccaltcontroller;
#define PREPARE_OP_RET_INT_TIMEOUT(op,tim) \
ccaltcontroller_oparams_t ret;\
ccaltcontroller_iparams_t idata;\
message.opcode = op; \
message.inputData=(void*)&idata;\
idata.timeout=tim;\
message.inputDataLength=sizeof(ccaltcontroller_iparams_t);\
message.resultDataLength=sizeof(ccaltcontroller_oparams_t);\
message.resultData = (void*)&ret;\

#define WRITE_OP_TIM(op,timeout) \
PREPARE_OP_RET_INT_TIMEOUT(op,timeout); \
accessor->send(&message);\
return ret.result;

#define WRITE_OP_INT32_T_TIM(op,VAR_int32_t1,timeout)\
PREPARE_OP_RET_INT_TIMEOUT(op,timeout); \
idata.int32_t1=VAR_int32_t1;\
accessor->send(&message);\
return ret.result;

#define WRITE_OP_STRING_INT32_T_DOUBLE_TIM(op,VAR_string1,VAR_int32_t1,VAR_double1,timeout)\
PREPARE_OP_RET_INT_TIMEOUT(op,timeout); \
idata.string1=VAR_string1;\
idata.int32_t1=VAR_int32_t1;\
idata.double1=VAR_double1;\
accessor->send(&message);\
return ret.result;

#define WRITE_OP_STRING_INT32_T_INT32_T_INT32_T_INT32_T_TIM(op,VAR_string1,VAR_int32_t1,VAR_int32_t2,VAR_int32_t3,VAR_int32_t4,timeout)\
PREPARE_OP_RET_INT_TIMEOUT(op,timeout); \
idata.string1=VAR_string1;\
idata.int32_t1=VAR_int32_t1;\
idata.int32_t2=VAR_int32_t2;\
idata.int32_t3=VAR_int32_t3;\
idata.int32_t4=VAR_int32_t4;\
accessor->send(&message);\
return ret.result;

int ChaosCCALTControllerInterface::PowerOn(int32_t on_state) {
	WRITE_OP_INT32_T_TIM(OP_POWERON,on_state,0);
} 
int ChaosCCALTControllerInterface::SetVoltagesOnGib(std::string gibUID,int32_t channel,double voltage) {
	WRITE_OP_STRING_INT32_T_DOUBLE_TIM(OP_SETVOLTAGESONGIB,gibUID,channel,voltage,0);
} 
int ChaosCCALTControllerInterface::SetPulseOnGib(std::string gibUID,int32_t channel,int32_t amplitude,int32_t width,int32_t state) {
	WRITE_OP_STRING_INT32_T_INT32_T_INT32_T_INT32_T_TIM(OP_SETPULSEONGIB,gibUID,channel,amplitude,width,state,0);
} 
int ChaosCCALTControllerInterface::GoToSetPoint() {
	WRITE_OP_TIM(OP_GOTOSETPOINT,0);
} 
