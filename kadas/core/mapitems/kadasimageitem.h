/***************************************************************************
    kadasimageitem.h
    ----------------
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

#ifndef KADASIMAGEITEM_H
#define KADASIMAGEITEM_H

#include <kadas/core/kadas_core.h>
#include <kadas/core/mapitems/kadasmapitem.h>


class KADAS_CORE_EXPORT KadasImageItem : public KadasMapItem
{
public:
  KadasImageItem(const QgsCoordinateReferenceSystem& crs, QObject* parent = nullptr);

  void setFilePath(const QString& path, double anchorX = 0.5, double anchorY = 0.5);
  const QString& filePath() const{ return mFilePath; }

  QgsRectangle boundingBox() const override;
  QRect margin() const override;
  QList<QgsPointXY> nodes(const QgsMapSettings &settings) const override;
  bool intersects( const QgsRectangle& rect, const QgsMapSettings& settings ) const override;
  void render( QgsRenderContext &context ) const override;

  bool startPart(const QgsPointXY& firstPoint) override;
  bool startPart(const AttribValues& values) override;
  void setCurrentPoint(const QgsPointXY& p, const QgsMapSettings* mapSettings=nullptr) override;
  void setCurrentAttributes(const AttribValues& values) override;
  bool continuePart() override;
  void endPart() override;

  AttribDefs drawAttribs() const override;
  AttribValues drawAttribsFromPosition(const QgsPointXY& pos) const override;
  QgsPointXY positionFromDrawAttribs(const AttribValues& values) const override;

  EditContext getEditContext(const QgsPointXY& pos, const QgsMapSettings& mapSettings) const override;
  void edit(const EditContext& context, const QgsPointXY& newPoint, const QgsMapSettings* mapSettings=nullptr) override;
  void edit(const EditContext& context, const AttribValues& values) override;

  AttribValues editAttribsFromPosition(const EditContext& context, const QgsPointXY& pos) const override;
  QgsPointXY positionFromEditAttribs(const EditContext& context, const AttribValues& values) const override;

  struct State : KadasMapItem::State {
    QgsPointXY pos;
    double angle;
    QSize size;
    void assign(const KadasMapItem::State* other) override { *this = *static_cast<const State*>(other); }
    State* clone() const override { return new State(*this); }
  };
  const State* state() const{ return static_cast<State*>(mState); }

private:
  enum AttribIds {AttrX, AttrY};
  QString mFilePath;
  double mAnchorX = 0.5;
  double mAnchorY = 0.5;

  State* state(){ return static_cast<State*>(mState); }
  State* createEmptyState() const override { return new State(); }
  void recomputeDerived() override;
  QList<QgsPointXY> rotatedCornerPoints() const;
};

#endif // KADASIMAGEITEM_H