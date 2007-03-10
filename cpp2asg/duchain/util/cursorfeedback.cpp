/* This file is part of the KDE project
   Copyright (C) 2003-2005 Hamish Rodda <rodda@kde.org>

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

#include "cursorfeedback.h"

CursorWatcher::CursorWatcher( )
  : m_wantDirectChanges(true)
{
}

CursorWatcher::~ CursorWatcher( )
{
}

void CursorWatcher::positionChanged( Cursor * )
{
}

void CursorWatcher::positionDeleted( Cursor * )
{
}

void CursorWatcher::characterDeleted( Cursor * , bool )
{
}

void CursorWatcher::characterInserted( Cursor * , bool )
{
}

bool CursorWatcher::wantsDirectChanges( ) const
{
  return m_wantDirectChanges;
}

void CursorWatcher::setWantsDirectChanges( bool wantsDirectChanges )
{
  m_wantDirectChanges = wantsDirectChanges;
}

CursorNotifier::CursorNotifier( )
  : m_wantDirectChanges(true)
{
}

bool CursorNotifier::wantsDirectChanges( ) const
{
  return m_wantDirectChanges;
}

void CursorNotifier::setWantsDirectChanges( bool wantsDirectChanges )
{
  m_wantDirectChanges = wantsDirectChanges;
}

void CursorWatcher::deleted( Cursor * )
{
}

// kate: space-indent on; indent-width 2; replace-tabs on;
