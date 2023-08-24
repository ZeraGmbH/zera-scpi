#include "variable.h"


Variable::Variable(QString name, VariableType type, void *value)
    : m_name(name), m_type(type), m_value(value)
{
}

Variable::~Variable()
{
    deleteValue();
}

QString Variable::getName() {
    return m_name;
}

VariableType Variable::getType()
{
    return m_type;
}

void *Variable::getValue()
{
    return this->m_value;
}

void Variable::setValue(void *value)
{
    deleteValue(); // delete old value
    this->m_value = value;
}

void Variable::deleteValue()
{
    switch (m_type) {
    case INT:
        delete (int*)m_value;
        break;
    case FLOAT:
        delete (float*)m_value;
        break;
    case BOOL:
        delete (bool*)m_value;
        break;
    case STRING:
        delete (QString*)m_value;
        break;
    }
}

QString Variable::toString()
{
    switch (m_type) {
    case INT:
        return QString::number(*((int*)m_value));
    case FLOAT:
        return QString::number(*((float*)m_value));
    case BOOL:
        return QString((*((bool*)m_value)) ? "TRUE" : "FALSE");
    case STRING:
        return *((QString*)m_value);
    }
}

QString Variable::toFullString()
{
    switch (m_type) {
    case INT:
        return QString("%1(%2)=%3").arg(m_name, qPrintable("INT"), QString::number(*((int*)m_value)));
    case FLOAT:
        return QString("%1(%2)=%3").arg(m_name, qPrintable("FLOAT"), QString::number(*((float*)m_value)));
    case BOOL:
        return QString("%1(%2)=%3").arg(m_name, qPrintable("BOOL"), qPrintable((*((bool*)m_value)) ? "TRUE" : "FALSE"));
    case STRING:
        return QString("%1(%2)=%3").arg(m_name, qPrintable("STRING"), *((QString*)m_value));
    }
}
