#include "xmldocument.h"

bool XmlDocument::loadXml(const QString &xml, bool fatalOnInvalidXml)
{
    bool loaded = m_doc.setContent(xml);
    if(!loaded && fatalOnInvalidXml) {
        qFatal("Invalid XML\n%s", qPrintable(xml));
    }
    return loaded;
}

bool XmlDocument::isEmpty()
{
    return begin()->getElem() == QDomElement();
}

XmlElemIterator XmlDocument::begin()
{
    return std::make_shared<XmlElemIteratorTree>(m_doc.documentElement());
}

bool XmlDocument::findElem(QStringList tagSearchPath, QDomElement &foundElem)
{
    if(tagSearchPath.isEmpty())
        return false;
    QString childName = tagSearchPath.takeFirst();
    foundElem = m_doc.firstChildElement(childName);
    while(!foundElem.isNull() && !tagSearchPath.isEmpty()) {
        childName = tagSearchPath.takeFirst();
        foundElem = foundElem.firstChildElement(childName);
    }
    return !foundElem.isNull();
}

XmlElemIterator XmlDocument::find(QStringList tagSearchPath)
{
    QDomElement foundElem;
    findElem(tagSearchPath, foundElem);
    return std::make_shared<XmlElemIteratorList>(foundElem);
}

bool XmlDocument::addOrFindElem(QStringList tagPath, QDomElement &insertedOrFoundElem)
{
    /* TODO:
     * XmlElemIteratorTemplate
     * Iterate sub trees
     * Iterators for neighbors
     * Validate Tags
     * Insert (Arrays): addOrFindElem -> addElem
     */

    if(findElem(tagPath, insertedOrFoundElem))
        return false;

    bool added = false;
    QStringList tagPathParent;
    while(tagPath.size() > 1) {
        tagPathParent.append(tagPath.takeFirst());
        added |= addOrFindElem(tagPathParent, insertedOrFoundElem);
        if(insertedOrFoundElem.isNull())
            return false;
    }
    if(isEmpty()) {
        insertedOrFoundElem = m_doc.createElement(tagPath.takeLast());
        added |= !m_doc.appendChild(insertedOrFoundElem).isNull();
    }
    else if(!insertedOrFoundElem.isNull()) {
        QDomElement addedSub = m_doc.createElement(tagPath.takeLast());
        insertedOrFoundElem = insertedOrFoundElem.appendChild(addedSub).toElement();
        added |= !insertedOrFoundElem.isNull();
    }
    return added;
}

QString XmlDocument::getDocType()
{
    return m_doc.doctype().name();
}
