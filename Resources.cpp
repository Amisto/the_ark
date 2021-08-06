//
// Created by Amisto on 4/2/2021.
//

#include "Resources.h"
#include "TheArk.h"
#include "TechnicalService.h"
#include "BiologicalService.h"
#include "EmergencyService.h"
#include "MedicalService.h"
#include "NavigationService.h"
#include "SocialService.h"
#include <vector>
#include <iostream>
#include <algorithm>

/*------------------------------------------CONSTRUCTORS----------------------------------------------*/

NotOrgRes::NotOrgRes(unsigned int total) : Resource(total), used_by_services({0, 0, 0, 0, 0, 0}) 
{

}

OrgRes::OrgRes(unsigned int total) : Resource(total)
{

}

Resource::Resource(unsigned int total)
{	
	this->components = std::stod(TheArk::get_instance()->getInterface()->getGeneral()["Proportion_of_components"]) * total;
	this->consumables = (1 -std::stod(TheArk::get_instance()->getInterface()->getGeneral()["Proportion_of_components"])) * total;
	this->used = 0; 
	this->junk = 0; 
	this->refuse = 0;
}

Resources::Resources()
{
	this->org_res = std::make_unique<OrgRes>(0);
	this->not_org_res = std::make_unique<NotOrgRes>(0);
}

/*---------------------------------------------GETTERS------------------------------------------------*/

unsigned int Resource::getComponents() const 
{
	return this->components;
}

unsigned int Resource::getRefuse() const 
{
	return this->refuse;
}

unsigned int Resource::getConsumables() const 
{
	return this->consumables;
}

unsigned int Resource::getUsed() const 
{
	return this->used;
}

unsigned int Resource::getJunk() const 
{
	return this->junk;
}

unsigned int Resources::getConsumables() const
{
	return this->org_res->getConsumables() + this->not_org_res->getConsumables();
}

unsigned int Resources::getComponents() const
{
	return this->org_res->getComponents() + this->not_org_res->getComponents();
}

unsigned int Resources::getJunk() const
{
	return this->org_res->getJunk() + this->not_org_res->getJunk();
}

unsigned int Resources::getRefuse() const
{
	return this->org_res->getRefuse() + this->not_org_res->getRefuse();
}

unsigned int  Resources::getUsed() const
{
	return this->org_res->getUsed() + this->not_org_res->getUsed();
}

unsigned int NotOrgRes::getUsedByService(Services id) const
{
	return this->used_by_services[id];
}

unsigned int Resources::getUsedByService(Services id) const
{
	return this->not_org_res->getUsedByService(id);
}

/*---------------------------------------------SETTERS-----------------------------------------------*/

unsigned int Resources::setComponentsToUsed(unsigned int current_usage, Services id) 
{
	return this->not_org_res->TakeComp(current_usage, id);
}

void Resources::setUsedToJunk(unsigned int current_broken, Services id) 
{
	this->not_org_res->ReturnJunk(current_broken, id);
}

void NotOrgRes::ReturnJunk(unsigned int isReturned, Services id)
{
    if (isReturned > used_by_services[id])
        isReturned = used_by_services[id];
    if (isReturned > used)
        isReturned = used;

	junk                 += isReturned;
	used_by_services[id] -= isReturned;
	used                 -= isReturned;
}

unsigned int NotOrgRes::TakeComp(unsigned int isNeeded, Services id)
{
	 if ( isNeeded <= this->components / 6 or used_by_services[id] == 0)
	 {
	     if (isNeeded > components)
	         isNeeded = components;

		 used                 += isNeeded;
		 components           -= isNeeded;
		 used_by_services[id] += isNeeded;

		 return isNeeded;
	 }

	 else
	 {
		 return (isNeeded * used) / ( 6 * used_by_services[id]);
	 }

}

/*-------------------------------------------SUPPORTING----------------------------------------------*/

double NotOrgRes::efficiencyConsumablesToComponents() const 
{
	return TheArk::get_instance()->getTechnicalService()->efficiencyConsumablesToComponents();
}

double NotOrgRes::efficiencyJunkToConsumables() const 
{
	return TheArk::get_instance()->getTechnicalService()->efficiencyJunkToConsumables();
}

double NotOrgRes::efficiencyJunkToRefuse() const 
{
	return TheArk::get_instance()->getTechnicalService()->efficiencyJunkToRefuse();
}

/*---------------------------------------------INIT--------------------------------------------------*/

void Resources::init(unsigned int total) 
{
	double not_org_prop = std::stod(TheArk::get_instance()->getInterface()->getGeneral()["Proportion_of_not_organic_resources"]); // !!!!!!!!
	if (not_org_prop < 1)
	{
		this->not_org_res = std::make_unique<NotOrgRes>(not_org_prop * total);
		this->org_res     = std::make_unique<OrgRes>( (1 - not_org_prop) * total);
	}
	else
	{
		std::cout<<"proportion of not org resources cannot be more than 1"<<std::endl;
	}
}

/*-----------------------------------------YEAR_PROCESS----------------------------------------------*/

void Resources::processYear() 
{	
	setComponentsToUsed(TheArk::get_instance()->getTechnicalService()->getResourceDemand(), Technical_Service);
	setComponentsToUsed(TheArk::get_instance()->getMedicalService()->getResourceDemand(), Medical_Service);
	setComponentsToUsed(TheArk::get_instance()->getNavigationService()->getResourceDemand(), Navigation_Service);
	setComponentsToUsed(TheArk::get_instance()->getEmergencyService()->getResourceDemand(), Emergency_Service);
	setComponentsToUsed(TheArk::get_instance()->getSocialService()->getResourceDemand(), Social_Service);
	
	this->org_res->YearProcess();
	this->not_org_res->YearProcess();
	
	setUsedToJunk(TheArk::get_instance()->getTechnicalService()->returnJunk(), Technical_Service);
	setUsedToJunk(TheArk::get_instance()->getMedicalService()->returnJunk(), Medical_Service);
	setUsedToJunk(TheArk::get_instance()->getNavigationService()->returnJunk(), Navigation_Service);
	setUsedToJunk(TheArk::get_instance()->getEmergencyService()->returnJunk(), Emergency_Service);
	setUsedToJunk(TheArk::get_instance()->getSocialService()->returnJunk(), Social_Service);
}

void NotOrgRes::YearProcess()
{
    auto d_consumables_components = std::min(this->consumables, (unsigned int)(this->consumables*efficiencyConsumablesToComponents()));
    this->consumables -= d_consumables_components;
	this->components  += d_consumables_components;

	// TODO: if we have an excess JunkToConsumables efficiency and not enough junk we end up with no refuse.
	// We should implement some kind of balance -  so that a part of junk goes both ways in any case.

	auto d_junk_consumables = std::min(this->junk, (unsigned int)(this->junk * efficiencyJunkToConsumables()));
    this->junk -= d_junk_consumables;
    this->consumables += d_junk_consumables;

    auto d_junk_refuse = std::min(this->junk, (unsigned int)(this->junk * efficiencyJunkToRefuse()));
    this->junk -= d_junk_refuse;
    this->refuse += d_junk_refuse;
}

void OrgRes::YearProcess()
{
	/*this->components    =   TheArk::get_instance()->getBiologicalService()->GetResource();
	this->refuse       -=   TheArk::get_instance()->getBiologicalService()->GetJunk();*/
}

