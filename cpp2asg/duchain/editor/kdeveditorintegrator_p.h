/* This file is part of KDevelop
    Copyright (C) 2006 Hamish Rodda <rodda@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef KDEVEDITORINTEGRATOR_P_H
#define KDEVEDITORINTEGRATOR_P_H

#include <QObject>
#include <QMap>
#include <QMutex>

#include <QUrl>

#include "range.h"
#include "rangefeedback.h"

#include "kdevdocumentcursor.h"

class DocumentRange;
class DocumentCursor;

class EditorIntegratorPrivate : public QObject, public RangeWatcher
{
  Q_OBJECT

public:
  EditorIntegratorPrivate();
  virtual ~EditorIntegratorPrivate();

  virtual void rangeDeleted(Range* range);

public:
  QMutex* mutex;
  QMap<QUrl, QVector<Range*> > topRanges;
};

#endif // EDITORINTEGRATOR_H

// kate: indent-width 2;
