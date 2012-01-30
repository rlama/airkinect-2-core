//
//  KinectDeviceManager.cpp
//  KinectExtension
//
//  Created by Wouter Verweirder on 24/01/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "KinectDeviceManager.h"

KinectDeviceManager::KinectDeviceManager()
{
    printf("KinectDeviceManager::KinectDeviceManager()\n");
}

bool KinectDeviceManager::isStarted()
{
    return started;
}

int KinectDeviceManager::getNumDevices()
{
    //printf("KinectDeviceManager::getNumDevices()\n");
    if(started)
    {
        XnStatus rc;
        xn::NodeInfoList deviceNodes;
        rc = context.EnumerateProductionTrees( XN_NODE_TYPE_DEVICE, NULL, deviceNodes, NULL );
        
        numDevices = 0;
        
        //get number of devices -------------------------------------------------------------------------------->
        xn::NodeInfoList::Iterator deviceIter = deviceNodes.Begin();
        for( ; deviceIter != deviceNodes.End(); ++deviceIter )
            ++numDevices;
    }
    return numDevices;
}

void KinectDeviceManager::startUp()
{
    //printf("KinectDeviceManager::startUp()\n");
    if(!started)
    {
        started = true;
        
        numDevices = 0;
        
        XnStatus rc;
        xn::EnumerationErrors errors;
        rc = context.Init();
        if (rc == XN_STATUS_NO_NODE_PRESENT)
        {
            XnChar strError[1024];
            errors.ToString(strError, 1024);
            printf("%s\n", strError);
            started = false;
        }
        else if (rc != XN_STATUS_OK)
        {
            printf("Open failed: %s\n", xnGetStatusString(rc));
            started = false;
        }
    }
}

void KinectDeviceManager::shutDown()
{
    //printf("KinectDeviceManager::shutDown()\n");
    if(started)
    {
        context.Release();
        
        numDevices = 0;
        
        started = false;
    }
}

KinectDevice *KinectDeviceManager::getDevice(int nr, FREContext freContext)
{
    //printf("KinectDeviceManager::getDevice()\n");
    KinectDevice* instance = NULL;
    std::map<int, KinectDevice*>::iterator it = deviceMap.find(nr);
    
    if (it != deviceMap.end())
    {
        instance = (KinectDevice*)(it->second);
    }
    else
    {
        instance = new KinectDevice(nr, context);
        deviceMap[nr] = instance;
    }
    
    instance->freContext = freContext;
    
    return instance;
}