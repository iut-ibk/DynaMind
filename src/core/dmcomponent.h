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

#ifndef DMCOMPONENT_H
#define DMCOMPONENT_H

#include <iostream>
#include <map>
#include <vector>
#include <list>
#include <set>
#include <dmview.h>
#include <dmcompilersettings.h>

#ifdef SWIG
#define DM_HELPER_DLL_EXPORT
#endif

namespace DM {

class Attribute;
class System;

/** @ingroup DynaMind-Core
  * @brief Basic class that contains to store informations in DynaMind
  *
  * All Objects like Nodes, Edges and Faces are derived from the component class.
  * Ever component can be identified by a unique ID created by the QT framework.
  * Components also manages the attributes. Components can be part of none or more views.
  */
class DM_HELPER_DLL_EXPORT Component
{
    friend class System;
protected:
    std::string uuid;
    std::string name;
    std::vector<Component*> childsview;
    std::vector<Attribute*> attributesview;
    std::vector<Component*> ownedchilds;
    std::vector<Attribute*> ownedattributes;
    std::set<std::string> inViews;
    System * currentSys;

    void removeView(const DM::View & view);

    Component *findComponent(std::string uuid, std::vector<Component*> & vec);
    Attribute *findAttribute(std::string name, std::vector<Attribute*> & vec);


public:
    /** @brief create a new component
      *
      * The default constructor creates a UUID for the component.
     */
    Component();
    /** @brief Copies a component, also the UUID is copied! */
    Component(const Component& s);
    /** @brief Destructor */
    virtual ~Component();
    /** @brief setUUID */
    void setUUID(std::string uuid);

    /** @brief return UUID */
    std::string getUUID();

    /** @brief adds a new Attribute to the Component.
      *
      * Returns true if the attribute has been added to the Component.
      * Returns false if the attribute with the same name already exists.
      */
    bool addAttribute(Attribute newattribute);

    /** @brief Add new double attribute to the component. If the Attribute already exists changeAttribute is called */
    bool addAttribute(std::string, double val);

    /** @brief Change existing Attribute. If attribute doesn't exist a new Attribute is added to the Component*/
    bool changeAttribute(Attribute newattribute);

    /** @brief Change existing double Attribute. It the Attribute doesn't exist a new double Attribute is added*/
    bool changeAttribute(std::string, double val);

    /** @brief Remove Attribute, returns false if no Attribute with this name exists */
    bool removeAttribute(std::string name);

    /** @brief Returns a pointer to an Attribute */
    Attribute* getAttribute(std::string name);

    /** @brief Returns a map of all Attributes */
    const std::map<std::string, Attribute*>  getAllAttributes() const;


    /** @brief adds Component to a View by using the name of the view */
    void setView(std::string view);

    /** @brief adds Component to a View.
      *
      * Therefore the name of the View is used as identifier.
      */
    void setView(const DM::View & view);

    /** @brief Create new UUID */
    void createNewUUID();

    /** @brief Retruns a set of Views in which the Compont is used */
    std::set<std::string> const &  getInViews() const;

    /** @brief Returns true if Component is in the View */
    bool isInView(DM::View view) const;

    /** @brief virtual clone method.
      *
      * To create a enw data object, like Node, Edge, Face, the pure virtal clone method needs to be implemented.
      * The method returns a pointer to a new data object (including attributes and uuid) */
    virtual Component* clone();
    /** @brief Sets name */
    void setName(std::string name);
    /** @brief Returns name */
    //std::string getName() const;
    //TODO No idea waht this is
    bool addChild(Component *newcomponent);
    bool changeChild(Component *newcomponent);
    bool removeChild(std::string name);
    Component* getChild(std::string name);
    std::map<std::string, Component*> getAllChilds();

    Component * updateChild(Component * c);

    System * getCurrentSystem();
    void setCurrentSystem(System * sys);

};
typedef std::map<std::string, DM::Component*> ComponentMap;


}

#endif // COMPONENT_H
