//
// Created by evgen on 22.04.2021.
//

#ifndef THE_ARK_ENUMS_CPP
#define THE_ARK_ENUMS_CPP


enum AccidentSeverity {
    // давайте обусловимся оценивать порядок серьёзности события, порядком в этом enum
    NEGLIGIBLE,
    LIGHT,
    MEDIUM,
    SEVERE,
    DISASTROUS,
    CATASTROPHIC
};

enum Profession {
    UNEMPLOYED,
    WORKER,
    UNDEFINED,
    STUDYING
};

enum Services {
    Technical_Service,
    Biological_Service,
    Medical_Service,
    Navigation_Service,
    Emergency_Service,
    Social_Service,
    Clients_Of_Social_Service
};

enum Education{
    NotStudying,
    FirstYear,
    SecondYear,
    ThirdYear,
    ForthYear,
    FifthYear
};

#endif //THE_ARK_ENUMS_CPP
