#include "AF.h"
#include "OSAL.h"
#include "ZComDef.h"
#include "ZDConfig.h"

#include "zcl.h"
#include "zcl_general.h"
#include "zcl_ha.h"

#include "zcl_freepadapp.h"

/*********************************************************************
 * CONSTANTS
 */

#define FREEPADAPP_DEVICE_VERSION 2
#define FREEPADAPP_FLAGS 0

#define FREEPADAPP_HWVERSION 1
#define FREEPADAPP_ZCLVERSION 1

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

// Global attributes
const uint16 zclFreePadApp_clusterRevision_all = 0x0001;
uint8 zclFreePadApp_BatteryVoltage = 0xff;
uint8 zclFreePadApp_BatteryPercentageRemainig = 0xff;

// Basic Cluster
const uint8 zclFreePadApp_HWRevision = FREEPADAPP_HWVERSION;
const uint8 zclFreePadApp_ZCLVersion = FREEPADAPP_ZCLVERSION;
//{lenght, 'd', 'a', 't', 'a'}
const uint8 zclFreePadApp_ManufacturerName[] = {9, 'm', 'o', 'd', 'k', 'a', 'm', '.', 'r', 'u'};
const uint8 zclFreePadApp_ModelId[] = {14, 'D', 'I', 'Y', 'R', 'u', 'Z', '_', 'F', 'r', 'e', 'e', 'P', 'a', 'd'};
const uint8 zclFreePadApp_DateCode[] = {8, '2', '0', '2', '0', '0', '4', '2', '4'};
const uint8 zclFreePadApp_PowerSource = POWER_SOURCE_BATTERY;

