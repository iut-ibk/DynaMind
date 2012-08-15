/**
 * @file
 * @author  Chrisitan Urich <christian.urich@gmail.com>
 * @author  Michael Mair <abroxos@gmail.com>
 * @version 1.0
 * @section LICENSE
 * This file is part of DynaMind
 *
 * Copyright (C) 2011  Christian Urich, Michael Mair

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include <QtCore>
#include <dmcomponent.h>
#include <dmattribute.h>
#include <dmsystem.h>
#include <assert.h>
#include <dmlogger.h>

#include <QUuid>

using namespace DM;
using namespace DM;


Component::Component()
{
    this->uuid = QUuid::createUuid().toString().toStdString();
    name = "";
    childsview = std::vector<Component*>();
    attributesview = std::vector<Attribute*>();
    ownedchilds = std::vector<Component*>();
    ownedattributes =  std::vector<Attribute*>();
    inViews = std::set<std::string>();
    currentSys = 0;
}

void Component::createNewUUID() {
    this->uuid = QUuid::createUuid().toString().toStdString();
}

bool Component::isInView(View view) const {
    foreach (std::string s, inViews) {
        if (view.getName().compare(s) == 0)
            return true;
    }

    return false;
}

void Component::setName(std::string name) {
    this->name = name;
}


Component::Component(const Component& c)
{
    this->uuid=c.uuid;
    attributesview=c.attributesview;
    ownedchilds=c.ownedchilds;
    inViews = c.inViews;


    foreach (Component * c, ownedchilds)
        childsview.push_back(c);

    ownedchilds.clear();

}
Component * Component::updateChild(Component * c) {
    if (this->findComponent(c->getUUID(),ownedchilds) == c)
        return c;
    Component * c_new = c->clone();
    ownedchilds.push_back(c_new);

    return c_new;
}

Component::~Component()
{
    while(ownedchilds.size())
    {
        delete *ownedchilds.begin();
        ownedchilds.erase(ownedchilds.begin());
    }

    while(ownedattributes.size())
    {
        delete *ownedattributes.begin();
        ownedattributes.erase(ownedattributes.begin());
    }
}

void Component::setUUID(std::string uuid)
{
    this->uuid=uuid;
}

std::string Component::getUUID()
{
    return uuid;
}

bool Component::addAttribute(std::string name, double val) {

    if(this->findAttribute(name, attributesview)) {
        return this->changeAttribute(name, val);
    }
    Attribute  attr = Attribute(name, val);
    return this->addAttribute(attr);

}

bool Component::addAttribute(Attribute newattribute)
{
    if(this->findAttribute(name, attributesview))
        return false;
    Attribute * a = new Attribute(newattribute);
    attributesview.push_back(a);
    ownedattributes.push_back(a);

    return true;
}

bool Component::changeAttribute(Attribute newattribute)
{

    Attribute * aInView = this->findAttribute(newattribute.getName(), attributesview) ;
    if(aInView == 0) {
        return this->addAttribute(newattribute);
    }

    if(this->findAttribute(newattribute.getName(), ownedattributes)  == 0){
        Attribute * a = new Attribute(*(this->findAttribute(newattribute.getName(), ownedattributes)));
        ownedattributes.push_back(a);
        int pos = std::find(attributesview.begin(), attributesview.end(), aInView) - attributesview.begin();
        attributesview[pos] = a;
    }
    Attribute * attr =this->findAttribute(newattribute.getName(), attributesview);
    Attribute::AttributeType type = attr->getType();
    attr->setDouble(newattribute.getDouble());
    attr->setDoubleVector(newattribute.getDoubleVector());
    attr->setString(newattribute.getString());
    attr->setStringVector(newattribute.getStringVector());
    attr->setType(type);
    return true;
}

bool Component::changeAttribute(std::string s, double val)
{
    return this->changeAttribute(Attribute(s, val));
}


bool Component::removeAttribute(std::string name)
{
    if(this->findAttribute(name, attributesview))
    {
        Attribute * attr =this->findAttribute(name, attributesview);
        if(this->findAttribute(name, ownedattributes))
        {

            delete attr;
            ownedattributes.erase(std::find(ownedattributes.begin(), ownedattributes.end(), attr));
        }

        attributesview.erase(std::find(ownedattributes.begin(), ownedattributes.end(), attr));
        return true;
    }

    return false;
}

Attribute* Component::getAttribute(std::string name)
{
    DM::Attribute * attr = this->findAttribute(name, attributesview);
    if(!attr) {
        this->addAttribute(Attribute(name));
    }
    return attr;
}

const std::map<std::string, Attribute*>  Component::getAllAttributes() const
{

    std::map<std::string, Attribute*>  ressMap;
    foreach(Attribute * attr, attributesview) {
        ressMap[attr->getName()] = attr;
    }
    return ressMap;
}

bool Component::addChild(Component *newcomponent)
{
    if(!newcomponent)
        return false;

    if(this->findComponent(newcomponent->getUUID(),childsview))
        return false;

    childsview.push_back(newcomponent);
    ownedchilds.push_back(newcomponent);


    return true;
}

Component* Component::clone() {
    return new Component(*this);
}

bool Component::changeChild(Component *newcomponent)
{
    if(!newcomponent)
        return false;

    if(this->findComponent(newcomponent->getUUID(), ownedchilds)){
        Component * cmp = this->findComponent(newcomponent->getUUID(), ownedchilds);
        int pos = std::find(ownedchilds.begin(), ownedchilds.end(), cmp) - ownedchilds.begin();
        ownedchilds[pos] = newcomponent;

    } else {
        ownedchilds.push_back(newcomponent);
    }

    if(this->findComponent(newcomponent->getUUID(), childsview)){
        Component * cmp = this->findComponent(newcomponent->getUUID(), childsview);
        int pos = std::find(childsview.begin(), childsview.end(), cmp) - childsview.begin();
        childsview[pos] = newcomponent;

    } else {
        childsview.push_back(newcomponent);
    }
    return true;
}



bool Component::removeChild(std::string name)
{
    if(this->findComponent(name, childsview))
    {
        Component * attr =this->findComponent(name, childsview);
        if(this->findComponent(name, ownedchilds))
        {

            delete attr;
            ownedchilds.erase(std::find(ownedchilds.begin(), ownedchilds.end(), attr));
        }
        childsview.erase(std::find(childsview.begin(), childsview.end(), attr));
        return true;
    }

    return false;
}

Component* Component::getChild(std::string name)
{
    return this->findComponent(name, childsview);
}

void Component::setView(std::string view)
{
    this->inViews.insert(view);
}

void Component::setView(const DM::View & view)
{
    this->inViews.insert(view.getName());
}

void Component::removeView(const View &view)
{
    this->inViews.erase(view.getName());
}

Component *Component::findComponent(string uuid, std::vector<Component*> & vec)
{
    foreach (Component * c, vec) {
        if(c->getUUID().compare(uuid) == 0)
            return c;
    }
    return 0;
}

Attribute *Component::findAttribute(string name, std::vector<Attribute*> & vec)
{
    foreach (Attribute * c, vec) {
        if(c->getName().compare(name) == 0)
            return c;
    }
    return 0;
}

const set<std::string> &Component::getInViews() const {
    return this->inViews;

}
std::map<std::string, Component*> Component::getAllChilds()
{

    std::map<std::string, Component*>  ressMap;
    foreach(Component * attr, childsview) {
        ressMap[attr->getUUID()] = attr;
    }
    return ressMap;
}

System * Component::getCurrentSystem() {
    return this->currentSys;
}

void Component::setCurrentSystem(System *sys) {
    this->currentSys = sys;
}

