/***************************************************************************
    kadaspointitem.cpp
    ------------------
    copyright            : (C) 2019 by Sandro Mani
    email                : smani at sourcepole dot ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <qgis/qgspoint.h>
#include <qgis/qgsmultipoint.h>

#include <kadas/core/mapitems/kadaspointitem.h>


KadasPointItem::KadasPointItem(const QgsCoordinateReferenceSystem &crs, IconType icon, QObject* parent)
  : KadasGeometryItem(crs, parent)
{
  double dMin = std::numeric_limits<double>::min();
  double dMax = std::numeric_limits<double>::max();
  mAttributes.insert(AttrX, NumericAttribute{"x", dMin, dMax, 0});
  mAttributes.insert(AttrY, NumericAttribute{"y", dMin, dMax, 0});

  setIconType(icon);
  reset();
}

bool KadasPointItem::startPart(const QgsPointXY& firstPoint, const QgsMapSettings &mapSettings)
{
  state()->points.append(firstPoint);
  recomputeDerived();
  return false;
}

bool KadasPointItem::moveCurrentPoint(const QgsPointXY& p, const QgsMapSettings &mapSettings)
{
  return false;
}

bool KadasPointItem::setNextPoint(const QgsPointXY& p, const QgsMapSettings &mapSettings)
{
  return false;
}

void KadasPointItem::endPart()
{
}

const QgsMultiPoint* KadasPointItem::geometry() const
{
  return static_cast<QgsMultiPoint*>(mGeometry);
}

QgsMultiPoint* KadasPointItem::geometry()
{
  return static_cast<QgsMultiPoint*>(mGeometry);
}

void KadasPointItem::recomputeDerived()
{
  QgsMultiPoint* multiGeom = new QgsMultiPoint();
  for ( const QgsPointXY& point : state()->points )
  {
    multiGeom->addGeometry(new QgsPoint(point));
  }
  setGeometry(multiGeom);
}

QList<double> KadasPointItem::recomputeAttributes(const QgsPointXY& pos) const
{
  QList<double> values;
  values.insert(AttrX, pos.x());
  values.insert(AttrY, pos.y());
  return values;
}

QgsPointXY KadasPointItem::positionFromAttributes(const QList<double>& values) const
{
  return QgsPointXY(values[AttrX], values[AttrY]);
}

bool KadasPointItem::startPart(const QList<double>& attributeValues)
{
  QgsPoint point(attributeValues[AttrX], attributeValues[AttrY]);
  state()->points.append(point);
  recomputeDerived();
  return false;
}

void KadasPointItem::changeAttributeValues(const QList<double>& values)
{
  // Do nothing
}

bool KadasPointItem::acceptAttributeValues()
{
  // No further action allowed
  return false;
}