//
// Created by evgen on 22.04.2021.
//

#ifndef THE_ARK_ENUMS_CPP
#define THE_ARK_ENUMS_CPP


enum AccidentSeverity {
    // давайте обусловимся оценивать порядок серьёзности события, порядком в этом enum
    NEGLIGIBLE,     // Практически бытовое событие, без последствий. Повд для баек и анекдотов.
    LIGHT,          // Небольшие потери в одной службе. Повод для жестоких шуток в адрес этой службы.
    MEDIUM,         // Отдельные слежбы несут умеренные потери. 
    SEVERE,         // Отдельные службы несут тяжелые потери.
    DISASTROUS,     // Корабль несет крайне серьезные потери, но едва выживает.
    CATASTROPHIC    // Корабль погибает с душераздирающими подробностями.
};

enum Profession {
    UNEMPLOYED,
    WORKER,
    DISABLED,
    UNDEFINED,
    STUDYING,
    CHILD,
    RETIRED
};

enum Services {
    Technical_Service,
    Biological_Service,
    Medical_Service,
    Navigation_Service,
    Emergency_Service,
    Social_Service,
};

enum Education {
    NotStudying,
    FirstYear,
    SecondYear,
    ThirdYear,
    ForthYear,
    FifthYear
};

enum RangeOfAge {
    Baby,
    Child,
    Teenager,
    Youth,
    Adult,
    Old
};

enum FlightStage {
    ACCELERATION,
    STABLE,
    MANEUVER // Increases wear of devices
};

#endif //THE_ARK_ENUMS_CPP
