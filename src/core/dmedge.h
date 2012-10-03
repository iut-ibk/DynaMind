/**
 * @file
 * @author  Chrisitan Urich <christian.urich@gmail.com>
 * @author  Michael Mair <abroxos@gmail.com>
 * @version 1.0
 * @section LICENSE
 * This file is part of DynaMite
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

#ifndef DMEDGE_H
#define DMEDGE_H
#include <dmcompilersettings.h>


namespace DM {

    class Component;
    /** @ingroup DynaMind-Core
      * @brief Provides an Edge object. An Edge just contains references to a start and end node.
      *
      * Lines are derived from the Component class. Therefore lines are identified by an UUID and can hold an
      * unlimeted number of Attributes. Edges only contain references to nodes stored in the system. As reference the uuid
      * of the Node is used.
      */
    class DM_HELPER_DLL_EXPORT Edge : public Component
    {
    private:
        std::string start;
        std::string end;

    public:
        /** @brief creates a new Edge. start and end point are references to existing Nodes in the same system. */
        Edge(std::string startpoint, std::string endpoint);
        /** @brief coyp Edge, also the Component is copied (Attributes and UUID!) */
        Edge(const Edge& e);
		/** @brief return Type */
		Components getType();
		/** @brief return Type */
		void getRawData(QBuffer* buf);
        /** @brief return uuid to the startpoint */
        std::string getStartpointName();
        /** @brief return uuid to the endpoint */
        std::string getEndpointName();
        /** @brief set uuid to that points to the start node */
        void setStartpointName(std::string name);
        /** @brief set uuid that points to the end node */
        void setEndpointName(std::string name);        
        /** @brief  Creates a pointer to a cloned Edge obejcts, including Attributes and uuid*/
        Component* clone();
    };
    typedef std::map<std::string, DM::Edge*> EdgeMap;
}
#endif // Edge_H
