#include <vector>
#include <string>
#include <algorithm>
#include <regex>
#include "variable.h"


Variable::Variable(std::string name, VariableType type, void *value)
    : m_name(name), m_type(type), m_value(value)
{
}

Variable::~Variable()
{
    deleteValue();
}

std::string Variable::getName() {
    return m_name;
}

VariableType Variable::getType()
{
    return m_type;
}

void* Variable::getValue()
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
    case VariableType::INT:
        delete (int*)m_value;
        break;
    case VariableType::FLOAT:
        delete (float*)m_value;
        break;
    case VariableType::BOOL:
        delete (bool*)m_value;
        break;
    case VariableType::STRING:
        delete (std::string*)m_value;
        break;
    }
}

std::string Variable::toString()
{
    switch (m_type) {
    case VariableType::INT:
        return std::to_string(*((int*)m_value));
    case VariableType::FLOAT:
        return std::to_string(*((float*)m_value));
    case VariableType::BOOL:
        return std::string((*((bool*)m_value)) ? "TRUE" : "FALSE");
    case VariableType::STRING:
        return *((std::string*)m_value);
    }
}

std::string Variable::toFullString()
{
    switch (m_type) {
    case VariableType::INT:
        return m_name + "(INT)" + std::to_string(*((int*)m_value));
    case VariableType::FLOAT:
        return m_name + "(FLOAT)" + std::to_string(*((float*)m_value));
    case VariableType::BOOL:
        return m_name + "(BOOL)" + (*((bool*)m_value) ? "TRUE" : "FALSE");
    case VariableType::STRING:
        return m_name + "(STRING)" + *((std::string*)m_value);
    }
}

std::string Variable::getTypeString()
{
    switch (m_type) {
    case VariableType::INT:
        return "INT";
    case VariableType::FLOAT:
        return "FLOAT";
    case VariableType::BOOL:
        return "BOOL";
    case VariableType::STRING:
        return "STRING";
    }
}

bool Variable::strIsKeyword(std::string keyword)
{
    std::string keywordUpper(keyword);
    std::transform(keywordUpper.begin(), keywordUpper.end(), keywordUpper.begin(), ::toupper);

    std::vector<std::string> keywords;
    keywords.push_back("VAR");
    keywords.push_back("SET");
    keywords.push_back("ADD");
    keywords.push_back("LOOP");
    keywords.push_back("END");
    keywords.push_back("BREAK");
    keywords.push_back("EXIT");
    keywords.push_back("PRINT");
    keywords.push_back("IF");
    keywords.push_back("ELSE");

    keywords.push_back("TRUE");
    keywords.push_back("FALSE");

    keywords.push_back("INT");
    keywords.push_back("FLOAT");
    keywords.push_back("BOOL");
    keywords.push_back("STRING");

    if (std::find(keywords.begin(), keywords.end(), keywordUpper) != keywords.end())
        return true;

    return false;
}

bool Variable::strToVarType(std::string str, VariableType &type)
{
    std::string strUpper(str);
    std::transform(strUpper.begin(), strUpper.end(), strUpper.begin(), ::toupper);

    if (strUpper == "INT") {
        type = VariableType::INT;
        return true;
    } else if (strUpper == "FLOAT") {
        type = VariableType::FLOAT;
        return true;
    } else if (strUpper == "BOOL") {
        type = VariableType::BOOL;
        return true;
    } else if (strUpper == "STRING") {
        type = VariableType::STRING;
        return true;
    }

    return false;
}

Variable* Variable::parseToVar(std::string name, std::string token, VariableType type)
{
    Variable *res = nullptr;

    switch (type) {
    case VariableType::INT: {
        try {
            int tmp = std::stoi(token, nullptr);
            res = new Variable(name, VariableType::INT, new int(tmp));
        }
        catch (...) {}
        break;
    }
    case VariableType::FLOAT: {
        try {
            float tmp = std::stof(token, nullptr);
            res = new Variable(name, VariableType::FLOAT, new float(tmp));
        }
        catch (...) {}
        break;
    }
    case VariableType::BOOL: {
        res = new Variable(name, VariableType::BOOL, new bool(token == "TRUE"));
        break;
    }
    case VariableType::STRING: {
        res = new Variable(name, VariableType::STRING, new std::string(token));
        break;
    }
    }

    return res;
}

bool Variable::varNameIsValid(std::string name)
{
    return std::regex_match("name", std::regex("[0-9A-Za-z_]+"));
}

