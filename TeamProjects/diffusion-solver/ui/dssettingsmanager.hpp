#ifndef DSSETTINGSMANAGER_H
#define DSSETTINGSMANAGER_H

#include <QFile>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include "dsparameterset.hpp"

class DSSettingsManager
{
private:
    DSParameterSet* parametersPtr;
    QXmlStreamWriter* writeStream;
    QXmlStreamReader* readStream;

    void handleParsingError();
    void writeInitialConditions();
    void readInitialConditions();
public:
    DSSettingsManager(DSParameterSet*);
    DSSettingsManager();
    void saveSettings(QFile&);
    void loadSettings(QFile&);
};

#endif // DSSETTINGSMANAGER_H
