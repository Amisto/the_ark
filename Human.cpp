//
// Created by evgen on 14.04.2021.
//

#include "Human.h"
#include "RandomNumberGenerator.h"
#include "TheArk.h"
#include "SocialService.h"


//-----------------------Human-------------------------//
Human::Human()
{
    this->MENTAL_STABILITY = 10;
    this->age = 0;
    this->type_as_a_worker = Profession::UNDEFINED;
    this->is_alive = true;
    this->is_able_to_work = false;
    this->is_able_to_study = false;

    this->physical_health = 100;
    this->moral_health = TheArk::get_instance()->getRandomGenerator()->\
            getRandomInt(static_cast<int>(TheArk::get_instance()->getSocialService()->getState()), 100);

    this->year_of_education = Education::NotStudying;
}

Human::Human(unsigned int common_status_of_the_ark):
        Human(){
    this->physical_health *= (common_status_of_the_ark / 100);
}

unsigned int Human::getMentalStability() const {
    return MENTAL_STABILITY;
}

unsigned int Human::getAge() const {
    return age;
}

unsigned int Human::getPhysicalHealth() const {
    return physical_health;
}

unsigned int Human::getMentalHealth() const {
    return moral_health;
}

Profession Human::getTypeAsAWorker() const {
    return type_as_a_worker;
}

Education Human::getYearOfEducation() const {
    return year_of_education;
}

bool Human::isAlive() const {
    return is_alive;
}

bool Human::isAbleToWork() const {
    return is_able_to_work;
}

void Human::setYearOfEducation(Education yearOfEducation) {
    year_of_education = yearOfEducation;
}

void Human::setAge(unsigned int Age) {
    this->age = Age;
}

void Human::setPhysicalHealth(unsigned int physicalHealth) {
    this->physical_health = physicalHealth;
}

void Human::setMentalHealth(unsigned int moralHealth) {
    this->moral_health = moralHealth;
}

void Human::setTypeAsAWorker(Profession typeAsAWorker) {
    this->type_as_a_worker = typeAsAWorker;
}

void Human::setIsAlive(bool isAlive) {
    this->is_alive = isAlive;
}

void Human::setIsAbleToWork(bool isAbleToWork) {
    is_able_to_work = isAbleToWork;
}

bool Human::isAbleToStudy() const {
    return this->is_able_to_study;
}

void Human::setIsAbleToStudy(bool isAbleToStudy) {
    this->is_able_to_study = isAbleToStudy;
}

