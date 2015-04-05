#ifndef DSSETTINGSMANAGER_H
#define DSSETTINGSMANAGER_H

#include <QFile>
#include "dsparameterset.hpp"

class DSSettingsManager
{
private:
    static void handleParsingError();
public:
    static void saveSettings(QFile&, const DSParameterSet*);
    static void loadSettings(QFile&, DSParameterSet*);
};

#endif // DSSETTINGSMANAGER_H
