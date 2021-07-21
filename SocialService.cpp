//
// Created by Amisto on 4/2/2021.
//

#include "SocialService.h"

SocialService::SocialService()
{
    this->efficiency_percentage = 15;
    this->n_years_of_education = 5; // <- все изначально учатся 5 лет
    this->n_staff_we_want = 50; // <- на всякий случай, вдруг что поломается
    this->update_n_staff_we_want();
    this->suicide_counter = 0;
}

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
                if (person->getMoralHealth() != 100
                && person->getMoralHealth() > 3
                && person->getPhysicalHealth() != 0) {
                    if ((rand() % 100) <= PROBABILITY_CHANGE_MENTAL_HEALTH_LIGHT) {
                        person->setMoralHealth(person->getMoralHealth() + CHANGE_MENTAL_HEALTH);
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
                if ((rand() % 2) == 0) {
                    if (person->getMoralHealth() != 100
                    && person->getMoralHealth() > 3
                    && person->getPhysicalHealth() != 0) {
                        if ((rand() % 100) <= PROBABILITY_CHANGE_MENTAL_HEALTH_NEGLIGIBLE) {
                            person->setMoralHealth(person->getMoralHealth() +
                            2 * CHANGE_MENTAL_HEALTH);
                        }
                    }
                }
                else {
                    if ((person->getMoralHealth() != 0) && (person->getPhysicalHealth() > 3)){
                        if ((rand() % 100) <= 100 - PROBABILITY_CHANGE_MENTAL_HEALTH_NEGLIGIBLE) {
                            person->setMoralHealth(person->getMoralHealth() - 2 * CHANGE_MENTAL_HEALTH);
                        }
                        if ((rand() % 100) <= 100 - PROBABILITY_CHANGE_PHYSICAL_HEALTH_NEGLIGIBLE) {
                            person->setPhysicalHealth(person->getMoralHealth() - CHANGE_PHYSICAL_HEALTH);
                        }
                        if (person->getPhysicalHealth() == 0 || person->getMoralHealth() <= 3) {
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
                if (person->getMoralHealth() > 3 && person->getPhysicalHealth() != 0){
                    if ((rand() % 100) <= PROBABILITY_CHANGE_MENTAL_HEALTH) {
                        person->setMoralHealth(person->getMoralHealth() - 3 * CHANGE_MENTAL_HEALTH);
                    }
                    if ((rand() % 100) <= PROBABILITY_CHANGE_PHYSICAL_HEALTH) {
                        person->setMoralHealth(person->getMoralHealth() - 2 * CHANGE_MENTAL_HEALTH);
                    }
                    if (person->getPhysicalHealth() == 0 || person->getMoralHealth() <= 3) {
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
        if ((rand() % 100) <= 70) {
            for (auto& person : TheArk::get_instance()->getPopulation()->getPeople())
            {
                if (person->isAlive()) {
                    if (person->getMoralHealth() > 3 && person->getPhysicalHealth() != 0){
                        if ((rand() % 100) <= PROBABILITY_CHANGE_MENTAL_HEALTH) {
                            person->setMoralHealth(person->getMoralHealth() -
                            3 * CHANGE_MENTAL_HEALTH);
                        }
                        if ((rand() % 100) <= PROBABILITY_CHANGE_PHYSICAL_HEALTH) {
                            person->setPhysicalHealth(person->getMoralHealth() -
                            2 * CHANGE_PHYSICAL_HEALTH);
                        }
                        if (person->getPhysicalHealth() == 0 || person->getMoralHealth() <= 3) {
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
                    if (person->getMoralHealth() > 3 && person->getPhysicalHealth() != 0){
                        if ((rand() % 100) <= PROBABILITY_CHANGE_MENTAL_HEALTH) {
                            person->setMoralHealth(person->getMoralHealth() -
                            4 * CHANGE_MENTAL_HEALTH);
                        }
                        if ((rand() % 100) <= PROBABILITY_CHANGE_PHYSICAL_HEALTH) {
                            person->setPhysicalHealth(person->getMoralHealth() -
                            3 * CHANGE_PHYSICAL_HEALTH);
                        }
                        if (person->getPhysicalHealth() == 0 || person->getMoralHealth() <= 3) {
                            person->setIsAlive(false);
                        }
                    }
                }
            }
            count_unresolved_accident_severity++;
        }
        // иногда буду убивать уже убитых людей, но пока что ничего с страшного, позже логика усложнится
        for (auto& person : TheArk::get_instance()->getPopulation()->getPeople()) {
            if (rand() % 1000 < 2) {
                person->setIsAlive(false);
            }
        }
    }

    else if (AccidentSeverity::DISASTROUS == as) {
        // логика: серьёзное чрезвычайное событие: c вероятностью 30 % его удастся успешно решить,
        // но могут быть человеческие потери
        // (в следствии какких-либо психологических факторов) и психологические потери
        if ((rand() % 100) <= 30) {
            for (auto& person : TheArk::get_instance()->getPopulation()->getPeople())
            {
                if (person->isAlive()) {
                    if (person->getMoralHealth() > 3 && person->getPhysicalHealth() != 0){
                        if ((rand() % 100) <= PROBABILITY_CHANGE_MENTAL_HEALTH) {
                            person->setMoralHealth(person->getMoralHealth() -
                            4 * CHANGE_MENTAL_HEALTH);
                        }
                        if ((rand() % 100) <= PROBABILITY_CHANGE_PHYSICAL_HEALTH) {
                            person->setPhysicalHealth(person->getMoralHealth() -
                            3 * CHANGE_PHYSICAL_HEALTH);
                        }
                        if (person->getPhysicalHealth() == 0 || person->getMoralHealth() <= 3) {
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
                    if (person->getMoralHealth() > 3 && person->getPhysicalHealth() != 0){
                        if ((rand() % 100) <= PROBABILITY_CHANGE_MENTAL_HEALTH) {
                            person->setMoralHealth(person->getMoralHealth() -
                            5 * CHANGE_MENTAL_HEALTH);
                        }
                        if ((rand() % 100) <= PROBABILITY_CHANGE_PHYSICAL_HEALTH) {
                            person->setPhysicalHealth(person->getMoralHealth() -
                            3 * CHANGE_PHYSICAL_HEALTH);
                        }
                        if (person->getPhysicalHealth() == 0 || person->getMoralHealth() <= 3) {
                            person->setIsAlive(false);
                        }
                    }
                }
            }
            count_unresolved_accident_severity++;
        }
        // иногда буду убивать уже убитых людей, но пока что ничего с страшного, позже логика усложнится
        for (auto& person : TheArk::get_instance()->getPopulation()->getPeople()) {
            if (rand() % 1000 < 5) {
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
                if (person->getMoralHealth() > 3 && person->getPhysicalHealth() != 0){
                    if ((rand() % 100) <= PROBABILITY_CHANGE_MENTAL_HEALTH) {
                        person->setMoralHealth(person->getMoralHealth() - 7 * CHANGE_MENTAL_HEALTH);
                    }
                    if ((rand() % 100) <= PROBABILITY_CHANGE_PHYSICAL_HEALTH) {
                        person->setPhysicalHealth(person->getMoralHealth() - 5 * CHANGE_PHYSICAL_HEALTH);
                    }
                    if (person->getPhysicalHealth() == 0 || person->getMoralHealth() <= 3) {
                        person->setIsAlive(false);
                    }
                }
            }
        }
        for (auto& person : TheArk::get_instance()->getPopulation()->getPeople()) {
            if (rand() % 1000 < 10) {
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
    double moral_health_sum = 0;
    unsigned int population_size = TheArk::get_instance()->getPopulation()->getTotal();
    if (population_size == 0)
        return 0;
    for (auto person : TheArk::get_instance()->getPopulation()->getPeople())
    {
        moral_health_sum += person->getMoralHealth();
    }
    return moral_health_sum / population_size;
}

void SocialService::setState(double s) {/*nothing to do*/}

void SocialService::update_n_staff_we_want() {
    this->n_staff_we_want = (TheArk::get_instance()->getPopulation()->getPeople().size()
            / (2 * this->efficiency_percentage));
}

unsigned int SocialService::getResourceDemand() {
    return 0; // наша служба не нуждается в ресурсах на начальном этапе придумывания
}

unsigned int SocialService::getResourcePriority() {return 6;} // последний приоритет

unsigned int SocialService::getStaffDemand(){
    return this->n_staff_we_want;
}

unsigned int SocialService::getStaffPriority(){
    unsigned int labor_shortage_rate = 6 -
            (TheArk::get_instance()->getPopulation()->getAllClassification()
            [Services::Clients_Of_Social_Service].size() /
            (TheArk::get_instance()->getPopulation()->getAllClassification()
            [Services::Social_Service].size() * this->efficiency_percentage));
    if (labor_shortage_rate <= 0) labor_shortage_rate = 1;
    return labor_shortage_rate;
}

bool SocialService::changeStaff (int delta) {/*сделаем, если потребуется*/
    return true;}

bool SocialService::changeResources(int delta){/*сделаем, если потребуется*/
    return true;}

void SocialService::update_people(){
    for (auto person : TheArk::get_instance()->getPopulation()->getPeople())
    {
        this->update_person(*person);
    }
}

// самый главный самый жырный метод
void SocialService::process_year() {
    this->update_people();

    // так же тут будет обработка действий службы и тд и тп
}

void SocialService::update_person(Human &person) {

    // логика "если плохих событий не меньше, чем ментальная стабильность - то минус ментальное здоровье"
    if (person.getMentalStability() <= person.getAmountOfBlackAccidents())
        person.setMoralHealth((unsigned int) (person.getMoralHealth() / (1.2)));

    // логика "влияние ментального здоровья на жизнедеятельность"
    if (person.getMoralHealth() < 30) {
        if (person.getMoralHealth() < 20)
            if (person.isAbleToWork())
                person.setIsAbleToWork(false);
        if (person.isAbleToStudy())
            person.setIsAbleToStudy(false);
        if (person.getMoralHealth() < 3)
            person.setIsAlive(false);
    } else {
        if (person.getTypeAsAWorker()) {
            if (person.getYearOfEducation()) {
                person.setIsAbleToWork(false);
                person.setIsAbleToStudy(true);
            } else {
                person.setIsAbleToWork(true);
                person.setIsAbleToStudy(false);
            }
        }
    }

    // логика "сособен ли обучаться"
    if (!person.isAbleToStudy()){
        if (person.getAge() >= this->borderChildrenToAdults()
                && person.getMoralHealth() >= 75
                && person.getAmountOfBlackAccidents() < 5) {
            person.setIsAbleToStudy(true);
            if (person.getYearOfEducation() == NotStudying)
                person.setYearOfEducation(FirstYear);
        }
    }

    // логика "обработка обучения": если может, человек начинает учиться
    if (person.isAbleToStudy()) {
        if (person.getMoralHealth() < 40 || person.getPhysicalHealth() < 40) {
            person.setIsAbleToStudy(false);
        } else if (person.getYearOfEducation() != FifthYear) {
            person.setYearOfEducation(static_cast<Education>(person.getYearOfEducation() + 1));
            person.setMoralHealth(person.getMoralHealth() - 3);
        } else {
            person.setYearOfEducation(NotStudying);
            person.setIsAbleToWork(true);
            person.setIsAbleToStudy(false);
        }
    }

    // логика добавления в клиенты службы
    /*if (person.getMoralHealth() < 70)
        TheArk::get_instance()->getPopulation()->getAllClassification()
        [Classification_of_humans::Clients_Of_Social_Service].
        push_back(shared_ptr<Human>(&person)); // <- добавил поинтер в очередь
        // пока думаю, как обрабатывать повторы*/
}
