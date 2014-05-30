/**
 * @file
 * @author  Chrisitan Urich <christian.urich@gmail.com>
 * @author  Michael Mair <abroxos@gmail.com>
 * @author  Markus Sengthaler <m.sengthaler@gmail.com>
 * @version 1.0
 * @section LICENSE
 * This file is part of DynaMind
 *
 * Copyright (C) 2011  Christian Urich, Michael Mair, Markus Sengthaler

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
#include <QMutex>
#include <QMutexLocker>

#include <ogrsf_frmts.h>

#ifdef SWIG
#define DM_HELPER_DLL_EXPORT
#endif

#define GDAL;

namespace DM {

#define UUID_ATTRIBUTE_NAME "_uuid"

enum Components
{
    COMPONENT =0,
    NODE=1,
    EDGE=2,
    FACE=3,
    SUBSYSTEM=4,
    RASTERDATA=5
};

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
    friend class DerivedSystem;
public:
    /** @brief =operator */
    Component& operator=(Component const& other);

    /** @brief The default constructor creates a UUID for the component */
	Component();

    /** @brief Copies a component */
    Component(const Component& s);

    /** @brief Copies a component */
    Component(QUuid id, DM::System * s);

    /** @brief Destructor */
    virtual ~Component();

    /** @brief return Type */
    virtual Components getType() const;

    /** @brief return UUID */
    std::string getUUID();

    /** @brief return UUID */
    QUuid getQUUID() const;

    /** @brief adds a new Attribute to the Component.
    *
    * Returns true if the attribute has been added to the Component.
    * Returns false if the attribute with the same name already exists.
    */
    bool addAttribute(const Attribute &newattribute);

    /** @brief Add new double attribute to the component. If the Attribute already exists changeAttribute is called */
	bool addAttribute(const std::string&, double val);

	/** @brief Add new string attribute to the component. If the Attribute already exists changeAttribute is called */
	bool addAttribute(const std::string&, const std::string&);

	/** @brief Add new string attribute to the component. If the Attribute already exists changeAttribute is called */
	Attribute* addAttribute(const std::string& name, Attribute::AttributeType type);

    /** @brief Change existing Attribute. If attribute doesn't exist a new Attribute is added to the Component*/
    bool changeAttribute(const Attribute &newattribute);

    /** @brief Change existing double Attribute. It the Attribute doesn't exist a new double Attribute is added*/
    bool changeAttribute(const std::string&, double val);

    /** @brief Change existing double Attribute. It the Attribute doesn't exist a new double Attribute is added*/
	bool changeAttribute(const std::string& s, const std::string& val);

    /** @brief Remove Attribute, returns false if no Attribute with this name exists */
	bool removeAttribute(const std::string& name);

    /** @brief Returns a pointer to an Attribute */
	Attribute* getAttribute(const std::string& name);

    /** @brief Returns a vector of all Attributes */
    const std::vector<Attribute*>& getAllAttributes();

    /** @brief virtual clone method.
    *
    * To create a enw data object, like Node, Edge, Face, the pure virtal clone method needs to be implemented.
    * The method returns a pointer to a new data object (including attributes and uuid) */
    virtual Component* clone();

    /** @brief Returns the owning system */
    System * getCurrentSystem() const;

    /** @brief Returns true if an attribute of the given name exists */
	bool hasAttribute(const std::string& name) const;

    /** @brief exports the component to the db, it can afterwards deleted safly */
    virtual void _moveToDb();

    /** @brief exports all attributes of this components to the db */
    void _moveAttributesToDb();

    QUuid getDynaMindID() const;
    void setDynaMindID(const std::string &value);

    long getOGRfeatureID() const;
    void setOGRfeatureID(long value);

protected:
    /* @brief Sets stateUuid and ownership in sql db*/
    virtual void SetOwner(Component *owner);

    /** @brief deletes the component from the db, independend of the actual type using getTableName() */
    void SQLDelete();

    /** @brief Constructor, for derived classes only, as it doesnt generate a sql entry */
    Component(bool b);

    /** @brief Copy constructor, for derived classes only, as it doesnt generate a sql entry */
    Component(const Component& s, bool bInherited);

    /** @brief return table name */
    virtual QString getTableName();

    QMutex*	mutex;
    QUuid	uuid;
    System* currentSys;
    bool	isInserted;
    std::vector<Attribute*> ownedattributes;


#ifdef GDAL
    long OGRfeatureID;
    std::string DynaMindID;
#endif

private:

    Attribute* getExistingAttribute(const std::string& name) const;
    bool addAttribute(Attribute *pAttribute);
    void CopyFrom(const Component &c, bool successor = false);

#ifdef GDAL
        void initFeature();
#endif

};
}
#endif // COMPONENT_H
