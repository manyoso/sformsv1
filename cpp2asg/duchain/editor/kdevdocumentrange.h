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

#ifndef KDEVDOCUMENTRANGE_H
#define KDEVDOCUMENTRANGE_H

#include <QUrl>

#include "range.h"

/**
 * Extends Range with information about the URL to which the range
 * refers.
 *
 * \todo parent/child relationships here too?
 */
class DocumentRange : public Range
{
public:
  DocumentRange(const QUrl& document, const Cursor& start, const Cursor& end, Range* parent = 0);
  DocumentRange(const QUrl& document, const Range& range = Range::invalid(), Range* parent = 0);
  DocumentRange(const DocumentRange& copy);
  virtual ~DocumentRange();

  /// Returns the associated document.
  const QUrl& document() const;

  /// Sets the associated document.
  void setDocument(const QUrl& document);

  Range* parentRange() const;
  void setParentRange(Range* parent);

  const QList<DocumentRange*>& childRanges() const;

private:
  QUrl m_document;
  Range* m_parentRange;
  QList<DocumentRange*> m_childRanges;
};

#endif // DOCUMENTRANGE_H

// kate: indent-width 2;
