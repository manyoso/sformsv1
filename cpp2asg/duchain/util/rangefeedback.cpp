/* This file is part of the KDE libraries
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

#include "rangefeedback.h"

RangeWatcher::~ RangeWatcher( )
{
}

RangeNotifier::RangeNotifier( )
  : m_wantDirectChanges(true)
{
}

bool RangeNotifier::wantsDirectChanges( ) const
{
  return m_wantDirectChanges;
}

void RangeNotifier::setWantsDirectChanges( bool wantsDirectChanges )
{
  m_wantDirectChanges = wantsDirectChanges;
}

RangeWatcher::RangeWatcher( )
  : m_wantDirectChanges(true)
{
}

bool RangeWatcher::wantsDirectChanges( ) const
{
  return m_wantDirectChanges;
}

void RangeWatcher::setWantsDirectChanges( bool wantsDirectChanges )
{
  m_wantDirectChanges = wantsDirectChanges;
}

void RangeWatcher::rangePositionChanged( Range* )
{
}

void RangeWatcher::rangeContentsChanged( Range* )
{
}

void RangeWatcher::rangeContentsChanged( Range*, Range* )
{
}

void RangeWatcher::mouseEnteredRange( Range*, View* )
{
}

void RangeWatcher::mouseExitedRange( Range*, View* )
{
}

void RangeWatcher::caretEnteredRange( Range*, View* )
{
}

void RangeWatcher::caretExitedRange( Range*, View* )
{
}

void RangeWatcher::rangeEliminated( Range* )
{
}

void RangeWatcher::rangeDeleted( Range* )
{
}

void RangeWatcher::childRangeInserted( Range*, Range* )
{
}

void RangeWatcher::childRangeRemoved( Range*, Range* )
{
}

void RangeWatcher::rangeAttributeChanged( Range*, Attribute::Ptr, Attribute::Ptr )
{
}

void RangeWatcher::parentRangeChanged( Range*, Range*, Range* )
{
}

// kate: space-indent on; indent-width 2; replace-tabs on;
