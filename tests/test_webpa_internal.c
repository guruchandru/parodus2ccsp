#include<stdio.h>
#include <CUnit/Basic.h>

#include "../source/include/webpa_rbus.h"
#include "../source/include/webpa_adapter.h"
#include "../source/broadband/include/webpa_internal.h"
#include "../source/app/libpd.h"

#define TEST_COMPONENT_NAME "TestComp"
#define UNUSED(x) (void )(x)

rbusHandle_t handle;
ANSC_HANDLE bus_handle;
int numLoops;

int getWebpaParameterValues(char **parameterNames, int paramCount, int *val_size, parameterValStruct_t ***val)
{
    UNUSED(parameterNames); UNUSED(paramCount); UNUSED(val_size); UNUSED(val);
    return 0;
}

int setWebpaParameterValues(parameterValStruct_t *val, int paramCount, char **faultParam )
{
    UNUSED(faultParam); UNUSED(paramCount); UNUSED(val);
    return 0;
}

void setAttributes(param_t *attArr, const unsigned int paramCount, money_trace_spans *timeSpan, WDMP_STATUS *retStatus)
{
    UNUSED(attArr); UNUSED(paramCount); UNUSED(timeSpan); UNUSED(retStatus);
    return;
}

void addRowTable(char *objectName, TableData *list,char **retObject, WDMP_STATUS *retStatus)
{
    UNUSED(objectName); UNUSED(list); UNUSED(retObject); UNUSED(retStatus);
    return;
}
void deleteRowTable(char *object,WDMP_STATUS *retStatus)
{
    UNUSED(object); UNUSED(retStatus);
    return;
}

void replaceTable(char *objectName,TableData * list,unsigned int paramcount,WDMP_STATUS *retStatus)
{
    UNUSED(objectName); UNUSED(list); UNUSED(paramcount); UNUSED(retStatus);
    return;
}
void getAttributes(const char *paramName[], const unsigned int paramCount, money_trace_spans *timeSpan, param_t **attr, int *retAttrCount, WDMP_STATUS *retStatus)
{
    UNUSED(paramName); UNUSED(paramCount); UNUSED(timeSpan); UNUSED(attr); UNUSED(retAttrCount); UNUSED(retStatus);
    return;
}

int CcspBaseIf_discComponentSupportingNamespace ( void* bus_handle, const char* dst_component_id, const char *name_space, const char *subsystem_prefix, componentStruct_t ***components, int *size)
{
    UNUSED(bus_handle); UNUSED(dst_component_id); UNUSED(name_space); UNUSED(subsystem_prefix); UNUSED(components); UNUSED(size);
    return 0;
}

int CcspBaseIf_isSystemReady (void* bus_handle, const char* dst_component_id, dbus_bool *val)
{
    UNUSED(bus_handle); UNUSED(dst_component_id);
    *val = 1;
    return 1;
}

int  CcspBaseIf_Register_Event(void* bus_handle, const char* sender, const char* event_name)
{
   printf("Inside reg\n");
   UNUSED(bus_handle); UNUSED(sender); UNUSED(event_name);
   return 0;
}

void CcspBaseIf_SetCallback2(void* bus_handle, char *name, void*  func, void * user_data)
{
    printf("Inside setcallbk\n");
    UNUSED(bus_handle); UNUSED(name); UNUSED(func); UNUSED(user_data);
    return;
}

// Test case for test_initComponentCaching success
void test_initComponentCaching()
{
    WalInfo("\n**************************************************\n");
    int ret = -1;
    const char *pComponentName = TEST_COMPONENT_NAME;
	WalInfo("********** Starting component: %s **********\n ", pComponentName); 
        drop_root_privilege();
	/* Backend Manager for Webpa Creation and Initilization 
    CosaWebpaBEManagerCreate( );*/
	if(isRbusEnabled())
	{
	        webpaRbusInit(pComponentName);
    }
    printf("After rbus\n");
	//ret = waitForOperationalReadyCondition();
    ret = 0;
	//libpd_client_mgr();
	//WalInfo("Syncing backend manager with DB....\n");
	//CosaWebpaSyncDB();
	//WalInfo("Webpa backend manager is in sync with DB\n");
    initComponentCaching(ret);
    sleep(70);
	//webpaRbus_Uninit();
}

void add_suites( CU_pSuite *suite )
{
	*suite = CU_add_suite( "tests", NULL, NULL );
    CU_add_test( *suite, "test initComponentCaching", test_initComponentCaching);
}


int main( int argc, char *argv[] )
{
	unsigned rv = 1;
    CU_pSuite suite = NULL;
 
    (void ) argc;
    (void ) argv;
    
    if( CUE_SUCCESS == CU_initialize_registry() ) {
        add_suites( &suite );

        if( NULL != suite ) {
            CU_basic_set_mode( CU_BRM_VERBOSE );
            CU_basic_run_tests();
            printf( "\n" );
            CU_basic_show_failures( CU_get_failure_list() );
            printf( "\n\n" );
            rv = CU_get_number_of_tests_failed();
        }
        CU_cleanup_registry();
    }
    return rv;
}


