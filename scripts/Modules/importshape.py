"""
@author  Chrisitan Urich <christian.urich@gmail.com>
@version 1.0
@section LICENSE

This file is part of VIBe2
Copyright (C) 2011  Christian Urich

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
"""

from pydynamite import *
import pydynamite
import shapefile

class ImportShapeFile(Module):
	"""Import Shapefiles
	
	Import for Shapfiles. All data and attributes are imported. The data are saved in the Dynamite Sytem format. 
	The elements are saved with Identifier_ID
	@ingroup Modules
	@author Christian Urich
	"""
        def __init__(self):
            Module.__init__(self)

	    self.FileName = p_string()
	    self.FileName.assign("/home/csae6550/Desktop/pipes")
	    self.addParameter("FileName",VIBe2.FILENAME,self.FileName,"Sample Description")
                
	    self.Identifier = p_string()
	    self.Identifier.assign("")
	    self.addParameter("Identifier",VIBe2.STRING,self.Identifier,"Sample Description")

            shape = pydynamite.View("Shape")
            shape.addComponent(NODE)
            shape.addComponent(EDGE)
            shape.addAttributes("Shapelist")
            
            views = pydynamite.viewvector()
            views.push_back(shape)
            
            self.addData("Shapefile",views)
            
        def run(self):
            vec = self.getData("Shapefile")
            
            if vec == None :
                pydynamite.log("Cannot get a valid system object",pydynamite.Error)
                return False
            
            sourcePath =  self.FileName.value()
            sf = shapefile.Reader(sourcePath)
            shaperecords = sf.shapeRecords()
            fields = sf.fields
            shapevec = pydynamite.stringvector()
            
            for r in shaperecords:
                shp = r.shape.points
                subsys = vec.createSubSystem("newsystem","Shape")
                shapevec.append(subsys.getName())
                points = pydynamite.nodevector()
                attr = r.record
                
                #Add all attribute at the current shape
                for index, value in enumerate(attr):
                    name = fields[index+1]
                    newattr = pydynamite.Attribute(name[0],name[0])
                    
                    if name[1]=='C':
                        newattr.setString(str(attr[index]))
                        
                    if name[1]=='N':
                        newattr.setDouble(float(attr[index]))
                        
                    subsys.addAttribute(newattr)
                
                #Add this state only save points
                for point in shp:
                    points.push_back(subsys.addNode(point[0],point[1],0,""))
                
                if r.shape.shapeType != shapefile.POINT:
                    for index, p1 in enumerate(points):
                        if(points.__len__()>index+1):
                            #Add this state no differenct between POLYLINE and POLYGON shape type
                            p2 = points[index+1]
                            subsys.addEdge(p1,p2)
                        else:
                            #If shape type is POLYGON close the current shape  
                            if r.shape.shapeType == shapefile.POLYGON:
                                p2 = points[0]
                                subsys.addEdge(p1,p2)
  
            newattr = pydynamite.Attribute("Shapelist","Shapelist")
            newattr.setStringVector(shapevec)
            if not vec.addAttribute(newattr) :
                pydynamite.log("Cannot add new attribute",pydynamite.Error)
            else:
                print vec.getAttribute("Shapelist").getName()
                
            pydynamite.log("Imported " + str(vec.getAllSubSystems().__len__()) + " shapes",pydynamite.Standard)