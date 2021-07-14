/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2021 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdbool.h>
#include <string.h>

#include <stdlib.h>
#include <wdmp-c.h>
#include <cimplog.h>
#include "webpa_adapter.h"
#include "webpa_rbus.h"

static rbusHandle_t rbus_handle;

static uint32_t  CMCVal = 0 ;
static char* CIDVal = NULL ;
static char* syncVersionVal = NULL ;
static char* ConnClientVal = NULL ;
static char* notifyVal = NULL ;
static char* VersionVal = NULL ;

static bool isRbus = false ;

bool get_global_isRbus(void)
{
    return isRbus;
}

rbusHandle_t get_global_rbus_handle(void)
{
     return rbus_handle;
}

bool isRbusEnabled() 
{
	if(RBUS_ENABLED == rbus_checkStatus()) 
	{
		isRbus = true;
	}
	else
	{
		isRbus = false;
	}
	WalInfo("Webpa RBUS mode active status = %s\n", isRbus ? "true":"false");
	return isRbus;
}

bool isRbusInitialized( ) 
{
    return rbus_handle != NULL ? true : false;
}

WDMP_STATUS webpaRbusInit(const char *pComponentName) 
{
	int ret = RBUS_ERROR_SUCCESS;   

	WalInfo("rbus_open for component %s\n", pComponentName);
	ret = rbus_open(&rbus_handle, pComponentName);
	if(ret != RBUS_ERROR_SUCCESS)
	{
		WalError("webpaRbusInit failed with error code %d\n", ret);
		return WDMP_FAILURE;
	}
	WalInfo("webpaRbusInit is success. ret is %d\n", ret);
	return WDMP_SUCCESS;
}

static void webpaRbus_Uninit( ) {
    rbus_close(rbus_handle);
}

/**
 * Data set handler for Webpa parameters
 */
