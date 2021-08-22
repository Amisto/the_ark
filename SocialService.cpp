//
// Created by Amisto on 4/2/2021.
//

#include "SocialService.h"
#include "RandomNumberGenerator.h"
#include "Interface.h"
#include <iostream>
#include <algorithm>

SocialService::SocialService()
{
    state = std::stoi(TheArk::get_instance()->getInterface()->getServices()[Services::Social_Service]["State"]);
    efficiency_percentage = 15;
    n_years_of_education = 5; // <- все изначально учатся 5 лет
    n_staff_we_want = 50; // <- на всякий случай, вдруг что поломается
    suicide_counter = 0;
}

//--------------------------------Private Methods-----------------------------------//
void SocialService::set_state() {
    unsigned sum_of_mental_health = 0;
    unsigned sum_of_ph_health = 0;
    for (const auto& human: TheArk::get_instance()->getPopulation()->getPeople()) {
        sum_of_mental_health += human->getMentalHealth();
        sum_of_ph_health += human->getPhysicalHealth();
    }
    unsigned n_peop = TheArk::get_instance()->getPopulation()->getPeople().size();
    if (n_peop)
        state = (sum_of_mental_health / n_peop) /** 0.33 + (sum_of_ph_health / n_peop) * 0.33 + (n_peop / std::stoi(TheArk::get_instance()->getInterface()->getGeneral()["Population"])) * 0.33*/;
    else state = 0;
}

void SocialService::update_people(){
    for (auto person : TheArk::get_instance()->getPopulation()->getPeople())
    {
        update_person(person);
    }
}

void SocialService::update_person(std::shared_ptr<Human> person) {

    // логика "влияние ментального здоровья на жизнедеятельность"
    if (person->getMentalHealth() < std::stoi(TheArk::get_instance()->getInterface()->getServices()[Services::Social_Service]["Border_to_be_ill"])) {
        if (person->getMentalHealth() < std::stoi(TheArk::get_instance()->getInterface()->getServices()[Services::Social_Service]["Border_to_be_ill"]) - 10)
            if (person->isAbleToWork())
                person->setIsAbleToWork(false);
        if (person->isAbleToStudy())
            person->setIsAbleToStudy(false);
        if (person->getMentalHealth() < std::stoi(TheArk::get_instance()->getInterface()->getServices()[Services::Social_Service]["Suicide_border"]))
            person->setIsAlive(false);
    } else {
        if (person->getTypeAsAWorker()) {
            if (person->getYearOfEducation()) {
                person->setIsAbleToWork(false);
                person->setIsAbleToStudy(true);
            } else {
                person->setIsAbleToWork(true);
                person->setIsAbleToStudy(false);
            }
        }
    }

    // логика "сособен ли обучаться"
    if (!person->isAbleToStudy()){
        if (person->getAge() >= this->borderChildrenToAdults()
            && person->getMentalHealth() >= 75) {
            person->setIsAbleToStudy(true);
            if (person->getYearOfEducation() == NotStudying)
                person->setYearOfEducation(FirstYear);
        }
    }

    // логика "обработка обучения": если может, человек начинает учиться
    if (person->isAbleToStudy()) {
        if (person->getMentalHealth() < 40 || person->getPhysicalHealth() < 40) {
            person->setIsAbleToStudy(false);
        } else if (person->getYearOfEducation() != FifthYear) {
            person->setYearOfEducation(static_cast<Education>(person->getYearOfEducation() + 1));
            person->setMentalHealth(person->getMentalHealth() - 3);
        } else {
            person->setYearOfEducation(NotStudying);
            person->setIsAbleToWork(true);
            person->setIsAbleToStudy(false);
        }
    }
#if 1
    if (person->getMentalHealth() < 70) {
        TheArk::get_instance()->getPopulation()->getAllClassification()
            [Services::Clients_Of_Social_Service].push_back(person); // <- добавил поинтер в очередь
    }
#endif

}

//--------------------------------Public Methods-----------------------------------//

