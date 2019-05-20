/***************************************************************************
    kadasgeometryitem.h
    -------------------
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

#ifndef KADASGEOMETRYITEM_H
#define KADASGEOMETRYITEM_H

#include <QBrush>
#include <QPen>

#include <qgis/qgsabstractgeometry.h>
#include <qgis/qgsdistancearea.h>

#include <kadas/core/mapitems/kadasmapitem.h>

struct QgsVertexId;

class KADAS_CORE_EXPORT KadasGeometryItem : public KadasMapItem
{
    Q_OBJECT
  public:
    enum IconType
    {
      /**
      * No icon is used
      */
      ICON_NONE,
      /**
       * A cross is used to highlight points (+)
       */
      ICON_CROSS,
      /**
       * A cross is used to highlight points (x)
       */
      ICON_X,
      /**
       * A box is used to highlight points (□)
       */
      ICON_BOX,
      /**
       * A circle is used to highlight points (○)
       */
      ICON_CIRCLE,
      /**
       * A full box is used to highlight points (■)
       */
      ICON_FULL_BOX,
      /**
       * A triangle is used to highlight points (△)
       */
      ICON_TRIANGLE,
      /**
       * A full triangle is used to highlight points (▲)
       */
      ICON_FULL_TRIANGLE
    };

    KadasGeometryItem(const QgsCoordinateReferenceSystem& crs, QObject* parent = nullptr );
    ~KadasGeometryItem();

    void render( QgsRenderContext &context ) const override;
    QgsRectangle boundingBox() const override;
    int margin() const override { return qMax(mIconSize, mPen.width()); }

    QList<QgsPointXY> nodes() const override;

    /** Returns whether the geometry contains the specified point */
    bool contains( const QgsPointXY& p, double tol ) const;

    void setFillColor( const QColor& c );
    QColor fillColor() const;
    void setOutlineColor( const QColor& c );
    QColor outlineColor() const;
    void setOutlineWidth( int width );
    int outlineWidth() const;
    void setLineStyle( Qt::PenStyle penStyle );
    Qt::PenStyle lineStyle() const;
    void setBrushStyle( Qt::BrushStyle brushStyle );
    Qt::BrushStyle brushStyle() const;

    void setIconType( IconType iconType ) { mIconType = iconType; }
    IconType iconType() const { return mIconType; }
    void setIconSize( int iconSize );
    int iconSize() const { return mIconSize; }
    void setIconFillColor( const QColor& c );
    const QColor& iconFillColor() const { return mIconBrush.color(); }
    void setIconOutlineColor( const QColor& c );
    QColor iconOutlineColor() const { return mIconPen.color(); }
    void setIconOutlineWidth( int width );
    int iconOutlineWidth() const { return mIconPen.width(); }
    void setIconLineStyle( Qt::PenStyle penStyle );
    Qt::PenStyle iconLineStyle() const { return mIconPen.style(); }
    void setIconBrushStyle( Qt::BrushStyle brushStyle );
    Qt::BrushStyle iconBrushStyle() const { return mIconBrush.style(); }

    QString getTotalMeasurement() const{ return mTotalMeasurement; }

  signals:
    void geometryChanged();

  protected:
    QgsAbstractGeometry* mGeometry = nullptr;

    QPen mPen;
    QBrush mBrush;
    int mIconSize;
    IconType mIconType;
    QPen mIconPen;
    QBrush mIconBrush;

    QgsDistanceArea mDa;
    bool mMeasureGeometry = false;

    struct MeasurementLabel {
      QString string;
      QgsPointXY mapPos;
    };
    QList<MeasurementLabel> mMeasurementLabels;
    QString mTotalMeasurement;


    void setGeometry( QgsAbstractGeometry* geom);
    const QgsAbstractGeometry* geometry() const { return mGeometry; }

    void drawVertex( QPainter* p, double x, double y ) const;
    QString formatLength( double value, QgsUnitTypes::DistanceUnit unit ) const;
    QString formatArea( double value, QgsUnitTypes::AreaUnit unit ) const;
    QString formatAngle( double value, QgsUnitTypes::AngleUnit unit ) const;
    void addMeasurements( const QStringList& measurements, const QgsPointXY &mapPos );
    virtual void measureGeometry() {}

  private slots:
    void updateMeasurements();
};

#endif // KADASGEOMETRYITEM_H