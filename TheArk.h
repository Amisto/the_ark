//
// Created by Amisto on 4/2/2021.
//

#ifndef THE_ARK_THEARK_H
#define THE_ARK_THEARK_H

#include <array>
#include <istream>
#include <ostream>
#include <iomanip>

class Service;
class TechnicalService;
class BiologicalService;
class MedicalService;
class NavigationService;
class EmergencyService;
class SocialService;
class Population;
class Resources;
class Interface;

class Service;
class TheArk {
private:
    unsigned int years_total;
    unsigned int current_year;
    std::array<Service*, 6> services;
    Population* population;
    Resources* resources;

    Interface* interface;

    void processYear();

    TheArk();
    static TheArk* instance;
public:
    static TheArk* get_instance();
    
    void init();     // интерфейс - ввод
    Interface* getInterface();

    unsigned int getYearsTotal() const;
    void setYearsTotal(unsigned int yearsTotal);
    unsigned int getCurrentYear() const;
    void setCurrentYear(unsigned int currentYear);

    TechnicalService* getTechnicalService();
    BiologicalService* getBiologicalService();
    MedicalService* getMedicalService();
    NavigationService* getNavigationService();
    EmergencyService* getEmergencyService();
    SocialService* getSocialService();
    std::array<Service*, 6>& getServices();
    Population *getPopulation() const;
    Resources *getResources() const;

    void flight();

    static void deleteArk();
    ~TheArk();
};

const unsigned int CELL_WIDTH = 11;

#endif //THE_ARK_THEARK_H