void SocialService::process_accident(AccidentSeverity as) {

    // элементарное изменение ментального здоровья в следствии каких-либо чрезвычайных событий:
    const unsigned int CHANGE_MENTAL_HEALTH = 1;
    // элементарное изменение физического здоровья в следствии каких-либо чрезвычайных событий:
    const unsigned int CHANGE_PHYSICAL_HEALTH = 1;
    // вероятность повышения ментального здоровья в следствии чрезвычайного события NEGLIGIBLE:
    const unsigned int PROBABILITY_CHANGE_MENTAL_HEALTH_LIGHT = 80;
    // вероятность повышения ментального здоровья в следствии чрезвычайных события LIGHT:
    const unsigned int PROBABILITY_CHANGE_MENTAL_HEALTH_NEGLIGIBLE = 40;
    // вероятность уменьшения физического здоровья в следствии чрезвычайных события LIGHT:
    const unsigned int PROBABILITY_CHANGE_PHYSICAL_HEALTH_NEGLIGIBLE = 40;
    // вероятность уменьшения ментального здоровья в следствии чрезвычайных событий
    // MEDIUM, SEVERE, DISASTROUS, CATASTROPHIC:
    const unsigned int PROBABILITY_CHANGE_MENTAL_HEALTH = 80;
    // вероятность уменьшения физического здоровья в следствии чрезвычайных событий
    // MEDIUM, SEVERE, DISASTROUS, CATASTROPHIC:
    const unsigned int PROBABILITY_CHANGE_PHYSICAL_HEALTH = 80;

    // Вероятность уменьшения того или иного парметра выходит на постоянный уровень ввиду того,
    // наш кораблся оборудован всеми необходимыми средствами безопасности, чтобы минимизировать потери

    // в самом начале выясняю какое чрезывачайное событие случилось

    if (AccidentSeverity::NEGLIGIBLE == as) {
        // логика: произошло чрезывачайное событие, которое людям удалось легко и успешно решить,
        // благодаря этому у каждого из них возрастает моральное здоровье с 70 % вероятностью на 2 условные единицы
        // (вероятность увеличения морального здоровья и само измененое могут корректироваться в константах функции)
        for (auto& person : TheArk::get_instance()->getPopulation()->getPeople())
        {
            if (person->isAlive()) {
                if (person->getMentalHealth() != 100
                && person->getMentalHealth() > 3
                && person->getPhysicalHealth() != 0) {
                    if ((TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 99)) <= PROBABILITY_CHANGE_MENTAL_HEALTH_LIGHT) {
                        person->setMentalHealth(person->getMentalHealth() + CHANGE_MENTAL_HEALTH);
                    }
                }
            }
        }
        count_resolved_accident_severity++;
    }

    else if (AccidentSeverity::LIGHT == as) {
        // логика: произошло чрезвычайное событие, которое людям удалось успешно решить,
        // но уже не так легко, как в предыдущем случае
        // благодаря этому у некоторых из них может увеличиться моральное здоровье на заданую константу,
        // так как человек молодец, потому что смог решить проблему
        // некоторые же люди же теряют моральное здоровье и физическое,
        // так как чрезвычайное событие как-то негативно на них повлияло
        for (auto& person : TheArk::get_instance()->getPopulation()->getPeople())
        {
            if (person->isAlive()) {
                if (TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 1)) {
                    if (person->getMentalHealth() != 100
                    && person->getMentalHealth() > 3
                    && person->getPhysicalHealth() != 0) {
                        if ((TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 99)) <= PROBABILITY_CHANGE_MENTAL_HEALTH_NEGLIGIBLE) {
                            person->setMentalHealth(person->getMentalHealth() +
                                                    2 * CHANGE_MENTAL_HEALTH);
                        }
                    }
                }
                else {
                    if ((person->getMentalHealth() != 0) && (person->getPhysicalHealth() > 3)){
                        if ((TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 99)) <= 100 - PROBABILITY_CHANGE_MENTAL_HEALTH_NEGLIGIBLE) {
                            person->setMentalHealth(person->getMentalHealth() - 2 * CHANGE_MENTAL_HEALTH);
                        }
                        if ((TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 99)) <= 100 - PROBABILITY_CHANGE_PHYSICAL_HEALTH_NEGLIGIBLE) {
                            person->setPhysicalHealth(person->getMentalHealth() - CHANGE_PHYSICAL_HEALTH);
                        }
                        if (person->getPhysicalHealth() == 0 || person->getMentalHealth() <= 3) {
                            person->setIsAlive(false);
                        }
                    }
                }
            }
        }
        count_resolved_accident_severity++;
    }

    else if (AccidentSeverity::MEDIUM == as) {
        // логика: чрезвычайное событие в следствии, которого удасться избежать человеческих потерь
        // (в следствии какких-либо психологических факторов), но будут значительные психологические потери
        for (auto& person : TheArk::get_instance()->getPopulation()->getPeople())
        {
            if (person->isAlive()) {
                if (person->getMentalHealth() > 3 && person->getPhysicalHealth() != 0){
                    if ((TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 99)) <= PROBABILITY_CHANGE_MENTAL_HEALTH) {
                        person->setMentalHealth(person->getMentalHealth() - 3 * CHANGE_MENTAL_HEALTH);
                    }
                    if ((TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 99)) <= PROBABILITY_CHANGE_PHYSICAL_HEALTH) {
                        person->setMentalHealth(person->getMentalHealth() - 2 * CHANGE_MENTAL_HEALTH);
                    }
                    if (person->getPhysicalHealth() == 0 || person->getMentalHealth() <= 3) {
                        person->setIsAlive(false);
                    }
                }
            }
        }
        count_resolved_accident_severity++;
    }

    else if (AccidentSeverity::SEVERE == as) {
        // логика: серьёзное чрезвычайное событие: c вероятностью 70 % его удасться успешно решить,
        // но могут быть маленькие человечекие потери
        // (в следствии какких-либо психологических факторов) и психологические потери
        if ((TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 99)) <= 70) {
            for (auto& person : TheArk::get_instance()->getPopulation()->getPeople())
            {
                if (person->isAlive()) {
                    if (person->getMentalHealth() > 3 && person->getPhysicalHealth() != 0){
                        if ((TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 99)) <= PROBABILITY_CHANGE_MENTAL_HEALTH) {
                            person->setMentalHealth(person->getMentalHealth() -
                                                    3 * CHANGE_MENTAL_HEALTH);
                        }
                        if ((TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 99)) <= PROBABILITY_CHANGE_PHYSICAL_HEALTH) {
                            person->setPhysicalHealth(person->getMentalHealth() -
                            2 * CHANGE_PHYSICAL_HEALTH);
                        }
                        if (person->getPhysicalHealth() == 0 || person->getMentalHealth() <= 3) {
                            person->setIsAlive(false);
                        }
                    }
                }
            }
            count_resolved_accident_severity++;
        }
        else {
            for (auto& person : TheArk::get_instance()->getPopulation()->getPeople())
            {
                if (person->isAlive()) {
                    if (person->getMentalHealth() > 3 && person->getPhysicalHealth() != 0){
                        if ((TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 99)) <= PROBABILITY_CHANGE_MENTAL_HEALTH) {
                            person->setMentalHealth(person->getMentalHealth() -
                                                    4 * CHANGE_MENTAL_HEALTH);
                        }
                        if ((TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 99)) <= PROBABILITY_CHANGE_PHYSICAL_HEALTH) {
                            person->setPhysicalHealth(person->getMentalHealth() -
                            3 * CHANGE_PHYSICAL_HEALTH);
                        }
                        if (person->getPhysicalHealth() == 0 || person->getMentalHealth() <= 3) {
                            person->setIsAlive(false);
                        }
                    }
                }
            }
            count_unresolved_accident_severity++;
        }
        // иногда буду убивать уже убитых людей, но пока что ничего с страшного, позже логика усложнится
        for (auto& person : TheArk::get_instance()->getPopulation()->getPeople()) {
            if (TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 999) < 2) {
                person->setIsAlive(false);
            }
        }
    }

    else if (AccidentSeverity::DISASTROUS == as) {
        // логика: серьёзное чрезвычайное событие: c вероятностью 30 % его удастся успешно решить,
        // но могут быть человеческие потери
        // (в следствии какких-либо психологических факторов) и психологические потери
        if ((TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 99)) <= 30) {
            for (auto& person : TheArk::get_instance()->getPopulation()->getPeople())
            {
                if (person->isAlive()) {
                    if (person->getMentalHealth() > 3 && person->getPhysicalHealth() != 0){
                        if ((TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 99)) <= PROBABILITY_CHANGE_MENTAL_HEALTH) {
                            person->setMentalHealth(person->getMentalHealth() -
                                                    4 * CHANGE_MENTAL_HEALTH);
                        }
                        if ((TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 99)) <= PROBABILITY_CHANGE_PHYSICAL_HEALTH) {
                            person->setPhysicalHealth(person->getMentalHealth() -
                            3 * CHANGE_PHYSICAL_HEALTH);
                        }
                        if (person->getPhysicalHealth() == 0 || person->getMentalHealth() <= 3) {
                            person->setIsAlive(false);
                        }
                    }
                }
            }
            count_resolved_accident_severity++;
        }
        else {
            for (auto& person : TheArk::get_instance()->getPopulation()->getPeople())
            {
                if (person->isAlive()) {
                    if (person->getMentalHealth() > 3 && person->getPhysicalHealth() != 0){
                        if ((TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 99)) <= PROBABILITY_CHANGE_MENTAL_HEALTH) {
                            person->setMentalHealth(person->getMentalHealth() -
                                                    5 * CHANGE_MENTAL_HEALTH);
                        }
                        if ((TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 99)) <= PROBABILITY_CHANGE_PHYSICAL_HEALTH) {
                            person->setPhysicalHealth(person->getMentalHealth() -
                            3 * CHANGE_PHYSICAL_HEALTH);
                        }
                        if (person->getPhysicalHealth() == 0 || person->getMentalHealth() <= 3) {
                            person->setIsAlive(false);
                        }
                    }
                }
            }
            count_unresolved_accident_severity++;
        }
        // иногда буду убивать уже убитых людей, но пока что ничего с страшного, позже логика усложнится
        for (auto& person : TheArk::get_instance()->getPopulation()->getPeople()) {
            if (TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 999) < 5) {
                person->setIsAlive(false);
            }
        }
    }

    else { // тут реализуется последний случай: AccidentSeverity::CATASTROPHIC == as
        // логика: самое серьёзное чрезвычайное событие: никогда его не удаться успешно решить, иногда, будут жертвы
        // (в следствии какких-либо психологических факторов), а так же большие психологические потери
        for (auto& person : TheArk::get_instance()->getPopulation()->getPeople())
        {
            if (person->isAlive()) {
                if (person->getMentalHealth() > 3 && person->getPhysicalHealth() != 0){
                    if ((TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 99)) <= PROBABILITY_CHANGE_MENTAL_HEALTH) {
                        person->setMentalHealth(person->getMentalHealth() - 7 * CHANGE_MENTAL_HEALTH);
                    }
                    if ((TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 99)) <= PROBABILITY_CHANGE_PHYSICAL_HEALTH) {
                        person->setPhysicalHealth(person->getMentalHealth() - 5 * CHANGE_PHYSICAL_HEALTH);
                    }
                    if (person->getPhysicalHealth() == 0 || person->getMentalHealth() <= 3) {
                        person->setIsAlive(false);
                    }
                }
            }
        }
        for (auto& person : TheArk::get_instance()->getPopulation()->getPeople()) {
            if (TheArk::get_instance()->getRandomGenerator()->getRandomInt(0, 999) < 10) {
                person->setIsAlive(false);
            }
        }
        count_unresolved_accident_severity++;
    }

    count_all_accident_severity++; // количество произошедших чрезвычайных событий увеличислось на 1
}

unsigned int SocialService::borderChildrenToAdults()
{
    return 18;
}

double SocialService::getState() {
    return state;
}

void SocialService::setState(double s) {state = static_cast<int>(s);};

unsigned int SocialService::getResourceDemand() {
    return 0; // наша служба не нуждается в ресурсах на начальном этапе придумывания
}

unsigned int SocialService::returnJunk() {
    return 0;
}

unsigned int SocialService::getStaffDemand(){
    return TheArk::get_instance()->getPopulation()->getAllClassification()[Services::Clients_Of_Social_Service].size() / 2;
}

//-----------------------------Main Method---------------------------//

void SocialService::process_year() {
    this->set_state();
    this->update_people();
#if 0
    std::clog << "In ss: " << TheArk::get_instance()->getPopulation()->getAllClassification()[Services::Social_Service].size() \
    << "Clients: " << TheArk::get_instance()->getPopulation()->getAllClassification()[Services::Clients_Of_Social_Service].size() << std::endl;
#endif

    // так же тут будет обработка действий службы и тд и тп
    TheArk::get_instance()->getPopulation()->getAllClassification()[Services::Clients_Of_Social_Service].clear();
}

