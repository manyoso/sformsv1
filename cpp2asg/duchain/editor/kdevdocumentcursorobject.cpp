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

#include "kdevdocumentcursorobject.h"

DocumentCursorObject::DocumentCursorObject(Cursor* cursor)
  : m_cursor(0)
{
  setTextCursor(cursor);
}

DocumentCursorObject::~ DocumentCursorObject( )
{
  delete m_cursor;
}

void DocumentCursorObject::setTextCursor( Cursor * cursor )
{
  Q_ASSERT(cursor);

  if (m_cursor == cursor)
    return;

  if (m_cursor) {
    delete m_cursor;
  }

  m_cursor = cursor;
}

const Cursor& DocumentCursorObject::textCursor( ) const
{
  return *m_cursor;
}

Cursor& DocumentCursorObject::textCursor( )
{
  return *m_cursor;
}

const DocumentCursor& DocumentCursorObject::textDocCursor() const
{
  return *static_cast<DocumentCursor*>(m_cursor);
}

QUrl DocumentCursorObject::url() const
{
  return url(m_cursor);
}

QUrl DocumentCursorObject::url( const Cursor * cursor )
{
  return static_cast<const DocumentCursor*>(cursor)->document();
}

Cursor* DocumentCursorObject::textCursorPtr() const
{
  return m_cursor;
}

// kate: indent-width 2;

void DocumentCursorObject::deleted(Cursor * cursor)
{
  Q_ASSERT(cursor == m_cursor);
  //Q_ASSERT(false);
  m_cursor = new DocumentCursor(m_url, *m_cursor);
}
