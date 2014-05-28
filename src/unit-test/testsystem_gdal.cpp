/**
 * @file
 * @author  Chrisitan Urich <christian.urich@gmail.com>
 * @version 1.0
 * @section LICENSE
 *
 * This file is part of DynaMind
 *
 * Copyright (C) 2014  Christian Urich

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

#include "testsystem_gdal.h"


#include <dmsimulation.h>
#include <dmsystem.h>
#include <dmlogger.h>
#include <dmlogsink.h>
#include <dmcomponent.h>

namespace DM {

TEST_F(TestSystem_GDAL, addNode) {

	ostream *out = &cout;
	DM::Log::init(new DM::OStreamLogSink(*out), DM::Debug);

	DM::Logger(DM::Standard) << "Test GDAL System";
	DM::System sys;
	Logger(Debug) << sys.getComponentLayer()->GetLayerDefn()->GetFieldCount();

	DM::Component cmp;
	sys.addComponent(&cmp);

	Logger(Debug) << "Done";
	//cmp.addAttribute("test", 100.);

	//Logger(Debug) << cmp.getAttribute("test")->getDouble();

/*	OGRLayer * poLayer = sys.getPoint_layer();
	OGRFieldDefn oField( "Name", OFTString );


	oField.SetWidth(32);
	poLayer->CreateField(&oField);

	OGRFeature *poFeature;
	double x =1;
	double y =2;

	char szName[33];
	poFeature = OGRFeature::CreateFeature( poLayer->GetLayerDefn() );
	poFeature->SetField( "Name", szName );

	OGRPoint pt;
	pt.setX( x );
	pt.setY( y );

	poFeature->SetGeometry( &pt );


	if( poLayer->CreateFeature( poFeature ) != OGRERR_NONE )
	{
	   printf( "Failed to create feature in shapefile.\n" );
	   exit( 1 );
	}
	Logger(Debug) << "fcount " << poFeature->GetFID();
	OGRFeature * f1 = poFeature;
	OGRFeature::DestroyFeature( poFeature );

	OGRFieldDefn oField_empty("bugsi", OFTInteger);

	OGRFeatureDefn featurDef;
	featurDef.AddFieldDefn(&oField_empty);
	poFeature = OGRFeature::CreateFeature(&featurDef );
	poFeature->SetField( "bugsi", 1 );

	pt.setX( x+1 );
	pt.setY( y+1 );

	poFeature->SetGeometry( &pt );


	if( poLayer->CreateFeature( poFeature ) != OGRERR_NONE )
	{
	   printf( "Failed to create feature in shapefile.\n" );
	   exit( 1 );
	}

	//OGRFeature * f2 = poFeature;
	Logger(Debug) << "fcount " << poFeature->GetFID();
	OGRFeature::DestroyFeature( poFeature );

	std::vector<OGRFeature *> list;

	Logger(Debug) << poLayer->GetFeatureCount();

	poLayer->ResetReading();
	while( (poFeature = poLayer->GetNextFeature()) != NULL )
	{

		if (std::find(list.begin(), list.end(), poFeature) != list.end()) {
			Logger(Debug) << "found";
		} else {
			Logger(Debug) << "not found";
		}
		Logger(Debug) << poFeature->GetFID();
		Logger(Debug) << "Feature Count";
		Logger(Debug) << poFeature->GetFieldCount();
		Logger(Debug) << poFeature->GetFieldDefnRef(0)->GetNameRef();
		Logger(Debug) << poFeature->GetFieldAsString(0);
	}*/



}
}

