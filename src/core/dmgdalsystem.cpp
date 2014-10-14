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

#include "dmgdalsystem.h"

#include <dmlogger.h>
#include <dmattribute.h>
#include <dmsystem.h>

#include <ogrsf_frmts.h>
#include <ogr_api.h>

#include <sstream>

#include <QUuid>
#include <dmviewcontainer.h>

namespace DM {

GDALSystem::GDALSystem(int EPSG)
{
	//Init SpatialiteServer
	OGRRegisterAll();

	poDrive = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName( "SQLite" );
	char ** options = NULL;
	options = CSLSetNameValue( options, "OGR_SQLITE_CACHE", "1024" );
	options = CSLSetNameValue( options, "SPATIALITE", "YES" );
	options = CSLSetNameValue( options, "OGR_SQLITE_SYNCHRONOUS", "OFF" );

	DBID = QUuid::createUuid().toString();

	QString dbname =  "/tmp/" + DBID + ".db";

	dbname =  "file:"+DBID+"?mode=memory&cache=shared";// + DBID;// + ".db";
	poDS = poDrive->CreateDataSource(dbname.toStdString().c_str() , options );


	if( poDS == NULL ) {
		DM::Logger(DM::Error) << "couldn't create source";
	}

	//Create State ID
	this->state_ids.push_back(QUuid::createUuid().toString().toStdString());

	//Init uuid
	latestUniqueId = 0;
	predecessor = NULL;
	this->EPSG = EPSG;
}

GDALSystem::GDALSystem(const GDALSystem &s)
{
	DM::Logger(DM::Warning) << "Split System";
	//Copy all that is needed
	poDrive = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName( "SQLite" );//s.poDrive;
	viewLayer = s.viewLayer;
	state_ids = s.state_ids;
	state_ids = s.state_ids;
	uniqueIdsTonfid = s.uniqueIdsTonfid;
	latestUniqueId = s.latestUniqueId;
	sucessors = std::vector<DM::GDALSystem*>();
	DBID = QUuid::createUuid().toString();
	EPSG = s.EPSG;

	//Copy DB
	QString origin =  "/tmp/" + s.DBID + ".db";
	QString dest = "/tmp/" + DBID + ".db";

	origin =   "file:"+s.DBID+"?mode=memory&cache=shared";// + s.DBID ;
	dest = "file:"+DBID+"?mode=memory&cache=shared";// + DBID;
	//QFile::copy(origin, dest);

	//Connect to DB
	char ** options = NULL;
	options = CSLSetNameValue( options, "OGR_SQLITE_CACHE", "1024" );
	options = CSLSetNameValue( options, "SPATIALITE", "YES" );
	options = CSLSetNameValue( options, "OGR_SQLITE_SYNCHRONOUS", "OFF" );

	//poDS = poDrive->Open(dest.toStdString().c_str(), true);
	poDS = poDrive->CreateDataSource(dest.toStdString().c_str() , options );
	if( poDS == NULL ) {
		DM::Logger(DM::Error) << "couldn't create source";
		return;
	}

	//Create new state
	state_ids.push_back(QUuid::createUuid().toString().toStdString());

	OGRSpatialReference* oSourceSRS;
	oSourceSRS = new OGRSpatialReference();
	oSourceSRS->importFromEPSG(this->EPSG);

	for (std::map<std::string, OGRLayer *>::const_iterator it = viewLayer.begin();
		 it != viewLayer.end(); ++it) {
		std::string name = it->first;
		OGRLayer * lyr_origin = it->second;
		OGRFeatureDefn * def =  (OGRFeatureDefn*)lyr_origin->GetLayerDefn();

		OGRLayer * lyr = poDS->CreateLayer(name.c_str(), oSourceSRS, def->GetGeomType(), NULL );

		int c_fields = def->GetFieldCount();
		for (int i = 0; i < c_fields; i++) {
			lyr->CreateField(def->GetFieldDefn(i));
		}

		OGRFeature * feat;
		lyr_origin->ResetReading();
		lyr->StartTransaction();
		int counter = 0;
		while (feat = lyr_origin->GetNextFeature()) {
			counter++;
			if (counter % 1000000 == 0){
				lyr->CommitTransaction();
				lyr->StartTransaction();
			}
			OGRFeature * f_new = OGRFeature::CreateFeature( lyr->GetLayerDefn() );
			for (int i = 0; i < c_fields; i++)
				f_new->SetField(i, feat->GetRawFieldRef(i));
			if (def->GetGeomType() != wkbNone) {
				OGRGeometry * geo = feat->GetGeometryRef();
				f_new->SetGeometry(geo);
			}
			lyr->CreateFeature(f_new);
			OGRFeature::DestroyFeature(f_new);
		}
		lyr->CommitTransaction();
	}
	//RebuildViewLayer
	for (std::map<std::string, OGRLayer *>::const_iterator it = viewLayer.begin();
		 it != viewLayer.end(); ++it) {
		std::string viewname = it->first;
		viewLayer[it->first] = poDS->GetLayerByName(viewname.c_str());
	}
}


void GDALSystem::updateView(const View &v)
{
	//if view is not in map create a new ogr layer
	if (viewLayer.find(v.getName()) == viewLayer.end()) {
		if ( v.getName() == "dummy" ) {
			return;
		}
		OGRLayer * lyr = this->createLayer(v);

		if (lyr == NULL) {
			DM::Logger(DM::Error) << "couldn't create layer " << v.getName();
			return;
		}

		//OGRFieldDefn oField_id( "dynamind_id", OFTInteger );
		//lyr->CreateField(&oField_id);

		//OGRFieldDefn oField_state_id( "dynamind_state_id", OFTString );
		//lyr->CreateField(&oField_state_id);
		viewLayer[v.getName()] = lyr;
	}

	OGRLayer * lyr = viewLayer[v.getName()];
	//Update Features
	foreach(std::string attribute_name, v.getAllAttributes()) {
		//Feature already in layer
		if (lyr->GetLayerDefn()->GetFieldIndex(attribute_name.c_str()) >= 0)
			continue;
		if (v.getAttributeType(attribute_name) == DM::Attribute::INT){
			OGRFieldDefn oField ( attribute_name.c_str(), OFTInteger );
			lyr->CreateField(&oField);
			continue;
		}
		if (v.getAttributeType(attribute_name) == DM::Attribute::STRING){
			OGRFieldDefn oField ( attribute_name.c_str(), OFTString );
			lyr->CreateField(&oField);
			continue;
		}
		if (v.getAttributeType(attribute_name) == DM::Attribute::DOUBLE){
			OGRFieldDefn oField ( attribute_name.c_str(), OFTReal );
			lyr->CreateField(&oField);
			continue;
		}
	}
}

void GDALSystem::updateViewContainer(DM::ViewContainer v)
{
	this->updateView(v);
}

OGRFeature *GDALSystem::createFeature(const View &v)
{
	OGRLayer * lyr = viewLayer[v.getName()];
	OGRFeature * f = OGRFeature::CreateFeature(lyr->GetLayerDefn());
	//f->SetField("dynamind_id", (int) latestUniqueId++);
	//f->SetField("dynamind_state_id", this->state_ids[state_ids.size()-1].c_str());
	return f;
}

OGRLayer *GDALSystem::getOGRLayer(const View &v)
{
	if (viewLayer.find(v.getName()) == viewLayer.end()) {
		Logger(Error) << "Layer not found";
		return 0;
	}
	return viewLayer[v.getName()];
}

OGRDataSource *GDALSystem::getDataSource()
{
	return this->poDS;
}

bool GDALSystem::resetReading(const View &v)
{
	if (viewLayer.find(v.getName()) == viewLayer.end()) {
		Logger(Error) << "Layer not found";
		return false;
	}
	OGRLayer * lyr = viewLayer[v.getName()];
	lyr->ResetReading();
	return true;
}

GDALSystem *GDALSystem::createSuccessor()
{
	Logger(Debug) << "Create Sucessor ";
	GDALSystem* result = new GDALSystem(*this);
	this->sucessors.push_back(result);
	result->setPredecessor(this);

	return result;
}

void GDALSystem::updateViews(const std::vector<View> &views)
{
	foreach (DM::View v, views) {
		this->updateView(v);
	}
}

GDALSystem *GDALSystem::getPredecessor() const
{
	return this->predecessor;
}

void GDALSystem::setPredecessor(GDALSystem * sys)
{
	this->predecessor = sys;
}

OGRFeature *GDALSystem::getNextFeature(const View &v)
{
	OGRLayer * lyr = viewLayer[v.getName()];
	return lyr->GetNextFeature();
}

string GDALSystem::getCurrentStateID()
{
	return this->state_ids[state_ids.size()-1];
}

void GDALSystem::registerFeature(OGRFeature * f, const View &v)
{
	//f->SetField("dynamind_id", (int) latestUniqueId++);
	//f->SetField("dynamind_state_id", this->state_ids[state_ids.size()-1].c_str());
}

string GDALSystem::getDBID()
{
	return this->DBID.toStdString();
}

OGRLayer *GDALSystem::createLayer(const View &v)
{
	OGRSpatialReference* oSourceSRS;
	oSourceSRS = new OGRSpatialReference();
	oSourceSRS->importFromEPSG(this->EPSG);
	//	oSourceSRS = NULL;

	switch ( v.getType() ) {
	case DM::COMPONENT:
		return poDS->CreateLayer(v.getName().c_str(), oSourceSRS, wkbNone, NULL );
		break;
	case DM::NODE:
		return poDS->CreateLayer(v.getName().c_str(), oSourceSRS, wkbPoint, NULL );
		break;
	case DM::EDGE:
		return poDS->CreateLayer(v.getName().c_str(), oSourceSRS, wkbLineString, NULL );
		break;
	case DM::FACE:
		return poDS->CreateLayer(v.getName().c_str(), oSourceSRS, wkbPolygon, NULL );
		break;
	}

	return NULL;
}

void GDALSystem::syncNewFeatures(const DM::View & v, std::vector<OGRFeature *> & df, bool destroy)
{
	OGRLayer * lyr = viewLayer[v.getName()];
	//Sync all features
	int counter = 0;
	int global_counter = 0;
	lyr->StartTransaction();
	foreach (OGRFeature * f, df) {
		counter++;
		global_counter++;
		if (counter == 10000) {
			lyr->CommitTransaction();
			lyr->StartTransaction();
			counter=0;
		}
		if (!f)
			continue;
		lyr->CreateFeature(f);
		uniqueIdsTonfid.push_back(f->GetFID());
		if (destroy)
			OGRFeature::DestroyFeature(f);
	}
	lyr->CommitTransaction();
	df.clear();
}

void GDALSystem::synsDeleteFeatures(const View &v, std::vector<long> &df)
{
	Logger(Debug) << "Start Delete";
	OGRLayer * lyr = viewLayer[v.getName()];
	lyr->StartTransaction();
	for (int i = 0; i < df.size(); i++) {
		lyr->DeleteFeature(df[i]);
		if (i % 1000000 == 0) {
			lyr->CommitTransaction();
			lyr->StartTransaction();
		}
	}
	lyr->CommitTransaction();
	df.clear();
}

void GDALSystem::syncAlteredFeatures(const DM::View & v, std::vector<OGRFeature *> & df, bool destroy)
{
	Logger(Debug) << "Sync Altered";
	OGRLayer * lyr = viewLayer[v.getName()];
	//Sync all features
	int counter = 0;
	int global_counter = 0;
	lyr->StartTransaction();
	foreach (OGRFeature * f, df) {
		counter++;
		global_counter++;
		if (counter == 100000) {
			lyr->CommitTransaction();
			lyr->StartTransaction();
			counter=0;
		}
		if (!f)
			continue;
		lyr->SetFeature(f);
		if (destroy)
			OGRFeature::DestroyFeature(f);
	}
	lyr->CommitTransaction();
	df.clear();
}

}
