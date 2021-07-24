//
// Created by Amisto on 4/2/2021.
//

#include "Resources.h"
#include "TheArk.h"
#include "TechnicalService.h"
#include <vector>
#include <iostream>

/*------------------------------------------CONSTRUCTORS----------------------------------------------*/

NotOrgRes::NotOrgRes(unsigned int total) : Resource(total), used_by_services({0, 0, 0, 0, 0, 0}) 
{

}

OrgRes::OrgRes(unsigned int total) : Resource(total)
{

}

Resource::Resource(unsigned int total) : components(0.5 * total), consumables(0.5 * total), used(0), junk(0), refuse(0)
{

}

Resources::Resources()
{
	this->org_res = new OrgRes(0);
	this->not_org_res = new NotOrgRes(0);
}

/*--------------------------------------------DESTRUCTORS---------------------------------------------*/

Resources::~Resources()
{
	delete this->org_res;
	delete this->not_org_res;
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

unsigned int Resources::getUsed() const
{
	return this->org_res->getUsed() + this->not_org_res->getUsed();
}

/*-----------------------------------------YEAR_PROCESS----------------------------------------------*/

void Resources::processYear() 
{
	this->org_res->YearProcess();
	this->not_org_res->YearProcess();
}

void NotOrgRes::YearProcess()
{
	
	
	if (this->consumables > 0)
	{
		this->consumables -= this->consumables*efficiencyConsumablesToComponents();
	}

	this->components    += this->consumables * efficiencyConsumablesToComponents();
	this->consumables   += this->junk * efficiencyJunkToConsumables();
	
	if (this->junk > 0)
	{
		this->junk -= this->junk * efficiencyJunkToConsumables();
	}
	
	if (this->junk > 0)
	{
		this->junk -= this->junk * efficiencyJunkToRefuse();
	}
	
	this->refuse       += this->junk * efficiencyJunkToRefuse();
}

void OrgRes::YearProcess()
{
	/*this->components    =   TheArk::get_instance()->getBiologicalService()->GetResource();
	this->refuse       -=   TheArk::get_instance()->getBiologicalService()->GetJunk();*/
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
	junk                 += isReturned;
	used_by_services[id] -= isReturned;
	used                 -= isReturned;
}

unsigned int NotOrgRes::TakeComp(unsigned int isNeeded, Services id)
{
	 if ( isNeeded <= this->components / 6 or used_by_services[id] == 0)
	 {
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
	not_org_res = new NotOrgRes(0.5 * total);
	org_res     = new OrgRes(0.5 * total);
}
