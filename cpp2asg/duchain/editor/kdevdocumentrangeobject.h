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

#ifndef KDEVDOCUMENTRANGEOBJECT_H
#define KDEVDOCUMENTRANGEOBJECT_H

#include <QMutex>

#include "range.h"
#include "rangefeedback.h"

#include "kdevdocumentcursor.h"
#include "kdevdocumentrange.h"

/**
 * Base class for any object which has an associated range of text.
 *
 * This allows text without a currently loaded text editor to be represented.
 */
class DocumentRangeObject : public RangeWatcher
{
public:
  DocumentRangeObject(Range* range);
  virtual ~DocumentRangeObject();

  /**
   * Sets the text \a range to this object.  If \a ownsRange is false, the range won't be
   * deleted when the object is deleted.
   */
  void setTextRange(Range* range, bool ownsRange = true);

  void setRange(const Range& range);
  const Range textRange() const;
  const DocumentRange textDocRange() const;
  Range* textRangePtr() const;

  QUrl url() const;
  static QUrl url(const Range* range);

  bool contains(const DocumentCursor& cursor) const;

  virtual void rangeDeleted(Range* range);

  /// Take the range from this object. USE WITH CARE!! You must be willing to
  /// immediately delete this range object if you take its range.
  Range* takeRange();

private:
  Q_DISABLE_COPY(DocumentRangeObject)

  mutable QMutex m_rangeMutex;
  Range* m_range;
  bool m_ownsRange;
  QUrl* m_url;
};

#endif // RANGEOBJECT_H

// kate: indent-width 2;
