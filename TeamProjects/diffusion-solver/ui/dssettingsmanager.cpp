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
    writeStream.writeAttribute("value", QString::number(settings->GetLambda1()));
    writeStream.writeEndElement(); //labmda1

    writeStream.writeStartElement("lambda2");
    writeStream.writeAttribute("value", QString::number(settings->GetLambda2()));
    writeStream.writeEndElement(); //labmda2

    writeStream.writeStartElement("K");
    writeStream.writeAttribute("value", QString::number(settings->GetK()));
    writeStream.writeEndElement(); //K

    writeStream.writeStartElement("C");
    writeStream.writeAttribute("value", QString::number(settings->GetC()));
    writeStream.writeEndElement(); //C

    writeStream.writeStartElement("Rho");
    writeStream.writeAttribute("value", QString::number(settings->GetRho()));
    writeStream.writeEndElement(); //Rho

    writeStream.writeStartElement("Nu");
    writeStream.writeAttribute("value", QString::number(settings->GetNu()));
    writeStream.writeEndElement(); //Nu

    writeStream.writeStartElement("Gamma");
    writeStream.writeAttribute("value", QString::number(settings->GetGamma()));
    writeStream.writeEndElement(); //Gamma

    writeStream.writeEndElement();//settings
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
                if(attributes.hasAttribute("value")) {
                    currentAttributeValue = attributes.value("value").toDouble();
                    if(currentAttributeValue < 0)
                        handleParsingError();
                }

                if(readStream.name() == "lambda1")
                    settings->SetLambda1(currentAttributeValue);
                else if(readStream.name() == "lambda2")
                    settings->SetLambda2(currentAttributeValue);
                else if(readStream.name() == "C")
                    settings->SetC(currentAttributeValue);
                else if(readStream.name() == "K")
                    settings->SetK(currentAttributeValue);
                else if(readStream.name() == "Rho")
                    settings->SetRho(currentAttributeValue);
                else if(readStream.name() == "Nu")
                    settings->SetNu(currentAttributeValue);
                else if(readStream.name() == "Gamma")
                    settings->SetGamma(currentAttributeValue);
                else
                    handleParsingError();
            }
        else
            handleParsingError();
    }
    else
        handleParsingError();
}