rbusError_t webpaDataSetHandler(rbusHandle_t handle, rbusProperty_t prop, rbusSetHandlerOptions_t* opts) {

    WalInfo("Inside webpaDataSetHandler\n");
    (void) opts;

    char const* paramName = rbusProperty_GetName(prop);
    if((strncmp(paramName,  WEBPA_CMC_PARAM, maxParamLen) != 0) &&
	(strncmp(paramName, WEBPA_CID_PARAM, maxParamLen) != 0) &&
	(strncmp(paramName, WEBPA_SYNCVERSION_PARAM, maxParamLen) != 0) &&
	(strncmp(paramName, WEBPA_CONNECTED_CLIENT_PARAM, maxParamLen) != 0) &&
	(strncmp(paramName, WEBPA_NOTIFY_PARAM, maxParamLen) != 0) &&
	(strncmp(paramName, WEBPA_VERSION_PARAM, maxParamLen) != 0)
	) {
        WalError("Unexpected parameter = %s\n", paramName); //free paramName req.?
        return RBUS_ERROR_ELEMENT_DOES_NOT_EXIST;
    }

    WalInfo("Parameter name is %s \n", paramName);
    rbusValueType_t type_t;
    rbusValue_t paramValue_t = rbusProperty_GetValue(prop);
    if(paramValue_t) {
        type_t = rbusValue_GetType(paramValue_t);
    } else {
	WalError("Invalid input to set\n");
        return RBUS_ERROR_INVALID_INPUT;
    }

    if(strncmp(paramName, WEBPA_CMC_PARAM, maxParamLen) == 0) {
        WalInfo("Inside CMC datamodel handler \n");
        if(type_t == RBUS_UINT32) {
            CMCVal = rbusValue_GetUInt32(paramValue_t);
	    WalInfo("CMCVal after processing\n");
        } else {
            WalError("Unexpected value type for property %s \n", paramName);
	    return RBUS_ERROR_INVALID_INPUT;
        }

    }else if(strncmp(paramName, WEBPA_CID_PARAM, maxParamLen) == 0) {
        WalInfo("Inside datamodel handler for CID \n");

        if(type_t == RBUS_STRING) {
            char* data = rbusValue_ToString(paramValue_t, NULL, 0);
            if(data) {
                WalInfo("Call datamodel function  with data %s \n", data);

                if(CIDVal) {
                    free(CIDVal);
                    CIDVal = NULL;
                }
                CIDVal = strdup(data);
                free(data);
		WalInfo("CIDVal after processing %s\n", CIDVal);
            }
        } else {
            WalError("Unexpected value type for property %s\n", paramName);
	    return RBUS_ERROR_INVALID_INPUT;
        }
    }else if(strncmp(paramName, WEBPA_SYNCVERSION_PARAM, maxParamLen) == 0) {
        WalInfo("Inside SYNC VERSION datamodel handler \n");
        if(type_t == RBUS_STRING) {
            char* data = rbusValue_ToString(paramValue_t, NULL, 0);
            if(data) {
                WalInfo("Call datamodel function  with data %s \n", data);

                if (syncVersionVal){
                    free(syncVersionVal);
                    syncVersionVal = NULL;
                }
                syncVersionVal = strdup(data);
                free(data);
		WalInfo("syncVersionVal after processing %s\n", syncVersionVal);
            }
        } else {
            WalError("Unexpected value type for property %s \n", paramName);
            return RBUS_ERROR_INVALID_INPUT;
        }

    }else if(strncmp(paramName, WEBPA_CONNECTED_CLIENT_PARAM, maxParamLen) == 0) {
        WalInfo("Inside datamodel handler for CONNECTED CLIENT \n");

        if(type_t == RBUS_STRING) {
            char* data = rbusValue_ToString(paramValue_t, NULL, 0);
            if(data) {
                WalInfo("Call datamodel function  with data %s \n", data);

                if(ConnClientVal) {
                    free(ConnClientVal);
                    ConnClientVal = NULL;
                }
                ConnClientVal = strdup(data);
                free(data);
		WalInfo("ConnClientVal after processing %s\n", ConnClientVal);
            }
        } else {
            WalError("Unexpected value type for property %s\n", paramName);
	    return RBUS_ERROR_INVALID_INPUT;
        }
    }else if(strncmp(paramName, WEBPA_NOTIFY_PARAM, maxParamLen) == 0) {
        WalInfo("Inside datamodel handler for NOTIFY PARAM \n");

        if(type_t == RBUS_STRING) {
            char* data = rbusValue_ToString(paramValue_t, NULL, 0);
            if(data) {
                WalInfo("Call datamodel function  with data %s \n", data);

                if(notifyVal) {
                    free(notifyVal);
                    notifyVal = NULL;
                }
                notifyVal = strdup(data);
                free(data);
		WalInfo("notifyVal after processing %s\n", notifyVal);
            }
        } else {
            WalError("Unexpected value type for property %s\n", paramName);
	    return RBUS_ERROR_INVALID_INPUT;
        }
    }else if(strncmp(paramName, WEBPA_VERSION_PARAM, maxParamLen) == 0) {
        WalInfo("Inside datamodel handler for VERSION PARAM \n");

        if(type_t == RBUS_STRING) {
            char* data = rbusValue_ToString(paramValue_t, NULL, 0);
            if(data) {
                WalInfo("Call datamodel function  with data %s \n", data);

                if(VersionVal) {
                    free(VersionVal);
                    VersionVal = NULL;
                }
                VersionVal = strdup(data);
                free(data);
		WalInfo("VersionVal after processing %s\n", VersionVal);
            }
        } else {
            WalError("Unexpected value type for property %s\n", paramName);
	    return RBUS_ERROR_INVALID_INPUT;
        }
    }
    WalInfo("webpaDataSetHandler End\n");
    return RBUS_ERROR_SUCCESS;
}

/**
 * Common data get handler for all parameters owned by Webpa
 */
