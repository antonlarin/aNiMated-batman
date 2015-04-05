#include "dssettingsmanager.hpp"

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <stdexcept>

void DSSettingsManager::handleParsingError()
{
    throw std::exception("Ошибка при чтении параметров");
}

void DSSettingsManager::saveSettings(QFile& outputFile, const DSParameterSet* settings)
{
    QXmlStreamWriter writeStream(&outputFile);

    writeStream.setAutoFormattingIndent(3);
    writeStream.setAutoFormatting(true);

    writeStream.writeStartDocument();
    writeStream.writeStartElement("dssettings");

    writeStream.writeStartElement("lambda1");
    writeStream.writeAttribute("value", QString::number(settings->GetLambda1(), 'g', 16));
    writeStream.writeEndElement(); //labmda1

    writeStream.writeStartElement("lambda2");
    writeStream.writeAttribute("value", QString::number(settings->GetLambda2(), 'g', 16));
    writeStream.writeEndElement(); //labmda2

    writeStream.writeStartElement("K");
    writeStream.writeAttribute("value", QString::number(settings->GetK(), 'g', 16));
    writeStream.writeEndElement(); //K

    writeStream.writeStartElement("C");
    writeStream.writeAttribute("value", QString::number(settings->GetC(), 'g', 16));
    writeStream.writeEndElement(); //C

    writeStream.writeStartElement("Rho");
    writeStream.writeAttribute("value", QString::number(settings->GetRho(), 'g', 16));
    writeStream.writeEndElement(); //Rho

    writeStream.writeStartElement("Nu");
    writeStream.writeAttribute("value", QString::number(settings->GetNu(), 'g', 16));
    writeStream.writeEndElement(); //Nu

    writeStream.writeStartElement("Gamma");
    writeStream.writeAttribute("value", QString::number(settings->GetGamma(), 'g', 16));
    writeStream.writeEndElement(); //Gamma

    writeStream.writeStartElement("TimeStep");
    writeStream.writeAttribute("value", QString::number(settings->GetTimeStep(), 'g', 16));
    writeStream.writeEndElement(); //TimeStep

    writeStream.writeStartElement("ActivatorAccuracy");
    writeStream.writeAttribute("value", QString::number(settings->GetActivatorAccuracy(), 'g', 16));
    writeStream.writeEndElement(); //ActivatorAccuracy

    writeStream.writeStartElement("InhibitorAccuracy");
    writeStream.writeAttribute("value", QString::number(settings->GetInhibitorAccuracy(), 'g', 16));
    writeStream.writeEndElement(); //InhibitorAccuracy

    writeStream.writeStartElement("GridDimension");
    writeStream.writeAttribute("value", QString::number(settings->GetGridDimension()));
    writeStream.writeEndElement(); //GridDimension

    writeStream.writeStartElement("IterationsLimit");
    writeStream.writeAttribute("value", QString::number(settings->GetIterationsLimit()));
    writeStream.writeEndElement(); //IterationsLimit

    writeStream.writeEndElement();//dssettings
    writeStream.writeEndDocument();
}

void DSSettingsManager::loadSettings(QFile& inputFile, DSParameterSet* settings)
{
    QXmlStreamReader readStream(&inputFile);

    if(readStream.readNext() == QXmlStreamReader::StartDocument)    {
        if(readStream.readNext() == QXmlStreamReader::StartElement &&
                readStream.name() == "dssettings")
            while(!readStream.atEnd())
            {
                if(readStream.readNext() == QXmlStreamReader::EndElement
                        || readStream.name() == "")
                    continue;

                QXmlStreamAttributes attributes = readStream.attributes();
                double currentAttributeValue = 0;
                bool isParameterValid = false;
                if(attributes.hasAttribute("value"))
                    currentAttributeValue = attributes.value("value").toDouble();

                if(readStream.name() == "lambda1")
                    isParameterValid = settings->SetLambda1(currentAttributeValue);
                else if(readStream.name() == "lambda2")
                    isParameterValid = settings->SetLambda2(currentAttributeValue);
                else if(readStream.name() == "C")
                    isParameterValid = settings->SetC(currentAttributeValue);
                else if(readStream.name() == "K")
                    isParameterValid = settings->SetK(currentAttributeValue);
                else if(readStream.name() == "Rho")
                    isParameterValid = settings->SetRho(currentAttributeValue);
                else if(readStream.name() == "Nu")
                    isParameterValid = settings->SetNu(currentAttributeValue);
                else if(readStream.name() == "Gamma")
                    isParameterValid = settings->SetGamma(currentAttributeValue);
                else if(readStream.name() == "TimeStep")
                    isParameterValid = settings->SetGamma(currentAttributeValue);
                else if(readStream.name() == "TimeStep")
                    isParameterValid = settings->SetTimeStep(currentAttributeValue);
                else if(readStream.name() == "ActivatorAccuracy")
                    isParameterValid = settings->SetActivatorAccuracy(currentAttributeValue);
                else if(readStream.name() == "InhibitorAccuracy")
                    isParameterValid = settings->SetInhibitorAccuracy(currentAttributeValue);
                else if(readStream.name() == "GridDimension")
                    isParameterValid = settings->SetGridDimension((int)currentAttributeValue);
                else if(readStream.name() == "IterationsLimit")
                    isParameterValid = settings->SetIterationsLimit((int)currentAttributeValue);
                else
                    handleParsingError();
                if(!isParameterValid || readStream.hasError())
                    handleParsingError();
            }
        else
            handleParsingError();
    }
    else
        handleParsingError();
}
