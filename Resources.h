//
// Created by Amisto on 4/2/2021.
//

#ifndef THE_ARK_RESOURCES_H
#define THE_ARK_RESOURCES_H

#include <vector>
#include "Enums.cpp"
#include <memory>

class Resource {								// interface for different types of resources
protected:
	unsigned int consumables, components, junk, refuse, used;
public:
	Resource(unsigned int total);

	unsigned int getConsumables() const;
	unsigned int getUsed() const;
	unsigned int getJunk() const;
	unsigned int getRefuse() const;
	unsigned int getComponents() const;
	
	void YearProcess();
};

class OrgRes: public Resource {							// organic resources are used only by biological service
public:
	OrgRes(unsigned int total);
	
	void YearProcess();
};

class NotOrgRes: public Resource {						// not organic resources
protected:
	std::vector<unsigned int> used_by_services;      			// vector contains an amount of current used resources for each service
public:
	NotOrgRes(unsigned int total);
	void YearProcess();

	unsigned int TakeComp(unsigned int isNeeded, Services id);			// add recycled junk to available resources
	void  ReturnJunk(unsigned int isReturned, Services id);			// remove an amount of currently used resources

	double efficiencyConsumablesToComponents() const;          		// processing consumables to components // technical service
	double efficiencyJunkToConsumables() const;                		// recycling junk to consumablse // technical service
	double efficiencyJunkToRefuse() const;                     		// recycling junk to refuse // technical service
};

class Resources {
protected:
	std::unique_ptr<OrgRes> org_res;
	std::unique_ptr<NotOrgRes> not_org_res;

public:
   	Resources();

	unsigned int setComponentsToUsed(unsigned int current_usage, Services id);	// method to be called by services to get resources
	void setUsedToJunk(unsigned int current_broken, Services id);			// method to be called by services to return junk

	unsigned int getConsumables() const;
	unsigned int getComponents() const;
	unsigned int getUsed() const;
	unsigned int getJunk() const;
	unsigned int getRefuse() const;
	
	void init(unsigned int total);
	void processYear(); 								// the process of year's changing of all resources' categories
};


#endif //THE_ARK_RESOURCES_H
