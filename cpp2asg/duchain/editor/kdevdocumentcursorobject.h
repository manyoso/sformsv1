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

#ifndef KDEVDOCUMENTCURSOROBJECT_H
#define KDEVDOCUMENTCURSOROBJECT_H

#include "cursor.h"
#include "cursorfeedback.h"

#include "kdevdocumentcursor.h"

/**
 * Base class for any object which has an associated range of text.
 *
 * This allows text without a currently loaded text editor to be represented.
 */
class DocumentCursorObject : public CursorWatcher
{
public:
  DocumentCursorObject(Cursor* cursor);
  virtual ~DocumentCursorObject();

  void setTextCursor(Cursor* cursor);

  Cursor& textCursor();
  const Cursor& textCursor() const;
  const DocumentCursor& textDocCursor() const;
  Cursor* textCursorPtr() const;

  QUrl url() const;
  static QUrl url(const Cursor* range);

  virtual void deleted(Cursor* cursor);

private:
  Cursor* m_cursor;
  QUrl m_url;
};

#endif // KDEVDOCUMENTCURSOROBJECT_H

// kate: indent-width 2;
