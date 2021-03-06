/**
 * @file
 * @author  Chrisitan Urich <christian.urich@gmail.com>
 * @version 1.0
 * @section LICENSE
 *
 * This file is part of DynaMind
 *
 * Copyright (C) 2011  Christian Urich

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
#ifndef DMMODULEREGISTRY_H
#define DMMODULEREGISTRY_H

#include "dmcompilersettings.h"
#include "dmmodule.h"
#include <map>
#include <string>
#include <list>
#include <vector>

namespace DM {

class INodeFactory;
class Module;

class DM_HELPER_DLL_EXPORT ModuleRegistry
{
public:
	ModuleRegistry();

	bool addNodeFactory(INodeFactory *factory);

	/** @brief add Native Plugin */
	bool addNativePlugin(const std::string &plugin_path);
	Module * createModule(const std::string & name) const;
	std::list<std::string> getRegisteredModules() const;
	bool contains(const std::string &name) const;
	std::map<std::string, std::vector<std::string> > getModuleMap() const {return this->moduleMap;}
	std::map<std::string, std::string > getDisplayNames() const {return this->displayNames;}
	void print();
private:
	std::map<std::string, std::vector<std::string> > moduleMap;
	std::map<std::string, std::string> displayNames;
	std::map<std::string, INodeFactory * > registry;

};

// type definition of callback use registerModules
typedef void (*regNodeFunProto)(ModuleRegistry *reg);
}
#endif // MODULEREGISTRY_H
