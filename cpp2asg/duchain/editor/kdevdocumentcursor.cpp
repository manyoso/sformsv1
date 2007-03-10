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

#include "kdevdocumentcursor.h"

#include "kdevdocumentrangeobject.h"

DocumentCursor::DocumentCursor(const QUrl& document, const Cursor& cursor)
  : Cursor(cursor)
  , m_document(document)
{
}

DocumentCursor::DocumentCursor(Range* range, Position position )
  : Cursor(position == Start ? range->start() : range->end())
  , m_document(DocumentRangeObject::url(range))
{
}

DocumentCursor::DocumentCursor(Cursor* cursor)
  : Cursor(*cursor)
{
    m_document = static_cast<DocumentCursor*>(cursor)->document();
}

DocumentCursor::DocumentCursor(const DocumentCursor& copy)
  : Cursor(copy)
  , m_document(copy.document())
{
}

const QUrl& DocumentCursor::document() const
{
  return m_document;
}

void DocumentCursor::setDocument(const QUrl& document)
{
  m_document = document;
}

// kate: indent-width 2;