rbusError_t webpaDataGetHandler(rbusHandle_t handle, rbusProperty_t property, rbusGetHandlerOptions_t* opts) {

    WalInfo("In webpaDataGetHandler\n");
    (void) handle;
    (void) opts;
    char const* propertyName;
    char* componentName = NULL;

    propertyName = strdup(rbusProperty_GetName(property));
    if(propertyName) {
        WalInfo("Property Name is %s \n", propertyName);
    } else {
        WalError("Unable to handle get request for property \n");
        return RBUS_ERROR_INVALID_INPUT;
    }

    if(strncmp(propertyName, WEBPA_CMC_PARAM, maxParamLen) == 0) {
        rbusValue_t value;
        rbusValue_Init(&value);

        rbusValue_SetUInt32(value, CMCVal);

        rbusProperty_SetValue(property, value);
        rbusValue_Release(value);
	//WalInfo("CMC value fetched is %s\n", value);

    }else if(strncmp(propertyName, WEBPA_CID_PARAM, maxParamLen) == 0) {
        rbusValue_t value;
        rbusValue_Init(&value);
        if(CIDVal)
            rbusValue_SetString(value, CIDVal);
        else
            rbusValue_SetString(value, "");
        rbusProperty_SetValue(property, value);
	WalInfo("CID value fetched is %s\n", value);
        rbusValue_Release(value);

    }else if(strncmp(propertyName, WEBPA_SYNCVERSION_PARAM, maxParamLen) == 0) {
        rbusValue_t value;
        rbusValue_Init(&value);
        if(syncVersionVal)
            rbusValue_SetString(value, syncVersionVal);
        else
            rbusValue_SetString(value, "");
        rbusProperty_SetValue(property, value);
        rbusValue_Release(value);
	WalInfo("Sync protocol version fetched is %s\n", value);
    }else if(strncmp(propertyName, WEBPA_CONNECTED_CLIENT_PARAM, maxParamLen) == 0) {
        rbusValue_t value;
        rbusValue_Init(&value);
        if(ConnClientVal)
            rbusValue_SetString(value, ConnClientVal);
        else
            rbusValue_SetString(value, "");
        rbusProperty_SetValue(property, value);
	WalInfo("ConnClientVal value fetched is %s\n", value);
        rbusValue_Release(value);

    }else if(strncmp(propertyName, WEBPA_NOTIFY_PARAM, maxParamLen) == 0) {
        rbusValue_t value;
        rbusValue_Init(&value);
        if(notifyVal)
            rbusValue_SetString(value, notifyVal);
        else
            rbusValue_SetString(value, "");
        rbusProperty_SetValue(property, value);
	WalInfo("notifyVal value fetched is %s\n", value);
        rbusValue_Release(value);

    }else if(strncmp(propertyName, WEBPA_VERSION_PARAM, maxParamLen) == 0) {
        rbusValue_t value;
        rbusValue_Init(&value);
        if(VersionVal)
            rbusValue_SetString(value, VersionVal);
        else
            rbusValue_SetString(value, "");
        rbusProperty_SetValue(property, value);
	WalInfo("VersionVal value fetched is %s\n", value);
        rbusValue_Release(value);

    }


    if(propertyName) {
        free((char*)propertyName);
        propertyName = NULL;
    }

    WalInfo("webpaDataGetHandler End\n");
    return RBUS_ERROR_SUCCESS;
}

/**
 * Register data elements for dataModel implementation using rbus.
 * Data element over bus will be Device.DeviceInfo.Webpa.X_COMCAST-COM_CMC,
 *    Device.DeviceInfo.Webpa.X_COMCAST-COM_CID
 */
WDMP_STATUS regWebpaDataModel()
{
	rbusError_t ret = RBUS_ERROR_SUCCESS;
	WDMP_STATUS status = WDMP_SUCCESS;

	WalInfo("Registering parameters deCMC %s, deCID %s, deSyncVersion %s deConnClient %s,deNotify %s, deVersion %s\n", WEBPA_CMC_PARAM, WEBPA_CID_PARAM, WEBPA_SYNCVERSION_PARAM,WEBPA_CONNECTED_CLIENT_PARAM,WEBPA_NOTIFY_PARAM, WEBPA_VERSION_PARAM);
	if(!rbus_handle)
	{
		WalError("regRbusWebpaDataModel Failed in getting bus handles\n");
		return WDMP_FAILURE;
	}

	rbusDataElement_t dataElements[NUM_WEBPA_ELEMENTS] = {

		{WEBPA_CMC_PARAM, RBUS_ELEMENT_TYPE_PROPERTY, {webpaDataGetHandler, webpaDataSetHandler, NULL, NULL, NULL, NULL}},
		{WEBPA_CID_PARAM, RBUS_ELEMENT_TYPE_PROPERTY, {webpaDataGetHandler, webpaDataSetHandler, NULL, NULL, NULL, NULL}},
		{WEBPA_SYNCVERSION_PARAM, RBUS_ELEMENT_TYPE_PROPERTY, {webpaDataGetHandler, webpaDataSetHandler, NULL, NULL, NULL, NULL}},
		{WEBPA_CONNECTED_CLIENT_PARAM, RBUS_ELEMENT_TYPE_PROPERTY, {webpaDataGetHandler, webpaDataSetHandler, NULL, NULL, NULL, NULL}},
		{WEBPA_NOTIFY_PARAM, RBUS_ELEMENT_TYPE_PROPERTY, {webpaDataGetHandler, webpaDataSetHandler, NULL, NULL, NULL, NULL}},
		{WEBPA_VERSION_PARAM, RBUS_ELEMENT_TYPE_PROPERTY, {webpaDataGetHandler, webpaDataSetHandler, NULL, NULL, NULL, NULL}}

	};
	ret = rbus_regDataElements(rbus_handle, NUM_WEBPA_ELEMENTS, dataElements);
	if(ret == RBUS_ERROR_SUCCESS)
	{
		WalInfo("Registered data element %s with rbus \n ", WEBPA_CMC_PARAM);
	}
	else
	{
		WalError("Failed in registering data element %s \n", WEBPA_CMC_PARAM);
		status = WDMP_FAILURE;
	}

	WalInfo("rbus reg status returned is %d\n", status);
	return status;
}