uint8 zclFreePadApp_LocationDescription[17] = {16,  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
                                               ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
uint8 zclFreePadApp_PhysicalEnvironment = 0;
uint8 zclFreePadApp_DeviceEnable = DEVICE_ENABLED;

// Identify Cluster
uint16 zclFreePadApp_IdentifyTime;

/*********************************************************************
 * ATTRIBUTE DEFINITIONS - Uses REAL cluster IDs
 */
CONST zclAttrRec_t zclFreePadApp_Attrs[] = {
    // *** General Basic Cluster Attributes ***
    {ZCL_CLUSTER_ID_GEN_BASIC, // Cluster IDs - defined in the foundation (ie.
                               // zcl.h)
     {
         // Attribute record
         ATTRID_BASIC_HW_VERSION,          // Attribute ID - Found in Cluster Library
                                           // header (ie. zcl_general.h)
         ZCL_DATATYPE_UINT8,               // Data Type - found in zcl.h
         ACCESS_CONTROL_READ,              // Variable access control - found in zcl.h
         (void *)&zclFreePadApp_HWRevision // Pointer to attribute variable
     }},
    {ZCL_CLUSTER_ID_GEN_BASIC,
     {// Attribute record
      ATTRID_BASIC_ZCL_VERSION, ZCL_DATATYPE_UINT8, ACCESS_CONTROL_READ, (void *)&zclFreePadApp_ZCLVersion}},
    {ZCL_CLUSTER_ID_GEN_BASIC,
     {// Attribute record
      ATTRID_BASIC_MANUFACTURER_NAME, ZCL_DATATYPE_CHAR_STR, ACCESS_CONTROL_READ,
      (void *)&zclFreePadApp_ManufacturerName}},
    {ZCL_CLUSTER_ID_GEN_BASIC,
     {// Attribute record
      ATTRID_BASIC_MODEL_ID, ZCL_DATATYPE_CHAR_STR, ACCESS_CONTROL_READ, (void *)&zclFreePadApp_ModelId}},
    {ZCL_CLUSTER_ID_GEN_BASIC,
     {// Attribute record
      ATTRID_BASIC_DATE_CODE, ZCL_DATATYPE_CHAR_STR, ACCESS_CONTROL_READ, (void *)&zclFreePadApp_DateCode}},
    {ZCL_CLUSTER_ID_GEN_BASIC,
     {// Attribute record
      ATTRID_BASIC_POWER_SOURCE, ZCL_DATATYPE_ENUM8, ACCESS_CONTROL_READ, (void *)&zclFreePadApp_PowerSource}},
    {ZCL_CLUSTER_ID_GEN_BASIC,
     {// Attribute record
      ATTRID_BASIC_LOCATION_DESC, ZCL_DATATYPE_CHAR_STR, (ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE),
      (void *)&zclFreePadApp_LocationDescription}},
    {ZCL_CLUSTER_ID_GEN_BASIC,
     {// Attribute record
      ATTRID_BASIC_PHYSICAL_ENV, ZCL_DATATYPE_ENUM8, (ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE),
      (void *)&zclFreePadApp_PhysicalEnvironment}},
    {ZCL_CLUSTER_ID_GEN_BASIC,
     {// Attribute record
      ATTRID_BASIC_DEVICE_ENABLED, ZCL_DATATYPE_BOOLEAN, (ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE),
      (void *)&zclFreePadApp_DeviceEnable}},

    // *** Identify Cluster Attribute ***
    {ZCL_CLUSTER_ID_GEN_IDENTIFY,
     {// Attribute record
      ATTRID_IDENTIFY_TIME, ZCL_DATATYPE_UINT16, (ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE),
      (void *)&zclFreePadApp_IdentifyTime}},

    {ZCL_CLUSTER_ID_GEN_BASIC,
     {// Attribute record
      ATTRID_CLUSTER_REVISION, ZCL_DATATYPE_UINT16, ACCESS_CONTROL_READ, (void *)&zclFreePadApp_clusterRevision_all}},

    {ZCL_CLUSTER_ID_GEN_POWER_CFG,
     {// Attribute record
      ATTRID_POWER_CFG_BATTERY_VOLTAGE, ZCL_DATATYPE_UINT8, ACCESS_CONTROL_READ,
      (void *)&zclFreePadApp_BatteryVoltage}},

    {ZCL_CLUSTER_ID_GEN_POWER_CFG,
     {// Attribute record
      ATTRID_POWER_CFG_BATTERY_PERCENTAGE_REMAINING, ZCL_DATATYPE_UINT8, ACCESS_CONTROL_READ,
      (void *)&zclFreePadApp_BatteryPercentageRemainig}},

    {ZCL_CLUSTER_ID_GEN_IDENTIFY,
     {// Attribute record
      ATTRID_CLUSTER_REVISION, ZCL_DATATYPE_UINT16, ACCESS_CONTROL_READ, (void *)&zclFreePadApp_clusterRevision_all}},
};

uint8 CONST zclFreePadApp_NumAttributes = (sizeof(zclFreePadApp_Attrs) / sizeof(zclFreePadApp_Attrs[0]));

const cId_t zclSampleSw_InClusterList[] = {ZCL_CLUSTER_ID_GEN_BASIC, ZCL_CLUSTER_ID_GEN_IDENTIFY,
                                           ZCL_CLUSTER_ID_GEN_ON_OFF_SWITCH_CONFIG};

#define ZCLSAMPLESW_MAX_INCLUSTERS (sizeof(zclSampleSw_InClusterList) / sizeof(zclSampleSw_InClusterList[0]))

const cId_t zclSampleSw_OutClusterList[] = {ZCL_CLUSTER_ID_GEN_IDENTIFY, ZCL_CLUSTER_ID_GEN_ON_OFF};

#define ZCLSAMPLESW_MAX_OUTCLUSTERS (sizeof(zclSampleSw_OutClusterList) / sizeof(zclSampleSw_OutClusterList[0]))

SimpleDescriptionFormat_t zclFreePadApp_SimpleDescs[FREEPAD_BUTTONS_COUNT];

void zclFreePadApp_InitClusters(void) {
    for (int i = 0; i < (int)FREEPAD_BUTTONS_COUNT; i++) {
        zclFreePadApp_SimpleDescs[i].EndPoint = i + 1;
        zclFreePadApp_SimpleDescs[i].AppProfId = ZCL_HA_PROFILE_ID;
        zclFreePadApp_SimpleDescs[i].AppDeviceId = ZCL_HA_DEVICEID_REMOTE_CONTROL;
        zclFreePadApp_SimpleDescs[i].AppDevVer = FREEPADAPP_DEVICE_VERSION;
        zclFreePadApp_SimpleDescs[i].Reserved = FREEPADAPP_FLAGS; // AF_V1_SUPPORT uses for AppFlags:4.
        zclFreePadApp_SimpleDescs[i].AppNumInClusters = ZCLSAMPLESW_MAX_INCLUSTERS;
        zclFreePadApp_SimpleDescs[i].pAppInClusterList = (cId_t *)zclSampleSw_InClusterList;
        zclFreePadApp_SimpleDescs[i].AppNumOutClusters = ZCLSAMPLESW_MAX_OUTCLUSTERS;
        zclFreePadApp_SimpleDescs[i].pAppOutClusterList = (cId_t *)zclSampleSw_OutClusterList;
    }
}
uint8 zclFreePadApp_SimpleDescsCount = FREEPAD_BUTTONS_COUNT;