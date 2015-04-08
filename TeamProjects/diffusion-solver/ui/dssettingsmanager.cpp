#include "dssettingsmanager.hpp"

#include <stdexcept>

DSSettingsManager::DSSettingsManager() : parametersPtr(nullptr)
{
}
DSSettingsManager::DSSettingsManager(DSParameterSet* parameters)
{
    if(parameters == nullptr)
        throw std::runtime_error("Error in DSSettings manager constructor.");
    parametersPtr = parameters;
}

void DSSettingsManager::handleParsingError()
{
    throw std::runtime_error("Ошибка при чтении параметров");
}

void DSSettingsManager::saveSettings(QFile& outputFile)
{
    writeStream = new QXmlStreamWriter(&outputFile);

    writeStream->setAutoFormattingIndent(3);
    writeStream->setAutoFormatting(true);

    writeStream->writeStartDocument();
    writeStream->writeStartElement("dssettings");

    writeStream->writeStartElement("lambda1");
    writeStream->writeAttribute("value", QString::number(parametersPtr->GetLambda1(), 'g', 16));
    writeStream->writeEndElement(); //labmda1

    writeStream->writeStartElement("lambda2");
    writeStream->writeAttribute("value", QString::number(parametersPtr->GetLambda2(), 'g', 16));
    writeStream->writeEndElement(); //labmda2

    writeStream->writeStartElement("K");
    writeStream->writeAttribute("value", QString::number(parametersPtr->GetK(), 'g', 16));
    writeStream->writeEndElement(); //K

    writeStream->writeStartElement("C");
    writeStream->writeAttribute("value", QString::number(parametersPtr->GetC(), 'g', 16));
    writeStream->writeEndElement(); //C

    writeStream->writeStartElement("Rho");
    writeStream->writeAttribute("value", QString::number(parametersPtr->GetRho(), 'g', 16));
    writeStream->writeEndElement(); //Rho

    writeStream->writeStartElement("Nu");
    writeStream->writeAttribute("value", QString::number(parametersPtr->GetNu(), 'g', 16));
    writeStream->writeEndElement(); //Nu

    writeStream->writeStartElement("Gamma");
    writeStream->writeAttribute("value", QString::number(parametersPtr->GetGamma(), 'g', 16));
    writeStream->writeEndElement(); //Gamma

    writeStream->writeStartElement("TimeStep");
    writeStream->writeAttribute("value", QString::number(parametersPtr->GetTimeStep(), 'g', 16));
    writeStream->writeEndElement(); //TimeStep

    writeStream->writeStartElement("ActivatorAccuracy");
    writeStream->writeAttribute("value", QString::number(parametersPtr->GetActivatorAccuracy(), 'g', 16));
    writeStream->writeEndElement(); //ActivatorAccuracy

    writeStream->writeStartElement("InhibitorAccuracy");
    writeStream->writeAttribute("value", QString::number(parametersPtr->GetInhibitorAccuracy(), 'g', 16));
    writeStream->writeEndElement(); //InhibitorAccuracy

    writeStream->writeStartElement("GridDimension");
    writeStream->writeAttribute("value", QString::number(parametersPtr->GetGridDimension()));
    writeStream->writeEndElement(); //GridDimension

    writeStream->writeStartElement("IterationsLimit");
    writeStream->writeAttribute("value", QString::number(parametersPtr->GetIterationsLimit()));
    writeStream->writeEndElement(); //IterationsLimit

    writeStream->writeStartElement("InitialConditions");
    writeInitialConditions();
    writeStream->writeEndElement(); //InitialConditions

    writeStream->writeEndElement();//dssettings
    writeStream->writeEndDocument();

    delete writeStream;
}

void DSSettingsManager::loadSettings(QFile& inputFile)
{
    readStream = new QXmlStreamReader(&inputFile);

    if(readStream->readNext() == QXmlStreamReader::StartDocument)    {
        if(readStream->readNext() == QXmlStreamReader::StartElement &&
                readStream->name() == "dssettings")
            while(!readStream->atEnd())
            {
                if(readStream->readNext() == QXmlStreamReader::EndElement
                        || readStream->name() == "")
                    continue;

                QXmlStreamAttributes attributes = readStream->attributes();
                double currentAttributeValue = 0;
                bool isParameterValid = true;
                if(attributes.hasAttribute("value"))
                    currentAttributeValue = attributes.value("value").toDouble();

                if(readStream->name() == "lambda1")
                    isParameterValid = parametersPtr->SetLambda1(currentAttributeValue);
                else if(readStream->name() == "lambda2")
                    isParameterValid = parametersPtr->SetLambda2(currentAttributeValue);
                else if(readStream->name() == "C")
                    isParameterValid = parametersPtr->SetC(currentAttributeValue);
                else if(readStream->name() == "K")
                    isParameterValid = parametersPtr->SetK(currentAttributeValue);
                else if(readStream->name() == "Rho")
                    isParameterValid = parametersPtr->SetRho(currentAttributeValue);
                else if(readStream->name() == "Nu")
                    isParameterValid = parametersPtr->SetNu(currentAttributeValue);
                else if(readStream->name() == "Gamma")
                    isParameterValid = parametersPtr->SetGamma(currentAttributeValue);
                else if(readStream->name() == "TimeStep")
                    isParameterValid = parametersPtr->SetTimeStep(currentAttributeValue);
                else if(readStream->name() == "ActivatorAccuracy")
                    isParameterValid = parametersPtr->SetActivatorAccuracy(currentAttributeValue);
                else if(readStream->name() == "InhibitorAccuracy")
                    isParameterValid = parametersPtr->SetInhibitorAccuracy(currentAttributeValue);
                else if(readStream->name() == "GridDimension")
                    isParameterValid = parametersPtr->SetGridDimension((int)currentAttributeValue);
                else if(readStream->name() == "IterationsLimit")
                    isParameterValid = parametersPtr->SetIterationsLimit((int)currentAttributeValue);
                else if (readStream->name() == "InitialConditions")
                    readInitialConditions();
                else
                    handleParsingError();
                if(!isParameterValid || readStream->hasError())
                    handleParsingError();
            }
        else
            handleParsingError();
    }
    else
        handleParsingError();
    delete readStream;
}
void DSSettingsManager::writeInitialConditions()
{
    writeStream->writeStartElement("ActivatorInitialConditions");

    writeStream->writeEndElement();


    writeStream->writeStartElement("InhibitorInitialConditions");

    writeStream->writeEndElement();
}
void DSSettingsManager::readInitialConditions()
{
    readStream->readNext();
    readStream->readNext();
    readStream->readNext();
    readStream->readNext();
    readStream->readNext();
    readStream->readNext();
}
