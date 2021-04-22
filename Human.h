//
// Created by evgen on 14.04.2021.
//

#ifndef THE_ARK_HUMAN_H
#define THE_ARK_HUMAN_H
#include "Enums.cpp"

class Human {
private:
    unsigned int MENTAL_STABILITY; // <- это поле контролирует СС

    unsigned int age;
    unsigned int physical_health;
    unsigned int moral_health; // <- это поле контролирует СС
    unsigned int amount_of_black_accidents; // количество негативных ситуаций, влияющих на человека
    // если их больше, чем моральная стабильность, то моральное здоровье человека уменьшается
    // <- это поле контролирует СС

    Profession type_as_a_worker; // возможный тип профессии можно увидеть в enum, в начале файла

    unsigned int year_of_education; // 0 - не учится
    // <- это поле контролирует СС

    bool is_alive;
    bool is_able_to_work; // <- это поле контролирует СС
    bool is_able_to_study; // <- это поле контролирует СС

public:
    Human();
    Human(unsigned int mentalStability, unsigned int PhysicalHealth);
    Human(unsigned int common_status_of_the_ark);

    unsigned int getMentalStability() const;
    unsigned int getAge() const;
    unsigned int getPhysicalHealth() const;
    unsigned int getMoralHealth() const;
    Profession getTypeAsAWorker() const;
    unsigned int getAmountOfBlackAccidents() const;
    unsigned int getYearOfEducation() const;
    bool isAlive() const;
    bool isAbleToWork() const;
    bool isAbleToStudy() const;

    void setAge(int Age);
    void setPhysicalHealth(unsigned int physicalHealth);
    void setMoralHealth(unsigned int moralHealth);
    void setTypeAsAWorker(Profession typeAsAWorker);
    void setIsAlive(bool isAlive);
    void setIsAbleToWork(bool isAbleToWork);
    void setIsAbleToStudy(bool isAbleToStudy);
    void setAmountOfBlackAccidents(unsigned int amountOfBlackAccidents);
    void setYearOfEducation(unsigned int yearOfEducation);

    friend class Population;
};


#endif //THE_ARK_HUMAN_H
